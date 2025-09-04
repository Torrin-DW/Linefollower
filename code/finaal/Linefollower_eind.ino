#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial1
#define Baudrate 9600

SerialCommand sCmd(SerialPort);
bool debug;
unsigned long previous, calculationTime;

const int sensor[] = {A0, A1, A2, A3, A4, A5};
const int MotorLeftForward = 5;
const int MotorLeftBackward = 3;
const int MotorRightForward = 9;
const int MotorRightBackward = 6;
bool running = LOW;
float iTerm;
float lastErr;
int hMotor = 13;
int knop = 7;


struct param_t
{
  unsigned long cycleTime;
  int black[6];
  int white[6];
  float diff;
  int power;
  float kp;
  float ki;
  float kd;
  
} params;

    float debugPosition;
    int normalised[6];
    float position;

    void onUnknownCommand(char *command);
    void onSet();
    void onDebug();
    void onCalibrate();
    void onRun();
    
void setup()
{
  SerialPort.begin(Baudrate);
  Serial.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("start", onStart);
  sCmd.addCommand("stop", onStop);
  
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);

  attachInterrupt(4, startKnop, RISING);

}

void loop()
{
  sCmd.readSerial();
  
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
    /* code die cyclisch moet uitgevoerd worden programmeer je hier ... */
    /* measure & normalize */
    for (int i = 0; i < 6; i++)
    {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
    }

    /* interpolate */
    int index= 0;
    float position = 0;
    for (int i = 1; i < 6; i++) if (normalised[i] < normalised[index]) index = i;
    
    if (normalised[index] > 3000) running = false;

    if (index == 0) position = -30;
    else if (index ==5) position = 30;
    else
    {
    int sNul = normalised[index];
    int sMinEen = normalised[index-1];
    int sPlusEen = normalised[index+1];
    
    float b = (sPlusEen - sMinEen);
    b = b / 2;

    float a = sPlusEen - b - sNul;

    position = -b / (2 * a);
    position += index;
    position -= 2.5;

    position *= 15;  
    }
    debugPosition = position;

    /* berekenen error = setpoint - positie */
    float error = -position;

    /*  proportioneel regelen */
    float output = error * params.kp;

    /*  integrerend regelen */
    iTerm += params.ki * error;
    iTerm = constrain(iTerm, -510, 510);
    output += iTerm;

    /* differentierend regelen */
    output += params.kd * (error - lastErr);
    lastErr = error;
    
    /*  output begrenzen tot wat fysiek mogelijk is */
    output = constrain(output, -510, 510);

    int powerLeft = 0;
    int powerRight = 0;

    if (running) if (output <= 0)
    {
      powerLeft = constrain(params.power + params.diff * output, -255, 255);
      powerRight = constrain(powerLeft - output, -255, 255);
      powerLeft = powerRight + output;
    }
    else
    {
      powerRight = constrain(params.power - params.diff * output, -255, 255);
      powerLeft = constrain(powerRight + output, -255, 255);
      powerRight = powerLeft - output;
    }

    analogWrite(MotorLeftForward, powerLeft > 0 ? powerLeft : 0);
    analogWrite(MotorLeftBackward, powerLeft < 0 ? -powerLeft : 0);
    analogWrite(MotorRightForward, powerRight > 0 ? powerRight : 0);
    analogWrite(MotorRightBackward, powerRight < 0 ? - powerRight : 0);
  }
  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0) 
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;
    
    params.cycleTime = atol(value);
  }
  else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
  }
  else if (strcmp(param, "kd") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / cycleTimeInSec;
  }
  else if (strcmp(param, "power") == 0) params.power = atol(value);
  else if (strcmp(param, "diff") == 0) params.diff = atof(value);
  else if (strcmp(param, "kp") == 0) params.kp = atof(value);
  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  
  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("normalised: ");
  for (int i = 0; i < 6; i++)
  {
  SerialPort.print(normalised[i]);
  SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("position: ");
  SerialPort.println(debugPosition);

  SerialPort.print("power: ");
  SerialPort.println(params.power);
  SerialPort.print("diff: ");
  SerialPort.println(params.diff);
  SerialPort.print("kp: ");
  SerialPort.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("ki: ");
  SerialPort.println(ki);

  float kd = params.kd * cycleTimeInSec;
  SerialPort.print("kd: ");
  SerialPort.println(kd);
  
  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i]=analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");    
    for (int i = 0; i < 6; i++) params.white[i]=analogRead(sensor[i]);  
    SerialPort.println("done");      
  }

  EEPROM_writeAnything(0, params);
}

void interrupt(){
  static unsigned long vorigeInterrupt = 0;
  unsigned long interruptTijd = millis();
  if (interruptTijd - vorigeInterrupt > 100){
    running =! running;
  }
  vorigeInterrupt = interruptTijd;
}

void onStart(){
  running = true;
  digitalWrite(hMotor, HIGH);
  SerialPort.println("STARTED");
}

void onStop(){
  running = false;
  digitalWrite(hMotor, LOW);
  SerialPort.println("STOPPED");
}

void startKnop(){
    if (running == false)
  {
    running = true;
    digitalWrite(hMotor, HIGH);
    SerialPort.println("Start");
   

  }

  else if (running == true)
  {

    running = false;
    digitalWrite(hMotor, LOW);
    SerialPort.println("Stop");
    
    
  }
}