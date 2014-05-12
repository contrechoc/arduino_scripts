//receiving

#include <JeeLib.h>

unsigned char receivedCode1 = '0';//piece of cloth

void setup () {

  Serial.begin(9600);
  //initializing RF
  rf12_initialize('R', RF12_433MHZ, 212);
  
  
  //intializing PINS for cloth and testing
  for (int i = 4 ; i < 10 ; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(250);
   for (int i = 4 ; i < 10 ; i++){
    digitalWrite(i, LOW);
  }
  delay(250);
  for (int i = 4 ; i < 10 ; i++){
    allLOW1();
    digitalWrite(i, HIGH);
     delay(250);
  }

  
  
}

void allLOW1(){ //cloth
     for (int i = 4 ; i < 10 ; i++){
    digitalWrite(i, LOW);
  }
}


void loop () {

  // RFM12 loop runner

  char* buf = (char*) rf12_data;
  if (rf12_recvDone() && rf12_crc == 0) {

    for ( int i=0;i<10;i++)
    {
      //Serial.print(buf[i]);
      if ( buf[i] == '#' ) {
        receivedCode1 = buf[i+1];//6 modes for cloth
  
        allLOW1();

      }
    }
    Serial.println(receivedCode1);
    Serial.flush();
    
    //piece of cloth
    if ( receivedCode1 == '0' )  for (int i = 0 ; i < 3 ; i++) pattern1();
    if ( receivedCode1 == '1' )  for (int i = 0 ; i < 3 ; i++) pattern2();
    if ( receivedCode1 == '2' )  for (int i = 0 ; i < 3 ; i++) pattern3();
    if ( receivedCode1 == '3' )  for (int i = 0 ; i < 3 ; i++) pattern4();
    if ( receivedCode1 == '4' )  for (int i = 0 ; i < 3 ; i++) pattern5();
    if ( receivedCode1 == '5' )  for (int i = 0 ; i < 3 ; i++) pattern6();
    
    
    delay(1000);
  }
  delay(10);
}


void pattern1(){
  for (int i = 4 ; i < 10 ; i++){
    allLOW1();
    digitalWrite(i, HIGH);
    delay(250);
  }
}

void pattern2(){
  for (int i = 9 ; i > 3 ; i--){
    allLOW1();
    digitalWrite(i, HIGH);
    delay(250);
  }
}

void pattern3(){
  allLOW1();
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(250);
  allLOW1();
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(250);
  allLOW1();
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(250);

}

void pattern4(){
  allLOW1();
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
   digitalWrite(6, HIGH);
  delay(250);
  allLOW1();
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(250);
}

void pattern5(){
  allLOW1();
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
  delay(250);
  allLOW1();
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(8, HIGH);
  delay(250);
}

void pattern6(){
  for (int i = 9 ; i > 4 ; i--){
    allLOW1();
    digitalWrite(i, HIGH);
     digitalWrite(i+1, HIGH);
    delay(250);
  }
}




