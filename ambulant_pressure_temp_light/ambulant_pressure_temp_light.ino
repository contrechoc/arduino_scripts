/// @dir heading_demo
/// Demo of the Heading Board based on the HDPM01 (with barometer and compass).
// 2010-03-22 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>
#include <OneWire.h>

HeadingBoard sensor (4);

int DS18S20_Pin = 16; //DS18S20 Signal pin on digital 2

OneWire ds(DS18S20_Pin); // 

long timer2 = 0;
long interval2 = 2000;

int pSensorCounter = 0;

int alternator = 0;

void setup () {

  Serial.begin(9600);
  Serial.println("radio start");

  // mySerial.begin(9600);

  rf12_initialize(1, RF12_915MHZ, 212);

  sensor.begin();
  pinMode(14, OUTPUT);
 // pinMode(15, INPUT);
}

char putInBuffer1( char *buf, char *message ){
  while( *(message++) )
  {
    *(buf++) = *(message++); 
  }
}

void blink(){
  digitalWrite(14, LOW );
  delay(50);
  digitalWrite(14, HIGH );
  delay(50);
}

void loop () {


  if (timer2 < millis()) {
    timer2 = millis() + interval2;
    digitalWrite( 14, HIGH );
    delay(50);
    digitalWrite( 14, LOW );
    delay(100);

    sendSensor(( pSensorCounter++) % 3 );
    delay(100);


    loopPrint () ;
  }
}

void sendSensor(int whichSensor){

  int an3 = 0;

  //ldr
  if ( whichSensor == 0 ){
    an3 = 1024 - analogRead( 1 );
    Serial.println(an3);
  }

  if ( whichSensor == 1 ){
    float temperature = getTemp();

    an3 = (int) (temperature*10);
    Serial.println(an3);
  }

  if ( whichSensor == 2 ){ //air pressure
    struct { 
      int temp, pres, xaxis, yaxis; 
    } 
    payload;

    sensor.pressure(payload.temp, payload.pres);
    //sensor.heading(payload.xaxis, payload.yaxis);
    an3 = payload.temp;
    Serial.println(an3);
    an3 = payload.pres/10;
    Serial.println(an3);
    //alternating between tem and press
    if ( alternator == 0){
       an3 = payload.pres/10;
       alternator = 1;
    }else{
       an3 = payload.temp;
       whichSensor = 3;
       alternator = 0;
    }
  }

  int an3_0 = an3/1000;
  int an3_1 = (an3 - an3_0*1000)/100;
  int an3_2 = (an3 - an3_0*1000- an3_1*100 )/10;
  int an3_3 = (an3 - an3_0*1000- an3_1*100- an3_2*10 );

  char buf7[] = {  
    "xpoll        "                          };

// whichSensor = 3 means the temp2
  if ( whichSensor == 0 )  buf7[0] = 'z'; //LDR sleeping room
  if ( whichSensor == 1 )  buf7[0] = 's'; //temp chip
  if ( whichSensor == 2 )  buf7[0] = 'g'; //press chip
  if ( whichSensor == 3 )  buf7[0] = 'f'; //temp2 chip

  buf7[6] = an3_0 + 48;
  buf7[7] = an3_1 + 48;
  buf7[8] = an3_2 + 48;
  buf7[9] = an3_3 + 48;

  Serial.print(whichSensor);
  Serial.print("\t");
  Serial.println(buf7);

  while ((!rf12_canSend()) )
    rf12_recvDone();
  rf12_sendStart(0, buf7, 10, 0);

  delay(10);


}


void loopPrint () {

  struct { 
    int temp, pres, xaxis, yaxis; 
  } 
  payload;

  sensor.pressure(payload.temp, payload.pres);
  sensor.heading(payload.xaxis, payload.yaxis);

  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &payload, sizeof payload, 2);

  Serial.print("HDPM ");
  Serial.print(payload.temp);
  Serial.print(' ');
  Serial.print(payload.pres);
  Serial.print(' ');
  Serial.print(payload.xaxis);
  Serial.print(' ');
  Serial.print(payload.yaxis);

  Serial.print(' ');
  float temperature = getTemp();
  Serial.print(temperature);
  Serial.print(' ');
  Serial.println(1024 - analogRead(1));

}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;


  return TemperatureSum;

}

