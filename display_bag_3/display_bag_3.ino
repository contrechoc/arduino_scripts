/* 3-wire connection demo to a HD44780-compatible LCD via shiftregister
 
 ShiftRegLCD123 wiring
 Requires 1, 2 or 3 pins from the Arduino.
 Also arduinoshiftreglcd / ShiftRegLCD compatible, except added 1-wire mode.
 
 SR output:  srlcd1123 wiring:
 Bit  #0   - N/C - not connected. Must be 0 in 1-wire and 2-wire mode.
 Bit  #1   - N/C or LCD backlight. Do not connect directly!
 Use a driver / transistor!
 Bit  #2   - connects to RS (Register Select) on the LCD
 Bits #3-6 - connects to LCD data inputs D4 - D7.
 Bit  #7   - 1-wire mode: To LCD Enable input.
 2-wire mode: To diode cathode of the diode-resistor AND "gate"
 3-wire mode: N/C, unused. LCD Enable directly from Arduino.
 LCD R/!W-pin hardwired to LOW (only writing to LCD).
 
 USAGE:
 
 1: Make an LCD object, set arduino output pins and LCD wiring scheme:
 
 ShiftRegLCD123 LCDobject( Datapin [, scheme] )
 or  ShiftRegLCD123 LCDobject( Datapin , Clockpin, scheme )
 or  ShiftRegLCD123 LCDobject( Datapin , Clockpin, Latchpin, scheme )
 
 where:
 Datapin : Arduino pin to shiftregister serial data input.
 Clockpin: Arduino pin to shiftregister clock input.
 Latchpin: Arduino pin to shiftregister latch/strobe/register clock input.
 scheme  : srlcd1123 or LCD3WIRE.
 Indicates shiftregister to LCD wiring type / variant.
 
 2: Initialize the LCD by calling begin() function with LCD size and font:
 
 LCDobject.begin( cols, lines [, font] )
 
 where:
 cols    : Nr. of columns in the LCD
 lines   : Nr. of "logical lines" in the LCD (not neccesarily physical)
 font    : 0 = small (default), 1 = large font for some 1-line LCD's only.
 */

#include <ShiftRegLCD123.h>



const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11
const byte enablePin1 = 12;   // LCD enable from Arduino pin 12
const byte enablePin2 = 13;   // LCD enable from Arduino pin 12 // orange wire 

const byte dataPin0  = 5;    // SR Data from Arduino pin 10
const byte clockPin0 = 6;    // SR Clock from Arduino pin 11
const byte enablePin0 = 7;   // LCD enable from Arduino pin 12 // green wire

const byte dataPin5  = 2;    // SR Data from Arduino pin 10
const byte clockPin5 = 3;    // SR Clock from Arduino pin 11
const byte enablePin5 = 4;   // LCD enable from Arduino pin 12 //purple wire

char ledOn1 = 8;
char ledOn2 = 9;

char changeText = 0;
unsigned char counter = 0;

// Instantiate an LCD object using ShiftRegLCD123 wiring.
// LCD enable directly from Arduino.
ShiftRegLCD123 srlcd0(dataPin0, clockPin0, enablePin0, SRLCD123);

ShiftRegLCD123 srlcd5(dataPin5, clockPin5, enablePin5, SRLCD123);


// Instantiate an LCD object using ShiftRegLCD123 wiring.
// LCD enable directly from Arduino.
ShiftRegLCD123 srlcd1(dataPin, clockPin, enablePin1, SRLCD123);
ShiftRegLCD123 srlcd2(dataPin, clockPin, enablePin2, SRLCD123);
/*
char text0[] = "can i do it?  1234567890123";
 char text1[] = "coooool!!!    1234567890123";
 char text2[] = "you can do it 1234567890123";
 char text3[] = "hot!!!        1234567890123";
 char text4[] = "abcdefghijklmnop";
 char text5[] = "1234567890123456";//16
 char text6[] = "test test test      ";
 char text7[] = "chips!!!!!!!     ;-)";
 
 char text8[] = "smart textiles 234567890123";
 char text9[] = "video-mania   1234567890123";
 char text10[] = "underground   1234567890123";
 char text11[] = "zero budget   1234567890123";
 char text12[] = "smart fashion!   :-))   ";//24
 char text13[] = "political fashion!  !   ";
 char text14[] = "noooo not again      ";
 char text15[] = "never say never     ";
 
 char* textPointer[] = { 
 text0, text1, text2, text3, text4, text5, text6,
 text7, text8, text9, text10, text11, text12, text13, text14, text15 };
 */
char text20[] = "let us have a go then and  ";
char text21[] = "rejoice! repent! rejoice!  ";
char text22[] = "why not give a ring????????";
char text23[] = "we will call you, maybe....";
char text24[] = "no pasaran        !    ?";
char text25[] = "->->->pasaremos .....:-(";
char text26[] = "warum gibt es et";
char text27[] = "nicht bloss nich";//16

char text28[] = "the joy of giving a ten 10 ";
char text29[] = "exhaustion extinction off +";
char text210[] = "between light and dark   *";
char text211[] = "twilight zone, obscurity *";
char text212[] = "always do your duty  and";
char text213[] = "rejoice rejoice, rejoice";
char text214[] = "crocodiles paris";//16
char text215[] = "moulin rouge :-)";

char* textPointer2[] = { 
  text20, text21, text22, text23, text24, text25, text26,
  text27, text28, text29, text210, text211, text212, text213, text214, text215 };

char text30[] = "oh god, protect me from art";
char text31[] = "curators, galeries, shows..";
char text32[] = "money, research, production";
char text33[] = "funding, awards, prestige..";
char text34[] = "invention, generation   ";
char text35[] = "new, originality, young ";
char text36[] = "warum .... etwas";
char text37[] = "und nicht nichts";//16

char text38[] = "survival strategies, self- ";
char text39[] = "contemplation gut feelings ";
char text310[] = "art is  leisure  industry  ";
char text311[] = "doubt:no excuse to produce ";
char text312[] = "what exactly = artschool";
char text313[] = "all about nowadays??????";
char text314[] = "elitist art you/I";//16
char text315[] = "system: be young";

char* textPointer3[] = { 
  text30, text31, text32, text33, text34, text35, text36,
  text37, text38, text39, text310, text311, text312, text313, text314, text315 };

char text40[] = "artist are great readers..";
char text41[] = "visitor number does not...";
char text42[] = "i am a very artist artist..";
char text43[] = "art has no diploma or phd  ";
char text44[] = "fuss over legitimacy....";
char text45[] = "new, originality, young ";
char text46[] = "did you read? NO";
char text47[] = "did you SEE, NOO";//16

char text48[] = "share knowledge experience";
char text49[] = "open source free ideas....";
char text410[] = "peaceful fight over money ";
char text411[] = "selection system curators ";
char text412[] = "interdisciplinary work  ";
char text413[] = "succes fame authority $$";
char text414[] = "feel free ahead ";//16
char text415[] = "develope being !";

char* textPointer4[] = { 
  text40, text41, text42, text43, text44, text45, text46,
  text47, text48, text49, text410, text411, text412, text413, text414, text415 };

void setup()
{
  // initialize LCD and set display size
  // LCD size 20 columns x 2 lines, small (normal) font
  srlcd1.begin(20,2);

  // Turn on backlight (if used)
  // srlcd1.backlightOn();

  // Print a message to the LCD.
  srlcd1.print("HELLO, WORLD!");

  // move to next line
  srlcd1.setCursor(0,1);
  srlcd1.print("srlcd1123 3-wire test");
  srlcd2.begin(20,2);
  srlcd2.print("HELLO, WORLD!");

  // move to next line
  srlcd2.setCursor(0,1);
  srlcd2.print("srlcd1123 3-wire test");

  // *******************************************
  srlcd0.begin(24,2);

  // Turn on backlight (if used)
  srlcd0.backlightOn();

  // Print a message to the LCD.
  srlcd0.print("HELLO, WORLD!");

  // move to next line
  srlcd0.setCursor(0,1);
  srlcd0.print("SRLCD123 3-wire test");

  // *******************************************
  srlcd5.begin(20,2);

  // Turn on backlight (if used)
  srlcd5.backlightOn();

  // Print a message to the LCD.
  srlcd5.print("HELLO, WORLD!");

  // move to next line
  srlcd5.setCursor(0,1);
  srlcd5.print("SRLCD123 3-wire test");

  // *******************************************sensors
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  digitalWrite(18, HIGH);
  digitalWrite(19, LOW);
  // *******************************************LED backlights
  pinMode(ledOn1,OUTPUT);
  pinMode(ledOn2,OUTPUT);

  digitalWrite(ledOn1, HIGH);
  digitalWrite(ledOn2, HIGH);

  Serial.begin(9600);
}

void displayText1( char* tP0, char* tP1 ){

  srlcd1.setCursor(0,0);
  srlcd1.print( tP0 );
  srlcd1.setCursor(0,1);
  srlcd1.print(tP1 );

}

void displayText2( char* tP0, char* tP1 ){

  srlcd2.setCursor(0,0);
  srlcd2.print( tP0 );
  srlcd2.setCursor(0,1);
  srlcd2.print(tP1 );

}

void displayText0( char* tP0, char* tP1 ){

  srlcd0.setCursor(0,0);
  srlcd0.print( tP0 );
  srlcd0.setCursor(0,1);
  srlcd0.print(tP1 );

}

void displayText5( char* tP0, char* tP1 ){

  srlcd5.setCursor(0,0);
  srlcd5.print( tP0 );
  srlcd5.setCursor(0,1);
  srlcd5.print(tP1 );

}

void loop()
{

  int sensor1 = analogRead(0);
  int sensor2 = analogRead(1);

  //char outByte = (char)(sensor1 / 4);
  if (sensor1 < 1024 ){
    //Serial.print((char)sensor1);
    Serial.print(counter);
    counter +=5;
  }
  else
  { 
    Serial.print(64);
  }

  if ( changeText == 0 ) { 
    changeText = 1;
  }
  else
  {
    changeText = 0; 
  }

  delay(5000);

  //  if ( sensor1 > 512 ){
  if ( changeText == 0 ){
    displayTextNumber ( 3 , 0 );
    delay(10 + sensor2);
    Serial.print(counter);
    counter++;
    delay(2000);
    displayTextNumber ( 3, 1 );
    delay(10 + sensor2);
    Serial.print(counter);
    counter ++;
  }
  else  
  {
    displayTextNumber ( 2, 0 );
    delay(10 + sensor2);
    Serial.print(counter);
    counter++;
    delay(2000);
    displayTextNumber ( 2, 1 );
    delay(10 + sensor2);
    Serial.print(counter);
    counter ++;
  }
  if ( (counter*4) > 255 ) counter = 0;
}

void displayTextNumber ( char whichNumber, char whichHalf ){
  char ** textBlockPointer;//pointer to a pointer
  if ( whichNumber == 0 ) textBlockPointer = textPointer2;
  if ( whichNumber == 1 ) textBlockPointer = textPointer2;
  if ( whichNumber == 2 ) textBlockPointer = textPointer3;
  if ( whichNumber == 3 ) textBlockPointer = textPointer4;

  if ( whichHalf == 1 ){
    displayText1( textBlockPointer[8], textBlockPointer[9]); 
    displayText2( textBlockPointer[10], textBlockPointer[11]); 
    displayText0( textBlockPointer[12], textBlockPointer[13]); 
    displayText5( textBlockPointer[14], textBlockPointer[15]);
    setLedOn(1);
  }
  else
  {
    displayText1( textBlockPointer[0], textBlockPointer[1]); 
    displayText2( textBlockPointer[2], textBlockPointer[3]); 
    displayText0( textBlockPointer[4], textBlockPointer[5]); 
    displayText5( textBlockPointer[6], textBlockPointer[7]);
    setLedOn(0);
  }
}

void setLedOn(char onOff){
  if ( onOff == 0)
  {
    digitalWrite(ledOn1, HIGH);
    digitalWrite(ledOn2, LOW);
  }
  else
  {
    digitalWrite(ledOn1, LOW);
    digitalWrite(ledOn2, HIGH);
  }

}













