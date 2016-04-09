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

#define LED_PIN   0     // Status LED on break out board
#define LED_OK    0     // Full brightness when HTTP control is OK
#define LED_WIFI  991   // Dimmer light when only WIFI is OK
#define LED_OFF   1023  // Off otherwise

/**
 * Set power on or off, based on the parameter.
 */
void power_control(bool value)
{
  Serial.print("Turning power to state: ");
  Serial.println(value);

  // Choose GPIO pin - if to turn power on or off
  int pin = PIN_POWER_OFF;
  if (value == true) pin = PIN_POWER_ON;

  pinMode(PIN_POWER_ON, OUTPUT);
  pinMode(PIN_POWER_OFF, OUTPUT);
  digitalWrite(PIN_POWER_ON, LOW);
  digitalWrite(PIN_POWER_OFF, LOW);
  delay(100);
  
  // "Press" the remote button three times for reliability
  for (int attempt=0; attempt<3; attempt++)
  {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(4000);
  }

  pinMode(PIN_POWER_ON, INPUT);
  pinMode(PIN_POWER_OFF, INPUT);
}

/**
 * Arduino main init function. Connect WiFi and start debug serial port.
 * Also set up initial GPIO pin states.
 */
void setup()
{
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);

  pinMode(LED_PIN, OUTPUT);

  Serial.println("Remote control started up.");
}

/**
 * Arduino main loop function. Fetch content if WiFi is connected,
 * then do an action based on the content.
 */     
void loop()
{
  analogWrite(LED_PIN, LED_OFF);
  
  // First, we need the WiFi connection established
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_POLLING_INTERVAL);
    Serial.print("Trying to connect to ");
    Serial.println(SSID);
    return;
  }

  analogWrite(LED_PIN, LED_WIFI);

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
  url += HTTP_HOST;
  url += ":";
  url += HTTP_PORT;
  url += HTTP_PATH;

  Serial.print("Connected, fetching ");
  Serial.println(url);

  String request = "GET ";
  request += HTTP_PATH;
  request += " HTTP/1.1\r\nHost: ";
  request += HTTP_HOST;
  request += "\r\nConnection: close\r\n\r\n";

  Serial.println(request);
  client.print(request);
  delay(HTTP_TIMEOUT);

  // Last, perform an action based on the content
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    if (line.indexOf("true") >= 0)
    {
      analogWrite(LED_PIN, LED_OK);
      power_control(true);
    }
    else if (line.indexOf("false") >= 0)
    {
      analogWrite(LED_PIN, LED_OK);
      power_control(false);
    }
  }

  Serial.println("Sleeping before next attempt...");
  delay(HTTP_POLLING_INTERVAL);
}
