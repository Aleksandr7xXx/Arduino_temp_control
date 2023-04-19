#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
#include <microDS18B20.h>
#define DS_PIN 2 // пин для термометра
uint8_t s1_addr[] = {0x28, 0xFF, 0x64, 0x1F, 0x49, 0xC6, 0x1D, 0x25};
MicroDS18B20<DS_PIN, s1_addr> sensor1;  // Создаем термометр с адресацией

float prevT;    // переменная для хранения температуры

void setup() {
  oled.init();        // инициализация
  oled.clear();       // очистка
//  oled.setScale(2);   // масштаб текста (1..4)
//  oled.setCursor(0, 1);          // курсор на 2 строку
//  oled.print("Ver. 1");
//  delay(1000);            // ждём ответа
  oled.setScale(1);   // масштаб текста (1..4)
  Serial.begin(9600);
  sensor1.requestTemp();     // запрос температуры
//  delay(1000);            // ждём ответа
  prevT = sensor1.getTemp(); // запомнили
}
void loop() {
  sensor1.requestTemp();   // запросить температуру
  delay(1000);            // ждём ответа
  oled.home();             // курсор в 0,0
  oled.print("Темп: ");
  oled.print(sensor1.getTemp());  // выводим температуру
  oled.print("C");
  oled.print(" > ");
  oled.print(prevT);  // вывод prevT
  oled.print("C");
  printTimeON();          // выводим время
  printTimer();          // выводим время
}

// функция вывода времени
void printTimeON() {
  oled.setCursor(0, 5);          // курсор на 2 строку
  oled.print("Время: ");
//  oled.setCursor(0, 4);          // курсор на 3 строку
  int sec = millis() / 1;    // общее количество секунд  
  byte thisH = sec / 3600;        // количество часов
  if (thisH < 9) oled.print(00); // ведущий 0
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

// функция вывода времени
void printTimer() {
  oled.setCursor(0, 2);          // курсор на 4 строку
  oled.print("Таймер:");
//  oled.setCursor(0, 7);          // курсор на 5 строку
  int sec = millis() / 1000;    // общее количество секунд  
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