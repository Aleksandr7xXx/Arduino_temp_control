#include <microDS18B20.h>
#define DS_PIN 2 // пин для термометров

// Уникальные адреса датчиков - считать можно в примере address_read
uint8_t s1_addr[] = {0x28, 0xFF, 0x64, 0x1F, 0x49, 0xC6, 0x1D, 0x25};

MicroDS18B20<DS_PIN, s1_addr> sensor1;  // Создаем термометр с адресацией

void setup() {
  Serial.begin(9600);
}

void loop() {
  // асинхронное чтение нескольких датчиков смотри в примере async_read_many
  sensor1.requestTemp();      // Запрашиваем преобразование температуры

  delay(1000);           // ожидаем результат 
  
  Serial.print("t1: ");
  if (sensor1.readTemp()) Serial.println(sensor1.getTemp());
  else Serial.println("error");
}
