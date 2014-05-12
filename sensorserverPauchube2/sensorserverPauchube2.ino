 #include <EtherCard.h>
#include <JeeLib.h>


// change these settings to match your own setup
#define APIKEY         "mgBqtrrcK44we6cVXS89Bk1oX5DF4EcigoGfqhdwn3M" // your cosm api key
#define FEEDID         "76944" // your feed ID

// ethernet interface mac address, must be unique on the LAN
byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };

char website[] PROGMEM = "api.pachube.com";

char alternator = 0;

Stash stash;

long timer;
long timer2;
long interval = 1000;
int tC = 0;

long timer3;
long interval3 = 1111;

char messageCounter = 0;

int c_temp = 0;
int b_light = 0;

int a_pir = 0;
int d_time = 0;
int f_temp2 = 0;
int g_pressure = 0;
int h_sound = 0;
int i_radio = 0;

int e_pollution1 = 0;
int j_pollution2 = 0;
int k_pollution3 = 0;
int l_pollution4 = 0;
int m_pollution5 = 0;
int n_pollution6 = 0;
int q_pollution7 = 0;

int o_sound2 = 0;
int p_sound3 = 0;

int r_humidity = 0;
int s_temp = 0;
int t_temp = 0;

byte Ethernet::buffer[1000];   // tcp/ip send and receive buffer

void setup () {
  //Serial.begin(9600);
  //Serial.println("\n[webClient]");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) ;
   // Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup());
  //  Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website));
   // Serial.println("DNS failed");

  ether.printIp("SRV: ", ether.hisip);

  rf12_initialize('R', RF12_915MHZ, 212);

  pinMode(7, OUTPUT);
  timer = millis() + interval;
}

void loop () {


  if ( timer < millis()){
    timer = millis() + interval;
    if ( (tC++)%2 == 0){
      digitalWrite(7, HIGH);
    }
    else
    {
      digitalWrite(7, LOW);
    }
  }

  // RFM12 loop runner

  char* buf = (char*) rf12_data;
  if (rf12_recvDone() && rf12_crc == 0) {
    // Serial.println(buf);

    messageCounter++;

    char indicatorPos = 0;

    char s1000 = indicatorPos + 6;
    char s100 =  indicatorPos + 7;
    char s10 =  indicatorPos + 8;
    char s1 =  indicatorPos + 9;

    int val =  (((char)rf12_data[s1000]) - 48)*1000 +  (((char)rf12_data[s100]) - 48)*100 + (((char)rf12_data[s10]) - 48)*10 +  (((char)rf12_data[s1]) - 48);
  char indicator = rf12_data[indicatorPos];

 if ( indicator == 'a' )  a_pir = val;
    if ( indicator == 'b' )  b_light = val;
    if ( indicator == 'c' )  c_temp = val;//analog
    if ( indicator == 'd' )  d_time = val;
    
    if ( indicator == 'e' )  e_pollution1 = val;
    
    if ( indicator == 'f' )  f_temp2 = val;//jlabs
    if ( indicator == 'g' )  g_pressure = val;
    if ( indicator == 'h' )  h_sound = val;
    if ( indicator == 'i' )  i_radio = val;

    if ( indicator == 'j' )  j_pollution2 = val;
    if ( indicator == 'k' )  k_pollution3 = val;
    if ( indicator == 'l' )  l_pollution4 = val;
    if ( indicator == 'm' )  m_pollution5 = val;
    if ( indicator == 'n' )  n_pollution6 = val;

    if ( indicator == 'o' )  o_sound2 = val;
    if ( indicator == 'p' )  p_sound3 = val;
    
     if ( indicator == 'q' ) q_pollution7 = val;
     if ( indicator == 'r' ) r_humidity = val;
     
     if ( indicator == 's' ) s_temp = val; //temp chip Temperature sensor module KY-013
     if ( indicator == 't' ) t_temp = val; //Temperature and humidity sensor module KY-015, DHT11 Temp & Humidity
     
     

/*
    for(int i =0; i<11; i++){
      Serial.print((char)(rf12_data[i]));
      Serial.print(' ');
    }
    Serial.println("");
    Serial.print(  (char)rf12_data[indicatorPos] );
    Serial.print('\t');
    Serial.println(val);
*/

  }


  ether.packetLoop(ether.packetReceive());




  if (millis() > timer2) {
    timer2 = millis() + 10000;

    // generate two fake values as payload - by using a separate stash,
    // we can determine the size of the generated message ahead of time
    byte sd = stash.create();
    alternator++;
    
    if ( ((alternator)%2) == 0 ){
      stash.print("messageCount_RM12,"); //server itself
      stash.println(messageCounter/4);
      messageCounter = 0;

      stash.print("temp_KT_130,");//KT 130 analog
      stash.println(c_temp);
     // stash.print("radioactivity,");//from arduino 2
     // stash.println(i_radio);
      stash.print("sensor-MQ2,");
      stash.println(l_pollution4);
      stash.print("sensor-TGS_822,");
      stash.println(m_pollution5);
      
       stash.print("PIR_ON_OFF,");
       stash.println(a_pir);
       
     //  stash.print("sensor-MQ6-2,");
     // stash.println(q_pollution7);

      stash.print("sound_piezo_frige,");//piezo on frige
      stash.println(o_sound2);

    }

    if ((alternator % 3) == 1)
    {
      stash.print("sensor-MQ3,");
      stash.println(e_pollution1);
      stash.print("sensor-MQ7,");
      stash.println(j_pollution2);
      stash.print("sensor-MQ5,");
      stash.println(k_pollution3);
      
      stash.print("light_LDR,");//ldr
      stash.println(b_light);

      stash.print("sound_electret,");//electret
      stash.println(h_sound);
      
       stash.print("humidity_dht11,");//dht11
      stash.println(r_humidity);


      o_sound2 = 0;
      p_sound3 = 0;
    }

    if ((alternator % 3) == 2)
    {
      stash.print("temperature_HDPM01,");//HDPM01
      stash.println(f_temp2);
      stash.print("pressure_HDPM01,");//HDPM01
      stash.println(g_pressure);
      stash.print("time_chip,");
      stash.println(d_time);
     stash.print("sensor-MQ6,");
      stash.println(n_pollution6);

      stash.print("sound_piezo_boiler,");//piezo on boiler
      stash.println(p_sound3);
      
        stash.print("temp_ds18b20,");
      stash.println(s_temp);
        stash.print("temp_dht11,");//dht11
      stash.println(t_temp);
      
      o_sound2 = 0;
      p_sound3 = 0;
    }

    alternator = alternator%100; 

    stash.save();

    // Serial.println("send");

    // generate the header with payload - note that the stash size is used,
    // and that a "stash descriptor" is passed in as argument using "$H"
    Stash::prepare(PSTR("PUT http://$F/v2/feeds/$F.csv HTTP/1.0" "\r\n"
      "Host: $F" "\r\n"
      "X-PachubeApiKey: $F" "\r\n"
      "Content-Length: $D" "\r\n"
      "\r\n"
      "$H"),
    website, PSTR(FEEDID), website, PSTR(APIKEY), stash.size(), sd);

    // send the packet - this also releases all stash buffers once done
    ether.tcpSend();
  }
}






