// include the library code:
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// sine lookup table pre-calculated
prog_uchar PROGMEM sinetable[256] = {
  128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174,
  176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216,
  218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,245,
  246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255,
  255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,
  246,245,243,242,240,239,237,236,234,232,230,228,226,224,222,220,
  218,216,213,211,209,206,204,201,199,196,193,191,188,185,182,179,
  176,174,171,168,165,162,159,156,152,149,146,143,140,137,134,131,
  128,124,121,118,115,112,109,106,103,99, 96, 93, 90, 87, 84, 81, 
  79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 
  37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10, 
  9,  8,  7,  6,  5,  4,  3,  3,  2,  1,  1,  0,  0,  0,  0,  0,  
  0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,  8,  
  9,  10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35, 
  37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 
  79, 81, 84, 87, 90, 93, 96, 99, 103,106,109,112,115,118,121,124
};

// lookup table for output waveform
unsigned char wavetable[256];

void sineWave()
{
  for (int i = 0; i < 256; ++i) {
    wavetable[i] = pgm_read_byte_near(sinetable + i);
  }
}

int phase = 0;

byte t1[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,

  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte t2[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,

  0b01110,
  0b01110,
  0b01110, 
  0b01110,
};

byte t3[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,

  0b01110,
  0b01110,
  0b01110, 
  0b01110,
};

byte t4[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,

  0b11111,
  0b11111,
  0b11111,
  0b11111
};


byte t5[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00001,
  0b00011,
  0b00111,
  0b01111,
  0b11111
};


byte t6[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b10000,

  0b11000,
  0b11100,
  0b11110,
  0b11111
};

byte t7[8] = {


  0b11111,
  0b01111,
  0b00111,
  0b00011,
  0b00001,
  0b00000,
  0b00000,
  0b00000,


};

byte t8[8] = {

  0b11111,
  0b11110,
  0b11100,
  0b11000,
  0b10000,
  0b00000,
  0b00000,
  0b00000,
};

void setup() {

  sineWave();
  lcd.begin(32, 2);

  lcd.setCursor(0, 0); 
  lcd.clear(); 
  lcd.home(); 

  lcd.createChar(1, t1);
  lcd.createChar(2, t2);
  lcd.createChar(3, t3);
  lcd.createChar(4, t4); 
  lcd.createChar(5, t5); 
  lcd.createChar(6, t6); 
  lcd.createChar(7, t7); 
  lcd.createChar(8, t8); 

}

void loop() {

  lcd.setCursor(0, 0); 
  lcd.clear(); 

  unsigned char screen[16][4];

  int myPhase = 0;
  for ( int i2 = 0; i2 < 256*4; i2+=32){
    int xp = i2/64;
    int yp = 4*wavetable[(i2%256]/256;
    screen[xp][yp] = 1;
  }
  

  for ( int yp = 0; yp < 4; yp++)
    for ( int xp = 0; xp < 16; xp++)

      if ( (screen[xp][yp] == 1 ) & ( screen[xp+1][yp] == 1 ))
      {
        if ( yp == 0  ){
          screen[xp][yp] = 2;
          screen[xp+1][yp] = 3;
        }
        if ( yp == 3  ){
          screen[xp][yp] = 4;
          screen[xp+1][yp] = 5;
        }
      }


  for ( int xp = 0; xp < 16; xp++)
    for ( int yp = 0; yp < 4; yp++)
    {
       
      if ( screen[xp][yp] !=1 )
      { 
        if ( yp == 0 ){ 
           lcd.setCursor(xp, yp);  
         // lcd.write(1); 
         lcdWrite( screen[xp][yp] );
        }
        if ( yp == 1 ){ 
         lcd.setCursor(xp, yp);  
         lcd.write(2);  
        }

        if ( yp == 2 ){ 
          lcd.setCursor(xp + 16, yp-2);  
          lcd.write(3); 
        } 
        if ( yp == 3 ){ 
          
          lcd.setCursor(xp + 16, yp-2);  
          // lcd.print("4"); 
         lcdWrite( screen[xp][yp] );
        } 
      }
    }
    
  delay(100);
}

void lcdWrite(int whichCode){
  if ( whichCode == 1)
    lcd.write(4);  
  else if ( whichCode == 2)
    lcd.write(5);  
  else if ( whichCode == 3)
    lcd.write(6);  
  else if ( whichCode == 4)
    lcd.write(7);  
  else if ( whichCode == 5)
    lcd.write(8);  
}








