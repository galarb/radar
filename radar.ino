//Radar Software for esp32
//Featuring Dual Core functionality
#include "radar.h"
TaskHandle_t Task1;
TaskHandle_t Task2;
radar myradar(1,2,3,4,5,6,7);

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

   

  }
}
void loop_1 ( void * pvParameters ){
  Serial.print("loop_1 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){      //this is an infinite loop on Core 1
   //Serial.println("Testing loop_1");
   delay(1000);
   if(myradar.getDis() < 100){
    Serial.print("target intercepted at angle: ");
    int targetangle = myradar.getAng();
    Serial.println(targetangle);
    myradar.shoot(targetangle);

   }
  }
}

void loop() {
//Must be left EMPTY
}