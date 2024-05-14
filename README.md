Lightninig Piggy - Alby Proof of Concept
====

Bitcoin Lightning piggybank using Alby API running on TTGO LilyGo T5 ePaper 2.13 inch DEPG0213BN display ePaper hardware with ESP32 microcontroller.

See https://www.lightningpiggy.com/ and https://makers.bolt.fun/project/lightningpiggy

Main Source code
===========

**Tested with:**

- Arduino IDE version 1.8.13
- ESP32 Board Support version 2.0.14
- Preferences -> Compiler warnings: Default
- Tools -> Board -> ESP32 Arduino -> ESP32 Dev Module
- Tools -> Upload Speed: 921600
- Tools -> CPU Frequency: 240Mhz
- Tools -> Flash Frequency: 80Mhz
- Tools -> Flash Mode: QIO
- Tools -> Flash Size: 4MB (32Mb)
- Tools -> Partition Scheme: Default 4MB with spiffs (1.2MB APP, 1.5MB SPIFFS)
- Tools -> Core Debug Level: Warn
- Tools -> PSRAM: Disabled
- Tools -> Port: /dev/ttyACM0

Make sure the Arduino IDE has permissions to access the serial port:

`sudo chmod -f 777 /dev/ttyACM* /dev/ttyUSB*`

Make sure the following Arduino libraries are installed

- GxEPD by Jean-Marc Zingg
- ArduinoJson by Benoît Blanchon

**On the Alby developer page:**
Request a new API key by https://getalby.com/developer/access_tokens/new

- Give permissions for account:read, invoices:read, balance:read
- Copy Your Access Token and paste it in the YOUR_ALBY_API_KEY_HERE setting

**Todo**
- Convert LNURLP to Bech32
- Show QR code of LNURLP
- Merge with lightning piggy core code
