#include <JeeLib.h>
#include <OneWire.h>
//#include <SoftwareSerial.h>

int DS18S20_Pin = 5; //DS18S20 Signal pin on digital 2
int DS18S20_PinV = 6;
int DS18S20_PinGND = 7;

//Temperature chip i/o
OneWire ds(DS18S20_Pin); // 

long timer1 = 0;
long interval1 = 1000;

long timer2 = 0;
long interval2 = 2000;

long timer3 = 0;
long interval3 = 2000;

int waiter = 10;

int sendON = 0;

char pSensorCounter = 0;

char *radioBuf = "iRadio        ";

int counter = 7;
int commaCounter = 0;
char radioSendCount = 0;
char whichDataCounter = 0;

//SoftwareSerial mySerial(5,6); // RX, TX

void setup () {

  Serial.begin(9600);
  Serial.println("radio start");

  // mySerial.begin(9600);

  rf12_initialize(1, RF12_915MHZ, 212);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(3,LOW);

  pinMode(DS18S20_PinV, OUTPUT);
  pinMode(DS18S20_PinGND, OUTPUT);
  digitalWrite( DS18S20_PinGND, LOW);
  digitalWrite( DS18S20_PinV, HIGH);
}

char putInBuffer1( char *buf, char *message ){
  while( *(message++) )
  {
    *(buf++) = *(message++); 
  }
}

void blink(){
  digitalWrite( 4, LOW );
  delay(50);
  digitalWrite( 4, HIGH );
  delay(50);
}

void loop () {


  if (timer2 < millis()) {
    timer2 = millis() + interval2;
    digitalWrite( 4, HIGH );
    delayWithOverflow();
    digitalWrite( 4, LOW );
    delayWithOverflow();

    sendSensor(( pSensorCounter++) % 2 );
    delayWithOverflow();
  }
}

void sendSensor(int whichSensor){

  int an3 = 0;
  if ( whichSensor == 0 ){
    an3 = analogRead( whichSensor );
  }

  if ( whichSensor == 1 ){
    float temperature = getTemp();
    Serial.println(temperature);
    an3 = (int) (temperature*10);
  }

  int an3_0 = an3/1000;
  int an3_1 = (an3 - an3_0*1000)/100;
  int an3_2 = (an3 - an3_0*1000- an3_1*100 )/10;
  int an3_3 = (an3 - an3_0*1000- an3_1*100- an3_2*10 );

  char buf7[] = {  
    "xpoll        "                        };

  if ( whichSensor == 0 )  buf7[0] = 'z'; //LDR sleeping room
  if ( whichSensor == 1 )  buf7[0] = 's'; //temp chip

  buf7[6] = an3_0 + 48;
  buf7[7] = an3_1 + 48;
  buf7[8] = an3_2 + 48;
  buf7[9] = an3_3 + 48;

  Serial.print(whichSensor);
  Serial.print("\t");
  Serial.println(buf7);

  while ((!rf12_canSend()) )
    rf12_recvDone();
  rf12_sendStart(0, buf7, 10, 0);

  delay(10);
  delayWithOverflow();

}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;


  return TemperatureSum;

}

void delayWithOverflow(){
  //if (mySerial.overflow()) 
  {
    blink();
    blink();
    blink();
  }
  delay(10);
  //if (mySerial.overflow())
  {
    blink();
    blink();
    blink();
  }
}

