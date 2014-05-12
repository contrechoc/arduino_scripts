
#include <LiquidCrystal.h>
//#include <Wire.h>
//#include <OneWire.h>


//Temperature chip i/o
//OneWire ds(DS18S20_Pin); // on digital pin 2

LiquidCrystal lcd(0,1,8,9,5,6,7);
//LiquidCrystal lcd(0,1,2,3,5,6,7); // (14,15,16, 7,6,5,4);

#define expanderTemp B1001000  // Address with three address pins grounded.
int expander = B1101000;

const int analogInPin0 = A0; // tgs 822 koolstofmonoxide, ammoniak, zwaveldioxide, alcohol, benzine 
const int analogInPin1 = A1; // mq-2 gas smoke alcohl, CH4 LPG 
const int analogInPin2 = A2; // mq-3 alcohol 
const int analogInPin3 = A3; // mq-5 gas LPG CH4
const int analogInPin4 = A4; // mq-6 all bad things ---- removed
const int analogInPin5 = A5; // mq-7 carbon monoxide

#define secondsRegister   0x00 //----timechip removed
#define minutesRegister   0x01
#define hoursRegister     0x02
#define dayRegister       0x03
#define dateRegister      0x04
#define monthRegister     0x05
#define yearRegister      0x06
#define controlRegister   0x07


long an0 = 0;
long an1 = 0;
long an2 = 0;
long an3 = 0;
long an4 = 0;
long an5 = 0;

int nT = 200;
int led = 13;

int counter = 0;

void setup()
{
  //Wire.begin();

  lcd.begin(16, 2);
  lcd.setCursor(0,0); 
  lcd.print("start data!");

  pinMode(led, OUTPUT);  
  Serial.begin(9600);
  Serial.println("start tracking");


  pinMode(3, INPUT);
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
}

void loop()
{

  lcd.setCursor(13,0); 
  lcd.print(counter);

  an0 += analogRead(analogInPin0);
  an1 += analogRead(analogInPin1);
  an2 += analogRead(analogInPin2);
  an3 += analogRead(analogInPin3);
  an4 += analogRead(analogInPin4);
  an5 += analogRead(analogInPin5);

  counter++;

  if ( counter > nT ) { 

    counter = 0;
    //doClock();
    printMean();

    an0 = 0;
    an1 = 0;
    an2 = 0;
    an3 = 0;
    an4 = 0;
    an5 = 0;

  }

  //float temperature = getTemp();
  //Serial.println(temperature);

}

void printMean(){
  
  lcd.clear();
  digitalWrite(led, HIGH);
  lcd.setCursor(12,0); 
  lcd.print( "?" );
  delay(200);
  digitalWrite(led, LOW);

  lcd.setCursor(12,0); 
  lcd.print( "*" );
  delay(200);



  lcd.setCursor(0,0); 
  lcd.print( an0/nT );

  lcd.setCursor(4,0); 
  lcd.print( an1/nT );

  lcd.setCursor(8,0); 
  lcd.print( an2/nT );

  lcd.setCursor(0,1); 
  lcd.print( an3/nT );

  lcd.setCursor(4,1); 
  lcd.print( an4/nT );

  lcd.setCursor(8,1); 
  lcd.print( an5/nT );

  // lcd.setCursor(6,1); 
  //  lcd.print(expanderTempRead());

}

static byte bin2bcd (byte val) {
  return val + 6 * (val / 10);
}

static byte bcd2bin (byte val) {
  return val - 6 * (val >> 4);
}

/*
void setMode(unsigned char whichMode){
 
 expanderSetPointer( 0x02 );
 byte reading = expanderRead();
 
 if ( whichMode == '0' ) reading &= ~(1<<6);
 if ( whichMode == '1' ) reading |= (1<<6);
 
 if ( whichMode == '2' ) reading |= (1<<5);
 if ( whichMode == '3' ) reading &= ~(1<<5);
 
 if ( whichMode == '4' )reading |= (1<<4);
 if ( whichMode == '5' ) reading &= ~(1<<4);
 
 expanderWrite(hoursRegister ,  reading );
 
 }
 
 void settingTime(){
 
 expanderWrite(secondsRegister ,  B00000000 + bin2bcd(57) );//seconds
 expanderWrite(minutesRegister ,  bin2bcd (1) );//minutes
 expanderWrite(hoursRegister ,  bin2bcd (19) );//hour 24 houres
 expanderWrite(dayRegister ,  bin2bcd (1) );  //day
 expanderWrite(dateRegister ,  bin2bcd (10) );  //date
 expanderWrite(monthRegister ,  bin2bcd (6) );   //month
 expanderWrite(yearRegister ,  bin2bcd (12) ); //year
 
 }
 
 void doClock(){
 
 Serial.print  ( "temp: " );
 Serial.println  (  expanderTempRead() );
/*
 expanderSetPointer( hoursRegister );
 byte reading = expanderRead();
 Serial.print  ( "time: " );
 Serial.print(bcd2bin(reading), DEC);
 
 expanderSetPointer( minutesRegister );
 reading = expanderRead();
 Serial.print  ( ":" );
 Serial.print((reading&B01001111) + 10*((reading>>4)&B00001111), DEC);
 
 expanderSetPointer( secondsRegister );
 reading = expanderRead();
 Serial.print  ( ":" );
 Serial.print(( reading&B00001111) + 10*((reading>>4)&B00001111), DEC);
 
 expanderSetPointer( dateRegister );
 reading = expanderRead(); 
 Serial.print  ( " date:" );
 Serial.print((reading&B00001111) + 10*((reading>>4)&B00000011), DEC);
 
 expanderSetPointer( monthRegister );
 reading = expanderRead(); 
 Serial.print  ( " - " );
 Serial.print((reading&B00001111) + 10*((reading>>4)&B00000011), DEC);
 
 expanderSetPointer( yearRegister );
 reading = expanderRead(); 
 Serial.print  ( " -20" );
 Serial.println((reading&B00001111) + 10*((reading>>4)&B00001111), DEC);
 
 Serial.println  ( " " );
 }
 
 void doTimerThings(){
 
 int timeData[] = { 
 0,0,0,0,0,0,0             };
 
 for(unsigned char ii = 0;ii<0x07;ii++){
 
 expanderSetPointer( ii );
 
 byte reading = expanderRead();
 
 if (  ii == secondsRegister  ){
 Serial.print  ( " sec: " );
 Serial.print(( reading&B00001111) + 10*((reading>>4)&B00001111), DEC);
 timeData[ii] =  ( reading&B00001111)  + 10*((reading>>4)&B00001111 )   ;
 }
 
 if (  ii == minutesRegister  ){
 Serial.print  ( " min: " );
 Serial.print((reading&B01001111) + 10*((reading>>4)&B00001111), DEC);
 timeData[ii] = (reading&B00001111)  + 10*((reading>>4)&B00001111 ) ;
 }
 if ( ii == hoursRegister)
 {
 Serial.print  ( " hours: " );
 Serial.print(bcd2bin(reading), DEC);
 Serial.print(" ");
 Serial.print( reading , BIN);
 int houres = reading&B00001111;
 Serial.print(" ");
 Serial.print(houres , DEC);
 
 if ( reading&B00010000 ) houres +=10;
 Serial.print(" ");
 Serial.print(houres , DEC);
 if ( reading&B00100000 ) houres +=10;
 Serial.print(" ");
 Serial.print(houres , DEC);
 timeData[ii] = houres ;
 
 }
 if (  ii == dayRegister  ){
 Serial.print  ( " day: " );
 Serial.print(reading, DEC);
 timeData[ii] = reading  ;
 }
 if (  ii == dateRegister  ){
 Serial.print  ( " date: " );
 Serial.print((reading&B00001111) + 10*((reading>>4)&B00000011), DEC);
 timeData[ii] = (reading&B00001111)  + 10*((reading>>4)&B00000011)  ;
 }
 if (  ii == monthRegister  ){
 Serial.print  ( " month: " );
 Serial.print((reading&B00001111) + 10*((reading>>4)&B00000011), DEC);
 timeData[ii] = (reading&B00001111)  + 10*((reading>>4)&B00000011 ) ;
 }
 if(ii == yearRegister)
 {
 Serial.print  ( " year: " );
 Serial.print((reading&B00001111) + 10*((reading>>4)&B00001111), DEC);
 timeData[ii] = (reading&B00001111)  + 10*((reading>>4)&B00001111)  ;
 }
 }
 
 //read 24 or 12
 expanderSetPointer( 0x02 );
 Serial.print(" ");
 // Serial.print  ( expanderRead(), BIN );
 Serial.print(" ");
 timeData[ 0x07 ] = (expanderRead() >> 6) ;
 Serial.print  ( "mode: " );
 Serial.print  ( timeData[ 0x07 ], BIN );
 
 Serial.print("*");
 Serial.print(" ");
 for ( unsigned char i = 0 ; i<0x08 ; i++)
 {
 Serial.print(  timeData[i]  );
 Serial.print(" ");
 }
 Serial.println("*");
 }
 */
/*
void expanderWrite(byte reg, byte _data ) {
 Wire.beginTransmission(expander);
 Wire.write(reg);
 Wire.write(_data);
 Wire.endTransmission();
 }
 
 void expanderSetPointer(byte reg    ) {
 Wire.beginTransmission(expander);
 Wire.write(reg);
 Wire.endTransmission();
 }
 
 byte expanderRead() {
 byte _data;
 Wire.requestFrom(expander, 1);
 if(Wire.available()) {
 _data = Wire.read();
 }
 //Serial.print('.');
 return _data;
 }
 
 byte expanderReadMultiple() {
 byte _data;
 Wire.requestFrom(expander, 0x02);
 
 for(int ii = 0;ii<0x01;ii++){
 
 for (byte i = 0; i<0x01 ;i++){
 if(Wire.available()) {
 _data = Wire.read();
 }
 Serial.print(_data, HEX);
 Serial.print("  ");
 delay(5);
 }
 
 Serial.println(" - ");
 }
 return _data;
 }
 
 void sendCommand (int address, int command) {
 
 Wire.beginTransmission(address);
 Wire.write(0x00);
 Wire.write(command);
 Wire.endTransmission();
 
 }
 */

/*
  setRegister() tells the SRF sensor to change the address pointer position
 */
/*
void setRegister(int address, int thisRegister) {
 
 Wire.beginTransmission(address);
 Wire.write(thisRegister);
 Wire.endTransmission();
 
 }
 */
/*
readData() returns a result from the SRF sensor
 */
/*
int readData(int address, int numBytes) {
 
 int result = 0;        // the result is two bytes long
 Wire.requestFrom(address, numBytes);
 
 while (Wire.available() < 2 ) ;
 
 // read the two bytes, and combine them into one int:
 result = Wire.read() * 256;
 result = result + Wire.read();
 return result;
 }
 
 int expanderTempRead() {
 int _data;
 Wire.requestFrom(expanderTemp, 1);
 if(Wire.available()) {
 _data = Wire.read();
 }
 return _data;
 }
 */





