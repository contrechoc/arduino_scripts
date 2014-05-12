/*

Sergei A. Minayev, 2013
DIYTJ LED Matrix Module test app
Working app: http://youtu.be/R10uR81O0YM

*/


#include <Wire.h>
#include <RTClib.h>

/*
root_VNET 2014 http://mystd.ru
Digital Clock on DIYJT 32x16 Dual-color led dot matrix
*/

int switchP = 0;

RTC_DS1307 RTC;

enum {
COLOR_RED = 1,
COLOR_GREEN = 2,
COLOR_ORANGE = 3,
};

const int aPin = 2;
const int bPin = 3;
const int cPin = 4;
const int dPin = 5;
const int oePin = 10;
const int redPin = 11;
const int greenPin = 6;
const int strPin = 12;
const int sckPin = 13;

int count = 0;
int number, i = 0;
int dig1, dig2, dig3, dig4 = 0;

byte pbuffer_r[4][16];
byte pbuffer_g[4][16];

// brutal font, zero element is 0; first element is 1 and so on...
byte digits[][16] = {
{B00000000, B01111110, B01111110, B01100110, B01100110, B01100110, B01100110, B01100110, B01100110, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000}, // 0
{B00000000, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00001100, B00000000}, // 1
{B00000000, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B01111110, B01111110, B01100000, B01100000, B01100000, B01100000, B01111110, B01111110, B00000000}, // 2
{B00000000, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B01111110, B01111110, B00000000}, // 3
{B00000000, B01100110, B01100110, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000000}, // 4
{B00000000, B01111110, B01111110, B01100000, B01100000, B01100000, B01100000, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B01111110, B01111110, B00000000}, // 5
{B00000000, B01111110, B01111110, B01100000, B01100000, B01100000, B01100000, B01111110, B01111110, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000}, // 6
{B00000000, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000110, B00000000}, // 7
{B00000000, B01111110, B01111110, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000}, // 8
{B00000000, B01111110, B01111110, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000110, B00000110, B00000110, B00000110, B01111110, B01111110, B00000000}, // 9
};

void draw_buffer(int draw_delay)
{
for(int row = 0; row < 16; row++)
{
digitalWrite(greenPin,HIGH);
digitalWrite(oePin, HIGH);
digitalWrite(aPin,row & B0001 );
digitalWrite(bPin,row & B0010 );
digitalWrite(cPin,row & B0100 );
digitalWrite(dPin,row & B1000 );
digitalWrite(strPin, LOW);
shiftOut(redPin, sckPin, MSBFIRST, pbuffer_r[0][row]);
shiftOut(redPin, sckPin, MSBFIRST, pbuffer_r[1][row]);
shiftOut(redPin, sckPin, MSBFIRST, pbuffer_r[2][row]);
shiftOut(redPin, sckPin, MSBFIRST, pbuffer_r[3][row]);
digitalWrite(strPin, HIGH);
digitalWrite(oePin, LOW);
delayMicroseconds(draw_delay); // delay for seeing red

digitalWrite(redPin,HIGH);
digitalWrite(oePin, HIGH);
digitalWrite(strPin, LOW);
shiftOut(greenPin, sckPin, MSBFIRST, pbuffer_g[0][row]);
shiftOut(greenPin, sckPin, MSBFIRST, pbuffer_g[1][row]);
shiftOut(greenPin, sckPin, MSBFIRST, pbuffer_g[2][row]);
shiftOut(greenPin, sckPin, MSBFIRST, pbuffer_g[3][row]);
digitalWrite(strPin, HIGH);
digitalWrite(oePin, LOW);
delayMicroseconds(draw_delay); // delay for seeing green
}
}

void ClearBuffer()
{
// clear buffer
memset(pbuffer_r,255,sizeof(pbuffer_r));
memset(pbuffer_g,255,sizeof(pbuffer_g));
}

// set pixel val = 0(off) or 1(on)
void SetPixel(byte x, byte y, byte val)
{
if(val & COLOR_RED)
{
pbuffer_r[x/byte(8)][y] &= ~(1 << 7 >> (x%8));
}
if(val & COLOR_GREEN)
{
pbuffer_g[x/byte(8)][y] &= ~(1 << 7 >> (x%8));
}
}

void printDigit(int x, int y, byte num, byte color)
{
for(byte i = 0; i < 16; i++)
{
byte line = digits[num][i];
byte k = 7;
while(line > 0)
{
byte bit = line % 2;
line /= 2;
if(bit) SetPixel(x + k ,y + i,color);
k--;
}
}
}

void setup()
{
//
Wire.begin();
RTC.begin();
//

//set pins to output because they are addressed in the main loop
pinMode(aPin, OUTPUT);
pinMode(bPin, OUTPUT);
pinMode(cPin, OUTPUT);
pinMode(dPin, OUTPUT);
pinMode(oePin, OUTPUT);
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(strPin, OUTPUT);
pinMode(sckPin, OUTPUT);

digitalWrite(oePin, LOW);
// turn off the light
digitalWrite(greenPin, HIGH);
digitalWrite(redPin, HIGH);
}

byte psec = 0, sec, cnt = 0;;

void loop()
{
//

DateTime now = RTC.now();
number = now.hour() * 100 + now.minute();

delay(1000);
if( switchP == 0){
i = number;
dig1 = 5;//i/1000;
i = i-(dig1*1000);
dig2 = 6;//i/100;
i = i-(dig2*100);
dig3 = 9;//i/10;
i = i-(dig3*10);
dig4 = 3;//i;

sec = 5;// now.second();}
switchP = 1;}
else
{
  
 i = number;
dig1 = 6;//i/1000;
i = i-(dig1*1000);
dig2 = 3;//i/100;
i = i-(dig2*100);
dig3 = 2;//i/10;
i = i-(dig3*10);
dig4 = 1;//i;

sec = 4;// now.second();} 
switchP = 0;
  
}

//

ClearBuffer();
printDigit(0-1,0,dig1,COLOR_RED);
printDigit(0+7,0,dig2,COLOR_RED);
printDigit(0+17,0,dig3,COLOR_GREEN);
printDigit(0+24+1,0,dig4,COLOR_GREEN);

if(sec == psec)
{
if(cnt == 0)
{
SetPixel(15,7-3,COLOR_ORANGE);
SetPixel(15,8-3,COLOR_ORANGE);
SetPixel(16,7-3,COLOR_ORANGE);
SetPixel(16,8-3,COLOR_ORANGE);
SetPixel(15,7+3,COLOR_ORANGE);
SetPixel(15,8+3,COLOR_ORANGE);
SetPixel(16,7+3,COLOR_ORANGE);
SetPixel(16,8+3,COLOR_ORANGE);
}
else cnt--;
}
else
{
psec = sec;
cnt = 20;
}

// draw the screen ==================================================

draw_buffer(80);

// END draw the screen ==============================================

}
