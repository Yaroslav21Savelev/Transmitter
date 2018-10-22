#include <SPI.h>
#include <EEPROM.h>
#include <Firmata.h>
#include "nRF24L01.h"
#include "RF24.h"
#define LeftY A0
#define LeftX A1
#define RightY A2
#define RightX A3
#define Anlg1 A4
#define Anlg2 A5
#define Anlg3 A6
#define D1 2
#define D2 3
#define D3 4
#define D4 5
#define D5 6
#define ledPin 8
int LYdata[2] = {512, 512},
    LXdata[2] = {512, 512},
    RYdata[2] = {512, 512},
    RXdata[2] = {512, 512};
int LYtrim, LXtrim, RYtrim, RXtrim, timesReady, timesAll;
byte data[9];
boolean TrimMode;
unsigned long time;
RF24 radio(9, 10);

void setup()
{
  //Serial.begin(115200);
  pinMode(Anlg1, INPUT);
  pinMode(Anlg2, INPUT_PULLUP);
  pinMode(Anlg3, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  //pinMode(ledPin, OUTPUT);
  EEPROMsetup(2);
  EEPROMsetup(3);
  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  Firmata.begin(115200);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  while (Firmata.available())
  {
    Firmata.processInput();
  }
  Nrf24l01Setup();
  //blink(3, 200);
}

void loop()
{
  //blink(0, 0);
  if (Firmata.available() > 0)
  {
    TrimMode = true;
    time = millis();
  }
  if (TrimMode == false)
  {
    data[0] = readSensors(0);
    data[1] = readSensors(1);
    data[2] = readSensors(2);
    data[3] = readSensors(3);
    data[4] = readSensors(4);
    data[5] = readSensors(5);
    data[6] = readSensors(6);
    data[7] = readSensors(7);
    data[8] = readSensors(8);
    radio.write(&data, sizeof(data)); // отправить по радио
  }
  else
  {
    while (Firmata.available()) 
    {
      Firmata.processInput();
    }
    FirmataSend();
    if (millis() - time > 700) 
    {
      TrimMode = false;
    }
  }
}
