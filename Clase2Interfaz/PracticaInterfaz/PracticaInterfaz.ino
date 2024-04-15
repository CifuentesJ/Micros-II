
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    String command = Serial.readStringUntil('\n');
    
    if(command == "LED1_ON"){
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (command == "LED1_OFF") {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
