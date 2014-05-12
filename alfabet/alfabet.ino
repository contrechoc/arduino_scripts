#include <Keypad.h>
#include <WaveHC.h>
#include <WaveUtil.h>

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file 
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

// time to play each tone in milliseconds
#define PLAY_TIME 200

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))


const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'0','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[ROWS] = {6, 7,8,9,}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {14,15,16,17,18, 19}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  
    if (!card.init()) error("card.init");

  // enable optimized read - some cards may timeout
  card.partialBlockRead(true);

  if (!vol.init(card)) error("vol.init");

  if (!root.openRoot(vol)) error("openRoot");

  PgmPrintln("Index files");
  indexFiles();

  PgmPrintln("Play files by index");
  playByIndex();

  PgmPrintln("Play files by name");
  playByName();
}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
playByName(key); /////// -------------------> deze nog aan de keys/filenames verbinden
  }
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

// Number of files.
#define FILE_COUNT 16

// Files are 'touch tone phone' DTMF tones, P = #, S = *
// Most phones don't have A, B, C, and D tones.
// file names are of the form DTMFx.WAV where x is one of
// the letters from fileLetter[]
char fileLetter[] =  {'0', '1', '2', '3', '4', '5', '6', 
      '7', '8', '9', 'A', 'B', 'C', 'D', 'P', 'S'}; 
      
// index of DTMF files in the root directory
uint16_t fileIndex[FILE_COUNT];
/*
 * Find files and save file index.  A file's index is is the
 * index of it's directory entry in it's directory file. 
 */
void indexFiles(void) {
  char name[10];
  
  // copy flash string to RAM
  strcpy_P(name, PSTR("DTMFx.WAV"));
  
  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    
    // Make file name
    name[4] = fileLetter[i];
    
    // Open file by name
    if (!file.open(root, name)) error("open by name");
    
    // Save file's index (byte offset of directory entry divided by entry size)
    // Current position is just after entry so subtract one.
    fileIndex[i] = root.readPosition()/32 - 1;   
  }
  PgmPrintln("Done");
}
/*
 * Play file by index and print latency in ms
 */
void playByIndex(void) {
  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    
    // start time
    uint32_t t = millis();
    
    // open by index
    if (!file.open(root, fileIndex[i])) {
      error("open by index");
    }
    
    // create and play Wave
    if (!wave.create(file)) error("wave.create");
    wave.play();
    
    // print time to open file and start play
    Serial.println(millis() - t);
    
    // stop after PLAY_TIME ms
    while((millis() - t) < PLAY_TIME);
    wave.stop();
    
    // check for play errors
    sdErrorCheck();
  }
  PgmPrintln("Done");
}
/*
 * Play file by name and print latency in ms
 */
void playByName(void) {
  char name[10];
  
  // copy flash string to RAM
  strcpy_P(name, PSTR("DTMFx.WAV"));
  
  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    // start time
    uint32_t t = millis();
    
    // make file name
    name[4] = fileLetter[i];
    
    // open file by name
    if (!file.open(root, name)) error("open by name"); 
    
    // create wave and start play
    if (!wave.create(file))error("wave.create");
    wave.play();
    
    // print time
    Serial.println(millis() - t);
    
    // stop after PLAY_TIME ms
    while((millis() - t) < PLAY_TIME);
    wave.stop();
    
    // check for play errors
    sdErrorCheck();
  }
  PgmPrintln("Done");
}
