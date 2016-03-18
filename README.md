ESP8266 Power Switch
====================

Use ESP8266 WiFi MCU to control a remote controlled mains power switch over Internet.

Copyright (C) 2016 Jussi Salin <salinjus@gmail.com> under GPLv3.


Hardware
--------

Adafruit ESP8266 break-out board connected to a remote controller of the remote controller mains power switch.

https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/

    Wiring:
    
    ESP8266            Remote
    
     3V o------------- Battery+ (do NOT insert battery)
    
    GND o------------- Battery- (do NOT insert battery)
    
     #4 o------------- Parallel of turn ON button
    
     #5 o------------- Parallel of turn OFF button

The remote controller is ran by 3V instead of whatever battery it came it, for example, a 12V battery. Three volts seems to be enough to get distance of several meters still though. This way there is no need for any level converter on the two switch pins.


Software
--------

Firmware has a hard coded URL and SSID + KEY configured in a include file. They are used to fetch content "true" or "false" which determines what action to take. Controlling is done by simply changing content of the file on a web server of choice. This kind of polling client approach is useful when the switch is located behind a NAT.
