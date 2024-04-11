/*
  Julian Cifuentes Vásquez
  Microcontroladores II
  Abril de 2024
  Práctica 1
  
  Implementación un firmware que permite sensar la temperatura de un sensor lm35 y muestra por serial la
  temperatura sensada. La medición de la temperatura la debe hacer cuando se genere una interrupción en bajo para el pin
  2. El programa debe permanecer el resto del tiempo en un modo de bajo consumo.

*/

#include 

#define sensorPin A0
#define ledPin 10
#define buttonPin 2

void sensorFunct();
void sleepFunct();
void interruptFunct();

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), interruptFunct, LOW);
}

void sensorFunct(){

}

void sleepFunct(){
  
}

void interruptFunct(){
  
}