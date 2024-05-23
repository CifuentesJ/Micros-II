/* --------------------------------------------
 * Ejemplo de lectura de tarjeta rFID con ESP32
 * --------------------------------------------
 * 
 * Conexión dispositivos:
 * ---------------------------------
 *             MFRC522      ESP32  
 *             Reader/PCD   Mini R1
 * Signal      Pin          Pin    
 * ---------------------------------
 * RST/Reset   RST          15 (TDO)      
 * SPI SS      SDA(SS)      5 
 * SPI MOSI    MOSI         19 
 * SPI MISO    MISO         23 
 * SPI SCK     SCK          18 
 * ----------------------------------
 * Julian
 * 1 - hex 93 F4 E8 A6
 * 2 - hex C3 56 4C A9
 *
 *
 *
 *
 *
 *
 *
 */
#include <SPI.h>
#include <MFRC522.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define pinPrueba 22

#define RST_PIN 15                 //Pin 15 para el reset del RC522
#define SS_PIN 5                   //Pin 5 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);  ///Creamos el objeto para el RC522

byte ActualUID[4];                              //almacenará el código del Tag leído
byte Usuario1[4] = { 0x93, 0xF4, 0xE8, 0xA6 };  //código del usuario 1
int saldoUser1 = 200;
byte Usuario2[4] = { 0xC3, 0x56, 0x4C, 0xA9 };  //código del usuario 2
int saldoUser2 = 2560;

int userID = 0;
int valorRecarga = 0;



void setup() {
  Serial.begin(115200);  //Iniciamos La comunicacion serial
  pinMode(pinPrueba, OUTPUT);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  SPI.begin();         //Iniciamos el Bus SPI
  mfrc522.PCD_Init();  // Iniciamos el MFRC522
  Serial.println("CONTROL DE ACCESO");
}


void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if (mfrc522.PICC_IsNewCardPresent()) {
    //Seleccionamos una tarjeta
    if (mfrc522.PICC_ReadCardSerial()) {
      // Enviamos serialemente su UID
      // Serial.print(F("Card UID:"));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
        ActualUID[i] = mfrc522.uid.uidByte[i];
      }
      // Serial.print("     ");
      //comparamos los UID para determinar si es uno de nuestros usuarios
      if (compareArray(ActualUID, Usuario1)) {
        userID = 1;
        Serial.print("I");
        Serial.print(userID);
        Serial.print("D");
        Serial.println(saldoUser1);
      } else if (compareArray(ActualUID, Usuario2)) {
        userID = 2;
        Serial.print("I");
        Serial.print(userID);
        Serial.print("D");
        Serial.println(saldoUser2);
      } else {
        Serial.println("Acceso denegado...");
      }

      // Terminamos la lectura de la tarjeta tarjeta  actual
      mfrc522.PICC_HaltA();
    }
  }
}

//Función para comparar dos vectores
boolean compareArray(byte array1[], byte array2[]) {
  if (array1[0] != array2[0]) return (false);
  if (array1[1] != array2[1]) return (false);
  if (array1[2] != array2[2]) return (false);
  if (array1[3] != array2[3]) return (false);
  return (true);
}

bool flag = true;
void serialEvent() {
  flag = !flag;
  digitalWrite(pinPrueba, flag);
  String serialData = "";
  serialData = Serial.readStringUntil('\n');
  valorRecarga = serialData.toInt();
  switch (userID) {
    case 1:
      saldoUser1 += valorRecarga;
      break;
    case 2:
      saldoUser2 += valorRecarga;
      break;
  }
  serialData = "";
  
}
