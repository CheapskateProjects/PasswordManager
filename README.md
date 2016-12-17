# PasswordManager
Arduino based personal password manager which reads passwords from SD-card and uses OLED display to help select the right password to insert. Password is inserted by arduino simulating keyboard.

This code tries to read file PWDS.TXT from the SD-card. If such file exists then it should contain one service name ( or username) and password on each line separated by semicolon. 

Service names are presented on OLED display one at a time when next button is pressed. When one of the service names is selected then login to that service may be done by selecting password input field and pressing password enter button. 

Dependencies: 
<https://github.com/adafruit/Adafruit_SSD1306>
<https://github.com/adafruit/Adafruit-GFX-Library>
