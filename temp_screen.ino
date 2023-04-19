#include <GyverOLED.h>  // библиотека олед экрана
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;  // конфигурация экрана без буфера
#include <microDS18B20.h>   // библиотека термометра DS18B20
#define DS_PIN 2  // пин для термометра D2
uint8_t s1_addr[] = {0x28, 0xFF, 0x64, 0x1F, 0x49, 0xC6, 0x1D, 0x25}; // термометр1 адрес
MicroDS18B20<DS_PIN, s1_addr> sensor1;  // Создаем термометр1 с адресацией
float newTemp;  // переменная для хранения заданной температуры
uint32_t Timer1_Temp; // таймер для опроса термометр1
uint32_t Timer2_stopwatch;  // таймер для обновления секундомера
uint32_t Timer3_ScreenOff;  // таймер для выключения экрана
uint32_t previousmillis;  // значение миллис при запуске секундомера
uint32_t duration;  // разница миллис-премиллис для секундомера

// объявляем массив кнопок
#define BTN_AMOUNT 2
#include <EncButton2.h>
EncButton2<EB_BTN> btn[BTN_AMOUNT];
int val_power = false; // true/false вкл-выкл питание экрана
int8_t step_power = 1;

void setup() {
  Serial.begin(9600);
  
  btn[0].setPins(INPUT_PULLUP, 3);
  btn[1].setPins(INPUT_PULLUP, 4);
  
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(1);   // масштаб текста (1..4)
  
  Delay(1000);
  newTemp = sensor1.getTemp(); // запомнили
}

void loop() {
  oled.setPower(val_power);    // true/false - включить/выключить дисплей
  oled.setContrast(1);   // яркость 0..255
  for (int i = 0; i < BTN_AMOUNT; i++) btn[i].tick();
  for (int i = 0; i < BTN_AMOUNT; i++) {
    if (btn[i].click()) {
      val_power = !val_power;
      previousmillis = millis();  // Сброс таймера
      Serial.print("click btn: ");
      Serial.println(i);
    }
  }
  if(millis() - Timer1_Temp >= 1000) { // Опрашивать каждые 1 сек
    Timer1_Temp = millis();        // Сброс таймера
    sensor1.requestTemp();   // запросить температуру
//    Serial.println("Температура:");
//    Serial.println(newTemp);
//    newTemp = (newTemp + 0.5);
  }
  if(millis() - Timer2_stopwatch >= 1000) { // Опрашивать каждые 1 сек
    Timer2_stopwatch = millis();        // Сброс таймера
    duration = (millis() - previousmillis);
    oled.setCursor(0, 3);
    uint32_t sec = duration / 1000;    // общее количество секунд  
    byte TimerSec = sec % 60;
    byte TimerMin = sec / 60 % 60;
    byte TimerHour = sec / 3600;
    oled.print("Таймер: ");
    if (TimerHour < 10) oled.print(0);
    oled.print(TimerHour);
    oled.print(':');
    if (TimerMin < 10) oled.print(0);
    oled.print(TimerMin);
    oled.print(':');
    if (TimerSec < 10) oled.print(0);
    oled.print(TimerSec);
  }
  if(millis() - Timer3_ScreenOff >= 60000) { // Опрашивать каждые 60 сек
    Timer3_ScreenOff = millis();        // Сброс таймера
    val_power = false;
  }
  
  oled.home();             // курсор в 0,0
  oled.print("Темп: ");
  oled.print(sensor1.getTemp());  // выводим температуру
  oled.print("C");
  oled.print(" > ");
  oled.print(newTemp);  // вывод prevT
  oled.print("C");
  printTimeON();          // выводим время
//  printTimer();          // выводим время
}

// функция вывода времени
void printTimeON() {
  oled.setCursor(0, 5);          // курсор на 2 строку
  oled.print("Время: ");
  uint32_t sec = millis() / 1000;    // общее количество секунд  
  byte thisH = sec / 3600;        // количество часов
  if (thisH < 10) oled.print(0); // ведущий 0
  oled.print(thisH);             // минуты
  oled.print(':');               // двоеточие
  byte thisM = sec / 60 % 60;        // количество минут
  if (thisM < 10) oled.print(0); // ведущий 0
  oled.print(thisM);             // минуты
  oled.print(':');               // двоеточие
  byte thisS = sec % 60;        // количество секунд
  if (thisS < 10) oled.print(0); // ведущий 0
  oled.print(thisS);             // секунды
}
