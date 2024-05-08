/*
  Julian Cifuentes Vásquez
  Microcontroladores II
  Abril de 2024
  Práctica 5

  IMPLEMENTACIÓN:
  -1 tarea que recibe un número en milisegundos por la comunicación serial y cambia el tiempo
  de encendido y apagado de un led.
  -1 tarea que encienda y apague un led en el tiempo definido por el dato recibido
  por serial.
  -1 tarea que encienda o apague un led adicional dependiendo del estado recibido por una variable
  que es controlada por el estado de un botón 
  -1 Interrupción de hardware de tipo FALLING que llama una rutina de atención de interrupciones 
  cuando el estado de un botón pasa de HIGH a LOW y envía dicho Estado a una variable usando una QUEUE 
  para que sea recibida por otra tarea
  -1 Interrupción de software de Timer que cada 15 seg muestre por consola la temperatura sensada por un 
  sensor de temperatura de su preferencia

*/

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Definiciones de pines globales que se necesitan
#define ledTask1 23
#define button 22
#define ledTask3 19


int time1 = 5000;


// Manejador de cole
QueueHandle_t xQueue;


// Funciones utilizadas por las tareas

void readTime(void *parameter) {
  String timeBySerial = "";

  while (true) {
    if (Serial.available() > 0) {
      timeBySerial = Serial.readStringUntil('\n');
      time1 = timeBySerial.toInt();
      timeBySerial = "";
      Serial.println("--------------------------------------------");
      Serial.println("              UPDATING TIME...           ");
      Serial.print("            Time set at: ");
      Serial.print(time1);
      Serial.println(" ms.");
      Serial.println("--------------------------------------------");
    }
  }
}



void onAndOffLed1(void *parameter) {
  while (true) {
    digitalWrite(ledTask1, HIGH);
    vTaskDelay(time1 / portTICK_PERIOD_MS);
    digitalWrite(ledTask1, LOW);
    vTaskDelay(time1 / portTICK_PERIOD_MS);
  }
}



void onAndOffLed2(void *parameter) {
  bool flagLed = HIGH;

  while (true) {
    if (xQueueReceive(xQueue, &flagLed, portMAX_DELAY)) {
      Serial.println("--------------------------------------------");
      Serial.println("           INTERRUPTION DETECTED           ");
      Serial.print("            LED's state set at: ");
      Serial.print(flagLed);
      Serial.println("");
      Serial.println("--------------------------------------------");
      digitalWrite(ledTask3, flagLed);
    }
  }
}


void IRAM_ATTR catchAndSendFlag() {
  bool pinState = digitalRead(button);

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xQueue, &pinState, &xHigherPriorityTaskWoken);

  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}




// Inicializaciones y funciones generales

void setup() {
  Serial.begin(115200);

  // Definiciones
  pinMode(ledTask1, OUTPUT);
  pinMode(ledTask3, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  // Inicialización del programa
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("");
  Serial.println("          Initializing the program... ");
  Serial.println("");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");


  // Tareas

  // Punto 1
  xTaskCreatePinnedToCore(
    readTime,   // Función a llamar
    "Lectura",  // Noombre de la tarea
    1024,       // Tamaño del Stack
    NULL,       // Parámetros a pasar
    1,          // Prioridad de tarea
    NULL,       // Manejador de la tarea
    app_cpu);   // Core en el que se va a manejar

  // Punto 2
  xTaskCreatePinnedToCore(
    onAndOffLed1,  // Función a llamar
    "led1",        // Noombre de la tarea
    1024,          // Tamaño del Stack
    NULL,          // Parámetros a pasar
    1,             // Prioridad de tarea
    NULL,          // Manejador de la tarea
    app_cpu);      // Core en el que se va a manejar

  // Punto 3
  xTaskCreatePinnedToCore(
    onAndOffLed2,  // Función a llamar
    "led2",        // Noombre de la tarea
    1024,          // Tamaño del Stack
    NULL,          // Parámetros a pasar
    1,             // Prioridad de tarea
    NULL,          // Manejador de la tarea
    app_cpu);      // Core en el que se va a manejar

  // Punto 4
  xQueue = xQueueCreate(10, sizeof(int));

  if (xQueue == NULL) {
    Serial.println("Error: No se pudo crear la cola");
    return;
  }

  attachInterrupt(digitalPinToInterrupt(button), catchAndSendFlag, CHANGE);
}

void loop() {}
