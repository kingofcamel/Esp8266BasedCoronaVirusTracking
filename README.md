# Esp8266 Covid Ticker mit i2c hd44780 20x4 LCD
# Bleibt zu hause

SCL D1
SDA D2
Display ID 0x27

Passendes Gehäuse zum Drucken
https://www.thingiverse.com/thing:72719
(Bilder folgen)

 1- Wlan Zugangsdaten in **WifiConnect.h** anpassen
  `char ssid[32] = "yourssid";`
  `char password[64] = "yourpass";`

 2- Land auswählen in **corona.ino** 
   `#define country_code "yourcountrycode"`
   
   GET (https://coronavirus-19-api.herokuapp.com/countries) -> all countries info
   
 3- Ich hoffe für alle das ich die Datenfelder viel zu gross gewählt habe
    Die Uhr läuft mit deutscher Sommerzeit. Auch da hoffe ich das das reicht ...
    Bleibt alle gesund
 
 ![alt text][logo]

[logo]: https://github.com/kingofcamel/Esp8266BasedCoronaVirusTracking/blob/master/91726393_3151714924839085_3326784480961626112_o.jpg "Logo Title Text 2"
