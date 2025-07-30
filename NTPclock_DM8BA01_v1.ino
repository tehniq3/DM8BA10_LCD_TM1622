/*
 * base: Simple example of text alignment (by padding) - https://github.com/road-t/DM8BA10
 * NTP clock on LCD DM8BA01 display
 * Nicu FLORICA (niq_ro) created this NTP clock based on info from  https://thesolaruniverse.wordpress.com/2022/11/01/an-internet-synced-clock-circular-display-with-gc9a01-controller-powered-by-an-esp8266/
 * v.0 - initial version for NTP clock
 * v.1 - added date as scroll text, date requested as at https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
 */

#include <DM8BA10.h>  // https://github.com/road-t/DM8BA10
#include <charset/latin_basic.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid =            "bbk2";                          // network wifi credentials  - fill in your wifi network name
const char* password =        "internet2";                     // network wifi credentials  - fill in your wifi key

const long utcOffsetInSeconds = 3*3600;                        // 3600 = western europe winter time - 7200 = western europe summer time
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int monthDay;
String weekDay;
int currentMonth;
String currentMonthName;
int currentYear;
String currentDate;

// pins for Wemos D1 Mini (ESP8266)
#define CS_PIN        D8 //9
#define WR_PIN        D5 //8
#define DATA_PIN      D7 //7
#define BACKLIGHT_PIN 0  //6

// display settings
#define PLACES_COUNT 10
#define REFRESH_RATE 1000 // display update speed in ms
#define ics 500 // 

DM8BA10* LCD;

const DM8BA10::Padding alignment[] = { DM8BA10::Padding::Right, DM8BA10::Padding::Left, DM8BA10::Padding::Both };
volatile byte curAlignmentIndex = 2;
String titlu  = "          NTP clock on LCD display v.1 by niq_ro";
//String titlu1 = "update";

  uint32_t targetTime = 0;                                                            // for next 1 second timeout

   int hh = 0;                                                                         // hours variable
   int mm = 0;                                                                         // minutes variable
   int ss = 0; 
   int ss1, ss2, ss3;

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

  for (int i = 0; i <= titlu.length(); i++) 
  {
  LCD->scroll(titlu, i);
  delay(ics);
  }
 
// delay(1000);
//  LCD->println(titlu1, alignment[curAlignmentIndex]);
  
   timeClient.begin();
   timeClient.update ();

   Serial.print ("internet server time: ");   

   time_t epochTime = timeClient.getEpochTime();
   Serial.print("Epoch Time: ");
   Serial.println(epochTime);
   
   Serial.println(timeClient.getFormattedTime());
 //  Serial.print("Formatted Time: ");
 //  Serial.println(formattedTime);  

   hh = timeClient.getHours ();
   mm = timeClient.getMinutes ();
   ss = timeClient.getSeconds ();
   Serial.print(hh);
   Serial.print(":");
   Serial.print(mm);
   Serial.print(":");
   Serial.println(ss);
   
   weekDay = weekDays[timeClient.getDay()];
   Serial.print("Week Day: ");
   Serial.println(weekDay);  

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

delay(1000);
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
               timeClient.update ();   // update at midnight 
      
   time_t epochTime = timeClient.getEpochTime();
   Serial.print("Epoch Time: ");
   Serial.println(epochTime);
   
   hh = timeClient.getHours ();
   mm = timeClient.getMinutes ();
   ss = timeClient.getSeconds ();
   Serial.print(hh);
   Serial.print(":");
   Serial.print(mm);
   Serial.print(":");
   Serial.println(ss);
   
   weekDay = weekDays[timeClient.getDay()];
   Serial.print("Week Day: ");
   Serial.println(weekDay);  

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);                        
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

  String data = "          ";
  data = data + weekDay;
  data = data + ", ";
  data = data + monthDay;
  data = data + "-";
  data = data + currentMonthName;
  data = data + "-";
  data = data + currentYear;
  data = data + " ";

  LCD->println(ora, alignment[curAlignmentIndex]);  

if (ss == 36)
 {
Serial.print("ss = ");
Serial.print(ss);
ss2 = millis()/1000;
//Serial.print(", ss2 = ");
//Serial.print(ss2);

  for (int i = 0; i <= data.length(); i++) 
  {
  LCD->scroll(data, i);
  delay(ics);
  }
 }
 }

delay(10); 
} // end main loop
