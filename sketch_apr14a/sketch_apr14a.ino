#include <JeeLib.h>


 
 
void setup(){
  
   rf12_initialize('R', RF12_433MHZ, 212);
   Serial.begin(9600);
}

void loop(){
  
   char* buf = (char*) rf12_data;
  if (rf12_recvDone() && rf12_crc == 0) {
    for (int i = 0; i<10;i++)
    Serial.print( buf[i] );
    Serial.println("");
    delay(500);
  }
  delay(1);
  
}
