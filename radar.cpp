#include <cmath>
//cpp code for radar class
#include "radar.h"
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

Radar::Radar(int echoPin, int trigPin, int servoRadarPin, int servoPin1, int servoPin2, int laserPin1, int laserPin2, int radarSpeed, int cannonDistance){
  _echoPin = echoPin;
  _trigPin = trigPin;
  _servoRadarPin = servoRadarPin;
  _servoPin1 = servoPin1;
  _servoPin2 = servoPin2;
  _laserPin1 = laserPin1;
  _laserPin2 = laserPin2;
  _radarSpeed = radarSpeed;
  _cannonDistance = cannonDistance;
  pinMode(_trigPin, OUTPUT); 
  pinMode(_echoPin, INPUT); 
  pinMode(_laserPin1, OUTPUT);
  pinMode(_laserPin2, OUTPUT);

  radarServo.attach(_servoRadarPin);
  servo1.attach(_servoPin1);
  servo2.attach(_servoPin2);
   
}

  
  

void Radar::setStripLEDColor (int lednum, int red, int green, int blue) {
      strip.setPixelColor(lednum, strip.Color(red, green, blue));
      strip.show();
      //strip.rainbow(0, 1, 255, 255, true);
      delay(50);
}
void Radar::setNeoPixelsColor (int red, int green, int blue) {
  for (int i = 0; i < 29; i++) {
      strip.setPixelColor (i, strip.Color(red, green, blue));
      // strip.rainbow(0, 1, 255, 255, true);
      strip.show();
      delay(_radarSpeed);
  }
}

void Radar::begin(double bdrate) {
  Serial.begin(bdrate);
  lcd.init();  
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Started - OK");
  
  Serial.print("Echo Pin = ");  
  Serial.println(_echoPin);
  Serial.print("Trig Pin = ");  
  Serial.println(_trigPin);
  Serial.print("Radar Servo Pin = ");  
  Serial.println(_servoRadarPin);
  Serial.print("Servo 1 Pin = ");  
  Serial.println(_servoPin1);
  Serial.print("Servo 2 Pin = ");  
  Serial.println(_servoPin2);
  Serial.print("Laser 1 Pin = ");  
  Serial.println(_laserPin1);
  Serial.print("Laser 2 Pin = ");  
  Serial.println(_laserPin2);
   
  strip.begin();
  strip.setBrightness(200); //(up to 255)
  strip.clear(); 
  setNeoPixelsColor (255, 0, 0); 
  previousTime = millis(); //otherwise the first Itegral value will be very high
  
  radarServo.setPeriodHertz(50);    // standard 50 hz servo
	radarServo.attach(_servoRadarPin, 1000, 2000); 
  servo1.setPeriodHertz(50);    
  servo1.attach(_servoPin1, 1000, 2000); 
	servo2.setPeriodHertz(50);
  servo2.attach(_servoPin2, 1000, 2000); 
}


double Radar::calculatePID(double inp, int sp){
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



uint8_t Radar::getDistance(){
  digitalWrite(_trigPin, LOW); //clears the US conditions
  delayMicroseconds(2);
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);
  duration = pulseIn(_echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return (distance);
}
int Radar::getAngle(){
    return (_radarAngle);
}

void Radar::showErrorOnLCD(int s, int e, int g){ //setpoint, error, gyro
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

void Radar::printInfo(char dirR, char dirL, int speedR, int speedL){
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

void Radar::scan(){
 for (int i = 0; i < 180; i++) {
  radarServo.write(i);
  delay(20);
  _radarAngle = i;
 }  
}

void Radar::shoot(int targetangle, int targetdistance){
  int angLaser1 = 180 - atan(targetdistance * sin(targetangle) / (_cannonDistance -  targetdistance *cos(targetangle)));
  int angLaser2 =  atan(targetdistance * sin(targetangle) / (_cannonDistance + targetdistance * cos(targetangle))); 
  servo1.write(angLaser1);
  servo2.write(angLaser2);
  digitalWrite(_laserPin1, HIGH);
  digitalWrite(_laserPin2, HIGH);
}

void Radar::pwmWrite(int pin1, int val1){
  if (pin1 == _servoRadarPin){
    ledcWrite(0, val1);
 }
 else if (pin1 == _servoPin1) {
  ledcWrite(1, val1);
 }
 else if (pin1 == _servoPin2){
  ledcWrite(2, val1);
 }
}