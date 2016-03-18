/*
 * ESP8266 based remote controller power switch over Internet
 * Copyright (C) 2016 Jussi Salin <salinjus@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ESP8266WiFi.h>

// Remember to copy config_example.h to this name and make your changes!
#include "config.h"

/**
 * Arduino main init function. Connect WiFi and start debug serial port.
 * Also set up initial GPIO pin states.
 */
void setup()
{
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);

  pinMode(PIN_POWER_ON, OUTPUT);
  pinMode(PIN_POWER_OFF, OUTPUT);
  digitalWrite(PIN_POWER_ON, LOW);
  digitalWrite(PIN_POWER_OFF, LOW);

  Serial.println("Remote control started up.");
}

/**
 * Arduino main loop function. Fetch content if WiFi is connected,
 * then do an action based on the content.
 */     
void loop()
{
  // First, we need the WiFi connection established
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_POLLING_INTERVAL);
    Serial.println("Trying to connect to " + SSID);
    return;
  }

  // Second, we need TCP connection to the HTTP server
  WiFiClient client;
  if (!client.connect(HTTP_HOST, HTTP_PORT))
  {
    Serial.println("HTTP connection failed");
    delay(HTTP_POLLING_INTERVAL);
    return;
  }

  // Third, we need to GET content of the URL
  String url = "http://";
  url << HTTP_HOST;
  url << ":";
  url << HTTP_PORT;
  url << HTTP_PATH;

  Serial.println("Connected, fetching " + url);

  String request = "GET ";
  request << HTTP_PATH;
  request << " HTTP/1.1\r\nHost: ";
  request << HTTP_HOST;
  request << "\r\nConnection: close\r\n\r\n";

  client.print(request);

  // Last, perform an action based on the content
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    if (line.indexOf("true") >= 0)
    {
      power_control(true);
    }
    else if (line.indexOf("false") >= 0)
    {
      power_control(false);
    }
  }

  delay(HTTP_POLLING_INTERVAL);
}

/**
 * Set power on or off, based on the parameter.
 */
void power_control(bool value)
{
  Serial.println("Turning power to state: " + value);

  // Choose GPIO pin - if to turn power on or off
  int pin = PIN_POWER_OFF;
  if (value == true) pin = PIN_POWER_ON;

  // "Press" the remote button three times for reliability
  for (int attempt=0; i<3; i++)
  {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(4000);
  }
}
