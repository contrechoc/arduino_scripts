/*
* 
* testing script
* running on a atmega328, using Jeelib.h
* Data coming from UART (Raspberry) are sent with a RF12 transceiver
* contrechoc 2013
*
*
*/


#include <JeeLib.h>


char inputString[] = "hello world!";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned char c = 0;
char buf7[] = {    
  "012345678901234567890123456789012345678901234567890123456789"       };

long timer;
int outputLED = 7;

unsigned char bufferLength = 40;

unsigned char offOn = 0;

void setup(){

  rf12_initialize('R', RF12_433MHZ, 212);
  Serial.begin(9600);

  //LED
  pinMode(outputLED, OUTPUT);//PB1
  digitalWrite(outputLED,HIGH);
  delay(100);
  digitalWrite(outputLED,LOW);

  //send test
  c =10;
  rf12_sendStart(0, inputString, c , 0);

}

void loop(){

  lookForSerial();

 
  if ( (timer < millis())){
   if ( offOn == 0){
   digitalWrite(7,HIGH);
   offOn = 1;
   }else{
   offOn = 0;
   digitalWrite(7,LOW);
   
   }
   timer = millis() + 1000;
  
   }
 

  delay(1);

}

void lookForSerial() {
  if ( Serial.available() ){

    Serial.readBytes(buf7, bufferLength) ;

    sendChars();
    Serial.flush();
    c=0;
    //signal the sending
    digitalWrite(7,HIGH);
    delay(100);
    digitalWrite(7,LOW);
  }

}

void sendChars(){

  while ((!rf12_canSend()) )
    rf12_recvDone();
    rf12_sendStart(0, buf7 , bufferLength , 0);

  delay(50);



}



