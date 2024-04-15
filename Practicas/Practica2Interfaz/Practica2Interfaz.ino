/*
  Julian Cifuentes Vásquez
  Microcontroladores II
  Abril de 2024
  Práctica 2

  Implementar un firmware que permita controlar (BOTÓN OFF y ON) 3 leds de diferentes colores al recibir comandos por la comunicación 
  serial con la interfaz creada en python corriendo en el PC. Tomar el firmware ejemplo y modificarlo para adaptarlo al requerimiento. 
  Para el led 3 se debe agregar en la interfaz de usuario un método adicional que al presionar el botón envíe la orden pasados 5s
  al microcontrolador ya sea al dar la orden de encender o al apagar.

*/

#define led1 2
#define led2 3
#define led3 4

void setup(){
  Serial.begin(9600);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  if(Serial.available() > 0){
    String command = Serial.readStringUntil('\n');
    
    if(command == "LED1_ON"){
      digitalWrite(led1, HIGH);
    } else if (command == "LED1_OFF") {
      digitalWrite(led1, LOW);
    }

    if(command == "LED2_ON"){
      digitalWrite(led2, HIGH);
    } else if (command == "LED2_OFF") {
      digitalWrite(led2, LOW);
    }

    if(command == "LED3_ON"){
      digitalWrite(led3, HIGH);
    } else if (command == "LED3_OFF") {
      digitalWrite(led3, LOW);
    }

    if(command == "OFF_ALL"){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }

  }
}