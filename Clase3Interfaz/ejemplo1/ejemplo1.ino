#include <TimerOne.h>

// Definimos pines
#define analogPin A0
#define led1 9
#define led2 11

// Definimos variables
int val = 0;
char dato;
String serialData;

void setup(){
  Serial.begin(9600);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  analogWrite(led2, 0);

  // Interrupción cada 1 segundo
  Timer1.initialize(500000);
  Timer1.attachInterrupt(analogData); // Función de interrupción
}

void loop(){
}

void analogData(){
  val = analogRead(analogPin);
  Serial.print("T");
  Serial.println(val*1.0);
}

void serialEvent(){
  dato = (char)Serial.read();
  switch(dato){
    case 'D':
      serialData = Serial.readStringUntil('\n');
      digitalWrite(led1, serialData.toInt());
      serialData = "";
    break;

    case 'S':
      serialData = Serial.readStringUntil('\n');
      analogWrite(led2, serialData.toInt());
      serialData = "";
    break;
  }
}