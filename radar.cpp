#include "stdint.h"
#include "esp32-hal-ledc.h"
//cpp code for radar class
#include "esp32-hal-gpio.h"
#include "radar.h"
#include "Arduino.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "HardwareSerial.h"
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <ESP32Servo.h>
unsigned long currentTime;
unsigned long previousTime;
double elapsedTime;
double error;
double lastError;
double input, output;
double cumError, rateError;
double kp = 0;
double ki = 0; 
double kd = 0;
int setcolor = 127; //default value for 50% white 50% Black
bool flag = true;
int steps = 0;
//const byte rxPin = 11;
//const byte txPin = 13;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
String cmd="";


LiquidCrystal_I2C lcd(0x27,16,2);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(29, 1, NEO_GRB + NEO_KHZ800);
Servo radarServo; //Scan Radar
Servo servo1;     //Right Cannon
Servo servo2;     //Left Cannon

radar::radar(int EchoPin, int TrigPin, int ServoPin1, int ServoRadarPin, int ServoPin2, int LaserPin1, int LaserPin2) {
  echoPin = EchoPin;
  trigPin = TrigPin;
  servoRadarPin = ServoRadarPin;
  servoPin1 = ServoPin1;
  servoPin2 = ServoPin2;
  laserPin1 = LaserPin1;
  laserPin2 = LaserPin2;
  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(laserPin1, OUTPUT);
  pinMode(laserPin2, OUTPUT);
  
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep

}

void radar::stripled (int lednum, int red, int green, int blue) {
      strip.setPixelColor(lednum, strip.Color(red, green, blue));
      strip.show();
      strip.rainbow(0, 1, 255, 255, true);
      delay(50);
}
void radar::neopixels (int red, int green, int blue) {
  for (int i = 0; i < 29; i++) {
      strip.setPixelColor (i, strip.Color(red, green, blue));
      strip.rainbow(0, 1, 255, 255, true);

      strip.show();
      delay(50);
  }
}

void radar::begin(double bdrate) {
  Serial.begin(bdrate);
  lcd.init();  
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Started - OK");
  
  Serial.print("Echo Pin = ");  
  Serial.println(echoPin);
  Serial.print("Trig Pin = ");  
  Serial.println(trigPin);
  Serial.print("Radar Servo Pin = ");  
  Serial.println(servoRadarPin);
  Serial.print("Servo 1 Pin = ");  
  Serial.println(servoPin1);
  Serial.print("Servo 2 Pin = ");  
  Serial.println(servoPin2);
  Serial.print("Laser 1 Pin = ");  
  Serial.println(laserPin1);
  Serial.print("Laser 2 Pin = ");  
  Serial.println(laserPin2);
   
  strip.begin();
  strip.setBrightness(200); //(up to 255)
  strip.clear(); 
  neopixels (255, 0, 0); 
  previousTime = millis(); //otherwise the first Itegral value will be very high
  
  radarServo.setPeriodHertz(50);    // standard 50 hz servo
	radarServo.attach(servoRadarPin, 1000, 2000); 
  servo1.setPeriodHertz(50);    
  servo1.attach(servoPin1, 1000, 2000); 
	servo2.setPeriodHertz(50);
  servo2.attach(servoPin2, 1000, 2000); 
}


double radar::PIDcalc(double inp, int sp){
   currentTime = millis();                //get current time
   elapsedTime = (double)(currentTime - previousTime)/1000; //compute time elapsed from previous computation (60ms approx). divide in 1000 to get in Sec
   //Serial.print(currentTime); //for serial plotter
   //Serial.println("\t"); //for serial plotter
   error = sp - inp;                                  // determine error
   cumError += error * elapsedTime;                   // compute integral
   rateError = (error - lastError)/elapsedTime;       // compute derivative deltaError/deltaTime
   double out = kp*error + ki*cumError + kd*rateError; //PID output               
   //Serial.println(cumError);
   lastError = error;                                 //remember current error
   previousTime = currentTime;                        //remember current time
   if(out > 254){out = 254;}    //limit the function for smoother operation
   if(out < -254){out = -254;}
   if(cumError > 255 || cumError < -255){cumError = 0; out = 0;} // reset the Integral commulator
   if(rateError < 0.3 || rateError > -0.3){cumError = 0;}             // reset the Integral commulator
   return out;                                        //the function returns the PID output value 
  
}



uint8_t radar::getDis(){
  digitalWrite(trigPin, LOW); //clears the US conditions
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return (distance);
}
int radar::getAng(){
    return (radarangle);
}

void radar::lcdershow(int s, int e, int g){ //setpoint, error, gyro
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SP | PID | gyro");
  lcd.setCursor(0,1);
  lcd.print(s);  
  lcd.setCursor(3,1);
  lcd.print("|"); 
  lcd.setCursor(5,1);
  lcd.print(e); 
  lcd.setCursor(9,1);
  lcd.print("|");  
  lcd.setCursor(11,1);
  lcd.print(g); 
}

void radar::printg(char dirR, char dirL, int speedR, int speedL){
  Serial.println("-------------------------------------");
  Serial.println("               | Direction |  Speed");
    Serial.print("Right Motor    |    ");
  Serial.print(dirR);
  Serial.print("      |  ");
  Serial.println(speedR);
  Serial.print("Left Motor     |    ");
  Serial.print(dirL);
  Serial.print("      |  ");
  Serial.println(speedL);
}

void radar::scan(){
 for (int i = 0; i < 180; i++) {
  radarServo.write(i);
  radarangle = i;
 }  
}

void radar::shoot(int ang){
  servo1.write(ang);
  servo2.write(ang);
  digitalWrite(laserPin1, HIGH);
  digitalWrite(laserPin2, HIGH);
}
void radar::pwmWrite(int pin1, int val1){
  if (pin1 == servoRadarPin){
    ledcWrite(0, val1);
 }
 else if (pin1 == servoPin1) {
  ledcWrite(1, val1);
 }
 else if (pin1 == servoPin2){
  ledcWrite(2, val1);
 }
}