/*
  Ejemplo de timer interrupt 

*/

#include "DHT.h"


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Definiciones de pines globales que se necesitan
#define dhtPin 32
#define DHTTYPE DHT11

volatile float cont;
float t;

DHT dht(dhtPin, DHTTYPE);

hw_timer_t *timer = NULL;  // Apuntador
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR getTemperature() {
  portENTER_CRITICAL_ISR(&timerMux);
  cont++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

// Inicializaciones y funciones generales

void setup() {
  Serial.begin(115200);

  dht.begin();

  // Inicialización del programa
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("");
  Serial.println("          Initializing the program... ");
  Serial.println("");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  // Punto 5
  timer = timerBegin(0, 80, true);                     // timer 0, prescalar: 80, UP counting
  timerAttachInterrupt(timer, &getTemperature, true);  // Attach interrupt
  timerAlarmWrite(timer, 15000000, true);               // 1000000 for 1 sec. delay.
  timerAlarmEnable(timer);
}

void loop() {
  if (cont > 0) {
    portENTER_CRITICAL(&timerMux);
    cont--;
    portEXIT_CRITICAL(&timerMux);

    t = dht.readTemperature();
    Serial.println("--------------------------------------------");
    Serial.println("           INTERRUPTION DETECTED           ");
    Serial.print("              Temperature: ");
    Serial.print(t);
    Serial.println("");
    Serial.println("--------------------------------------------");
  }
}



/*
#define LED_PIN 2             	// In some ESP32 board have inbuilt LED

volatile int interruptCounter;  //for counting interrupt
int totalInterruptCounter;   	//total interrupt counting
int LED_STATE=LOW;
hw_timer_t * timer = NULL;      //H/W timer defining (Pointer to the Structure)

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {      //Defining Inerrupt function with IRAM_ATTR for faster access
 portENTER_CRITICAL_ISR(&timerMux);
 interruptCounter++;
 portEXIT_CRITICAL_ISR(&timerMux);
}
 
void setup() {
 Serial.begin(115200);
 pinMode (LED_PIN, OUTPUT);
 
 timer = timerBegin(0, 80, true);           	// timer 0, prescalar: 80, UP counting
 timerAttachInterrupt(timer, &onTimer, true); 	// Attach interrupt
 timerAlarmWrite(timer, 10000000, true);  		// Match value= 1000000 for 1 sec. delay.
 timerAlarmEnable(timer);           			// Enable Timer with interrupt (Alarm Enable)
}
 
void loop() {
 if (interruptCounter > 0) {
 
   portENTER_CRITICAL(&timerMux);
   interruptCounter--;
   portEXIT_CRITICAL(&timerMux);
 
   totalInterruptCounter++;         	//counting total interrupt

   LED_STATE= !LED_STATE;  				//toggle logic
   digitalWrite(LED_PIN, LED_STATE);    //Toggle LED
   Serial.print("An interrupt as occurred. Total number: ");
   Serial.println(totalInterruptCounter);
 }
}
*/