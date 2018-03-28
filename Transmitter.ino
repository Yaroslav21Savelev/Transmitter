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
int LYtrim, LXtrim, RYtrim, RXtrim, timesReady, timesAll;
byte data[9];
boolean TrimMode, ledState;
unsigned long time, blinkTime, wait;
RF24 radio(9, 10);

void setup()
{
  pinMode(Anlg1, INPUT);
  pinMode(Anlg2, INPUT_PULLUP);
  pinMode(Anlg3, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  EEPROMsetup(2);
  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  Firmata.begin(115200);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  while (Firmata.available())
  {
    Firmata.processInput();
  }
  Nrf24l01Setup();
  blink(3, 200);
}

void loop()
{
  blink(0, 0);
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
    while (Firmata.available()) {
      Firmata.processInput();
    }
    FirmataSend();
    if (millis() - time > 700) {
      TrimMode = false;
    }
  }
}






















void blink (int val1, int val2)
{
  boolean flag = 0;
  if (val1 > 0 && val2 > 0)
  {
    timesAll = val1;
    wait = val2;
  }
  if (timesReady < timesAll)
  {
    if (millis() - blinkTime > wait)
    {
      if (!digitalRead(ledPin))
      {
        blinkTime = millis();
        digitalWrite(ledPin, 1);
        flag = 1;
      }
      if (digitalRead(ledPin) && !flag)
      {
        timesReady++;
        blinkTime = millis();
        digitalWrite(ledPin, 0);
      }
    }
  }
  else
  {
    timesReady = 0;
    timesAll = 0;
  }
}
void EEPROMsetup(int val)
{
  switch (val)
  {
    case 0:
      EEPROM.write(0, LYtrim + 127);
      EEPROM.write(1, LXtrim + 127);
      EEPROM.write(2, RYtrim + 127);
      EEPROM.write(3, RXtrim + 127);
      break;
    case 1:
      EEPROM.write(0, 127);
      EEPROM.write(1, 127);
      EEPROM.write(2, 127);
      EEPROM.write(3, 127);
      break;
    case 2:
      LYtrim =  EEPROM.read(0) - 127;
      LXtrim =  EEPROM.read(1) - 127;
      RYtrim =  EEPROM.read(2) - 127;
      RXtrim =  EEPROM.read(3) - 127;
      break;
  }
}

int readSensors(int val)
{
  switch (val)
  {
    case 0:
      return (constrain(map(constrain(analogRead(LeftY), 150, 817), 150, 817, 255, 0) + LYtrim, 0, 255));
      break;
    case 1:
      return (constrain(map(constrain(analogRead(LeftX), 100, 820), 100, 820, 0, 255) + LXtrim, 0, 255));
      break;
    case 2:
      return (constrain(map(constrain(analogRead(RightY), 190, 865), 190, 865, 255, 0) + RYtrim, 0, 255));
      break;
    case 3:
      return (constrain(map(constrain(analogRead(RightX), 145, 847), 145, 847, 0, 255) + RXtrim, 0, 255));
      break;
    case 4:
      return (map(constrain(analogRead(Anlg1), 0, 1023), 1023, 0, 0, 255));
      break;
    case 5:
      if (digitalRead(Anlg2)) {
        return (0);
      }
      else {
        return (255);
      }
      break;
    case 6:
      if (digitalRead(D1)) {
        return (0);
      }
      else {
        return (255);
      }
      break;
    case 7:
      if (!digitalRead(D2)) {
        return (0);
      }
      if (!digitalRead(D3)) {
        return (255);
      }
      if (digitalRead(D2) && digitalRead(D3)) {
        return (127);
      }
      break;
    case 8:
      if (!digitalRead(D4)) {
        return (0);
      }
      if (!digitalRead(D5)) {
        return (255);
      }
      if (digitalRead(D4) && digitalRead(D5)) {
        return (127);
      }
      break;
  }
}
void FirmataSend()
{
  Firmata.sendAnalog(0, readSensors(0));
  Firmata.sendAnalog(1, readSensors(1));
  Firmata.sendAnalog(2, readSensors(2));
  Firmata.sendAnalog(3, readSensors(3));
  Firmata.sendAnalog(4, readSensors(4));
  Firmata.sendAnalog(5, readSensors(5));
  Firmata.sendAnalog(6, readSensors(6));
  Firmata.sendAnalog(7, readSensors(7));
  Firmata.sendAnalog(8, readSensors(8));
}
void Nrf24l01Setup()
{
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(0xF0F1F2F3F4LL);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}
void analogWriteCallback(byte wave, int val)
{

  switch (wave)
  {
    case 0:
      LYtrim = constrain(LYtrim + val - 1, -127, 127);
      blink(1, 100);
      break;
    case 1:
      LXtrim = constrain(LXtrim + val - 1, -127, 127);
      blink(1, 100);
      break;
    case 2:
      RYtrim = constrain(RYtrim + val - 1, -127, 127);
      blink(1, 100);
      break;
    case 3:
      RXtrim = constrain(RXtrim + val - 1, -127, 127);
      blink(1, 100);
      break;
    case 4:
      if (!val)
      {
        EEPROMsetup(0);
        blink(3, 50);
      }
      if (val == 1)
      {
        EEPROMsetup(1);
        EEPROMsetup(2);
        blink(3, 50);
      }
      if (val == 1)
      {
        TrimMode = true;
      }
      break;
  }
}


