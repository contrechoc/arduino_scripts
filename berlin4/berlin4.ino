#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#include <avr/pgmspace.h>

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

  byte black[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  byte white[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };
  byte topBlack[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };

  byte bottomBlack[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };

// lookup table for output waveform
unsigned char wavetable[256];

byte t6[8];
int g = 1;
int flip = 0;
int counter = 0;
char text[] = "fabrics for ever ";
char text1[] = "happy happyhap";
char text2[] = "textile textil";
int dir = 0;
int textLength = 0;
int whichText = 0;
int chaosNumber = 64;

int recycleNum = 11;

int posRandom[4] = {
  0,1,2,3};

char screen1[5][17] = { 
  "                ", "                ", "                ", "                " };


void sineWave()
{
  for (int i = 0; i < 256; ++i) {
    wavetable[i] = pgm_read_byte_near(sinetable + i);
  }
}

void setup() {

  sineWave();

  lcd.begin(32, 2);

  lcd.setCursor(0, 0); 
  lcd.clear(); 
  lcd.home(); 

patches();

  // circle();
  // writeMessage();

  /*
  while ( g < 17 ){
   for ( int q = 0; q < 8; q++)
   t6[q] = 15+random(8);
   lcd.createChar(g, t6);
   
   for ( int xp = 0; xp < 16; xp++)
   for ( int yp = 0; yp < 4; yp++)
   {
   
   lcd.setCursor(xp, yp);  
   if ( yp>1 )   lcd.setCursor(xp + 16, yp - 2); 
   //lcd.print("*");
   
   lcd.write(xp%16 + 1);
   }
   g++;
   }
   */
}

void loop() {

  // lcd.setCursor(0, 0); 
  // lcd.clear(); 
  //int yp = 1;


  for ( int q = 0; q < 8; q++)
    if( g%2 == 0 )
      t6[q] = random(8)<<2;
    else
      t6[q] = random(64);

  lcd.createChar(g%17 + 1, t6);
  g++;

  if ( flip == 0){
    for ( int i = 0; i < 13; i++ ){
      lcd.setCursor(random (32), random(2));  
      // lcd.print( text[i]);
      doPrint(textLength);
      counter++;
      delay(10);
    }
  }
  else
  {

    for ( int i = 0; i < 13; i++ ){
      lcd.setCursor(random (32), random(2)); 
      //lcd.print("*");

      lcd.write(counter%16 + 1);
      counter++;
    }
  }

  if (dir == 0)
    textLength++;
  else
    textLength--;

  if (( textLength < 0)||(textLength > 12 )) flipDir();

  if ( counter >= chaosNumber ){
    reflip();
    counter = 0;
    //delay(1000);
    if ( (recycleNum)%30 == 0 ) writeRecycle();
    if ( (recycleNum)%30 == 10 )writeMessage();
    if ( (recycleNum++)%30 == 20 )circle();
  }


  delay(100);

}

void reflip(){
  if ( flip == 0 ){
    chaosNumber = 255;
    flip = 1;
    if ( (whichText++) % 2 == 0 ){
      for ( int i = 0; i < 13; i++ )
        text[i] = text1[i];
    }
    else
    {
      for ( int i = 0; i < 13; i++ )
        text[i] = text2[i];
    }
  }
  else{
    flip = 0;
    chaosNumber = 64;
  }
}

void flipDir(){
  if ( dir == 0 )
    dir = 1;
  else
    dir = 0;
}

void doPrint(int l){
  for ( int i = 0; i < l; i++ ){
    lcd.print(text[i]);
  }
}

void writeRecycle(){

  emptyBuffer();
  unsigned char nextAddress = 0;

  nextAddress = myStrCopy( 0,"Please          " , screen1[0], 16);
  nextAddress = myStrCopy( 0,"Recycle         " , screen1[1], 16);
  nextAddress = myStrCopy( 0,"me!!!!!!        " , screen1[2], 16);
  nextAddress = myStrCopy( 0,"   :-) ^_^ (-:  " , screen1[3], 16);

  bufferToScreen();

  delay(1000);

}

void writeMessage(){

  emptyBuffer();

  char a[4][14] = { 
    "Laugh ", "Love  ", "Live   " , ":-)   "                  };
  char b[4][14] = { 
    "well ", "often  ", "much  " , "^_^   "                  };
  int con = 2;
  while ( con-- > 0 ){
    getRandomThree();
    unsigned char nextAddress = 0;

    nextAddress = myStrCopy( 0, a[posRandom[0]], screen1[0], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[0]], screen1[0], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[1]], screen1[0], 6 );

    nextAddress = myStrCopy( 0, a[posRandom[1]], screen1[1], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[1]], screen1[1], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[1]], screen1[1], 6 );

    nextAddress = myStrCopy( 0, a[posRandom[2]], screen1[2], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[2]], screen1[2], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[2]], screen1[2], 6 );

    nextAddress = myStrCopy( 0, a[posRandom[3]], screen1[3], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[3]], screen1[3], 6 );
    nextAddress = myStrCopy(nextAddress , b[posRandom[3]], screen1[3], 6 );

    bufferToScreen();
    delay(1000);

  }

}

void getRandomThree(){

  int pos[4] = { 
    random(4),random(4),random(4),random(4)                };
  int h = random(4);
  while ( h == pos[0] ) h = random(4);
  pos[1] = h;
  h = random(4);
  while ( (h == pos[0]) || (h == pos[1]) ) h = random(4);
  pos[2] = h;
  while ( (h == pos[0]) || (h == pos[1])|| (h == pos[2]) ) h = random(4);
  pos[3] = h;

  posRandom[0] = pos[0];
  posRandom[1] = pos[1];
  posRandom[2] = pos[2];
  posRandom[3] = pos[3];

}

unsigned char myStrCopy( unsigned char startAddress,  char *arraySource,  char *arrayDest, unsigned char arrayLength){
  unsigned char counter = 0;
  while ( counter < arrayLength){
    arrayDest[ startAddress + counter ] = arraySource [ counter ];
    counter++;
  }
  return startAddress + counter;
}

void bufferToScreen(){
  // lcd.clear(); 
  int cc = 0;
  while ( cc< 255 ){
    int hx = random(4);
    int hy = random (16);
    int hpx = 0;
    int hpy = 0;
    if ( hx == 1 || hx == 3)
      hpx = 1; 
    if ( hx > 1 )
      hpy = 16;
    lcd.setCursor(hy+hpy, hpx);
    lcd.print( screen1[hx][hy] );
    delay(10);
    cc++;
  }
  delay(100);

  lcd.setCursor(0, 0); 
  lcd.print( screen1[0] );
  lcd.setCursor(0, 1); 
  lcd.print( screen1[1] );
  lcd.setCursor(16, 0); 
  lcd.print( screen1[2] );
  lcd.setCursor(16, 1); 
  lcd.print( screen1[3] );

}

void emptyBuffer(){
  int c2 = 0;
/*
  while ( c2 < 17 ){
    screen1[0][c2] = ' ';
    screen1[1][c2] = ' ';
    screen1[2][c2] = ' ';
    screen1[3][c2] = ' ';
    c2++;
  }
*/

}

void circle(){
  emptyBuffer();

  for ( int i2 = 0; i2 < 255 ; i2+=5){
    int xp = 16*wavetable[  i2]/256;
    int yp = 4*wavetable[ (i2 + 64)%255]/256;
    screen1[yp][xp] = '*';
  }
  bufferToScreen();
  delay(500);

  unsigned char nextAddress = 0;

  nextAddress = myStrCopy( 2, "Threading", screen1[1], 9 );
  nextAddress = myStrCopy( 2, "the Universe", screen1[2], 12 );

  bufferToScreen();
  delay(1000);

}

void patches(){

 lcd.setCursor(0,0);

  lcd.createChar(1, black);
  lcd.createChar(2, white);
  lcd.createChar(3, topBlack);
  lcd.createChar(4, bottomBlack);
  
  char patches[4][28] = { 
"111011101110111010101110111", 
"101010100100100012101200120", 
"111011100100100013101300031", 
"100010100100111010101110111"};
int cc = 0;
while ( cc++ <  4 ){
 for ( int j = 0; j < 4; j++ )
 {
 for ( int i = 0; i < 16; i++ )
 {
   
     int hpx = 0;
    int hpy = 0;
    if ( j == 1 || j == 3)
      hpx = 1; 
    if ( j > 1 )
      hpy = 16;
    lcd.setCursor(i+hpy, hpx);
   
   
 if ( patches[j][i] == '1' ) { lcd.write(1);}
 else if ( patches[j][i] == '0' )  {  lcd.write(2);}
 else if ( patches[j][i] == '2' )  {  lcd.write(4);}
 else if ( patches[j][i] == '3' )  {  lcd.write(3);}
 }
 }
delay(2000 -cc*400 );
 
 
 for ( int j = 0; j < 4; j++ )
 {
 for ( int i = 0; i < 16; i++ )
 {
   
     int hpx = 0;
    int hpy = 0;
    if ( j == 1 || j == 3)
      hpx = 1; 
    if ( j > 1 )
      hpy = 16;
    lcd.setCursor(i+hpy, hpx);
   
   
 if ( patches[j][i+11] == '1' ) { lcd.write(1);}
 else if ( patches[j][i+11] == '0' )  {  lcd.write(2);}
 else if ( patches[j][i+11] == '2' )  {  lcd.write(4);}
 else if ( patches[j][i+11] == '3' )  {  lcd.write(3);}
 }
 }
delay(2000 -cc*400);
}


}



















