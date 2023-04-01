#ifndef WIFIGAL_H
#define WIFIGAL_H
#include "AdafruitIO_WiFi.h"


class Wifigal {
      
  public:
   Wifigal(bool status);
   void start();
   void run();
   void sendwifi(int ang, int dis);

  private:


};
#endif
