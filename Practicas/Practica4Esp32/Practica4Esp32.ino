/*
  Julian Cifuentes Vásquez
  Microcontroladores II
  Abril de 2024
  Práctica 3

  la implementación de un MCU con 2 led,  manejar un  encendido y apagado para el led 1 de cada 2 segundos. Y para el led 2 un 
  encendido y apagado de cada 3 segundos.
  Adicionar un sensor de temperatura y realizar una tarea nueva que consulte el estado del sensor cada 30 seg y que muestre su valor por 
  serial

*/

#include "DHT.h"

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// LED rates
#define rate_1 2000
#define rate_2 3000
#define rate_3 30000

// Pins
#define led_pin 23
#define led_pin2 22

// Definiciones para el DHT 11
#define dhtPin 32
#define DHTTYPE DHT11

int cont = 0;

// Se intancia la función
DHT dht(dhtPin, DHTTYPE);



// Our Task: blink a LED at one rate
void toggleLED_1(void *parameter) {
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}

void toggleLED_2(void *parameter) {
  while (1) {
    digitalWrite(led_pin2, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin2, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
  }
}

void senseDHT(void *parameter) {
  while (1) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    cont++;

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    Serial.print(F("Humedad: "));
    Serial.print(h);
    Serial.print(F("% Temperatura: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("\t Toma: "));
    Serial.println(cont);
    vTaskDelay(rate_3 / portTICK_PERIOD_MS);
  }
}



void setup() {
  // Monitor serial
  Serial.begin(9600);

  // Inicialización del DHT
  dht.begin();

  //Configure pin
  pinMode(led_pin, OUTPUT);
  pinMode(led_pin2, OUTPUT);

  // task to run forever
  xTaskCreatePinnedToCore(
    toggleLED_1,  // Función a llamar
    "Toggle 1",   // Noombre de la tarea
    1024,         // Tamaño del Stack
    NULL,         // Parámetros a pasar
    1,            // Prioridad de tarea
    NULL,         // Manejador de la tarea
    app_cpu);     // Core en el que se va a manejar


  xTaskCreatePinnedToCore(
    toggleLED_2,  // Función a llamar
    "Toggle 2",   // Noombre de la tarea
    1024,         // Tamaño del Stack
    NULL,         // Parámetros a pasar
    1,            // Prioridad de tarea
    NULL,         // Manejador de la tarea
    app_cpu);     // Core en el que se va a manejar

    xTaskCreatePinnedToCore(
    senseDHT,  // Función a llamar
    "DHT sense",   // Noombre de la tarea
    1024,         // Tamaño del Stack
    NULL,         // Parámetros a pasar
    1,            // Prioridad de tarea
    NULL,         // Manejador de la tarea
    app_cpu);     // Core en el que se va a manejar
}

void loop() {}
