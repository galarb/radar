/**************************************************************************

            Radar Control of laser beams software
						
			      			 	 Using: Gyro MPU-6050
                            LCD 16x2
                            HC-06 Bluethooth
                            LM298N Motor Driver
                            hc-sr04 Ultrasonic module
                            LM393 Optical encoder
                            Reflected light Sensor (line tracking)
                            NRF24L01 Radio (NOTE: Must have capacitor soldered on VCC/GND)
               
      by Gal Arbel
      2022-2023
      
 NRF24L01 connected to default SPI pins (CLK, MISO, MOSI = 13, 12, 11) and pins 9, 10 (CE, CSN)

****************************************************************************/

#ifndef RADAR_H
#define RADAR_H
 
 class radar {
  private:
   void lcdershow(int e, int g, int s); //show error 
   void lcdenshow(int c, int o, int t); //show encoder 
   int echoPin = 0;
   int trigPin = 0;
   int servoRadarPin = 0;
   int servoPin1 = 0;
   int servoPin2 = 0;
   int laserPin1 = 0;
   int laserPin2 = 0;
   int encoderPin = 0;
   uint8_t radarangle = 90;

   void printg(char dirR, char dirL, int speedR, int speedL);//internal info printing procedure

  public:
   radar(int EchoPin, int TrigPin, int servoRadarPin, int ServoPin1, int ServoPin2, int LaserPin1, int LaserPin2);
   void begin(double bdrate); //will start both Bluetooth and Serial at this Baud rate
   uint8_t getDis(); //return the Ultrasonic distance
   int getAng(); //return the Ultrasonic distance
   double PIDcalc(double inp, int sp);
   void stripled (int lednum, int red, int green, int blue);
   void neopixels (int red, int green, int blue);
   void pwmWrite(int pin1,int val1);
   void scan();
   void shoot(int ang);
 };
#endif 