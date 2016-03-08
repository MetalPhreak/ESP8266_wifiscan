# ESP8266_wifiscan
Simple Wifi AP Scanner

Flash onto ESP8266. Connect serial console/terminal on 76800baud. Device will continuously scan for available APs and dump the raw stats.

Auth stat is as per AUTH_MODE enum from SDK:
00 = No Auth (Open)
01 = WEP
02 = WPA
03 = WPA2
04 = WPA/WPA2

Compiled firmware is available in the firmware directory. Flash with esptool.
