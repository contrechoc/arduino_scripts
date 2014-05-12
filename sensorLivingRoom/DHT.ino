
void dht11_setup(){
  
  DDRD |= _BV(DHT11_PIN);
	PORTD |= _BV(DHT11_PIN);
}


void loop_dht11_dat()
{
	
	byte dht11_in;
	byte i;
	// start condition
	// 1. pull-down i/o pin from 18ms
	PORTD &= ~_BV(DHT11_PIN);
	delay(18);
	PORTD |= _BV(DHT11_PIN);
	delayMicroseconds(40);
	
	DDRD &= ~_BV(DHT11_PIN);
	delayMicroseconds(40);
	
	dht11_in = PIND & _BV(DHT11_PIN);
	
	if(dht11_in){
		Serial.println("dht11 start condition 1 not met");
		return;
	}
	delayMicroseconds(80);
	
	dht11_in = PIND & _BV(DHT11_PIN);
	
	if(!dht11_in){
		Serial.println("dht11 start condition 2 not met");
                delay(1000);
		return;
	}
	delayMicroseconds(80);
	// now ready for data reception
	for (i=0; i<5; i++)
		dht11_dat[i] = read_dht11_dat();
		
	DDRD |= _BV(DHT11_PIN);
	PORTD |= _BV(DHT11_PIN);
	
        byte dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];
	// check check_sum
	if(dht11_dat[4]!= dht11_check_sum)
	{
		Serial.println("DHT11 checksum error");
	}
	
	Serial.print("Current humdity = ");
	Serial.print(dht11_dat[0], DEC);
	Serial.print(".");
	Serial.print(dht11_dat[1], DEC);
	Serial.print("%  ");
	Serial.print("temperature = ");
	Serial.print(dht11_dat[2], DEC);
	Serial.print(".");
	Serial.print(dht11_dat[3], DEC);
	Serial.println("C  ");
	

	delay(2000);
}

byte read_dht11_dat()
{
	byte i = 0;
	byte result=0;
	for(i=0; i< 8; i++){
		
		
		while(!(PIND & _BV(DHT11_PIN)));  // wait for 50us
		delayMicroseconds(30);
		
		if(PIND & _BV(DHT11_PIN)) 
			result |=(1<<(7-i));
              while((PIND & _BV(DHT11_PIN)));  // wait '1' finish
         
		
	}
	return result;
}
