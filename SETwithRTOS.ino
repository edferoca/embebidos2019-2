#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include <SPI.h>
#include <MFRC522.h>
#include "HX711.h"

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522


byte RFID[4];
byte Transmilenio1[4] = {0xAA, 0xAA, 0xAA, 0xAA}; // Código PIC del transmilenio
byte Transmilenio2[4] = {0xBB, 0xBB, 0xBB, 0xBB}; // Código PIC del transmilenio
byte UartOut = 0x00;

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int confirmar=8;


int peso;

HX711 scale;

QueueHandle_t structQueue;

void setup() {
  Serial.begin(57600); //Iniciamos la comunicación  serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  pinMode(confirmar, OUTPUT);

  xTaskCreate(RFIDReady, // Task function
                "RFIDReady", // A name just for humans
                128,  // This stack size can be checked & adjusted by reading the Stack Highwater
                NULL, 
                2, // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
                NULL);


    // Create task that publish data in the queue if it was created.
    xTaskCreate(Pesar, // Task function
                "Pesar", // Task name
                128,  // Stack size
                NULL, 
                1, // Priority
                NULL);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
}

void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
 


}




void RFIDReady(void *pvParameters)
{
  (void) pvParameters;
  
  for (;;)
  {
        if (scale.is_ready()) {
         long reading = scale.read();
                if(reading <= 220000){
                  peso = 0xA1;
                }else if(reading <= 300000){
                  peso = 0xA2;
                }else{
                 peso = 0xA3;
                }
          
          //Serial.println(reading);
        } else {
         Serial.println(0, BIN);
        }
}

 

   
    xQueueSend(structQueue, &currentPinRead, portMAX_DELAY);

    // One tick delay (15ms) in between reads for stability
    vTaskDelay(1);
  }


void Pesar(void * pvParameters) {
  (void) pvParameters;

  for (;;) 
  {

    if ( mfrc522.PICC_IsNewCardPresent()) 
       {  
            digitalWrite(confirmar, HIGH);

      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                 // Serial.print("Card UID:");
                  for (byte i = 0; i < 1; i++) {
                          //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.write(mfrc522.uid.uidByte[i]);
                          
                          
                          RFID[i] = mfrc522.uid.uidByte[i]; // Se guerda el código PIC
                  } 
                       Serial.write(peso);        
            digitalWrite(confirmar, LOW);  
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();  
                delay(1000);      
               
            }
                
      }
    
  }

}
