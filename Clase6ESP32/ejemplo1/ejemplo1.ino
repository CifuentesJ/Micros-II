#include <WiFi.h>
#include <WebServer.h>

#define ssid "Julian"
#define password "JulianCifuentes"

WebServer server(80);

// Maneja el servidor web 
void handleServer(void* param) {
  for (;;) {
    server.handleClient();    // Maneja las solicitudes del cliente 
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Pequeña espera 
  }
}

String getHTML() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>BIENVENIDO</title></head>";
  html += "<body>";
  html += "<h1>BIENVENID@ INGENIER@</h1>";
  html += "</body>";
  html += "</html";
  return html;
}

// Configuración de la conexión Wifi y el servidor web 
void setup() {
  Serial.begin(115200);

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
    server.send(200, "text/html", getHTML());  // Se envía el get 
  });

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