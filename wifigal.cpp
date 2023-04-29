#include "AdafruitIO_Feed.h"
#include "WString.h"
#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "AdafruitIO_WiFi.h"
#include "Wifigal.h"

#define ssid "arbs"   
#define pass "lafamilia"

#define AIO_USERNAME  "galarb"            //personal info from adafruit.io
#define AIO_KEY       "aio_pAYD82KJpGasJG6voCrZ0M2fT3l0"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  8883     

WiFiClientSecure client;  // WiFi ClientSecure for SSL/TLS support

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, ssid, pass);

// io.adafruit.com root CA
 const char* adafruitio_root_ca = \   
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
    "-----END CERTIFICATE-----\n";


AdafruitIO_Feed *radarAng = io.feed("Radar Angle");
AdafruitIO_Feed *radarDistance = io.feed("Radar Distance");

Wifigal::Wifigal(bool x){}

void Wifigal::start(){
   WiFi.softAP("Radar");
   WiFi.begin(ssid, pass);
   Serial.println("Connecting");
   if(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print("NOT CONNECTED");
    delay(250);
   }   
   Serial.println("");
   Serial.print("Connected to WiFi network with IP Address: ");
   Serial.println(WiFi.localIP());
   io.connect();
  // wait for a connection
   while(io.status() < AIO_CONNECTED) {
     Serial.print(".");
     delay(500);
  }
   client.setCACert(adafruitio_root_ca);  // Set Adafruit IO's root CA
   Serial.println("Connected to Adafruit IO");
  
 }
void Wifigal::run(){
  io.run();
}

void Wifigal::sendwifi(int ang, int dis){
  Serial.print("sending -> ");
  Serial.print(ang);

  radarAng->save(ang);
  radarDistance->save(dis);
  delay(2000);
}



