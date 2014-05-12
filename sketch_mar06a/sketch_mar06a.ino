
char counter = 0;

void setup(){
  
  DDRB = 0xFF;
  DDRD = 0xFF;
  
  
}


void loop(){
  
  counter++;
  
   PORTD = counter; 
 
  
   PORTB ^= 1 << 5;    /* toggle the LED */
  
  delay(50);
  
  
}
