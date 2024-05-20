#include <WiFi.h>
#include <WebServer.h>

#define ssid "UNE_HFC_7010(2)"
#define password "AFAFF929"

#define ledPin 23

WebServer server(80);

bool ledState = false;

// Maneja el servidor web 
void handleServer(void* param) {
  for (;;) {
    server.handleClient();    // Maneja las solicitudes del cliente 
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Pequeña espera 
  }
}

//Página web para servir al cliente 
String getHTML(bool ledState ) {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>Control de LED </title></head>";
  html += "<body>";
  html += "<h1>On and Off LED</h1>";
  html += "<form action =\"/toogle\" method=\"POST\">";
  if(ledState){
    html += "<button type=\"submit\">APAGAR LED</button>";
  } else{
    html += "<button type=\"submit\">ENCENDER LED</button>";
  }
  html += "</form>";
  html += "</body>";
  html += "</html";
  return html;
}


void handleToggle(){
  ledState = !ledState; 
  digitalWrite(ledPin, ledState);
  server.send(200, "text/html", getHTML(ledState));
}


// Configuración de la conexión Wifi y el servidor web 
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  // Conectar al wifi
  Serial.println("Conectando al WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado al WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configurar el servidor 
  server.on("/", HTTP_GET,[](){
    server.send(200, "text/html", getHTML(ledState));  // Se envía el get 
  });

  server.on("/toogle", HTTP_POST, handleToggle);

  server.begin(); // Inicia el servidor 

  // Crea la tarea para manejar el servidor
  xTaskCreatePinnedToCore(
    handleServer,
    "ServerTask",
    10000, 
    NULL,
    1,
    NULL,
    1);
}


void loop() {
  // put your main code here, to run repeatedly:
}