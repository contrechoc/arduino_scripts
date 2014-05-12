/* 
 RFM70 - adaptation of the HopeRF example 
 to my RFM70 library and the Arduino platform
   
 Created 30 Dec. 2011
 by Wouter van Ooijen
*/

#include "Arduino.h"
#include <rfm70.h>

pins::uc_pin ce( 8 ), csn(  10 ), mosi( 11 ), miso( 12 ), sck( 5 );
pins::uc_pin led_tx( 13 ), led_rx( 9 );
rfm70 radio( sck, mosi, miso, csn, ce );

const unsigned char tx_buf[17]={
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x78};
    
void setup(){
Serial.begin(9600);
}

void loop() {
   led_tx.direction_set_output();
   led_rx.direction_set_output();
   radio.init();
   radio.channel( 10 );
   
   if( ! radio.is_present() ) for(;;){
      delay( 300 );
      led_rx.set( 0 );      
      led_tx.set( 1 );      
      delay( 300 );
      led_rx.set( 1 );      
      led_tx.set( 0 );      
   }
  
   unsigned char len,i,chksum, n = 0;
   unsigned char rx_buf[ RFM70_MAX_PACKET_LEN ];
   unsigned char transmit_led = 0, receive_led = 0;   
   
   for(;;){
      delay( 4 );

      led_tx.set( transmit_led > 0 );
      if( transmit_led > 0 ){ transmit_led--; }
 
      led_rx.set( receive_led > 0 );
      if( receive_led > 0 ){ receive_led--; }       
   
      if( ++n > 250 ){
         radio.mode_transmit();
         transmit_led = 20;	
         radio.buffer_write( RFM70_CMD_W_TX_PAYLOAD_NOACK, tx_buf, 17 ); 
         Serial.println("send");
         delay( 1 );
         radio.mode_receive(); 
         n = 0; 	
      }

      if( ! radio.receive_fifo_empty() ){

         len=radio.register_read( RFM70_CMD_R_RX_PL_WID );
         if( len <= RFM70_MAX_PACKET_LEN ){
            radio.buffer_read( RFM70_CMD_R_RX_PAYLOAD,rx_buf,len );
            
             Serial.println("receive");
             unsigned char *bufPtr = rx_buf;
             for (int i = 0;i<len;i++)
               Serial.print(*bufPtr++);
               
   	    chksum = 0;
            for(i=0;i<16;i++){
               chksum +=rx_buf[i]; 
            }

            if(chksum==rx_buf[16]&&rx_buf[0]==0x30){
               receive_led = 20;
            }	
         }
         radio.register_write( RFM70_CMD_FLUSH_RX,0 );
      }         
   }
}

