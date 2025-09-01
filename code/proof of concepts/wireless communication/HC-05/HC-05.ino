#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial BTSerial(9, 8);
void setup() {
  // put your setup code here, to run once:
  pinMode(9, INPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);

}

String message = "";
String messagebuffer = "";
void loop() {
  // put your main code here, to run repeatedly:
  while (BTSerial.available() > 0){
    char data = (char) BTSerial.read();
    if (data == ";") {
      message = messagebuffer;
      messagebuffer ="";
      Serial.print(message);
      message ="You sent" + message;
      BTSerial.print(message);
    }
  }
}
