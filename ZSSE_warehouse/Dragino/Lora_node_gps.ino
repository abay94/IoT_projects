/*
  LoRa Simple Client for Arduino :
  Support Devices: LoRa Shield + Arduino 
  
  Example sketch showing how to create a simple messageing client, 
  with the RH_RF95 class. RH_RF95 class does not provide for addressing or
  reliability, so you should only use RH_RF95 if you do not need the higher
  level messaging abilities.

  It is designed to work with the other example LoRa Simple Server
  User need to use the modified RadioHead library from:
  https://github.com/dragino/RadioHead

  modified 16 11 2016
  by Edwin Chen <support@dragino.com>
  Dragino Technology Co., Limited
*/
#include <SoftwareSerial.h>

#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(11, 12);


#include "cactus_io_DS18B20.h" 
#include "U8glib.h"
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  


int DS18B20_Pin = 5; //DS18S20 Signal pin on digital 9 
//int ledPin = 13; 
//int inputPin = 6;               // choose the input pin (for PIR sensor)
//int pirState = LOW;             // we start, assuming no motion detected
//int val = 0; 


// Create DS18B20 object 
DS18B20 ds(DS18B20_Pin); 
int analog_pin=A0;



#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
float frequency = 433.0;

float flat, flon;




void setup() 
{
  Serial.begin(115200);
  ss.begin(9600);

  
//  pinMode(inputPin, INPUT);
  Serial.begin(9600);
   ds.readSensor(); 
  //while (!Serial) ; // Wait for serial port to be available
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);

  // Setup Spreading Factor (6 ~ 12)
  rf95.setSpreadingFactor(7);
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);
}

void loop()
{

// val = digitalRead(inputPin);  // read input value
//  if (val == HIGH) {            // check if the input is HIGH
//    digitalWrite(ledPin, HIGH);  // turn LED ON
//    if (pirState == LOW) {
//      // we have just turned on
//      Serial.println("Motion detected!");
//      // We only want to print on the output change, not state
//      pirState = HIGH;
//    }
//  } else {
//    digitalWrite(ledPin, LOW); // turn LED OFF
//    if (pirState == HIGH){
//      // we have just turned of
//      Serial.println("Motion ended!");
//      // We only want to print on the output change, not state
//      pirState = LOW;
//    }
//  }

bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
if (newData)
  {
    
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
//    Serial.print("LAT=");
//    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
//    Serial.print(" LON=");
//    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
  }

  
  // rebuild the picture after some delay
 // delay(500);
int gas;
gas=analogRead(analog_pin);
String gaslora = String(gas);
Serial.print(gaslora);
    ds.readSensor(); 
   String tempgas = String("Temp: " + String(ds.getTemperature_C())+" Gas: "+gaslora+ " Lat: " + String(flat)+" Lon: " + String(flon)); 
  //String tempgas = String("Temp: " + String(ds.getTemperature_C()) + " Lat: " + String(flat)+" Lon: " + String(flon)); 
 
    Serial.print(tempgas);
String tempds = String(ds.getTemperature_C());


  
  Serial.print("Sending to LoRa Server");
  // Send a message to LoRa Server
  uint8_t data[50];  
  tempgas.toCharArray(data, 50);
  rf95.send(data, sizeof(data));

delay(500);

   u8g.firstPage();  
  do {
    // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 15); 
  // call procedure from base class, http://arduino.cc/en/Serial/Print
  u8g.print("Gas: "+gaslora+"");
  u8g.setPrintPos(0, 30);
    u8g.print("Temp: "+tempds+"C");

  } while( u8g.nextPage() );



  /*rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }*/
  delay(5000);
}


