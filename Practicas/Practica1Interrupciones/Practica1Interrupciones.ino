/*
  Julian Cifuentes Vásquez
  Microcontroladores II
  Abril de 2024
  Práctica 1
  
  Implementación un firmware que permite censar la temperatura de un sensor lm35 y muestra por serial la
  temperatura censada. La medición de la temperatura la debe hacer cuando se genere una interrupción en bajo para el pin
  2. El programa debe permanecer el resto del tiempo en un modo de bajo consumo.

*/

#include <avr/sleep.h>

#define sensorPin A0
#define ledPin 10
#define buttonPin 2
#define state 13

void sensorFunct();
void sleepFunct();
void interruptFunct();

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(state, OUTPUT);

  digitalWrite(state, HIGH);

  attachInterrupt(digitalPinToInterrupt(buttonPin), interruptFunct, LOW);
}

void loop() {
  sleepFunct();
}



void sensorFunct(){
  int temp = analogRead(sensorPin);

  float voltage = temp * 5.0 / 1023.0;
  float tempC = voltage * 100.0;

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

}

void sleepFunct(){
  Serial.println("Entering deep sleep mode...");
  
  delay(750);
  digitalWrite(state, LOW);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  digitalWrite(state, HIGH);

}

void interruptFunct(){
  sleep_disable();

  digitalWrite(ledPin, HIGH);
  sensorFunct();
  digitalWrite(ledPin, LOW);
}
