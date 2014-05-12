 
#ifndef RFID
#define RFID
 
#define DATA0 0x01 //Data pin 0 from RFID module
#define DATA1 0x02 //Data pin 1 from RFID module
#define D_PORT PORTB
#define D_DIR  DDRB
#define D_IN   PINB
 
#endif
 


void setup()
{
  Serial.begin(115200);
  D_DIR&=~(DATA0+DATA1);
}
void loop()
{
  unsigned char recieve_count=0;
  unsigned char card_num[4]={ 0,0,0,0  };
  
  for(;;)
  {
    unsigned char data0=0,data1=0;
    if(D_IN&DATA0)  //DATA0 incoming signal
    {
      data0=1;
    }
    if(D_IN&DATA1)  //DATA1 incoming signal
    {
      data1=1;
    }
    //--------------------------------------------    
    if(data0!=data1)  // card detected
    {
      recieve_count++;
      if(recieve_count==1) //drop even bit
      {
      }
      else if(recieve_count<10)// card data group 1
      {
        if(!data1)
        {
          card_num[0]|=(1<<(9-recieve_count));
        }
      }
      else if(recieve_count<18)// card data group 2
      {
        if(!data1)
        {
          card_num[1]|=(1<<(17-recieve_count));
        }
      }
      if(!data1) // card data group 3
      {		
        card_num[2]|=(1<<(25-recieve_count));
      }
      delayMicroseconds(80);  //Data impulse width delay 80us
    }
    else    // no card incoming or finish reading card
    {
      unsigned char i=0;
      if(recieve_count>= 25)  //output card number
      {
        recieve_count = 0; //reset flag
        for(i=0;i<4;i++)
        {			
          Serial.print(card_num[i],DEC);
          card_num[i]=0; //reset card_number array
        }
        Serial.println(" output debug value");// output debug value
      }
      //----------------------------------------------------      
    }
  }
}

 



