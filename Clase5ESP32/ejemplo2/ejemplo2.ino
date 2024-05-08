/*
  ejemplo que permite configurar una interrupción por software de timer con su
  rutina de atención de la interrupción. El ejemplo recibe por serial algún dato, 
  cuando eso pasa enciende un led y activa el timer para que al pasar 5 seg se 
  apague el led en la rutina de atención de la interrupción del timer
*/



// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Configuraciones 
static const TickType_t dim_delay = 5000 / portTICK_PERIOD_MS;

// Globals 
static TimerHandle_t one_shot_timer = NULL;

// Pins
#define led_pin 23


// Apaga el led cuando el timer se acaba
void autoDimmerCallBack(TimerHandle_t xTimer){
  digitalWrite(led_pin, LOW);
}



// Our Task: blink a LED at one rate
void doCLI(void *parameter) {
  char c;

  while (1) {
    if(Serial.available() > 0){
      // Cuando le llega algo en serial 
      c = Serial.read();
      Serial.print(c);

      // Enciende el LED 
      digitalWrite(led_pin, HIGH);

      // Empieza el timer 
      xTimerStart(one_shot_timer, portMAX_DELAY);

    }
  }
}


void setup() {
  //Configure pin
  pinMode(led_pin, OUTPUT);

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("\n\n");
  Serial.println("--- FreeRTOS Timer Solution ---");
  


  // task to run forever
  one_shot_timer = xTimerCreate(
    "One-shot timer",     // Nombre del timer
    dim_delay,            // Periodo del timer 
    pdTRUE,              // Auto-reload
    (void *)0,            // Timer ID
    autoDimmerCallBack);  // Función


  xTaskCreatePinnedToCore(
    doCLI,        // Función a llamar
    "Do CLI",     // Noombre de la tarea
    1024,         // Tamaño del Stack
    NULL,         // Parámetros a pasar
    1,            // Prioridad de tarea
    NULL,         // Manejador de la tarea
    app_cpu);     // Core en el que se va a manejar

  vTaskDelete(NULL);
}

void loop() {}
