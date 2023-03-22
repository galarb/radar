#include "stdint.h"
#ifndef RADAR_H
#define RADAR_H

#include "Arduino.h"


const int DEFAULT_COLOR = 127;
const int NUM_PIXELS = 29;

class Radar {
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

    void printInfo(char dirR, char dirL, int speedR, int speedL); // print internal info to serial monitor
    void showErrorOnLCD(int e, int g, int s); // show error on LCD
    void showEncoderReadingOnLCD(int c, int o, int t); // show encoder reading on LCD

  public:
    Radar(int echoPin, int trigPin, int servoRadarPin, int servoPin1, int servoPin2, int laserPin1, int laserPin2, int radarSpeed, int _cannonDistance);
    void begin(double bdrate); // start both Bluetooth and Serial at this Baud rate
    uint8_t getDistance(); // return the ultrasonic distance
    int getAngle(); // return the ultrasonic angle
    double calculatePID(double input, int setPoint);
    void setStripLEDColor(int ledNum, int red, int green, int blue);
    void setNeoPixelsColor(int red, int green, int blue);
    void pwmWrite(int pin, int value);
    void scan();
    void shoot(int targetangle, int targetdistance);
};
#endif
