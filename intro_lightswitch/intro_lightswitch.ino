

void setup(){
  
  Serial.begin(9600);

  pinMode( 7 , OUTPUT );
}


void loop(){

   digitalWrite ( 7, LOW ) ;

  int measureLight = analogRead( A0 ) ;
  Serial.println( measureLight ) ;

  if ( measureLight > 800 )
  {
    digitalWrite ( 7, HIGH ) ;
  }
  
  delay(1000);
}




