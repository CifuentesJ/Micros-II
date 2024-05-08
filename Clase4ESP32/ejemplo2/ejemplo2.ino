// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// LED rates
#define rate_1 500
#define rate_2 323

// Pins
#define led_pin 23

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
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}



void setup() {
  //Configure pin
  pinMode(led_pin, OUTPUT);

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
}

void loop() {}
