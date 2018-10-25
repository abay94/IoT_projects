  #include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <SoftwareSerial.h>
#include <BH1750.h>
#include <Wire.h>
#include <math.h> 
#include <TinyGPS++.h>     

/* ---------- DO NOT EDIT ANYTHING ABOVE THIS LINE ---------- */

//Only edit the settings in this section

/* WIFI Settings */
// Name of wifi network
const char* ssid = "SSE1";

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
char* outTopic1 = "ZSSE_SKLAD_SWITCH_1"; 
char* subscribeTopic = "ZSSE_SKLAD_LISTEN_NODERED_1"; 
//MQTT Server IP Address
const char* server = "192.168.1.74";

//Unique device ID 
const char* mqttDeviceID = "MK-SmartHouseDeviceBhMotionRelay"; 
String payload1 = " ";


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
int inputSwitch = 12;
int lastpirSensor =  0;
int lastinputSwitch = 0;

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
  pinMode(inputSwitch, INPUT);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  client.begin(server, net);
 client.onMessage(messageReceived);
  connect();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
 
  Serial.begin(115200);
  delay(1000);
 /* lightSensor.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
 
  delay(1000);*/ 
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
client.subscribe(subscribeTopic);
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

   
 if (digitalRead(pirSensor) != lastpirSensor) {
    // if the state has changed, increment the counter
    if (digitalRead(pirSensor) == HIGH) {
      String noone = String(1, HEX);
     client.publish(outTopic, noone); 
    } else {
      String noone1 = String(0, HEX);
     client.publish(outTopic, noone1); 
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastpirSensor = digitalRead(pirSensor);



   
 if (digitalRead(inputSwitch) != lastinputSwitch) {
    // if the state has changed, increment the counter
    if (digitalRead(inputSwitch) == HIGH) {
       String noon2 = String(1, HEX);
     client.publish(outTopic1, noon2); 
    } else {
         String noone3 = String(0, HEX);
     client.publish(outTopic1, noone3); 
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastinputSwitch = digitalRead(inputSwitch);

 
   
       
 if (payload1 == "1")
  {
    digitalWrite(relayInput, HIGH);
    lockLow = true;
    delay(250);
  }
 if (payload1 == "0")
  {
    digitalWrite(relayInput, LOW);
    lockLow = false;
    delay(250); 
     delay(250);
  }
}
void messageReceived(String &topic,String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  payload1 =payload;

  //This sensor does not recieve anything from MQTT Server so this is blank
}



