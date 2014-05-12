 

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
 
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // read the analog in value:
  int sensorValue = analogRead(analogInPin);            
  
  Serial.println(sensorValue);      
 
  delay(10);                     
}
