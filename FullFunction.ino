/***************************************************
 DFPlayer - A Mini MP3 Player For ESP Boards
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 Modified to work with ESP32/ESP8266 using hardware serial.
 ****************************************************/

#include "Arduino.h"
#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use HardwareSerial (ESP32/ESP8266)
HardwareSerial mySerial(1); // Use Serial1 for communication
DFRobotDFPlayerMini myDFPlayer;

void printDetail(uint8_t type, int value);

void setup() {
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17 (adjust pins as needed)
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySerial)) {  // Use hardware serial to communicate with MP3
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); // Set serial communication timeout to 500ms

  // ---- Set volume ----
  myDFPlayer.volume(30);  // Set volume value (0~30)

  // ---- Set different EQ ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  // ---- Set device ----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  // ---- Play an MP3 ----
  myDFPlayer.play(2);  // Play the first MP3
}

void loop() {
  static unsigned long timer = millis();

  if (millis() - timer > 20000) {
    timer = millis();
    myDFPlayer.next();  // Play next MP3 every 3 seconds
  }

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); // Handle errors and states
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
