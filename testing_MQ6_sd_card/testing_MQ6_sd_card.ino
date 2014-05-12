#include "FileLogger.h"
#include <OneWire.h>
// define the pin that powers up the SD card
#define MEM_PW 8

#define MESSAGE "time       val       temp                  \r\n"
unsigned long length = sizeof(MESSAGE)-1;
byte buf7[] = MESSAGE;

int DS18S20_Pin = 5; //DS18S20 Signal pin on digital 2
int DS18S20_PinV = 6;
int DS18S20_PinGND = 7;

//Temperature chip i/o
OneWire ds(DS18S20_Pin); // on digital pin 2

void setup(){
  
   pinMode(MEM_PW, OUTPUT);
  digitalWrite(MEM_PW, HIGH);
  
  Serial.begin(9600);
  
    pinMode(DS18S20_PinV, OUTPUT);
  pinMode(DS18S20_PinGND, OUTPUT);
  digitalWrite( DS18S20_PinGND, LOW);
digitalWrite( DS18S20_PinV, HIGH);
  
}

void loop(){
  
  delay(250);
  
    int an1 = analogRead(0);
    int an1_0 = an1/1000;
    int an1_1 = (an1 - an1_0*1000)/100;
    int an1_2 = (an1 - an1_0*1000- an1_1*100 )/10;
    int an1_3 = (an1 - an1_0*1000- an1_1*100- an1_2*10 );
    
    
  buf7[15] = an1_0 + 48;
  buf7[16] = an1_1 + 48;
  buf7[17] = an1_2 + 48;
  buf7[18] = an1_3 + 48;
  
  long time = millis();
  int an2 = time/1000;
  
   int an2_0 = an2/1000;
    int an2_1 = (an2 - an2_0*1000)/100;
    int an2_2 = (an2 - an2_0*1000- an2_1*100 )/10;
    int an2_3 = (an2 - an2_0*1000- an2_1*100- an2_2*10 );
    
  buf7[6] = an2_0 + 48;
  buf7[7] = an2_1 + 48;
  buf7[8] = an2_2 + 48;
  buf7[9] = an2_3 + 48; 
  
      float temperature = getTemp();
  
   int an3 = (int) (temperature*10);


  int an3_0 = an3/1000;
  int an3_1 = (an3 - an3_0*1000)/100;
  int an3_2 = (an3 - an3_0*1000- an3_1*100 )/10;
  int an3_3 = (an3 - an3_0*1000- an3_1*100- an3_2*10 );
  
  buf7[26] = an3_0 + 48;
  buf7[27] = an3_1 + 48;
  buf7[28] = an3_2 + 48;
  buf7[29] = an3_3 + 48; 
  
    int result = FileLogger::append("data.log", buf7, length);

     if( result == 0) {
          Serial.println("OK");
           
        } else if( result == 1) {
          Serial.println("Fail initializing");
           
        } else if( result == 2) {
          Serial.println("Fail appending");
           
        }
for(int i = 0; i<30; i++)
  Serial.print( buf7[i] );
  
  Serial.print( "\t" );
    Serial.print( time );
    Serial.print( "\t" );
    Serial.print( an1 );
      Serial.print( "\t" );
      Serial.println(an3);
   Serial.println("");
  
  
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





