

/* 
 *  
 
 Use the I2C bus with EEPROMof  2K 
 
 the address is only 1 int
 
 the amount written to one page? address can be only 8 bytes (i2c_eeprom_write_page)
 
 so the data are to be fed to the eeprom in 8 bytes
 
 *   
 */

#include <Wire.h> //I2C library



void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage)); // MSB
  byte c;
  for ( c = 0; c < length; c++){
    Wire.write(data[c]);
  } 
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress)); // MSB
  // Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void setup() 
{
  //to give the eeprom voltage the PIN's to right next to it are used
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  digitalWrite(16, LOW);

  Wire.begin(); // initialise the connection
  Serial.begin(9600);

  char bigCHunkOfText[] = "1234567890abcdefghijklmnopqrstuvwxyz_and the rest of it too.........1234567890abcdefghijklmnopqrstuvwxyz_and the rest of it too.........1234567890abcdefghijklmnopqrstuvwxyz_and the rest of it too.........1234567890abcdefghijklmnopqrstuvwxyz_and the rest of it too.........";

//the text to be written is cut up in smaller units of 8 bytes
  for (int i = 0; i < sizeof(bigCHunkOfText)/8 ; i++){ // be carefull : the last bit over /8 is not written
    char somedata[8]; // data to write
    for (int i2 = 0; i2 < 8 ; i2++)
    {
      somedata[i2] = bigCHunkOfText[i*8+i2];
    }
    i2c_eeprom_write_page(0x50 , i*8, (byte *)somedata, sizeof(somedata));
  }

  Serial.println("Memory written");
  delay(100); 

}

void loop() 
{
  int addr=0; //first address
  byte b; // access the first address from the memory

  while (addr<1024) 
  {

    b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    Serial.print((char)b); //print content to serial port
    addr++; //increase address
  }
  Serial.println(" ");
  delay(2000);

}



