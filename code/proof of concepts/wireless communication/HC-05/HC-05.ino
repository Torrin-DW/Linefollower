int LED = 4;
int state;


void setup ()
{
  pinMode (LED, OUTPUT);
  //Serial1.begin (9600);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  if (Serial1.available())
  {
    state = Serial1.read();
  }  

  if (state == '1')
    {
      digitalWrite(LED,HIGH);
      Serial.println("LEd aan");
    }
  else if (state == '0')
  {
    digitalWrite(LED,LOW);
    Serial.println("Led uit");
  }

  //Serial.println(state);
  delay(100);
}