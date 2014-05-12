//receiving

#include <JeeLib.h>

char ccc = 0;
char ledONOFF = 0;
char ledSet = 0;

void setup () {

  Serial.begin(9600);
  //initializing RF
  rf12_initialize('R', RF12_433MHZ, 212);

  pinMode(5, INPUT);
  digitalWrite( 5, HIGH);
  
   pinMode(4, INPUT);
  digitalWrite( 4, HIGH);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}


void loop () {
  // RFM12 loop runner

  if ( (digitalRead( 5 ) == 0) & (digitalRead( 4 ) == 0) ) {
    char* buf = (char*) rf12_data;
    char* bufToSend;
    if (rf12_recvDone() && rf12_crc == 0) {
      blink6(5, 1, 550);
      int check = 0;
      int counter = 0;
      for ( int i=0;i<9;i++)
      {
        //Serial.print(buf[i]);
        if ( check == 1){
          //bufToSend[counter] = buf[i];
          Serial.print(buf[i]);
          blink6(5, 1, 100);
          counter++;
        }

        if ( buf[i] == '#' ) {
          check = 1;
        }
      }
      // Serial.print(bufToSend);
      // Serial.flush();
      //  blink6(1, 550);
      delay(1);
    }
  }
  else if  ( (digitalRead( 5 ) == 0) & (digitalRead( 4 ) == 1) )
  {

    char code[] = { 
      B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000                                           };

    for ( int i2=0;i2<8*8;i2++){
      int k = i2 + 10*sin((0.0+millis())/900.0) + 64;
      k = k%64;
      i2 = i2%64;
      if ( (i2/8 ) == ( k / 8))
        code[i2/8] = (B00000001<<(i2%8)) | (B00000001<<(k%8));

      if ( (i2/8 ) != ( k / 8)){
        code[i2/8] = (B00000001<<(i2%8));
        code[k/8] = (B00000001<<(k%8));
      }

      for ( int i3=0;i3<8;i3++){
        code[i3] = code[i3];
        Serial.print( code[i3] );
      }
      code[i2/8] = B00000000;
      code[k/8] = B00000000;

      delay(50);
    }




    /*
    char code[] = { 
     B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000                                   };
     
     while ( 1 ==1 ){
     int i=0;
     int k = 1;
     code[i] = B00000001;
     for ( int i2=0;i2<8*8;i2++){
     for ( int i3=0;i3<8;i3++){
     code[i3] = code[i3];
     Serial.print( code[i3] );
     }
     code[i] = code[i]<<1;
     delay(50);
     if ( ((k++)%8) == 0 )
     {
     code[i] = B00000000;
     i++;
     code[i] = B00000001;
     }
     }
     }
     
     */


    /*
    char code[] = { 
     B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000                           };
     
     for ( int i2=0;i2<8;i2++){
     code[i2] = B00000001;
     for ( int i=0;i<8;i++){
     code[i2] = code[i2]<<1;
     for ( int i3=0;i3<8;i3++){
     Serial.print( code[i3]   );
     delay(5);
     }
     }
     }
     */


  else if  ( (digitalRead( 5 ) == 1) & (digitalRead( 4 ) == 0) )
  {
    char code[] = { 
     B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000           };
     for ( int i2=0;i2<8;i2++){
     blink6(6, i2, 50);
     code[i2] = B11111111;
     for ( int i=0;i<8;i++){
     Serial.print(code[i]   );
     delay(10);
     }
     code[i2] = B00000000;
     delay(100);
     }
     */

  }
   else if  ( (digitalRead( 5 ) == 1) & (digitalRead( 4 ) == 1) )
  {
    
    
  }
}

void blink6(int whichLed, int notimes, int lengthblink){
  for ( int i=0;i<notimes;i++)
  {
    digitalWrite(whichLed, HIGH);
    delay(lengthblink);
    digitalWrite(whichLed, LOW);
    delay(lengthblink);
  }
}

















