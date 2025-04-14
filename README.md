# VKprop_v2
Version 2 of my Voight-Kampff themed TTRPG prop.
The original project is here: https://github.com/TechDocN/VKprop

I'm improving the overall build and adding an automated camera stalk and bellows.

---

What is it made from?
- 1 Arduino Nano
- 2 Seeeduino Xiao
- 1 LCD display, with built-in microSD card reader
- 2 OLED displays, 0.96" 128x64
- 4 momentary push buttons
- 1 SPST rocker switch
- 1 passive buzzer
- 1 9V rechargeable battery
- 1 9V battery connector
- 3D printing filament of your choice (I used dark gray PLA)
- Optional red and green LEDs

---

How is everything wired together?
- Nano to LCD
- Nano to microSD
- Nano to Xiao x2
- Nano to buttons
- Nano to buzzer
- Xiao x2 to OLED x2

---

NOTE:
The BMP image rendering routine was adapted from the spitftbitmap.ino example by Adafruit Industries, part of the Adafruit ST7735 library.
Used under the MIT License.

---

LICENSES:
The software and content of this project is licensed under the Creative Commons Attribution Share Alike 4.0 International, and the hardware and hardware design elements are licensed under the CERN Open Hardware Licence Version 2 - Strongly Reciprocal.
