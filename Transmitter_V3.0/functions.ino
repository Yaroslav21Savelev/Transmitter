/*void blink (int val1, int val2)
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
}*/
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
    case 3:
      EEPROM.get(5, LYdata);
      EEPROM.get(15, LXdata);
      EEPROM.get(20, RYdata);
      EEPROM.get(25, RXdata);
      /*Serial.print(LYdata[0]); Serial.print("   "); Serial.println(LYdata[1]);
      Serial.print(LXdata[0]); Serial.print("   "); Serial.println(LXdata[1]);
      Serial.print(RYdata[0]); Serial.print("   "); Serial.println(RYdata[1]);
      Serial.print(RXdata[0]); Serial.print("   "); Serial.println(RXdata[1]);*/
      break;
  }
}
void getCorners()
{
  LYdata[0] = 512;
  LYdata[1] = 512;
  LXdata[0] = 512;
  LXdata[1] = 512;
  RYdata[0] = 512;
  RYdata[1] = 512;
  RXdata[0] = 512;
  RXdata[1] = 512;
  while(!digitalRead(D1))
  {
    if(analogRead(LeftY) > LYdata[0]) LYdata[0] = analogRead(LeftY);
    if(analogRead(LeftY) < LYdata[1]) LYdata[1] = analogRead(LeftY);
    if(analogRead(LeftX) > LXdata[0]) LXdata[0] = analogRead(LeftX);
    if(analogRead(LeftX) < LXdata[1]) LXdata[1] = analogRead(LeftX);
    if(analogRead(RightY) > RYdata[0]) RYdata[0] = analogRead(RightY);
    if(analogRead(RightY) < RYdata[1]) RYdata[1] = analogRead(RightY);
    if(analogRead(RightX) > RXdata[0]) RXdata[0] = analogRead(RightX);
    if(analogRead(RightX) < RXdata[1]) RXdata[1] = analogRead(RightX);
  }
  EEPROM.put(5, LYdata);
  delay(4);
  EEPROM.put(15, LXdata);
  delay(4);
  EEPROM.put(20, RYdata);
  delay(4); 
  EEPROM.put(25, RXdata);  
}
int readSensors(int val)
{
  switch (val)
  {
    case 0:
      return (constrain(map(constrain(analogRead(LeftY), LYdata[1], LYdata[0]), LYdata[1], LYdata[0], 255, 0) + LYtrim, 0, 255));
      break;
    case 1:
      return (constrain(map(constrain(analogRead(LeftX), LXdata[1], LXdata[0]), LXdata[0], LXdata[1], 0, 255) + LXtrim, 0, 255));
      break;
    case 2:
      return (constrain(map(constrain(analogRead(RightY), RYdata[1], RYdata[0]), RYdata[1], RYdata[0], 255, 0) + RYtrim, 0, 255));
      break;
    case 3:
      return (constrain(map(constrain(analogRead(RightX), RXdata[1], RXdata[0]), RXdata[0], RXdata[1], 0, 255) + RXtrim, 0, 255));
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
      //blink(1, 100);
      break;
    case 1:
      LXtrim = constrain(LXtrim + val - 1, -127, 127);
      //blink(1, 100);
      break;
    case 2:
      RYtrim = constrain(RYtrim + val - 1, -127, 127);
      //blink(1, 100);
      break;
    case 3:
      RXtrim = constrain(RXtrim + val - 1, -127, 127);
      //blink(1, 100);
      break;
    case 4:
      if (!val)
      {
        EEPROMsetup(0);
        //blink(3, 50);
      }
      if (val == 1)
      {
        EEPROMsetup(1);
        EEPROMsetup(2);
        //blink(3, 50);
      }
      if (val == 1)
      {
        TrimMode = true;
      }
      break;
    case 5:
      EEPROMsetup(1);
      getCorners();
      EEPROMsetup(3);
      break;
  }
}
