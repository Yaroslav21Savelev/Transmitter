#include <TimerOne.h>// Библиотека прерываний
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
byte msg[9];
volatile int delays[8] = {1800, 1700, 1600, 1500, 1400, 1300, 2200, 2200}; //настройка PPM сигнала
byte i = 0;
unsigned long time;
byte state = false;
byte pulse = false;

void setup()
{
  Timer1.initialize();
  pinMode(3, OUTPUT);
  Timer1.attachInterrupt(Timer1_action, 300);
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах
  radio.openReadingPipe(1, 0xF0F1F2F3F4LL);     //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)
  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  radio.powerUp(); //начать работу
  radio.startListening();
}

void Timer1_action()
{
  digitalWrite(3, state);
  if (!pulse)
  {
    Timer1.setPeriod(300);
    pulse = 1;
  }
  else
  {
    Timer1.setPeriod(delays[i] - 300);
    pulse = 0;
    i++;
  }
  state = !state;
  if (i == 9)
  {
    int pause = 0;
    for (byte t = 0; t < 8; t++) 
    {
      pause += delays[t];
    }
    Timer1.setPeriod(22500 - pause);
    i = 0;
  }
}

void loop() 
{
  if (radio.available()) 
  {
    time = millis();
    radio.read(&msg, sizeof(msg));
  }
  if (millis() - time > 1000)
  {
    delays[0] = 880;
  }
  else 
  {
    delays[0] = map(msg[0], 0, 255, 1000, 2000);
  }
  delays[1] = map(msg[1], 0, 255, 2000, 1000);
  delays[2] = map(msg[2], 0, 255, 1000, 2000);
  delays[3] = map(msg[3], 0, 255, 2000, 1000);
  delays[4] = map(msg[4], 0, 255, 1000, 2000);
  delays[5] = map(msg[5], 0, 255, 1000, 2000);
  delays[6] = map(msg[6], 0, 255, 1000, 2000);
  delays[7] = map(msg[7], 0, 255, 1000, 2000);
}
