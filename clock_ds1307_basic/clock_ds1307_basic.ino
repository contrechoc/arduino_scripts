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

  char myNumber[] = {
  10 , 11, 12, 13, 13, 0, 10, 10};

  char myNumber2[] = {
  42 , 10, 42, 10, 42, 10, 42, 42};
  
    char hello[] = {
  18, 15, 21, 21, 22, 42, 42, 42};
  


volatile int ledCounter = 0;
volatile int ledCounter2 = 0;

int tCounter = 0;
unsigned char tCounter2 = 0;

volatile int timerForSet = 0;

volatile unsigned char settingMode = 0;
unsigned char onOff = 0;
unsigned char setTimeComponent = 0;

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

  //  Serial.begin(57600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  rtc.adjust(DateTime(__DATE__, __TIME__));

  noInterrupts();           // disable all interrupts

    TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 5; //10 - 200    // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  //----------------------
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0  = 0;

  OCR0A = 50;            // compare match register 16MHz/256/2Hz
  TCCR0B |= (1 << WGM02);   // CTC mode
  TCCR0B |= (1 << CS01);    // 256 prescaler 
  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt

  interrupts();             // enable all interrupts
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

  letterTransfer(num);
  //SPI.transfer(myNumber[ledCounter%8]);

  digitalWrite(slaveSelectPin,HIGH); 

}

void loop() {

  if ( settingMode == 0){// *********************************************************************clock running mode

    DateTime now = rtc.now();

    int nowMonth =  now.month();
    int nowHour = now.hour();

    myNumber[2] = nowHour/10;
    myNumber[3] = nowHour % 10;

    int nowDay = now.day();

    myNumber[0] = nowDay/10;
    myNumber[1] = nowDay % 10;

    myNumber[4] = now.minute()/10;
    myNumber[5] = now.minute() % 10;

    myNumber[6] = now.second()/10;
    myNumber[7] = now.second() % 10;

    delay(100);

    tCounter++;

    //dimming or brightening
    if ( tCounter%100 == 0 ) {
      int hhh = analogRead(0)/4;
      if ( hhh < 150 ) hhh = 0; 
      else if ( hhh < 175 ) hhh = 10;
      else if ( hhh < 200 ) hhh = 20; 
      else if ( hhh < 250 ) hhh = 50;

      noInterrupts();
      OCR1A = 10 + hhh ;
      interrupts();

    }

 

    if ( digitalRead( 8) == LOW) { //changing the mode

      for (int i = 0; i<32000; i++ )
        int h = digitalRead( 8);

      settingMode = 1;
      setTimeComponent = 0;
    }
 
    
  }
  else if ( settingMode == 1) { // *********************************************************************setting mode
    //Serial.print("setting ");
    // Serial.println(timerForSet);
    if ( digitalRead( 9) == LOW) { //changing the digit

      for (int i = 0; i<32000; i++ )
        int h = digitalRead( 9);//debounce, no delay available
      timeAdjust( setTimeComponent);
    }

    if ( digitalRead( 8) == LOW) { //shifting to the next digit
      for (int i = 0; i<32000; i++ )
        int h = digitalRead( 8);//debounce, no delay available

      setTimeComponent++;
      if ( setTimeComponent > 3 ) 
      {
        setTimeComponent = 0;
        settingMode = 0;
      }
      Serial.println(setTimeComponent);
    }

    if ( timerForSet > 5000){
      // Serial.println("ping");
      timerForSet = 0;
      //setTimeComponent = (++setTimeComponent)%4;
      if ( onOff == 0 ){
        onOff = 1;
        setUpSelect(setTimeComponent, onOff );
      }
      else{
        onOff = 0;
        setUpSelect(setTimeComponent, onOff );
      }
    }
  }
 
}

 

void timeAdjust( unsigned char whichDigit ){
  DateTime now = rtc.now();
  DateTime myNewTime;
  unsigned char  lengthMonth = 30;
  //correcting adjusting time, only for day, hour minutes not seconds
  if ( whichDigit == 0)
  {
    unsigned char nowMonth = now.month();
    if ( (nowMonth == 1)||(nowMonth == 3)||(nowMonth == 5)||(nowMonth == 7)||(nowMonth == 8)||(nowMonth == 10)||(nowMonth == 12) )
    {
      lengthMonth = 31;
    }
    else
    {
      lengthMonth = 30;
      if (nowMonth == 2) lengthMonth = 29; //leaving out the leap year construction as this demands also the year which makes setting more complicated
     // if ( (now.year() % 4 == 0 ) && (nowMonth == 2)) lengthMonth = 29;
    }
  }

  if ( whichDigit == 0){
    unsigned char lm = now.day() + 1;
    if (lm > lengthMonth ) lm = 1;
    myNewTime = DateTime ( now.year(),now.month(),lm ,now.hour() ,now.minute(),now.second() );
  }
  else  if ( whichDigit == 1)
  {
    myNewTime = DateTime ( now.year(),now.month(),now.day() ,(now.hour() + 1)%24 ,now.minute(),now.second() );
  }
  else  if ( whichDigit == 2)
  {
    myNewTime = DateTime ( now.year(),now.month(),now.day()  ,now.hour() ,(now.minute() + 1)%60,now.second() );
  }
  else  if ( whichDigit == 3)
  {
    myNewTime = DateTime ( now.year(),now.month(),now.day()  ,now.hour() ,now.minute(),(now.second()+ 1)%60 );
  }
  rtc.adjust( myNewTime );

}

 



void setUpSelect( unsigned char whichTimeComponent, unsigned char oNoff){

  DateTime now = rtc.now();

  myNumber[0] = 42;
  myNumber[1] = 42;
  myNumber[2] = 42;
  myNumber[3] = 42;
  myNumber[4] = 42;
  myNumber[5] = 42;
  myNumber[6] = 42;
  myNumber[7] = 42;
  if ( oNoff ){
    if ( whichTimeComponent == 0 )
    {
      myNumber[0] = now.day()/10;
      myNumber[1] = now.day() % 10;
    }
    else if ( whichTimeComponent == 1 )
    {
      myNumber[2] = now.hour()/10;
      myNumber[3] = now.hour() % 10;
    }
    else if ( whichTimeComponent == 2)
    {
      myNumber[4] = now.minute()/10;
      myNumber[5] = now.minute() % 10;
    }
    else if ( whichTimeComponent == 3 ) //seconds -> not important for time (indeed for a timer function it is)
    {
      myNumber[6] = now.second()/10;
      myNumber[7] = now.second() % 10;
    }
  }
}

void letterTransfer(int num){

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

  //numbers
  if ( num == 1 )  SPI.transfer(255 - B00000110);//1
  if ( num == 2 )  SPI.transfer(255 - B01011011);//2
  if ( num == 3 )  SPI.transfer(255 - B01001111);//3
  if ( num == 4 )  SPI.transfer(255 - B01100110);//4
  if ( num == 5 )  SPI.transfer(255 - B01101101);//5
  if ( num == 6 )  SPI.transfer(255 - B01111101);//6
  if ( num == 7 )  SPI.transfer(255 - B00000111);//7 
  if ( num == 8 )  SPI.transfer(255 - B01111111);//8
  if ( num == 9 )  SPI.transfer(255 - B01101111);//9
  if ( num == 0 )  SPI.transfer(255 - B00111111);//0
  //comma
  if ( num == 10 )  SPI.transfer(255 -B10000000);//comma
  //some letters
  if ( num == 11 )  SPI.transfer(255 - B01110111);//A
  if ( num == 12 )  SPI.transfer(255 - B01111111);//B like 8
  if ( num == 13 )  SPI.transfer(255 - B00111001);//C
  if ( num == 14 )  SPI.transfer(255 - B00111101);//D small like 6 without upper stroke
  if ( num == 15 )  SPI.transfer(255 - B01111001);//E
  if ( num == 16 )  SPI.transfer(255 - B01111000);//F
  if ( num == 17 )  SPI.transfer(255 - B01101111);//small G like 9
  if ( num == 18 )  SPI.transfer(255 - B01110110);//H
  if ( num == 19 )  SPI.transfer(255 - B00000110);//I like 1
  if ( num == 20 )  SPI.transfer(255 - B00011110);//J
  // K?
  if ( num == 21 )  SPI.transfer(255 - B00111000);//L
  //M
  //N
  if ( num == 22 )  SPI.transfer(255 - B00111111);//O like 0
  if ( num == 23 )  SPI.transfer(255 - B01110011);//P
  if ( num == 24 )  SPI.transfer(255 - B10111111);//Q like 0.
  //R
  if ( num == 25 )  SPI.transfer(255 - B01101101);//S like 5
  if ( num == 26 )  SPI.transfer(255 - B01111001);//t small
  if ( num == 27 )  SPI.transfer(255 - B00111110);//U
  //V
  //W
  //X
  //Y
  //Z

  //special signs
  if ( num == 40 )  SPI.transfer(255 - B01011100);//under circle
  if ( num == 41 )  SPI.transfer(255 - B01100011);//upper circle
  if ( num == 42 )  SPI.transfer(255 - B00000000);//empty


}




















