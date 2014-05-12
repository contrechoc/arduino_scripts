
#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 64   ;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;

int nLEDs2 = 52   ;
int dataPin2  = 4;
int clockPin2 = 5;


int nLEDs3 = 52   ;
int dataPin3  = 6;
int clockPin3 = 7;

unsigned char g,b,c, cMode = 0;
unsigned char r =127;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);
LPD8806 strip2 = LPD8806(nLEDs2, dataPin2, clockPin2);
LPD8806 strip3 = LPD8806(nLEDs3, dataPin3, clockPin3);
// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();

  // Start up the LED strip
  strip2.begin();

  // Update the strip, to start they are all 'off'
  strip2.show();
  
   // Start up the LED strip
  strip3.begin();

  // Update the strip, to start they are all 'off'
  strip3.show();
  
  Serial.begin(9600);
}

void loop() {

  changeColor();
  for(int i=0; i<strip3.numPixels(); i++) 
    strip3.setPixelColor(i, strip3.Color(r,g,b));
  strip3.show();
    for(int i=0; i<strip.numPixels(); i++) 
    strip.setPixelColor(i, strip.Color(g,r,b));
  strip.show();
    for(int i=0; i<strip2.numPixels(); i++) 
    strip2.setPixelColor(i, strip2.Color(b,r,g));
  strip2.show();
  
  delay(1);

  
  colorChase(strip, strip.Color(1 ,  0,  0),strip.Color(127,  0,  0),strip.Color(1,  1,  1), 10); // Red
   colorChase(strip, strip.Color(  0,127,  0),strip.Color(  127,127,  0),strip.Color(  0,127,  0), 100); // Green
   colorChase(strip, strip.Color(  0,  127,127),strip.Color(  0,  0,127),strip.Color(  0,  127,127), 10); // Blue
   colorChase(strip, strip.Color(1,127,0),strip.Color(0,127,127),strip.Color(1,127,0), 1); // White
   /*
   colorChase(strip2, strip2.Color(1 ,  0,  0),strip2.Color(127,  0,  0),strip2.Color(1,  1,  1), 1); // Red
   colorChase(strip2, strip2.Color(  0,127,  0),strip2.Color(  127,127,  0),strip2.Color(  0,127,  0), 5); // Green
   colorChase(strip2, strip2.Color(  0,  127,127),strip2.Color(  0,  0,127),strip2.Color(  0,  127,127), 5); // Blue
   colorChase(strip2, strip2.Color(1,127,0),strip2.Color(0,127,127),strip2.Color(1,127,0), 1); // White
*/

  // for(int i=0; i<strip2.numPixels(); i++) 
  // strip2.setPixelColor(i, strip2.Color(127,  50,  30));

  //  strip2.show();
  //  delay(500);

}

void changeColor(){

  if ( cMode == 0 ){
    if (r>0) r--;
    if ( g < 127) g++;
    b=0;
  }
  if ( cMode == 1 ){
    r=0;
    if (g>0) g--;
   if ( b < 127) b++;
  }

  if ( cMode == 2 ){
    if(r<127) r++;
    g=0;
    b=127;
  }
  if ( cMode == 3){
    r=127;
   if (g < 127)  g++;
   if (b > 0)  b--;
  }
  if ( cMode == 4 ){
    if (r>0) r--;
    g=127;
    if ( b < 127) b++;
  }
  if ( cMode == 5 ){
    if ( r < 127)r++;
    g=127;
    b=127;
  }
  if ( cMode == 6 ){
    r=127;
     if (g>0)g--;
    if (b>0) b--;
  }
  
  c++;
  c++;
  
  if ( c>=254 ) cMode++;
  if (cMode > 6)cMode = 0;
  
  /*
  Serial.print(c, DEC);
  Serial.print(" ");
  Serial.print(cMode, DEC);
   Serial.print(" ");
     Serial.print(r, DEC);
    Serial.print(" ");
      Serial.print(g, DEC);
     Serial.print(" ");
       Serial.print(b, DEC);
      Serial.println(" ");
      */
}


// Chase one dot down the full strip.  Good for testing purposes.
void colorChase(LPD8806 strip, uint32_t c0,uint32_t c1,uint32_t c3, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=1; i<strip.numPixels()-1; i++) {
    strip.setPixelColor(i-1, c1);
    strip.setPixelColor(i, c0); // Set new pixel 'on'
    strip.setPixelColor(i+1, c3);
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    strip.setPixelColor(i+1, 0);
    strip.setPixelColor(i-1, 0);
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}


