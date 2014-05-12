/*

a bare bones LED BLOCK MATRIX script for the Arduino

uses setting of the interrupt TIMER0

to do: extension of the bit alphabet
making a longer text (now only HELLO )

making interactive, using the loop and soft buttons or LDR to make textual variations

2011 five march || contrechoc || beam

*/

#include <avr/interrupt.h>

#define TIMER0_PRESCALE 3 // can be 2 3 4  --- 2 is the fastest

//used for testing:
volatile unsigned char switch13 = 0;
volatile unsigned char interruptCounter = 0;

// ATTENTION, old hacked script, array starts at 1!!!!!!!
int pins[17] = {-1, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2};

//get the LEDS connections for TX and RX out for uploading

// col[xx] of leds = pin yy on led matrix
volatile int cols[8] = {pins[13], pins[3], pins[4], pins[10], pins[06], pins[11], pins[15], pins[16]};

// row[xx] of leds = pin yy on led matrix
volatile int rows[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};

int pattern = 0;

volatile int nextPattern = 0;
volatile byte col = 0;
volatile byte leds[8][8];

int letterCounter = 17;
 
byte E[4][8] = {\
{0,1,1,1,1,1,1,0},\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,1,1,1,1,1,0}\
};

byte H[4][4] = {\
{0,1,0,0},\
{0,1,0,0},\
{0,1,0,0},\
{0,1,1,1}\
};

byte L[8][8] = {\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,0,0,0,0,0,0},\
{0,1,1,1,1,1,1,0}\
};

byte O[4][4] = {\
{0,0,0,1}, \
{0,0,1,0}, \
{0,1,0,0}, \
{0,1,0,0} \
};

#define EMPTY   {\
{0,0,0,0,0,0,0,0},\
{0,0,0,0,0,0,0,0},\
{0,0,0,0,0,0,0,0},\
{0,0,0,1,1,0,0,0},\
{0,0,0,1,1,0,0,0},\
{0,0,0,0,0,0,0,0},\
{0,0,0,0,0,0,0,0},\
{0,0,0,0,0,0,0,0}\
}

const int numPatterns = 1; //max = 9
byte patterns[numPatterns][8][8] = { EMPTY    };

void setup()
{
  
  //setting output pins for LED BLOCK
  
  // ATTENTION, old hacked script, array starts at 1!!!!!!!
    for (int i = 1; i <= 17; i++) pinMode(pins[i], OUTPUT);
  
  // set up cols and rows
  for (int i = 1; i <= 8; i++)
    digitalWrite(cols[i - 1], HIGH);

  for (int i = 1; i <= 8; i++)
    digitalWrite(rows[i - 1], HIGH);

  clearLeds();
  

  //interrupt settings for the display
  //we use timer 0
  TCCR0B = (1 << WGM02) | TIMER0_PRESCALE; //WGNM12 bit3 of tccr1b timer control register
  OCR0A = (uint16_t)100;//output compare register combined with WGM02 set
  TIMSK0 |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
  //enable interrupt:
  sei();
  
  setPattern(pattern);
  //testing
 
  Serial.begin(57600);
  Serial.println("starting up LEDBLOCK");
 
}

void setUpE()
{
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 8; j++)
    {
       patterns[0][i][j] = E[i][j];
       patterns[0][7-i][j] = E[i][j];
    }
}

void setUpH()
{
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
       patterns[0][i][j] = H[i][j];
       patterns[0][i][7-j] = H[i][j];
       patterns[0][7-i][j] = H[i][j];
       patterns[0][7-i][7-j] = H[i][j];
    }
}

void setUpL()
{
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
       patterns[0][i][j] = L[i][j];
}

void setUpO()
{
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
       patterns[0][i][j] = O[i][j];
       patterns[0][i][7-j] = O[i][j];
       patterns[0][7-i][j] = O[i][j];
       patterns[0][7-i][7-j] = O[i][j];
    }
}

 void setUpSPACE(){

    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        patterns[0][i][j] = 0;
        
     patterns[0][3][3] = 1;
  patterns[0][4][4] = 1;   

}

void showingText(){
   
    Serial.print ( "showing next Letter" );
     Serial.println ( letterCounter );
     
    pattern = ++pattern % numPatterns;
    slidePattern(pattern, 100);
 
      if (letterCounter == 0) setUpH();
      if (letterCounter == 1) setUpE();
      if (letterCounter == 2) setUpL();
      if (letterCounter == 3) setUpL();
      if (letterCounter == 4) setUpO();
      if (letterCounter == 5) setUpSPACE();
      if (letterCounter == 6) setUpSPACE();
      
     letterCounter++ ;
     if ( letterCounter > 6)  letterCounter = 0;
 
 }


void loop(){
  
  Serial.println("next line");
  showingText();
  
}

void clearLeds() {
   Serial.println( "clear" );
  // Clear display array
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      leds[i][j] = 0;

}

void setPattern(int pattern) {
    Serial.println( "set pattern" );
  for (int i = 0; i < 8; i++)  
    for (int j = 0; j < 8; j++) 
      leds[i][j] = patterns[pattern][i][j];

}

void slidePattern(int pattern, int del) {
    Serial.println( "slide pattern" );
  for (int l = 0; l < 8; l++) {
    for (int i = 0; i < 7; i++)
      for (int j = 0; j < 8; j++)
        leds[j][i] = leds[j][i+1];

    for (int j = 0; j < 8; j++) 
      leds[j][7] = patterns[pattern][j][0 + l];

    delay(del);
  }
}


// Interrupt routine
SIGNAL( TIMER0_COMPA_vect   ) {
  
   //write whole PORT D (all 8 PINS at once)

  digitalWrite(cols[col], HIGH);  // Turn whole previous column off
  
  col++;
  if (col == 8)
    col = 0;

  for (int row = 0; row < 8; row++) {
    if (leds[col][7 - row] == 1)
      digitalWrite(rows[row], HIGH);  // Turn on this led
    else
      digitalWrite(rows[row], LOW); // Turn off this led
  }
  digitalWrite(cols[col], LOW); // Turn whole column on at once (for equal lighting times)
 
 
}

  



