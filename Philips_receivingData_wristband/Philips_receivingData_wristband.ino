//receiving

#include <JeeLib.h>

unsigned char receivedCode1 = '0';//piece of cloth
unsigned char receivedCode2 = '0';//wrist band


void setup () {

  Serial.begin(9600);
  //initializing RF
  rf12_initialize('R', RF12_433MHZ, 212);



  //initializing for wrist band --- and testing
  for (int i = 14 ; i < 20 ; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(250);
  for (int i = 14 ; i < 20 ; i++){
    digitalWrite(i, LOW);
  }
  delay(250);
  for (int i = 14 ; i < 20 ; i++){
    allLOW2();
    digitalWrite(i, HIGH);
    delay(250);
  }

  pattern1();
  pattern2();
  pattern3();
  pattern4();
  pattern5();

  allLOW2();
}


void allLOW2(){ //wrist band
  for (int i = 14 ; i < 20 ; i++){
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

        receivedCode2 = buf[i+1];//only 5 triacs for writs band
        // allLOW1();
        allLOW2();
      }
    }
    Serial.println(receivedCode1);
    Serial.flush();


    //wrist band
    if ( receivedCode2 == '0' )  for (int i = 0 ; i < 3 ; i++) pattern1();
    if ( receivedCode2 == '1' )  for (int i = 0 ; i < 3 ; i++) pattern2();
    if ( receivedCode2 == '2' )  for (int i = 0 ; i < 3 ; i++) pattern3();
    if ( receivedCode2 == '3' )  for (int i = 0 ; i < 3 ; i++) pattern4();
    if ( receivedCode2 == '4' )  for (int i = 0 ; i < 3 ; i++) pattern5();
    //if ( receivedCode2 == '5' )pattern1();

    delay(1000);
  }
  delay(10);
}


void pattern1(){
  for (int i = 15 ; i < 20 ; i++){
    allLOW2();
    digitalWrite(i, HIGH);
    delay(250);
  }
}

void pattern2(){
  for (int i = 19 ; i > 13 ; i--){
    allLOW2();
    digitalWrite(i, HIGH);
    delay(250);
  }
}

void pattern3(){
  allLOW2();
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  delay(250);
  allLOW2();
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  delay(250);
  allLOW2();
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  delay(250);
  allLOW2();
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  delay(250);

}

void pattern4(){
  allLOW2();
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  delay(250);
  allLOW2();
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  delay(250);
}

void pattern5(){
  allLOW2();
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  delay(250);
  allLOW2();
  digitalWrite(19, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  delay(250);
}






