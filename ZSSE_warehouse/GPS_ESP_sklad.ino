#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <math.h> 
#include <TinyGPS++.h>     

/* ---------- DO NOT EDIT ANYTHING ABOVE THIS LINE ---------- */

//Only edit the settings in this section

/* WIFI Settings */
// Name of wifi network
const char* ssid = "SSE3";

// Password to wifi network
const char* password = "terranova"; 

/* Web Updater Settings */
// Host Name of Device
const char* host = "MK-DoorSensor1";

// Path to access firmware update page (Not Neccessary to change)
const char* update_path = "/firmware";

// Username to access the web update page
const char* update_username = "admin";

// Password to access the web update page
const char* update_password = "Admin";

/* MQTT Settings */
// Topic which listens for commands
char* outTopic1 = "ZSSE_SKLAD_GPS_LAT"; 
char* outTopic2 = "ZSSE_SKLAD_GPS_LON"; 
//MQTT Server IP Address
const char* server = "192.168.1.74";

//Unique device ID 
const char* mqttDeviceID = "MK-SmartHouseDeviceBhMotionRelay"; 


/* ---------- DO NOT EDIT ANYTHING BELOW THIS LINE ---------- */

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all detection has stopped
long unsigned int pause = 100;  

//sensor variables
boolean lockLow = true;
boolean takeLowTime;  


static const int RXPin = 4, TXPin = 5;                // Ublox 6m GPS module to pins 12 and 13
static const uint32_t GPSBaud = 9600;    
TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin); 

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//MQTT 
WiFiClient net;
MQTTClient client;

//Time Variable
unsigned long lastMillis = 0;
float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS

//Connect to WiFI and MQTT
void connect();
unsigned int move_index = 1;       // fixed location for now
  
//Setup pins, wifi, webserver and MQTT
void setup() 
{
 
    Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  client.begin(server, net);

  connect();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  delay(1000);
 /* lightSensor.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
 
  delay(1000);*/
 
}


void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
//      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}

//Connect to wifi and MQTT
void connect() 
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
  }

  while (!client.connect(mqttDeviceID)) 
  {
    delay(1000);
  }

}




void loop() 
{
  // MQTT Loop
  client.loop();
  delay(10);

  // Make sure device is connected
  if(!client.connected()) 
  {
    connect();
  }

  httpServer.handleClient();



    while (ss.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        displayInfo();
  }


}
 
  


 void displayInfo()
{ 

  if (gps.location.isValid() ) 
  {
    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
     String lat = String(latitude, HEX);
 String lon = String(longitude, HEX);
 client.publish(outTopic1, lat); 
 client.publish(outTopic2, lon); 
//    Blynk.virtualWrite(V1, String(latitude, 6));   
//    Blynk.virtualWrite(V2, String(longitude, 6));  
//    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
     //  Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
  //     Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
     //  Blynk.virtualWrite(V5, bearing);               
      
    
  }
}



