void setup(){
  
  
  Serial.begin(9600);
  
     pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);
   
     pinMode(8, OUTPUT);
   pinMode(9, OUTPUT);
   digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
  
}


void loop(){
  
    
   Serial.println(   analogRead(A0) );
   
   delay(50);
  
}
