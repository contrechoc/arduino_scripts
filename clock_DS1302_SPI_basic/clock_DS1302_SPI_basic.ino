
/**********************************************************************
 * CLOCK object contrechoc 2014, 
 * 
 * feel free to modify etc....
 * 
 * using a display of 8 7 segments numbers:
 * http://dx.com/p/8x-led-display-digital-tube-module-for-arduino-595-driver-148820#.UtuAmPaDNsQ
 * 
 * and a time chip: ds1302 (SPI), using the library of arduino.cc user "Krodal" (see tab)
 * http://dx.com/p/ds1302-real-time-clock-module-with-battery-cr2032-126453#.UtuA7_aDNsQ
 * 
 * a similar script uses the ds1307 (i2c)
 * 
 * to buttons are used to alter the settings
 * digital pin 8, 9
 * one LDR is used to dim the light at night
 * A0
 * 
 ***********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

  int myNumber[] = {
  10 , 11, 12, 13, 13, 0, 10, 10};

 // char myNumber2[] = {
  //42 , 10, 42, 10, 42, 10, 42, 42};
 
   char myNumber2[] = {
  42 , 42, 42, 42, 42, 42, 42, 42};

volatile int ledCounter = 0;
volatile int ledCounter2 = 0;

int tCounter = 0;
unsigned char tCounter2 = 0;

volatile int timerForSet = 0;

volatile unsigned char settingMode = 0;
unsigned char onOff = 0;
unsigned char setTimeComponent = 0;

unsigned char controlPIN1 = 8;
unsigned char controlPIN2 = 9;

// Set your own pins with these defines !
#define DS1302_SCLK_PIN   5    // Arduino pin for the Serial Clock
#define DS1302_IO_PIN     6    // Arduino pin for the Data I/O
#define DS1302_CE_PIN     7    // Arduino pin for the Chip Enable

// Macros to convert the bcd values of the registers to normal
// integer variables.
// The code uses seperate variables for the high byte and the low byte
// of the bcd, so these macros handle both bytes seperately.
#define bcd2bin(h,l)    (((h)*10) + (l))
#define bin2bcd_h(x)   ((x)/10)
#define bin2bcd_l(x)    ((x)%10)


// Register names.
// Since the highest bit is always '1', 
// the registers start at 0x80
// If the register is read, the lowest bit should be '1'.
#define DS1302_SECONDS           0x80
#define DS1302_MINUTES           0x82
#define DS1302_HOURS             0x84
#define DS1302_DATE              0x86
#define DS1302_MONTH             0x88
#define DS1302_DAY               0x8A
#define DS1302_YEAR              0x8C
#define DS1302_ENABLE            0x8E
#define DS1302_TRICKLE           0x90
#define DS1302_CLOCK_BURST       0xBE
#define DS1302_CLOCK_BURST_WRITE 0xBE
#define DS1302_CLOCK_BURST_READ  0xBF
#define DS1302_RAMSTART          0xC0
#define DS1302_RAMEND            0xFC
#define DS1302_RAM_BURST         0xFE
#define DS1302_RAM_BURST_WRITE   0xFE
#define DS1302_RAM_BURST_READ    0xFF



// Defines for the bits, to be able to change 
// between bit number and binary definition.
// By using the bit number, using the DS1302 
// is like programming an AVR microcontroller.
// But instead of using "(1<<X)", or "_BV(X)", 
// the Arduino "bit(X)" is used.
#define DS1302_D0 0
#define DS1302_D1 1
#define DS1302_D2 2
#define DS1302_D3 3
#define DS1302_D4 4
#define DS1302_D5 5
#define DS1302_D6 6
#define DS1302_D7 7


// Bit for reading (bit in address)
#define DS1302_READBIT DS1302_D0 // READBIT=1: read instruction

// Bit for clock (0) or ram (1) area, 
// called R/C-bit (bit in address)
#define DS1302_RC DS1302_D6

// Seconds Register
#define DS1302_CH DS1302_D7   // 1 = Clock Halt, 0 = start

// Hour Register
#define DS1302_AM_PM DS1302_D5 // 0 = AM, 1 = PM
#define DS1302_12_24 DS1302 D7 // 0 = 24 hour, 1 = 12 hour

// Enable Register
#define DS1302_WP DS1302_D7   // 1 = Write Protect, 0 = enabled

// Trickle Register
#define DS1302_ROUT0 DS1302_D0
#define DS1302_ROUT1 DS1302_D1
#define DS1302_DS0   DS1302_D2
#define DS1302_DS1   DS1302_D2
#define DS1302_TCS0  DS1302_D4
#define DS1302_TCS1  DS1302_D5
#define DS1302_TCS2  DS1302_D6
#define DS1302_TCS3  DS1302_D7


// Structure for the first 8 registers.
// These 8 bytes can be read at once with 
// the 'clock burst' command.
// Note that this structure contains an anonymous union.
// It might cause a problem on other compilers.
typedef struct ds1302_struct
{
uint8_t Seconds:
  4;      // low decimal digit 0-9
uint8_t Seconds10:
  3;    // high decimal digit 0-5
uint8_t CH:
  1;           // CH = Clock Halt
uint8_t Minutes:
  4;
uint8_t Minutes10:
  3;
uint8_t reserved1:
  1;
  union
  {
    struct
    {
uint8_t Hour:
      4;
uint8_t Hour10:
      2;
uint8_t reserved2:
      1;
uint8_t hour_12_24:
      1; // 0 for 24 hour format
    } 
    h24;
    struct
    {
uint8_t Hour:
      4;
uint8_t Hour10:
      1;
uint8_t AM_PM:
      1;      // 0 for AM, 1 for PM
uint8_t reserved2:
      1;
uint8_t hour_12_24:
      1; // 1 for 12 hour format
    } 
    h12;
  };
uint8_t Date:
  4;           // Day of month, 1 = first day
uint8_t Date10:
  2;
uint8_t reserved3:
  2;
uint8_t Month:
  4;          // Month, 1 = January
uint8_t Month10:
  1;
uint8_t reserved4:
  3;
uint8_t Day:
  3;            // Day of week, 1 = first day (any day)
uint8_t reserved5:
  5;
uint8_t Year:
  4;           // Year, 0 = year 2000
uint8_t Year10:
  4;
uint8_t reserved6:
  7;
uint8_t WP:
  1;             // WP = Write Protect
};




void setup()
{      
 
  //testing pins providing voltage to the test ds1302 board attached to the arduino
  pinMode ( 4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode ( 3, OUTPUT);
  digitalWrite(3, HIGH);

  pinMode (slaveSelectPin, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("DS1302 Real Time Clock"));
  Serial.println(F("Version 2, March 2013"));

  // initialize SPI:
  SPI.begin(); 

  pinMode ( controlPIN1, INPUT_PULLUP);
  pinMode ( controlPIN2, INPUT_PULLUP);

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

 ds1302_struct rtc;
  // Start by clearing the Write Protect bit
  // Otherwise the clock data cannot be written
  // The whole register is written, 
  // but the WP-bit is the only bit in that register.
  DS1302_write (DS1302_ENABLE, 0);

  // Disable Trickle Charger.
  DS1302_write (DS1302_TRICKLE, 0x00);

  // Remove the next define, 
  // after the right date and time are set.
 
  // Set a time and date
  // This also clears the CH (Clock Halt) bit, 
  // to start the clock.

  // Fill the structure with zeros to make 
  // any unused bits zero
  memset ((char *) &rtc, 0, sizeof(rtc));

  DateTime now = DateTime(__DATE__, __TIME__);

  rtc.Seconds    = bin2bcd_l( now.second());//seconds);
  rtc.Seconds10  = bin2bcd_h( now.second());//seconds);
  rtc.CH         = 0;      // 1 for Clock Halt, 0 to run;
  rtc.Minutes    = bin2bcd_l( now.minute());//minutes);
  rtc.Minutes10  = bin2bcd_h( now.minute());//minutes);
  // To use the 12 hour format,
  // use it like these four lines:
  //    rtc.h12.Hour   = bin2bcd_l( hours);
  //    rtc.h12.Hour10 = bin2bcd_h( hours);
  //    rtc.h12.AM_PM  = 0;     // AM = 0
  //    rtc.h12.hour_12_24 = 1; // 1 for 24 hour format
  rtc.h24.Hour   = bin2bcd_l( now.hour());//hours);
  rtc.h24.Hour10 = bin2bcd_h( now.hour());//hours);
  rtc.h24.hour_12_24 = 0; // 0 for 24 hour format
  rtc.Date       = bin2bcd_l( now.day());//dayofmonth);
  rtc.Date10     = bin2bcd_h( now.day());//dayofmonth);
  rtc.Month      = bin2bcd_l( now.month());// month);
  rtc.Month10    = bin2bcd_h( now.month());//month);
  rtc.Day        = 0;
  rtc.Year       = bin2bcd_l( now.year()-2000);//year - 2000);
  rtc.Year10     = bin2bcd_h( now.year()-2000);//);
  rtc.WP = 0;  

  // Write all clock data at once (burst mode).
  DS1302_clock_burst_write( (uint8_t *) &rtc);
 
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


void loop()
{
ds1302_struct rtc;

  if ( settingMode == 0){// *********************************************************************clock running mode

     
    char buffer[80];     // the code uses 70 characters.

    // Read all clock data at once (burst mode).
    DS1302_clock_burst_read( (uint8_t *) &rtc);

    myNumber[2] = rtc.h24.Hour10;
    myNumber[3] = rtc.h24.Hour;
    myNumber[0] = rtc.Date10;
    myNumber[1] = rtc.Date;
    myNumber[4] = rtc.Minutes10;
    myNumber[5] = rtc.Minutes;
    myNumber[6] = rtc.Seconds10;
    myNumber[7] = rtc.Seconds;

    tCounter++;

    //dimming or brightening
    if ( tCounter%500 == 100 ) {
      int hhh = analogRead(0)/4;
      if ( hhh < 150 ) hhh = 0; 
      else if ( hhh < 175 ) hhh = 5;
      else if ( hhh < 200 ) hhh = 10; 
      else if ( hhh < 250 ) hhh = 20;

      noInterrupts();
      OCR1A = 5 + hhh ;
      interrupts();

    }

 

    if ( digitalRead( controlPIN1) == LOW) { //changing the mode

      for (int i = 0; i<32000; i++ )
        int h = digitalRead( controlPIN1);

      settingMode = 1;
      setTimeComponent = 0;
    }
  }
  else {
    // *********************************************************************setting mode
    //Serial.print("setting ");
    // Serial.println(timerForSet);
    if ( digitalRead( controlPIN2) == LOW) { //changing the digit

      for (int i = 0; i<32000; i++ )
        int h = digitalRead( controlPIN2);//debounce, no delay available
        
        //get time in case it is set anwe:
         DS1302_clock_burst_read( (uint8_t *) &rtc);

      timeAdjust( setTimeComponent, rtc.Year10*10 + rtc.Year, rtc.Month10*10 + rtc.Month
      , rtc.Date10*10+ rtc.Date, rtc.h24.Hour10*10 + rtc.h24.Hour,rtc.Minutes10*10 + rtc.Minutes, rtc.Seconds10*10 + rtc.Seconds );
    }

    if ( digitalRead( controlPIN1) == LOW) { //shifting to the next digit

      for (int i = 0; i<32000; i++ )
        int h = digitalRead( controlPIN1);//debounce, no delay available

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


 

void letterTransfer(int num){

  /*
  
   -*-       0000 0001
   *-*  0010 0000  0000 0010
   -*-       0100 0000
   *-*  0001 0000  0000 0100
   -*-       0000 1000
   comma: 1000 0000
   
   */

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
  if ( num == 13 )  SPI.transfer(255 - B01111001);//C
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
  if ( num == 23 )  SPI.transfer(255 - B01110111);//P
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

void setUpSelect( unsigned char whichTimeComponent, unsigned char oNoff){

  ds1302_struct rtc;
  char buffer[80];     // the code uses 70 characters.

  // Read all clock data at once (burst mode).
  DS1302_clock_burst_read( (uint8_t *) &rtc);


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
      myNumber[0] = rtc.Date10;
      myNumber[1] = rtc.Date;
    }
    else if ( whichTimeComponent == 1 )
    {
      myNumber[2] = rtc.h24.Hour10;
      myNumber[3] = rtc.h24.Hour;
    }
    else if ( whichTimeComponent == 2)
    {
      myNumber[4] = rtc.Minutes10;
      myNumber[5] = rtc.Minutes;
    }
    else if ( whichTimeComponent == 3 ) //seconds -> not important for time (indeed for a timer function it is)
    {
      myNumber[6] = rtc.Seconds10;
      myNumber[7] = rtc.Seconds;
    }
  }
}


void timeAdjust( int whichDigit, int nowYear, int nowMonth, 
int nowDay, int nowHour, int  nowMinutes, int  nowSeconds ){
  
  DateTime myNewTime;
  
  unsigned char  lengthMonth = 30;
  //correcting adjusting time, only for day, hour minutes not seconds
  if ( whichDigit == 0)
  {
     
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
    int lm = nowDay+ 1;
    if (lm > lengthMonth ) lm = 1;
    myNewTime = DateTime ( nowYear ,nowMonth,lm ,nowHour ,nowMinutes,nowSeconds);
  }
  else  if ( whichDigit == 1)
  {
    myNewTime = DateTime ( nowYear,nowMonth,nowDay ,(nowHour + 1)%24 ,nowMinutes,nowSeconds );
  }
  else  if ( whichDigit == 2)
  {
    myNewTime = DateTime ( nowYear, nowMonth, nowDay  ,nowHour , (nowMinutes + 1)%60 , nowSeconds );
  }
  else  if ( whichDigit == 3)
  {
    myNewTime = DateTime ( nowYear,nowMonth,nowDay  ,nowHour ,nowMinutes, (nowSeconds+ 1)%60 );
  }
 
  //use write like in the setup:
  
  ds1302_struct rtc;
  // Start by clearing the Write Protect bit
  // Otherwise the clock data cannot be written
  // The whole register is written, 
  // but the WP-bit is the only bit in that register.
  DS1302_write (DS1302_ENABLE, 0);

  // Disable Trickle Charger.
  DS1302_write (DS1302_TRICKLE, 0x00);

  // Fill the structure with zeros to make 
  // any unused bits zero
  memset ((char *) &rtc, 0, sizeof(rtc));

cli();

  rtc.Seconds    = bin2bcd_l( myNewTime.second());//seconds);
  rtc.Seconds10  = bin2bcd_h( myNewTime.second());//seconds);
  rtc.CH         = 0;      // 1 for Clock Halt, 0 to run;
  rtc.Minutes    = bin2bcd_l( myNewTime.minute());//minutes);
  rtc.Minutes10  = bin2bcd_h( myNewTime.minute());//minutes);
  // To use the 12 hour format,
  // use it like these four lines:
  //    rtc.h12.Hour   = bin2bcd_l( hours);
  //    rtc.h12.Hour10 = bin2bcd_h( hours);
  //    rtc.h12.AM_PM  = 0;     // AM = 0
  //    rtc.h12.hour_12_24 = 1; // 1 for 24 hour format
  rtc.h24.Hour   = bin2bcd_l( myNewTime.hour());//hours);
  rtc.h24.Hour10 = bin2bcd_h( myNewTime.hour());//hours);
  rtc.h24.hour_12_24 = 0; // 0 for 24 hour format
  rtc.Date       = bin2bcd_l( myNewTime.day());//dayofmonth);
  rtc.Date10     = bin2bcd_h( myNewTime.day());//dayofmonth);
  rtc.Month      = bin2bcd_l( myNewTime.month());// month);
  rtc.Month10    = bin2bcd_h( myNewTime.month());//month);
  rtc.Day        = 0;//not used
  rtc.Year       = bin2bcd_l( (myNewTime.year()-2000));//year - 2000);
  rtc.Year10     = bin2bcd_h( (myNewTime.year()-2000));//);
  rtc.WP = 0;  

  // Write all clock data at once (burst mode).
  DS1302_clock_burst_write( (uint8_t *) &rtc);
 
 sei();
 
  Serial.println("writing new time");

}







