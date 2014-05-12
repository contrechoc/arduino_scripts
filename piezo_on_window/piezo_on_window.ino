int hitCounter = 0;
int nonhitCounter = 0;
long timer;

void setup()
{
  Serial.begin(9600);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);

  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);

  pinMode(5, OUTPUT);
  tone(5, 2000,  100 );
  delay(10);
  tone(5, 2000,  100 );
  delay(10);
  tone(5, 2000,  100);
  delay(10);

}

void loop()
{
  int sensorValue = analogRead(5); //read the piezo value
  // print to serial for debug
  // Serial.println(sensorValue, DEC); 
  if (sensorValue > 6) //physically taping on the piezo gives results to around 400
  {                    //its about 30 when still
    if ( hitCounter == 0) {
      timer = millis();
      // Serial.println("start"); 
    }
    hitCounter++;
    // Serial.print(sensorValue, DEC); 

    // Serial.println(millis(), DEC); 
    if ( hitCounter > 4 ){
      tone(5, sensorValue*50,  10 );
     // Serial.println(hitCounter); 
    }

    if ( (( millis() - timer ) > 15) &&  (hitCounter > 5))  {
      Serial.println( millis() - timer ); 
      Serial.println("  ----------> tram"); 
      hitCounter = 0;
    }


    delay(2); //delay to reduce amount of data, one tap can count as multipule because the arduino sample too often.
  }
  else
  {
    hitCounter = 0;



  }
}

