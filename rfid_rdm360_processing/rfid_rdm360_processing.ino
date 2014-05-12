  

#include <SoftwareSerial.h>

#define rxPin 5 ////here the one pin
#define txPin 6

// Create a software serial object for the connection to the RFID module
SoftwareSerial rfid = SoftwareSerial( rxPin, txPin );

unsigned char action = 0;
int incomingByte = 0; // To store incoming serial data

void setup() {

  Serial.begin(9600); // Serial port for connection to host
  rfid.begin(9600); // Serial port for connection to RFID module

  //
  Serial.println("RFID reader starting up");
  
  Serial.flush(); 
  
  pinMode(2,INPUT);
  attachInterrupt(0, active, RISING);
 
}



/**

* Loop

*/

void loop() {

  if (action == 1) 
  {
    action = 0;
    
  byte i = 0;
  byte val = 0;
  byte checksum = 0;
  byte bytesRead = 0;
  byte tempByte = 0;
  byte tagBytes[6]; // "Unique" tags are only 5 bytes but we need an extra byte for the checksum
  char tagValue[10];

Serial.print("Tag read --->");
val = rfid.read();
  if((val) == 2) { // Check for header

      readTag();
  }
  else
  {
    Serial.println( val  , HEX );
  }
  
  }

}

void readTag(){
  
  byte i = 0;
  byte val = 0;
  byte checksum = 0;
  byte bytesRead = 0;
  byte tempByte = 0;
  byte tagBytes[6]; // "Unique" tags are only 5 bytes but we need an extra byte for the checksu
 char tagValue[10];
  char tagString[] = "012345678901234567890";
  
  //Serial.println("Tag read --->");
    bytesRead = 0;
    while (bytesRead < 12) { // Read 10 digit code + 2 digit checksum
    //Serial.print("Tag reading ");
     // Serial.println(bytesRead, DEC);
      val = rfid.read();
      if (bytesRead < 10) tagValue[bytesRead] = val;
      if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) {
       Serial.print("Tag reading stopped......");
       break; // Stop reading
      }
      // Ascii/Hex conversion:
      if ((val >= '0') && (val <= '9'))  val = val - '0';
      else if ((val >= 'A') && (val <= 'F')) val = 10 + val - 'A';
      // Every two hex-digits, add a byte to the code:
      if (bytesRead & 1 == 1) {
        // Make space for this hex-digit by shifting the previous digit 4 bits to the left
        tagBytes[bytesRead >> 1] = (val | (tempByte << 4));
        if (bytesRead >> 1 != 5) { // If we're at the checksum byte,
          checksum ^= tagBytes[bytesRead >> 1]; // Calculate the checksum... (XOR)
        }
      } else {
        tempByte = val; // Store the afirst hex digit first
      };
      bytesRead++; // Ready to read next digit
    }
    
     Serial.println("Tag read fin");
     Serial.println(bytesRead, DEC);
     
    // Send the result to the host connected via USB
    if (bytesRead == 12) { // 12 digit read is complete
      tagValue[10] = '\0'; // Null-terminate the string
      Serial.print("Tag read: ");
      
      for (i=0; i<5; i++) {
        // Add a leading 0 to pad out values below 16
        if (tagBytes[i] < 16) {
          Serial.print("0");
        }
        
      Serial.print(tagBytes[i], HEX);
      
      Serial.print("*");
        
      // tagString[i] = tagBytes[i] + 128 + 48;
      }
    
       delay(100);

      Serial.println("tag executed");
   }
    bytesRead = 0;
 

}


void active()
{
   Serial.println( "irq" );
   action = 1;
}
 
