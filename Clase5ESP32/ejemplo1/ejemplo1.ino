/*
  ejemplo que permite configurar por medio de la comunicación  serial el tiempo de encendido y apagado de un led usando 2 tareas

*/



// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// LED rates
static int rate_1 = 1000;

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

void readSerial(void *parameter){
  String serialData = "";

  while(1){
    // Se leen los caracteres desde el serial 
    if(Serial.available() > 0){
      //Recibe caracteres hasta el retorno
      serialData = Serial.readStringUntil('\n');
      //Actualiza la variabale para el tiempo del blink 
      rate_1 = serialData.toInt();
      //Reinicia el string para obtener dato del serial
      serialData = "";
      Serial.print("Updated LED delay to: ");
      Serial.println(rate_1);
    }
  }
}



void setup() {
  //Configure pin
  pinMode(led_pin, OUTPUT);

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Multi-Task LED Demo");
  Serial.println("Enter a number in milliseconds to change the LED delay.");
  


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
    readSerial,  // Función a llamar
    "Lectura",   // Noombre de la tarea
    1024,         // Tamaño del Stack
    NULL,         // Parámetros a pasar
    1,            // Prioridad de tarea
    NULL,         // Manejador de la tarea
    app_cpu);     // Core en el que se va a manejar

  vTaskDelete(NULL);
}

void loop() {}
