#include <Servo.h>

// #define parking_led_1 5
//inizialize micro servo 9g
Servo servo;
const int parking_led_1 = 4;
const int pir1 = 2; // input pin (IR LED parking space 1)


void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(parking_led_1, OUTPUT);
  pinMode(pir1, INPUT);

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


}
