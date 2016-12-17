/*
  PasswordManager

  This code tries to read file PWDS.TXT from the SD-card. If such file exists then it should contain one service name ( or username) and password 
  on each line separated by semicolon. 

  service names are presented on OLED display one at a time when next button is pressed. When one of the service names is selected then
  login to that service may be done by selecting password input field and pressing password enter button. 

  created   Dec 2016
  by CheapskateProjects
  ---------------------------
  Dependencies: 
  https://github.com/adafruit/Adafruit_SSD1306
  https://github.com/adafruit/Adafruit-GFX-Library
  
  ---------------------------
  The MIT License (MIT)

  Copyright (c) 2016 CheapskateProjects

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

// Define ports for oled
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Define control pins
const int nextPin = 2;
const int enterPin = 7;

// Few globals
File myFile;
String key = "";
String value = "";

void setup()
{
  // Delay at the begining is not required but it helps somewhat with reprogramming. Sometimes Arduino gets somewhat unstabile with screen and card reader. 
  delay(5000);

  // Initialize SD
  if (!SD.begin(4))
  {
    return;
  }
  myFile = SD.open("PWDS.TXT");

  // Initialize Pins
  pinMode(nextPin, INPUT);
  pinMode(enterPin, INPUT);

  // Initialize Display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(2000);
}

void loop()
{
  // If we have pressed enter button to enter the selected password
  int enterState = digitalRead(enterPin);
  if(enterState == HIGH)
  {
    Keyboard.begin();
    Keyboard.println(value.c_str());
    delay(1000);
  }

  // If we have pressed next button to scroll to next password
  int nextState = digitalRead(nextPin);
  if (nextState == HIGH)
  {
    key = "";
    value = "";
    bool inkey = true;
    while (myFile.available())
    {
      char c = (char)myFile.read();
      if(c == '\n')
      {
        if(key == "")
        {
          key = "";
          value = "";
          if(!myFile.available())
          {
            myFile.close();
            myFile = SD.open("PWDS.TXT");
          }
          continue;  
        }
        
        inkey = true;

        display.clearDisplay();
        display.setTextSize(3);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(key.c_str());
        display.display();

        delay(500);
        return;
      }
      else
      {
        if(inkey)
        {
          if(c == ';')
          {
            inkey = false; 
          }
          else
          {
            key += c;
          }
        }
        else
        {
          value += c;
        }
      }

      if(!myFile.available())
      {
        myFile.close();
        myFile = SD.open("PWDS.TXT");
      }
    }
  }
}
