// sending words for the presenter

//  

//
//connections: (these PIN numbers must not be changed...this is hardware stuff using interrupt INT0 and SPI (MOSI, MISO, CLK, SS) which have fixed PIN uUmbers
/*
PIN connections RF12 module
13
12
11
10 from left side of module

2 for the interrupt
*/


#include <RF12.h>

void setup() {
 
	rf12_initialize(1, RF12_868MHZ, 212);

        Serial.begin(9600);
        Serial.println(":-) presenter!!! :-)")'
        Serial.println("starting up sending & receiving:");
}

 

void loop() {
  
	delay(500);
        Serial.println("send \t");
        
         char bufSend[] = { "goodbye love!" };
         while (!rf12_canSend()) rf12_recvDone(); rf12_sendStart(0, buf, sizeof buf);
         
         
	delay(500);
        Serial.print("receive? \t");
        char* bufReceivePtr = (char*) rf12_data;
	if (rf12_recvDone() && rf12_crc == 0) {
        Serial.print ( bufReceivePtr );
   
}
