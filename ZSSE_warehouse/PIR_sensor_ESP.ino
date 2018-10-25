#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <BH1750.h>
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
char* outTopic = "ZSSE_SKLAD_MOTION_SENSOR"; 
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
int pirSensor = 14;
int relayInput = 16;

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

//Connect to WiFI and MQTT
void connect();

//Setup pins, wifi, webserver and MQTT
void setup() 
{
 
  pinMode(pirSensor, INPUT);
  pinMode(relayInput, OUTPUT);  
  
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  client.begin(server, net);

  connect();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  Serial.begin(115200);
  delay(1000);
 /* lightSensor.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
 
  delay(1000);*/
 // Wire.begin(4,5);
  Serial.begin(57600);//init Serail band rate
   ss.begin(GPSBaud);  
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
 String lat = String(gps.location.lat(), HEX);
 String lon = String(gps.location.lng(), HEX);
 Serial.println("lat");
 Serial.println(gps.location.lat());
 Serial.println("lon");
 Serial.println(gps.location.lng());
// client.publish(outTopic1, lat); 
// client.publish(outTopic2, lon); 

  

    if(digitalRead(pirSensor) == HIGH){
       digitalWrite(relayInput,HIGH );   //the led visualizes the sensors output pin state
       if(lockLow){  
        String constan = String(1, HEX);
         client.publish(outTopic, constan); 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }


     if(digitalRead(pirSensor) == LOW){       
       digitalWrite(relayInput,LOW);  //the led visualizes the sensors output pin state


       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
             String noone = String(0, HEX);
         client.publish(outTopic, noone); 
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }



  }



