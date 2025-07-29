/*
 * base: Simple example of text alignment (by padding) - https://github.com/road-t/DM8BA10
 * NTP clock on LCD DM8BA01 display
 * Nicu FLORICA (niq_ro) created this NTP clock based on info from  https://thesolaruniverse.wordpress.com/2022/11/01/an-internet-synced-clock-circular-display-with-gc9a01-controller-powered-by-an-esp8266/
 * v.0 - initial version for NTP clock
 */

#include <DM8BA10.h>  // https://github.com/road-t/DM8BA10
#include <charset/latin_basic.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid =            "bbk2";                          // network wifi credentials  - fill in your wifi network name
const char* password =        "internet2";                     // network wifi credentials  - fill in your wifi key

const long utcOffsetInSeconds = 3*3600;                        // 3600 = western europe winter time - 7200 = western europe summer time
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// pins for Wemos D1 Mini (ESP8266)
#define CS_PIN        D8 //9
#define WR_PIN        D5 //8
#define DATA_PIN      D7 //7
#define BACKLIGHT_PIN 0  //6

// display settings
#define PLACES_COUNT 10
#define REFRESH_RATE 1000 // display update speed in ms

DM8BA10* LCD;

const DM8BA10::Padding alignment[] = { DM8BA10::Padding::Right, DM8BA10::Padding::Left, DM8BA10::Padding::Both };
volatile byte curAlignmentIndex = 0;
String titlu = " NTP clock";

  uint32_t targetTime = 0;                                                            // for next 1 second timeout

   int hh = 0;                                                                         // hours variable
   int mm = 0;                                                                         // minutes variable
   int ss = 0;                                                                         // seconds variable

   WiFiUDP ntpUDP;                                                                     // define NTP client to get time
   NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);  

bool initial = 1;

void setup()
{
   Serial.begin (9600);
   Serial.println ();
   Serial.println ();
   WiFi.begin (ssid, password);

   while (WiFi.status() != WL_CONNECTED ) 
      {
      delay (500);
      Serial.print (".");
      }
   Serial.print ("connection with ");
   Serial.println (ssid);  
   Serial.println ("-------------------------------"); 
   
   timeClient.begin();
   timeClient.update ();
   Serial.print ("internet server time: ");   
   Serial.println(timeClient.getFormattedTime());
  
  LCD = new DM8BA10(new LatinBasicCharset(), CS_PIN, WR_PIN, DATA_PIN, -1);
//  LCD->backlight();

 LCD->println(titlu, alignment[curAlignmentIndex]);
 delay(5000);

   hh = timeClient.getHours ();
   mm = timeClient.getMinutes ();
   ss = timeClient.getSeconds ();
   targetTime = millis();
}

void loop()
{
   if (targetTime < millis())
      {
      targetTime += 1000;
      ss++;                                                                            // advance second
      if (ss==60)
         {
         ss=0;
         mm++;                                                                         // advance minute
         if(mm>59)
            {
            mm=0;
            hh++;                                                                      // advance hour
            if (hh>23) 
               {
               hh=0;
               timeClient.update ();                                                   // update at midnight
               }
            }
         }
      String ora = " ";
  if (hh/10 == 0)
    ora = ora + " ";
   else
    ora = ora + hh/10;
  ora = ora + hh%10;
  if (ss%2 == 0)
    ora = ora + "-";
   else
    ora = ora + " ";
  ora = ora + mm/10;
  ora = ora + mm%10;
   if (ss%2 == 0)
    ora = ora + "-";
   else
    ora = ora + " ";
  ora = ora + ss/10;
  ora = ora + ss%10; 

  LCD->println(ora, alignment[curAlignmentIndex]);      
      }

delay(10);

  
} // end main loop
