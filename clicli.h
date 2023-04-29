#ifndef CLICLI_H
#define CLICLI_H
#include "Radar.h"

 class clicli {
  private:
  Radar &myradar;
  int number;
  
  public:
   clicli(Radar &myradar);
   void begin();   //must be called from  void setup()
   void run();   //must be called from  void loop()

 };
#endif 