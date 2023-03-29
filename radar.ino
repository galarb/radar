/*  *******
               Radar Software for esp32
  Featuring Dual Core functionality
  Supports: LCD 16x02
            HC04 Ultrasonic Sensor


  By: Gal Arbel
  April   2023    
          
*/


#include "radar.h"
TaskHandle_t Task1;
TaskHandle_t Task2;

//echoPin, trigPin, servoRadarPin, servoPin1, servoPin2, laserPin1, laserPin2, radarSpeed, cannonn distance in cm
Radar myradar(5, 14, 33, 2, 4, 32, 12, 20, 20);

void setup() {
  myradar.begin(115200);

  xTaskCreatePinnedToCore(
                    loop_0,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
   delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    loop_1,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
   

}

void loop_0 ( void * pvParameters ){
  Serial.print("loop_0 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){      //this is an infinite loop on Core 0
   myradar.scan();
   //delay(30);

  }
}
void loop_1 ( void * pvParameters ){
  Serial.print("loop_1 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){      //this is an infinite loop on Core 1
    int targetdistance = myradar.calculateDist();
    if(targetdistance < 20){
     int targetangle = myradar.getAngle();
     myradar.shoot(targetangle, targetdistance);
     delay(1); 
  }
 }
}
void loop() {
//Must be left EMPTY2
}