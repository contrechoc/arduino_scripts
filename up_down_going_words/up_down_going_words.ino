/**********************************************************************
 * CLOCK object contrechoc 2014, 
 * 
 * feel free to modify etc....
 * 
 * using a display of 8 7 segments numbers:
 * http://dx.com/p/8x-led-display-digital-tube-module-for-arduino-595-driver-148820#.UtuAmPaDNsQ
 * 
 * time chip: ds1307 (i2c)
 * 
 * a similar script has a time chip: ds1302 (SPI), using the library of arduino.cc user "Krodal" (see tab)
 * http://dx.com/p/ds1302-real-time-clock-module-with-battery-cr2032-126453#.UtuA7_aDNsQ
 * 
 * to buttons are used to alter the settings
 * digital pin 8, 9
 * one LDR is used to dim the light at night
 * A0
 * 
 ***********************************************************************/

// include the SPI library:
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

unsigned char myNumber[] = {
  10 , 11, 12, 13, 13, 0, 10, 10};

unsigned char myNumber2[] = {
  42 , 42, 42, 42, 42, 42, 42, 10};


volatile int ledCounter = 0;
volatile int ledCounter2 = 0;

volatile unsigned char effect = 0;

int tCounter = 0;
unsigned char tCounter2 = 0;

volatile int timerForSet = 0;

volatile unsigned char settingMode = 0;
unsigned char onOff = 0;
unsigned char setTimeComponent = 0;

unsigned char timer0 = 0;

unsigned char words[8][8];

unsigned char myWord0[] = {  
  44,45,46,47,44,45,46,47          };//ragged line
unsigned char myWord1[] = { 
  42, 18,15,21,21,22,42, 42            };//hello
unsigned char myWord2[] = {  
  42, 42, 13,19,11,22,42, 42           };//ciao
unsigned char myWord3[] = {  
  42, 18, 22,26,42,18,22, 26           };//hot hot
unsigned char myWord4[] = {  
  42, 42, 13,22,22,21,42, 42           };//cool
unsigned char myWord5[] = {  
  42, 14, 40,26,42,14,40, 26           };//dot dot
unsigned char myWord6[] = {  
  42, 42,  18,22,21,11,42, 42          };//hola
unsigned char myWord7[] = {  
  40, 41,  40,40,41,41,40, 41          };//circles


void setup() {
  // set the slaveSelectPin as an output:

  Serial.begin(9600);
  pinMode (slaveSelectPin, OUTPUT);

  pinMode ( 8,INPUT_PULLUP);
  pinMode ( 9,INPUT_PULLUP);

  //for testing on an arduino:
  pinMode (16, OUTPUT);
  digitalWrite(16,LOW);
  pinMode (17, OUTPUT);
  digitalWrite(17,HIGH);

  // initialize SPI:
  SPI.begin(); 

  noInterrupts();           // disable all interrupts

    TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 5; //10 - 200    // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  //----------------------
  if( timer0 == 1){

    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0  = 0;

    OCR0A = 50;            // compare match register 16MHz/256/2Hz
    TCCR0B |= (1 << WGM02);   // CTC mode
    TCCR0B |= (1 << CS01);    // 256 prescaler 
    TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt
  }

  interrupts();             // enable all interrupts

    myStr8Copy(myWord0, words[0]);
  myStr8Copy(myWord1, words[1]);
  myStr8Copy(myWord2, words[2]);
  myStr8Copy(myWord3, words[3]);
  myStr8Copy(myWord4, words[4]);
  myStr8Copy(myWord5, words[5]);
  myStr8Copy(myWord6, words[6]);
    myStr8Copy(myWord7, words[7]);

  myStr8Copy(myWord0, myNumber);
  //for ( int j = 0; j < 800; j++)
    myDelay(32000);


}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{

  timerForSet++;

  ledCounter2++;
  ledCounter2 = (ledCounter2)%8;
  digitalWrite(slaveSelectPin,LOW);

  SPI.transfer(1<<(ledCounter2));
  unsigned char num = myNumber2[ledCounter2];


  letterTransfer(num);

  //SPI.transfer(myNumber[ledCounter%8]);

  digitalWrite(slaveSelectPin,HIGH); 

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  ledCounter++;
  ledCounter = (ledCounter)%8;
  digitalWrite(slaveSelectPin,LOW);

  SPI.transfer(1<<(ledCounter));
  unsigned char num = myNumber[ledCounter];

  if ( effect == 0 )
    letterTransfer(num);
  else
    SPI.transfer( B11111111 -  myNumber[ledCounter%8] );

  digitalWrite(slaveSelectPin,HIGH); 

}

void loop() {

  moveWord(words[random(8)], words[random(8)],words[random(8)]);
}

void moveWord( unsigned char* myWord1, unsigned char* myWord2, unsigned char* myWord3  ){
  if ( random ( 100) < 50 ){
    upWord(myWord1);
    upWord(myWord2);
    upWord(myWord3);
  }
  else
  {
    downWord(myWord3);
    downWord(myWord2);
    downWord(myWord1);
  }

}

void upWord(unsigned char *wordArrayIN){
  effect = 0;

  if( timer0 == 1){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    sei();
  }

  unsigned char wordArray2[8]  ;//copy source
  myStr8Copy(wordArrayIN, wordArray2);
  myStr8Copy(wordArray2, myNumber);

  unsigned char wordArray3[8];
  myStr8Copy(wordArrayIN, wordArray3);

  for ( int j = 0; j < 8; j++){
    wordArray2[j] =   ( letterAndByte (  wordArray2[j])) ;
  }

  myStr8Copy(wordArray2, myNumber);
  myStr8Copy(wordArray2, wordArray3);
  if( timer0 == 1){
    cli();
    TIMSK0 = 0 ;
    sei();
  }
  effect = 1;

  //for shifting the first line in we have to shift two times down (only horizontal left)
  for ( int j = 0; j < 8; j++){
    wordArray2[j] = ( letterShiftDown (  wordArray2[j])) ;
    wordArray2[j] = ( letterShiftDown (  wordArray2[j])) ;
  }

  myStr8Copy(wordArray2, myNumber);
  myDelay(32000);

  //for shifting the second line in we have to shift one times down
  for ( int j = 0; j < 8; j++){
    wordArray3[j] = ( letterShiftDown (  wordArray3[j])) ;
  }
  myStr8Copy(wordArray3, myNumber);

  myDelay(32000);

  effect = 0;
  if( timer0 == 1){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    sei();
  }
  unsigned char wordArray4[8]  ;//copy source
  myStr8Copy(wordArrayIN, wordArray4);

  //normal line - no shifting
  myStr8Copy(wordArray4, myNumber);
  myDelay(32000);

  effect = 0;
  if( timer0 == 1){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    sei();
  }

  unsigned char wordArray[8]  ;//hello
  myStr8Copy(wordArrayIN, wordArray);
  myStr8Copy(wordArray, myNumber);

  //shifting up one line
  for ( int j = 0; j < 8; j++){ 
    myNumber[j] =  letterShiftUp( letterAndByte (  myNumber[j])) ;
  }
  if( timer0 == 1){
    cli();
    TIMSK0 = 0 ;
    sei();
  }
  effect = 1;

  //shifting up two line
  myDelay(32000);
  for ( int j = 0; j < 8; j++){
    myNumber[j] =  letterShiftUp(  (  myNumber[j])) ;
  }
  myDelay(32000);
  for ( int j = 0; j < 8; j++){
    myNumber[j] =  letterShiftUp(  (  myNumber[j])) ;
  }

  myDelay(32000);
}

void downWord(unsigned char *wordArrayIN){ //later downword
  effect = 0;

  if( timer0 == 1){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    sei();
  }

  unsigned char wordArray2[8]  ;//hello
  myStr8Copy(wordArrayIN, wordArray2);
  myStr8Copy(wordArray2, myNumber);

  unsigned char wordArray3[8];
  myStr8Copy(wordArrayIN, wordArray3);

  for ( int j = 0; j < 8; j++){
    wordArray2[j] =   ( letterAndByte (  wordArray2[j])) ;
  }

  myStr8Copy(wordArray2, myNumber);
  myStr8Copy(wordArray2, wordArray3);
  if( timer0 == 1){
    cli();
    TIMSK0 = 0 ;
    sei();
  }
  effect = 1;
  //---------------------------------------2 times
  for ( int j = 0; j < 8; j++){
    wordArray2[j] = ( letterShiftUp (  wordArray2[j])) ;
    wordArray2[j] = ( letterShiftUp (  wordArray2[j])) ;
  }

  myStr8Copy(wordArray2, myNumber);
  myDelay(32000);
  //---------------------------------------1 times
  for ( int j = 0; j < 8; j++){
    wordArray3[j] = ( letterShiftUp (  wordArray3[j])) ;
  }
  myStr8Copy(wordArray3, myNumber);

  myDelay(32000);

  effect = 0;
  if( timer0 == 1){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    sei();
  }
  unsigned char wordArray4[8]  ;//hello
  myStr8Copy(wordArrayIN, wordArray4);
  //---------------------------------------0 times
  myStr8Copy(wordArray4, myNumber);
  //  myDelay(32000);

  effect = 0;
  if( timer0 == 1){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    sei();
  }

  unsigned char wordArray[8]  ;//hello
  myStr8Copy(wordArrayIN, wordArray);
  myStr8Copy(wordArray, myNumber);

  for ( int j = 0; j < 8; j++){
    myNumber[j] =  ( letterAndByte (  myNumber[j])) ;
  }
  if( timer0 == 1){
    cli();
    TIMSK0 = 0 ;
    sei();
  }
  effect = 1;
  //--------------------------------------- -1 times
  myDelay(32000);
  for ( int j = 0; j < 8; j++){
    myNumber[j] =  letterShiftDown(  (  myNumber[j])) ;
  }
  myDelay(32000);
  //--------------------------------------- -2 times
  for ( int j = 0; j < 8; j++){
    myNumber[j] =  letterShiftDown(  (  myNumber[j])) ;
  }
  myDelay(32000);
  //--------------------------------------- -2 times
  for ( int j = 0; j < 8; j++){
    myNumber[j] =  letterShiftDown(  (  myNumber[j])) ;
  }
  myDelay(32000);
}



void letterTransfer(unsigned char num){

  /*
  
   -*-       0000 0001
   *-*  0010 0000  0000 0010
   -*-       0100 0000
   *-*  0001 0000  0000 0100
   -*-       0000 1000
   comma: 1000 0000
   
   */

  //ciao 13,19,11,22
  //house 18, 22, 27, 25, 15
  //plus 23,21, 27, 25

  SPI.transfer((unsigned char)(B11111111 - letterAndByte( num)));

  //numbers


}

unsigned char letterAndByte(unsigned char num){
  unsigned char newNum = 0;

  if ( effect == 0 ){

    //numbers
    if ( num == 1 )  newNum = B00000110;//1
    if ( num == 2 )  newNum =  B01011011;//2
    if ( num == 3 )  newNum =  B01001111;//3
    if ( num == 4 )  newNum =  B01100110;//4
    if ( num == 5 )  newNum =  B01101101;//5
    if ( num == 6 )  newNum =  B01111101;//6
    if ( num == 7 )  newNum =  B00000111;//7 
    if ( num == 8 )  newNum =  B01111111;//8
    if ( num == 9 )  newNum =  B01101111;//9
    if ( num == 0 )  newNum =  B00111111;//0
    //comma
    if ( num == 10 )  newNum = B10000000;//comma
    //some letters
    if ( num == 11 )  newNum =  B01110111;//A
    if ( num == 12 )  newNum =  B01111111;//B like 8
    if ( num == 13 )  newNum =  B00111001;//C
    if ( num == 14 )  newNum =  B01011110;//D small like 6 without upper stroke
    if ( num == 15 )  newNum =  B01111001;//E
    if ( num == 16 )  newNum =  B01111000;//F
    if ( num == 17 )  newNum =  B01101111;//small G like 9
    if ( num == 18 )  newNum =  B01110110;//H
    if ( num == 19 )  newNum =  B00000110;//I like 1
    if ( num == 20 )  newNum =  B00011110;//J
    // K?
    if ( num == 21 )  newNum =  B00111000;//L
    //M
    //N
    if ( num == 22 )  newNum =  B00111111;//O like 0
    if ( num == 23 )  newNum =  B01110011;//P
    if ( num == 24 )  newNum =  B10111111;//Q like 0.
    //R
    if ( num == 25 )  newNum =  B01101101;//S like 5
    if ( num == 26 )  newNum =  B01111000;//t small
    if ( num == 27 )  newNum =  B00111110;//U
    //V
    //W
    //X
    //Y
    //Z

    //special signs
    if ( num == 40 )  newNum =  B01011100;//under circle
    if ( num == 41 )  newNum =  B01100011;//upper circle
    if ( num == 42 )  newNum =  B00000000;//empty
    if ( num == 43 )  newNum =  B10001000;//.-.-.-.-.-.-.-. line 

    //square sequence
    if ( num == 44 )  newNum =  B00001100; // under + side under right
    if ( num == 45 )  newNum =  B01000010; //middle side upper right
    if ( num == 46 )  newNum =  B00000011;
    if ( num == 47 )  newNum =  B01000100;
    // side o
  }
  else
  {
    newNum = num;
  }

  return newNum;
}

void  myStr8Copy( unsigned    char *arraySource,  unsigned char *arrayDest ){
  unsigned char counter = 0;
  while ( counter < 8){
    arrayDest[  counter ] = arraySource [ counter ];
    counter++;
  }
}

void myDelay(int numDelay){
  for (int i = 0; i<numDelay; i++ )
    int h = digitalRead( 9);//debounce, no delay available

}

unsigned char letterShiftUp(unsigned char myLetter){
  /*
  
   -*-       0000 0001
   *-*  0010 0000  0000 0010
   -*-       0100 0000
   *-*  0001 0000  0000 0100
   -*-       0000 1000
   comma: 1000 0000
   
   */



  unsigned char outputLetter = B00000000;

  if ( (myLetter & B00001000) > 0 ) outputLetter |= B01000000;
  if ( (myLetter & B00000100) > 0 ) outputLetter |= B00000010;
  if ( (myLetter & B00010000) > 0 ) outputLetter |= B00100000;
  if ( (myLetter & B01000000) > 0 ) outputLetter |= B00000001;
  return outputLetter;
}

unsigned char letterShiftDown(unsigned char myLetter){

  unsigned char outputLetter = B00000000;

  if ( (myLetter & B01000000) > 0) outputLetter |= B00001000;
  if ( (myLetter & B00000010) > 0) outputLetter |= B00000100;
  if ( (myLetter & B00100000) > 0) outputLetter |= B00010000;
  if ( (myLetter & B00000001) > 0) outputLetter |= B01000000 ;
  return outputLetter;
}









