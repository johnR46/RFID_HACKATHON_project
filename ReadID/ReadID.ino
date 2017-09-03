
#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;


boolean Stg = false;
String User_UD[20][4];
int j = 0;

// Init array that will store new NUID
byte nuidPICC[4];

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  Dum_checkData(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In: "));
  Dum_checkData(rfid.uid.uidByte, rfid.uid.size);
  j++;
  Serial.println();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}


/**
   Helper routine to dump a byte array as hex values to Serial.
*/

void Dum_checkData(byte * buffer, byte bufferSize) {
  if (j == 0) {
    for (byte i = 0; i < bufferSize; i++) {
      User_UD[j][i] = buffer[i];
    }
  }
  if (j != 0) {

    for (int x = 0; x < j; x++) {
      for (int i = 0; i < 4; i++) {
          if(  User_UD[x][i] == buffer[i]){
            Serial.print("xX");
            break;
          }{ 
            if(User_UD[j][i] =! buffer[i]){
              User_UD[j][i] = buffer[i];
              Serial.print(User_UD[j][i]);
            }
          }
                  
      }

    }

  }

}






