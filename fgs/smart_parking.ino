#include <Servo.h>

// #define parking_led_1 5
//inizialize micro servo 9g
Servo servo;
const int parking_led_1 = 6;
const int parking_led_2 = 7;
const int parking_led_3 = 8;
const int parking_led_4 = 10;
const int pir1 = 2; // input pin (IR LED parking space 1)
const int pir2 = 3;
const int pir3 = 4;
const int pir4 = 5;


void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(parking_led_1, OUTPUT);
  pinMode(parking_led_2, OUTPUT);
  pinMode(parking_led_3, OUTPUT);
  pinMode(parking_led_4, OUTPUT);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(pir3, INPUT);
  pinMode(pir4, INPUT);

  //attach the servo to pin 9
  servo.attach(9);
}

void loop() {
  
  // reads the input on analog pin A2 (value between 0 and 1023)
  int analogValueEntry = analogRead(A2);

  //ENABLE THE FOLLOWING CODE TO CHECK THE ISOLATION OF THE LIGHT SENSOR
  /*
  Serial.print("Analog reading entry: ");
  Serial.print(analogValueEntry);   // the raw analog reading

  
  // We'll have a few threshholds, qualitatively determined
  if (analogValueEntry < 10) {
    Serial.println(" - Dark");
  } else if (analogValueEntry < 200) {
    Serial.println(" - Dim");
  } else if (analogValueEntry < 500) {
    Serial.println(" - Light");
  } else if (analogValueEntry < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }
  */

  
  // reads the input on analog pin A1 (value between 0 and 1023)
  int analogValueExit = analogRead(A1);

  //ENABLE THE FOLLOWING CODE TO CHECK THE ISOLATION OF THE LIGHT SENSOR
  /*
  Serial.print("Analog reading exit: ");
  Serial.print(analogValueExit);   // the raw analog reading

  
  // We'll have a few threshholds, qualitatively determined
  
  if (analogValueExit < 10) {
    Serial.println(" - Dark");
  } else if (analogValueExit < 200) {
    Serial.println(" - Dim");
  } else if (analogValueExit < 500) {
    Serial.println(" - Light");
  } else if (analogValueExit < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }
  */

  if(analogValueEntry < 800 || analogValueExit < 800){
    Serial.println("Entry or exit: open the bar and leave it open");
    servo.write(110);
  } else{
    Serial.println("Leave the bar closed or close it");
    servo.write(150);
  }

  delay(500);



  //parking space 1
  int sensorOut1 = digitalRead(pir1);

  if(sensorOut1==LOW){
    digitalWrite(parking_led_1, LOW);
    Serial.println("Parking space 1 FREE");
  }
  else{
    digitalWrite(parking_led_1, HIGH);
    Serial.println("Parking space 1 BUSY");
  }
  // delay(1000);


  //parking space 2
  int sensorOut2 = digitalRead(pir2);

  if(sensorOut2==LOW){
    digitalWrite(parking_led_2, LOW);
    Serial.println("Parking space 2 FREE");
  }
  else{
    digitalWrite(parking_led_2, HIGH);
    Serial.println("Parking space 2 BUSY");
  }



  //parking space 3
  int sensorOut3 = digitalRead(pir3);

  if(sensorOut3==LOW){
    digitalWrite(parking_led_3, LOW);
    Serial.println("Parking space 3 FREE");
  }
  else{
    digitalWrite(parking_led_3, HIGH);
    Serial.println("Parking space 3 BUSY");
  }


  //parking space 4
  int sensorOut4 = digitalRead(pir4);

  if(sensorOut4==LOW){
    digitalWrite(parking_led_4, LOW);
    Serial.println("Parking space 4 FREE");
  }
  else{
    digitalWrite(parking_led_4, HIGH);
    Serial.println("Parking space 4 BUSY");
  }
}
