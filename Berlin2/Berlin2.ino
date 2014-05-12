

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

char a[3][14] = { 
  "Happy doing", "Happy fabrics", "Happy sewing" };

// make some custom characters:
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

byte smile1[8] = {
  0b10000,
  0b10111,
  0b10000,
  0b00000,
  0b01110,
  0b00000,
  0b10001,
  0b01110
};

byte smile2[8] = {
  0b01010,
  0b11111,
  0b01010,
  0b00100,
  0b00100,
  0b00100,
  0b10001,
  0b01110
};

byte smile3[8] = {
  0b00001,
  0b11101,
  0b01001,
  0b00000,
  0b01110,
  0b00000,
  0b11011,
  0b01110
};

byte xxxxx[8] = {
  0b10101,
  0b01010,
  0b10101,
  0b01010,
  0b10101,
  0b01010,
  0b10101,
  0b01010
};

byte yyyyy[8] = {
  0b10101,
  0b01010,
  0b00100,
  0b01110,
  0b00100,
  0b01010,
  0b10101,
  0b01010
};

byte p1[8] = {
  0b00000,
  0b00100,
  0b00000,
  0b01000,

  0b00010,
  0b00000,
  0b10000,
  0b00010,
};

byte p2[8] = {
  0b10000,
  0b00100,
  0b00010,
  0b01010,
  0b01010,
  0b00010,
  0b10100,
  0b10010,
};

int px, px1 = 3;
volatile int ff = 5;
int timer1_counter, ccc;
char cc, recycle, turn = 0;
char t1 = 0;
volatile char noInt = 1;

void sineWave()
{
  for (int i = 0; i < 256; ++i) {
    wavetable[i] = pgm_read_byte_near(sinetable + i);
  }
}

void setup() {

  sineWave();

  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, frownie);
  lcd.createChar(3, armsDown);  
  lcd.createChar(4, armsUp);  

  lcd.createChar(5, smile1);  
  lcd.createChar(6, smile2);  
  lcd.createChar(7, smile3); 
  lcd.createChar(8, xxxxx);  
  lcd.createChar(9, yyyyy);  
  lcd.createChar(10, p1); 
  lcd.createChar(11, p2); 

  // set up the lcd's number of columns and rows: 
  lcd.begin(16, 4);

  lcd.setCursor(0, 0); 
  lcd.clear(); 
  lcd.home(); 
saw();
  circle();

  turn = 1;
  writeFlight(9,10);
  writeTextiles();

  delay(1000);
  writeFlight(10, 11);
  writeRecycle();
  delay(1000);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 64286/11*10;   // preload timer 65536-16MHz/256/2Hz

  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS10) | (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

    ff = 0;

  writeHappy();
}

void loop() {

  if ( px > 12 ) {
    px = 4;
    writeHappy();
  }

  cli(); 
  lcd.setCursor(0, 0); 
  sei();
  character(px++,1);

  if (px1 < 1) px1 = 5; 
  cli(); 
  lcd.setCursor(0,0); 
  sei();
  character(px1--,3);

  turn++;
  if ( turn > 2 ){
    writeTextiles();
    turn = 0;
    writeHappy();
    recycle++;
  }

  if ( recycle > 3){
    recycle = 0;
    writeRecycle();
  }

  /*

   while(ff> 0)
   {
   lcd.setCursor(0, 0);
   lcd.clear();
   
   int fx = analogRead(A0)/255; 
   int fy = analogRead(1)/64;
   lcd.setCursor(fx,fy);
   
   lcd.write(5);
   delay(10); 
   lcd.setCursor(fx,fy); 
   lcd.write(6);
   delay(10); 
   lcd.setCursor(fx,fy); 
   lcd.write(7);
   delay(100); 
   ff--;
   }
   */


}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{

  if ( noInt == 1 )
  {
    TCNT1 = timer1_counter;   // preload timer
    lcd.setCursor((cc++)/4,ccc); 
    if (ff == 0)  lcd.write(5);
    else if (ff == 1)  lcd.write(6);
    else if (ff == 2)  lcd.write(7);
    ff++;
    if ( ff > 3) ff = 0;
    if ( cc > 60 ) { 
      cc = 0; 
      ccc = random(4);
    }
  }
}

void character(int cx, int cy){
  // map the result to 200 - 1000:

  int delayTime = 150;
  // set the cursor to the bottom row, 5th position:
  cli(); 
  lcd.setCursor(cx, cy); 

  lcd.write(3); 
  sei();
  delay(delayTime);
  cli();
  lcd.setCursor(cx, cy); 

  lcd.write(4); 
  sei();
  delay(delayTime); 

}

void writeHappy(){
  cli(); 
  lcd.setCursor(0, 0); 
  lcd.clear(); 
  sei();

  if ( t1 == 0 )
  { 
    cli(); 
    lcd.print("Happy Fabrics");  
    sei(); 
    t1=2;
  }
  else if ( t1 == 1)
  { 
    cli(); 
    lcd.print("Happy Threading");  
    sei(); 
    t1++;
  }
  else if ( t1 == 2)
  { 
    cli(); 
    lcd.print("Happy Living");  
    sei(); 
    t1++;
  }
  else if ( t1 == 3)
  { 
    cli(); 
    lcd.print("Happy Working");  
    sei(); 
    t1++;
  }
  else if ( t1 == 4)
  { 
    cli(); 
    lcd.print("Happy Thinking");  
    sei(); 
    t1++;
  }
  else if ( t1 == 5)
  { 
    cli(); 
    lcd.print("Happy Doing");  
    sei(); 
    t1 = 0;
  }


  int pos[3] = { 
    random(3),random(3),random(3)       };

  int h = random(3);
  while ( h == pos[0] ) h = random(3);
  pos[1] = h;

  h = random(3);
  while ( (h == pos[0]) || (h == pos[1]) ) h = random(3);
  pos[2] = h;


  cli(); 
  lcd.setCursor(random(8),1); 
  lcd.print( a[pos[0]] ); 
  lcd.setCursor(random(4),2); 
  lcd.print(a[pos[1]]); 
  lcd.setCursor(random(5),3); 
  lcd.print(a[pos[2]] ); 
  sei();

  delay(500);
}

void writeTextiles(){

  while (turn-- > 0 ){
    //lcd.scrollDisplayLeft();
    cli(); 
    lcd.clear(); 
    lcd.home(); 
    sei();

    for ( int i = 0; i < 4 ; i++){
      cli(); 
      lcd.setCursor(15, i+1); 
      delay(30);
      lcd.write(5);  
      sei();
    }
    delay(500);

    for ( int i = 0; i < 2 ; i++)
      for ( int i2 = 0; i2 < 16 ; i2++){
        cli(); 
        lcd.setCursor(i2, i); 

        lcd.write(8);  
        sei();
        delay(30);
      }

    for ( int i = 2; i < 4 ; i++)
      for ( int i2 = 0; i2 < 16 ; i2++){
        cli(); 
        lcd.setCursor(i2-4, i); 
        lcd.write(9);  
        sei();
        delay(50);
      }
    delay(500);


    for ( int i2 = 0; i2 < 10 ; i2++){
      cli(); 
      lcd.setCursor(i2+3, 1); 
      lcd.write(" "); 

      lcd.setCursor(i2-1, 2); 
      lcd.write(" ");  

      sei(); 
      delay(50);
    }
    delay(500);

    cli(); 
    lcd.setCursor( 3, 1); 
    lcd.write(" happy e-"); 
    lcd.setCursor(0, 2); 
    lcd.write("textiles");  
    sei();
    delay(500);

  }
}

void writeRecycle(){
  noInt = 0;
  cli(); 
  lcd.setCursor(0, 0); 
  lcd.clear(); 
  lcd.print("Please");  
  lcd.setCursor(0,1); 
  lcd.print(" Recycle! "); 
  lcd.setCursor(1,2); 
  lcd.print("me!  "); 
  lcd.setCursor(2,3); 
  lcd.print(" :-):-):-):-):-) "); 
  sei();

  delay(1000);
  noInt = 1;
}

void writeFlight(int whichSign, int whichSign2){

  cli(); 
  lcd.setCursor(0, 0); 
  lcd.clear(); 
  sei(); 

  lcd.setCursor(0, 0); 
  lcd.write(whichSign);  
  lcd.setCursor(0, 1); 
  lcd.write(whichSign);   
  lcd.setCursor(-4, 2); 
  lcd.write(whichSign);  
  lcd.setCursor(-4, 3); 
  lcd.write(whichSign); 
  lcd.scrollDisplayLeft();

  for ( int i2 = 0; i2 < 16 ; i2++){
    cli();
    lcd.scrollDisplayRight();
    sei();
    delay(150);
  }
  for ( int i2 = 0; i2 < 16 ; i2++){
    cli();
    lcd.scrollDisplayLeft();
    sei();
    delay(150);
  }


  for ( int i = 0; i < 4 ; i++){
    for ( int i2 = 0; i2 < 16 ; i2++){
      if ( i < 2 )
        lcd.setCursor(i2, i); 
      else
        lcd.setCursor(i2 - 4, i); 
      lcd.write(whichSign);  
    }
    delay(150);

    for ( int i2 = 0; i2 < 16 ; i2++){
      if ( i < 2 )
        lcd.setCursor(i2, i); 
      else
        lcd.setCursor(i2 - 4, i); 
      lcd.print(" ");  
    }
    delay(150);
  }

  //  delay(2000);
  lcd.setCursor(0, 0); 
  lcd.clear();   

  for ( int i = 3; i >=0 ; i--){
    for ( int i2 = 0; i2 < 17 ; i2++){
      if ( i < 2 )
        lcd.setCursor(i2, i); 
      else
        lcd.setCursor(i2 - 4, i); 
      lcd.write(whichSign2);  
    }
    delay(150);

    for ( int i2 = 0; i2 < 16 ; i2++){
      if ( i < 2 )
        lcd.setCursor(i2, i); 
      else
        lcd.setCursor(i2 - 4, i); 
      lcd.print(" ");  
    }
    delay(150);
  }
  delay(500);

  lcd.setCursor(0, 0); 
  lcd.clear();   
}

void circle(){
  lcd.setCursor(0, 0); 
  lcd.clear(); 
  for ( int i2 = 0; i2 < 255 ; i2+=5){
    int xp = 16*wavetable[  i2]/256;
    int yp = 4*wavetable[ (i2 + 64)%255]/256;
    
    if ( yp < 2 )
      lcd.setCursor(xp, yp); 
    else
      lcd.setCursor(xp-4, yp); 

    lcd.print("*");  
    delay(100);
  }
  delay(500);
   lcd.setCursor(1, 1); 
   lcd.print("threading");  
    lcd.setCursor(-2, 2); 
   lcd.print("the universe");  
  delay(1000);
}

void saw(){
  lcd.setCursor(0, 0); 
  lcd.clear(); 
  for ( int i2 = 0; i2 < 256*4; i2+=25){
    int xp = i2/64;
    int yp = 4*wavetable[ i2%255]/256;
    
    if ( yp < 2 )
      lcd.setCursor(xp, yp); 
    else
      lcd.setCursor(xp-4, yp); 

    lcd.print("*");  
    //delay(50);
  }
  delay(500);
   lcd.setCursor(1, 1); 
   lcd.print("  fabrics");  
    lcd.setCursor(-2, 2); 
   lcd.print(" of space");  
  delay(1000);
}







