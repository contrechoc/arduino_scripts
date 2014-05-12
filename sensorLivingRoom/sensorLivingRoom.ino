#include <JeeLib.h>
#include <avr/eeprom.h>

#include <Wire.h>

int expander = B1101000;

#define secondsRegister   0x00
#define minutesRegister   0x01
#define hoursRegister     0x02
#define dayRegister       0x03
#define dateRegister      0x04
#define monthRegister     0x05
#define yearRegister      0x06
#define controlRegister   0x07

long timer = millis();
long interval = 10*1000;
long intervalShort = 1000;

long timer2 = millis();
long interval2 = 1000;

long timer3 = millis();

//int lightOn = 0;

int counter = 0;
int an1 = 800;
 int myTime;

#define DHT11_PIN 3      //  on leonardo 
byte dht11_dat[5];


static byte bin2bcd (byte val) {
  return val + 6 * (val / 10);
}

static byte bcd2bin (byte val) {
  return val - 6 * (val >> 4);
}


unsigned char sensorArray[] = { 
  1,1,1,1 };

int teller = 0;

void setup() {

  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  Wire.begin();
  settingTime();

  rf12_initialize(1, RF12_915MHZ, 212);

  char buf[] = { 
    "hello data!      "                     };
  while (!rf12_canSend()) rf12_recvDone(); 
  rf12_sendStart(0, buf, sizeof buf);

  //relays
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  //PIR
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);

  // LED
  pinMode (8, OUTPUT);

  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);

  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);

  dht11_setup();

}



void blinker(int blNumber){
  for (int i =0; i< blNumber;i++){
    digitalWrite(8, LOW);
    delay(100);
    digitalWrite(8, HIGH);
    delay(100);
  }
}

void sendAway( char *buff ){

  while ((!rf12_canSend()) )
    rf12_recvDone();
  rf12_sendStart(0, buff, 10, 0);
  Serial.println( buff);
  delay(103);
  ;

}

void loop() {

  if ( timer2 < millis()){
    timer2 = millis() + interval2;
    blinker (1);
  }

  if ( timer < millis()){

    if ( counter++ > 10 )
      timer = millis() + interval;
    else
      timer = millis() + intervalShort;

     myTime =  doClock()/100;
    int myMinutes = doClock() - myTime*100;
    if (  myTime > 23 ) Serial.println("uit");

    int pirThing = 0;
    if ( digitalRead(4) == LOW){
      Serial.println("PIR_LOW");

    }
    else
    {
      Serial.println("PIR_HIGH");
      pirThing = 1;
    }
    char buf3[] = { 
      "aPIR      "                 };
    buf3[6] = 0 + 48;
    buf3[7] = pirThing + 48;
    buf3[8] = 0 + 48;
    buf3[9] = 0 + 48;
    sendAway( buf3 );

    Serial.println(buf3);

    //___________________light_____________________________


     an1 = analogRead(0);
    int an1_0 = an1/1000;
    int an1_1 = (an1 - an1_0*1000)/100;
    int an1_2 = (an1 - an1_0*1000- an1_1*100 )/10;
    int an1_3 = (an1 - an1_0*1000- an1_1*100- an1_2*10 );

    char buf[] = { 
      "blight      "                                   };
    buf[6] = an1_0 + 48;
    buf[7] = an1_1 + 48;
    buf[8] = an1_2 + 48;
    buf[9] = an1_3 + 48;

    sendAway( buf );

    //___________________humidity_____________________________

    loop_dht11_dat();
    char buf22[] = { 
      "rhumi      "            };

    int an22 = dht11_dat[0]*10 + dht11_dat[1]*10;
    int an22_0 = an22/1000;
    int an22_1 = (an22 - an22_0*1000)/100;
    int an22_2 = (an22 - an22_0*1000- an22_1*100 )/10;
    int an22_3 = (an22 - an22_0*1000- an22_1*100- an22_2*10 );
    //  Serial.println("temp " + an2);

    buf22[6] = an22_0 + 48;
    buf22[7] = an22_1 + 48;
    buf22[8] = an22_2 + 48;
    buf22[9] = an22_3 + 48;

    sendAway( buf22 );

    char buf33[] = { 
      "ttemp      "            };

    int an33 = dht11_dat[2]*10 + dht11_dat[3]*10;
    int an33_0 = an33/1000;
    int an33_1 = (an33 - an33_0*1000)/100;
    int an33_2 = (an33 - an33_0*1000- an33_1*100 )/10;
    int an33_3 = (an33 - an33_0*1000- an33_1*100- an33_2*10 );
    //  Serial.println("temp " + an2);

    buf33[6] = an33_0 + 48;
    buf33[7] = an33_1 + 48;
    buf33[8] = an33_2 + 48;
    buf33[9] = an33_3 + 48;

    sendAway( buf33 );



    //___________________temperature_____________________________


    char buf2[] = { 
      "ctemp      "            };

    int an2 = 1024 - analogRead(2);
    // an2 = 10 * ( an2 - 444 ) * 100 / ( 607 - 444 );
    int an2_0 = an2/1000;
    int an2_1 = (an2 - an2_0*1000)/100;
    int an2_2 = (an2 - an2_0*1000- an2_1*100 )/10;
    int an2_3 = (an2 - an2_0*1000- an2_1*100- an2_2*10 );
    //  Serial.println("temp " + an2);

    buf2[6] = an2_0 + 48;
    buf2[7] = an2_1 + 48;
    buf2[8] = an2_2 + 48;
    buf2[9] = an2_3 + 48;

    sendAway( buf2 );

    //___________________time_____________________________

    char buf6[] = { 
      "dtime             "             };

    buf6[6] = myTime/10 + 48;
    buf6[7] = myTime - 10*int(myTime/10) + 48;
    buf6[8] = myMinutes/10 + 48;
    buf6[9] = myMinutes - 10*int(myMinutes/10) + 48;

    sendAway( buf6 );

    //___________________pollution MG 6 sensor______________________________
/*
    int an3 = analogRead(4);
    Serial.println(an3);
    int an3_0 = an3/1000;
    int an3_1 = (an3 - an3_0*1000)/100;
    int an3_2 = (an3 - an3_0*1000- an3_1*100 )/10;
    int an3_3 = (an3 - an3_0*1000- an3_1*100- an3_2*10 );

    char buf7[] = { 
      "qpoll        "                    };
    buf7[6] = an3_0 + 48;
    buf7[7] = an3_1 + 48;
    buf7[8] = an3_2 + 48;
    buf7[9] = an3_3 + 48;

    sendAway( buf7)*/
  }
    //___________________light on off______________________________
if ( timer3 < millis() ){
    if ((an1 < 900) && (myTime < 24) && (myTime > 6) ) 
    {
      digitalWrite(6, HIGH);
      // lightOn = 1;
      digitalWrite(7, HIGH);
      timer3 = millis() + 5*60*1000;
    }
    else {
      digitalWrite(6, LOW);
      // lightOn = 0;
      digitalWrite(7, LOW);
       timer3 = millis() + 1*60*1000;
    }
    delay(10);

    //// Serial.println("send");
  }

  /*
    char buf7[] = { 
   "received      "               };
   
   while (!rf12_canSend()){
   rf12_recvDone(); 
   delay(10);
   Serial.println("RF12 looping");
   }
   rf12_sendStart(0, buf7, sizeof buf7); 
   delay(50);
   */
  /*
    if ( lightOn == 0){
   digitalWrite(6, HIGH);
   lightOn = 1;
   }
   else{
   digitalWrite(6, LOW);
   lightOn = 1;
   }
   */


}

int doClock(){


  expanderSetPointer( hoursRegister );
  byte reading = expanderRead();
  Serial.print  ( " hours: " );
  int hours = 0 + bcd2bin(reading);
  Serial.print(hours , DEC);
  Serial.print(" ");

  expanderSetPointer( minutesRegister );
  reading = expanderRead();
  Serial.print  ( " min: " );
  byte minutes = (reading&B00001111)  + 10.0*((reading>>4) & B00001111) ;
  Serial.print(minutes, DEC);      

  expanderSetPointer( secondsRegister );
  reading = expanderRead();
  Serial.print  ( " sec: " );
  Serial.print(( reading&B00001111) + 10*((reading>>4)&B00001111), DEC);

  expanderSetPointer( dateRegister );
  reading = expanderRead(); 
  Serial.print  ( " date: " );
  Serial.print((reading&B00001111) + 10*((reading>>4)&B00000011), DEC);

  expanderSetPointer( monthRegister );
  reading = expanderRead(); 
  Serial.print  ( " month: " );
  Serial.print((reading&B00001111) + 10*((reading>>4)&B00000011), DEC);

  expanderSetPointer( yearRegister );
  reading = expanderRead(); 
  Serial.print  ( " year: " );
  Serial.print((reading&B00001111) + 10*((reading>>4)&B00001111), DEC);

  Serial.println  ( " " );
  return hours*100 + 1*minutes;
}


void setMode(unsigned char whichMode){

  expanderSetPointer( 0x02 );
  byte reading = expanderRead();

  if ( whichMode == '0' ) reading &= ~(1<<6);
  if ( whichMode == '1' ) reading |= (1<<6);

  if ( whichMode == '2' ) reading |= (1<<5);
  if ( whichMode == '3' ) reading &= ~(1<<5);

  if ( whichMode == '4' )reading |= (1<<4);
  if ( whichMode == '5' ) reading &= ~(1<<4);

  expanderWrite(hoursRegister ,  reading );

}

void expanderWrite(byte reg, byte _data ) {
  Wire.beginTransmission(expander);
  Wire.write(reg);
  Wire.write(_data);
  Wire.endTransmission();
}

void expanderSetPointer(byte reg    ) {
  Wire.beginTransmission(expander);
  Wire.write(reg);
  Wire.endTransmission();
}

byte expanderRead() {
  byte _data;
  Wire.requestFrom(expander, 1);
  if(Wire.available()) {
    _data = Wire.read();
  }
  //Serial.print('.');
  return _data;
}

void settingTime(){

  expanderWrite(secondsRegister ,  B00000000 + bin2bcd(57) );//seconds
  expanderWrite(minutesRegister ,  bin2bcd (25) );//minutes
  expanderWrite(hoursRegister ,  bin2bcd (20) );//hour 24 houres
  expanderWrite(dayRegister ,  bin2bcd (1) );  //day
  expanderWrite(dateRegister ,  bin2bcd (14) );  //date
  expanderWrite(monthRegister ,  bin2bcd (12) );   //month
  expanderWrite(yearRegister ,  bin2bcd (12) ); //year

}













