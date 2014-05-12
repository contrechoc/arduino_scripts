#include "WaveUtil.h"
#include "WaveHC.h"


SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're playing


WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

#define DEBOUNCE 100  // button debouncer

#define error(msg) error_P(PSTR(msg))

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads


//declarations for distance sensor
#include <Wire.h>
#define sensorAddress 0x70
#define readInches 0x50
#define readCentimeters 0x51
#define readMicroseconds 0x52
#define resultRegister 0x02
#define minimumRegister 0x04

int iconMode = 0;
unsigned char iconMode0 = 0;
int oldDistance = 0;
unsigned char speedOfChange = 1;

unsigned char LEDeffectPIN = 6;
unsigned char soundC = 0;
int oldLDRVal = 0;

void setup() {
  // set up serial port
  Serial.begin(9600);
  putstring_nl("WaveHC with 6 buttons");
  
   Wire.begin();
  
  // putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  //Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
 
  // pin13 LED
  pinMode(13, OUTPUT);
  
  pinMode( LEDeffectPIN, OUTPUT);
  digitalWrite( LEDeffectPIN, LOW);
 
  // enable pull-up resistors on switch pins (analog inputs)
 
 
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");
  dirLevel = 0;
  
   playcomplete("bird3.wav");
   
        setLight(1);
        delay(1000);
        setLight(7);
       
    
}

void loop() {
    int newLDRVal = analogRead(A0);
    if ( abs ( oldLDRVal - newLDRVal) > 5 ){
      
    switch (soundC++) {
    case 0:
      playcomplete("bird3.wav");
      break;
    case 1:
      playcomplete("blink.wav");
      break;
    case 2:
      playcomplete("wahwah.wav");
      break;
    case 3:
      playcomplete("hoot.wav");
      break;
    case 4:
      playcomplete("horse1.wav");
      break;
    case 5:
      playcomplete("hoot.wav");
         case 6:
      playcomplete("fall.wav");
        case 7:
      playcomplete("papier.wav");
     
          
  } 
  
  soundC = soundC%8;
  
       oldLDRVal = analogRead(A0);
  }
  
 // putstring(".");            // uncomment this to see if the loop isnt running
  unsigned char cs = readDistance();
  Serial.println(cs);
 if ( cs < 150 ){
     
     setLight(1);
        delay(1000);
        setLight(7);

  }
 

}

void setLight(unsigned char nT){
   for (int index = 0; index < nT; ++index)
   {
      digitalWrite( LEDeffectPIN, HIGH);
        delay(150);
        digitalWrite( LEDeffectPIN, LOW);
          delay(150);
   }
}

byte check_switches()
{
  static byte previous[6];
  static long time[6];
  byte reading;
  byte pressed;
  byte index;
  pressed = 0;

  for (byte index = 0; index < 6; ++index) {
    reading = digitalRead(14 + index);
    if (reading == LOW && previous[index] == HIGH && millis() - time[index] > DEBOUNCE)
    {
      // switch pressed
      time[index] = millis();
      pressed = index + 1;
      break;
    }
    previous[index] = reading;
  }
  pressed = 1 + readDistance()%6;
  
  // return switch number (1 - 6)
  return (pressed);
}

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}


