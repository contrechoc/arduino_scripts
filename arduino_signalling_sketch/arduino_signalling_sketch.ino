int signallingPIN = 13;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode ( signallingPIN , OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValueHIGH = analogRead(A0);
  int sensorValueLOW = analogRead(A1);

  digitalWrite(  signallingPIN, HIGH);
  delay( sensorValueHIGH*10);
  digitalWrite(  signallingPIN, LOW);
  delay( sensorValueLOW*10);

  Serial.println(sensorValueHIGH);
  Serial.println(sensorValueLOW);

}

