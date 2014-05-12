/*

gets a first input, gets a second input, makes an interval and starts using this interval

using time

input can be serial, button, shock etc
output can be LED's, speaker, relais

interval can be refined

*/
 
 long time1 = 0;
 long time2 = 0;
 long time3 = 0;
 long interval = 0;
 unsigned char danceCounter = 0;
 unsigned char interValFlag = 0;
  
void setup() {                
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  time3 = millis()+interval;
}

void loop() {
  if (Serial.available()  && ( interValFlag == 0 ) ){
  digitalWrite(13, HIGH);   // set the LED on
  if ( time1 < 5){
    time1 = millis();
      Serial.print("time1 ");
    Serial.println(time1);
  }
    else{
    time2 = millis();
      Serial.print("time2 ");
     Serial.println(time2);
    }
    Serial.flush();
  delay(50);    
    
  }
else{  // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(2);              // wait for a second
}

 if (( time1 > 0) && ( time2 > 0) && ( interValFlag == 0 ) )
   {
     interval = time2 - time1;
     time3 = millis()+interval;
      Serial.print("interval ");
      Serial.println(interval);
      interValFlag = 1;
   }
   
   if ( interval > 0)
   {
     if (time3 < millis()){
       time3 = millis() + interval;
       doLEDDance();
       Serial.print("doLEDDance ");
      Serial.println(danceCounter, DEC);
       danceCounter++;
       if ( danceCounter > 5 )   resetValues();
     }
     
  }
}

void resetValues(){
  
   time1 = 0;
   time2 = 0;
   time3 = 0;
   interval = 0;
 danceCounter = 0;
 interValFlag = 0;
  
}

void doLEDDance(){
   digitalWrite(13, HIGH);
   delay(50);  
   digitalWrite(13, LOW); 
     delay(50);  
    digitalWrite(13, HIGH);
    delay(50);    
    digitalWrite(13, LOW); 
    delay(50);  
    digitalWrite(13, HIGH);
    delay(50);    
    digitalWrite(13, LOW); 
    delay(50);  
}

