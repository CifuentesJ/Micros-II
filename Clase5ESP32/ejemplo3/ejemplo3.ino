/*
  ejemplo que permite configurar una interrupción por Hardware
*/


// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Definiciones de PINES
#define pinBoton 22
#define pinLed 23

// Manejador de cole 
QueueHandle_t xQueue;


// Función para la interrupción
void IRAM_ATTR isrHandler(){
  // Se lee el estado del pin
  int pinState = digitalRead(pinBoton);

  // Se envía el estado a la cola 
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xQueue, &pinState, &xHigherPriorityTaskWoken);

  // Esto verifica si se debe dar paso a otra tarea de mayor prioridad
  if(xHigherPriorityTaskWoken){
    portYIELD_FROM_ISR();
  }
}


void taskHandler(void *parameter){
  int receivedValue;

  while(true){
    //Esperamos un elemento en la cola 
    if(xQueueReceive(xQueue, &receivedValue, portMAX_DELAY)){
      // Acción que se realiza cuando se recibe un elemento
      Serial.print("Interrupción detectada, nuevo estado del pin: ");
      Serial.println(receivedValue);
      digitalWrite(pinLed, receivedValue);
    }
  }
}


void setup() {
  //Configure pin
  pinMode(pinLed, OUTPUT);
  pinMode(pinBoton, INPUT_PULLUP);

  Serial.begin(115200);
  

  xQueue = xQueueCreate(10, sizeof(int));
  
  if(xQueue == NULL){
    Serial.println("Error: No se pudo crear la cola");
    return;
  }


  xTaskCreatePinnedToCore(
    taskHandler,        // Función a llamar
    "Interrupcion",     // Noombre de la tarea
    1024,         // Tamaño del Stack
    NULL,         // Parámetros a pasar
    1,            // Prioridad de tarea
    NULL,         // Manejador de la tarea
    app_cpu);     // Core en el que se va a manejar


  attachInterrupt(digitalPinToInterrupt(pinBoton), isrHandler, CHANGE);
}

void loop() {}
