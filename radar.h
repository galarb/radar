#ifndef RADAR_H
#define RADAR_H

#include "stdint.h"
#include "Arduino.h"


const int DEFAULT_COLOR = 127;
const int NUM_PIXELS = 29;

class Radar {
      
  public:
    Radar(int echoPin, int trigPin, int servoRadarPin, int servoPin1, int servoPin2, int laserPin1, int laserPin2, int radarSpeed, int _cannonDistance);
    void begin(double bdrate); // start both Bluetooth and Serial at this Baud rate
    uint8_t getDistance(); // return the ultrasonic distance
    int getAngle(); // return the ultrasonic angle
    double calculatePID(double input, int setPoint);
    void setStripLEDColor(int ledNum, int red, int green, int blue);
    void setNeoPixelsColor(int red, int green, int blue);
    void scan();
    void shoot(int targetangle, int targetdistance);

    private:
    int _echoPin;
    int _trigPin;
    int _servoRadarPin;
    int _servoPin1;
    int _servoPin2;
    int _laserPin1;
    int _laserPin2;
    int _radarSpeed;
    uint8_t _radarAngle = 90;
    uint8_t _cannonDistance;

    void printInfo(int dist, int ang, int rightang, int leftang); // print internal info to serial monitor
    void ShowInfoLcd(int ang, int rightang, int leftang); // show error on LCD
    void showEncoderReadingOnLCD(int c, int o, int t); // show encoder reading on LCD
    double degtoradian(double degrees);
    double radiantodeg(double radians);

};
#endif
