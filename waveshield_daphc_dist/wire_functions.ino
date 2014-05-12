
int readDistance(){
    // send the command to read the result in inches:
  sendCommand(sensorAddress, readCentimeters);
  // wait at least 70 milliseconds for a result:
  delay(70);
  // set the register that you want to read the result from:
  setRegister(sensorAddress, resultRegister);
// read the result:
  int sensorReading = readData(sensorAddress, 2);
  if ( sensorReading > (oldDistance + 1) ) oldDistance += speedOfChange;// speedOfChange, can be made bigger than 1 to make the changes go faster
  if ( sensorReading < (oldDistance - 1) ) oldDistance -= speedOfChange;// speedOfChange, can be made bigger to make the changes go faster
  
  //returning the iconMode number that is the animation which will be shown depending on the distance
  return oldDistance/28;
}

/*

  SendCommand() sends commands in the format that the SRF sensors expect

 */

void sendCommand (int address, int command) {

  // start I2C transmission:

  Wire.beginTransmission(address);

  // send command:

  Wire.write(0x00);

  Wire.write(command);

  // end I2C transmission:

  Wire.endTransmission();

}

/*

  setRegister() tells the SRF sensor to change the address pointer position

 */

void setRegister(int address, int thisRegister) {

  // start I2C transmission:

  Wire.beginTransmission(address);

  // send address to read from:

  Wire.write(thisRegister);

  // end I2C transmission:

  Wire.endTransmission();

}

/*

readData() returns a result from the SRF sensor

 */

int readData(int address, int numBytes) {

  int result = 0;        // the result is two bytes long

// send I2C request for data:

  Wire.requestFrom(address, numBytes);

  // wait for two bytes to return:

  while (Wire.available() < 2 )   {

    // wait for result

  }

  // read the two bytes, and combine them into one int:

  result = Wire.read() * 256;

  result = result + Wire.read();

  // return the result:

  return result;

}
