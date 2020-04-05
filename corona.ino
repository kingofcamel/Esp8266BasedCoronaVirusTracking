#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include "json_parser.h"
#include "WifiConnect.h"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);
#define s2ms(second) (second*1000)
unsigned long long prev_millis(0);

#define country_code "Germany"

LiquidCrystal_I2C lcd(0x27, 20, 4);

int interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
bool bFirstKickMillis = false;

extern bool bGotIpFlag;
static String build_url_from_country(String country)
{
  String url = "http://coronavirus-19-api.herokuapp.com/countries/";
  url = url + country;
  return url;
}

void setup(void)
{
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Covid19  DE  Display");
  timeClient.begin();
#if defined JSON_DEBUG
  Serial.begin(9600);
#endif

  JSON_LOG("Connecting...");
  vConnWifiNetworkViaSdk();
}

void loop()
{
  if (bGotIpFlag) bGotIp();
  if (bFirstKickMillis) CurrentMillis = millis();
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval)
  {
    if (!bFirstKickMillis) CurrentMillis = 0;
    bFirstKickMillis = true;
    PreviousMillis = CurrentMillis;
    HTTPClient http;
    http.begin(build_url_from_country(country_code));
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String payload = http.getString();
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      payload.toCharArray(JsonArray, payload.length() + 1);

      JSON_LOG(JsonArray);
      timeClient.update();
      if (json_validate(JsonArray))
      {
        int cases = (int)get_json_value(JsonArray, "cases", INT);
        int todayCases = (int)get_json_value(JsonArray, "todayCases", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int todayDeaths = (int)get_json_value(JsonArray, "todayDeaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
        int active1 = (int)get_json_value(JsonArray, "active", INT);
        int critical = (int)get_json_value(JsonArray, "critical", INT);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alle");
        lcd.setCursor(14, 0);
        lcd.print("+");
        lcd.setCursor(0, 1);
        lcd.print("Tote");
        lcd.setCursor(14, 1);
        lcd.print("+");
        lcd.setCursor(0, 3);
        lcd.print("OK");
        lcd.setCursor(0, 2);
        lcd.print("Akut");
        lcd.setCursor(13, 2);
        lcd.print("IC");
        lcd.setCursor(15, 3);
        if (timeClient.getHours() < 10) lcd.print(" ");
        lcd.print(timeClient.getHours());
        lcd.print(":");
        if (timeClient.getMinutes() < 10) lcd.print("0"); 
        lcd.print(timeClient.getMinutes());

        lcd.setCursor(4, 0);
        if (cases < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (cases < 100) lcd.print(" "); // ein leerzeichen
        if (cases < 1000) lcd.print(" "); // ein leerzeichen
        if (cases < 10000) lcd.print(" "); // ein leerzeichen
        if (cases < 100000) lcd.print(" "); // ein leerzeichen
        if (cases < 1000000) lcd.print(" "); // ein leerzeichen
        if (cases < 10000000) lcd.print(" "); // ein leerzeichen
        lcd.print(cases);

        lcd.setCursor(15, 0);
        if (todayCases < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (todayCases < 100) lcd.print(" "); // ein leerzeichen
        if (todayCases < 1000) lcd.print(" "); // ein leerzeichen
        if (todayCases < 10000) lcd.print(" "); // ein leerzeichen
        lcd.print(todayCases);

        lcd.setCursor(5, 1);
        if (deaths < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (deaths < 100) lcd.print(" "); // ein leerzeichen
        if (deaths < 1000) lcd.print(" "); // ein leerzeichen
        if (deaths < 10000) lcd.print(" "); // ein leerzeichen
        if (deaths < 100000) lcd.print(" "); // ein leerzeichen
        if (deaths < 1000000) lcd.print(" "); // ein leerzeichen
        lcd.print(deaths);

        lcd.setCursor(15, 1);
        if (todayDeaths < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (todayDeaths < 100) lcd.print(" "); // ein leerzeichen
        if (todayDeaths < 1000) lcd.print(" "); // ein leerzeichen
        if (todayDeaths < 10000) lcd.print(" "); // ein leerzeichen
        lcd.print(todayDeaths);

        lcd.setCursor(4, 3);
        if (recovered < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (recovered < 100) lcd.print(" "); // ein leerzeichen
        if (recovered < 1000) lcd.print(" "); // ein leerzeichen
        if (recovered < 10000) lcd.print(" "); // ein leerzeichen
        if (recovered < 100000) lcd.print(" "); // ein leerzeichen
        if (recovered < 1000000) lcd.print(" "); // ein leerzeichen
        if (recovered < 10000000) lcd.print(" "); // ein leerzeichen
        lcd.print(recovered);

        lcd.setCursor(4, 2);
        if (active1 < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (active1 < 100) lcd.print(" "); // ein leerzeichen
        if (active1 < 1000) lcd.print(" "); // ein leerzeichen
        if (active1 < 10000) lcd.print(" "); // ein leerzeichen
        if (active1 < 100000) lcd.print(" "); // ein leerzeichen
        if (active1 < 1000000) lcd.print(" "); // ein leerzeichen
        if (active1 < 10000000) lcd.print(" "); // ein leerzeichen
        lcd.print(active1);

        lcd.setCursor(15, 2);
        if (critical < 10) lcd.print(" "); // ein leerzeichen  oder auch Zahl Null
        if (critical < 100) lcd.print(" "); // ein leerzeichen
        if (critical < 1000) lcd.print(" "); // ein leerzeichen
        if (critical < 10000) lcd.print(" "); // ein leerzeichen
        lcd.print(critical);

      }

      free(JsonArray);
    }

    http.end();
  }
}
