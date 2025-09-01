// Setting up pins for the DRV8833 board to use as inputs. 
// (11, 10, 9, 6, 5 and 3 are PWM capable on UNO - using a PWM capable pin will
// allow us to control motor speed)

// Setting the 4 "signal" pins for the DRV8833 - sig1 and sig2 control one motor,
// sig3 and sig4 control another.
const int sig1 = 3;
const int sig2 = 5;
const int sig3 = 6;
const int sig4 = 9;


void setup() {


pinMode(sig1, OUTPUT);
pinMode(sig2, OUTPUT);
pinMode(sig3, OUTPUT);
pinMode(sig4, OUTPUT);


digitalWrite(sig1, LOW);
digitalWrite(sig2, LOW);
digitalWrite(sig3, LOW);
digitalWrite(sig4, LOW);

}

void loop() {
  
d
digitalWrite(sig1,HIGH);
digitalWrite(sig3,HIGH);
}

void stopAllMotors() {

  digitalWrite(sig1, LOW);
  digitalWrite(sig2, LOW);
  digitalWrite(sig3, LOW);
  digitalWrite(sig4, LOW);
}

void motorA_CCW() {
  
  digitalWrite(sig1, HIGH);
  digitalWrite(sig2, LOW);
}

void motorA_CW() {


  digitalWrite(sig1, LOW);
  digitalWrite(sig2, HIGH);
}

void motorA_CCW_Half() {

  analogWrite(sig1, 127);
  digitalWrite(sig2, LOW);
}

void motorA_CW_Half() {


  digitalWrite(sig1, LOW);
  analogWrite(sig2, 127);
}

void motorB_CCW() {


  digitalWrite(sig3, HIGH);
  digitalWrite(sig4, LOW);
}

void motorB_CW() {


  digitalWrite(sig3, LOW);
  digitalWrite(sig4, HIGH);
}

void motorB_CCW_Half() {

  analogWrite(sig3, 127);
  digitalWrite(sig4, LOW);
}

void motorB_CW_Half() {

  digitalWrite(sig3, LOW);
  analogWrite(sig4, 127);
}