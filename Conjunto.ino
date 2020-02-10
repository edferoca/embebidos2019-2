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


int pesar();

HX711 scale;

void setup() {
  Serial.begin(57600); //Iniciamos la comunicación  serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  pinMode(confirmar, OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
}

void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
 
    

          

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
                       pesar();        
            digitalWrite(confirmar, LOW);  
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();  
                delay(1000);      
               
            }
                
  }
 
delay(1000); 
  
  
}

 boolean comparecode(byte array1[],byte array2[]) // Función para comparar los cuatro bytes del código escaneado con los almacenados previamente
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
