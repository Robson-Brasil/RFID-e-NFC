// Test sketch for PN532 on ESP8266
// Used libraries from https://github.com/elechouse/PN532
// Copy PN532 and PN532_I2C folders to the Arduino libraries folder

#include <EEPROM.h>  // We are going to read and write PICC's UIDs from/to EEPROM
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

#define BUZZER_PIN 36
#define SOLENOID_PIN 37

#define led_green 39
#define led_red 34
#define door_sens 38  //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada

void setup(void) {
  Serial.begin(115200);
  Serial.println("Programa Iniciado...");

  pinMode(SOLENOID_PIN, OUTPUT); digitalWrite(SOLENOID_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, HIGH), delay(100), digitalWrite(BUZZER_PIN, LOW);

  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(door_sens, INPUT_PULLUP);  //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada

  //saídas iniciam desligadas
  digitalWrite(SOLENOID_PIN, HIGH);
  digitalWrite(led_green, HIGH);
  digitalWrite(led_red, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  nfc.begin();

  Serial.println("Aproxime o seu cartão do leitor...");
  Serial.println("Se Autorizado, a porta se abrirá!!");  for (byte address = 1; address <= 127; address++ )
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)
    {
      Serial.println(address, HEX);
    }
  }

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.println("Problema com o Leitor PN532");
    Serial.println("Problema com o Programa!");
    while (1)
      delay(1000);
  }

  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.println("Aguardando a Leitura da TAG");
  solenoidLock();
  }

unsigned long counter=0;
//**********************************************************************************************//
unsigned int hex_num = 0;// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

void solenoidLock() {
  hex_num = 0;
  digitalWrite(SOLENOID_PIN, HIGH);
}

void solenoidUnlock() {
  hex_num = 0;
  Serial.println("Acesso Liberado");
  digitalWrite(SOLENOID_PIN, LOW);
  delay(2000);
  solenoidLock();
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;   

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.print(++counter);
    Serial.println("Encontrou um Cartão ISO14443A com.... ");
    Serial.println("UID Número: "); 
    Serial.print(uidLength, DEC); Serial.println(" bytes"); (uidLength, HEX);
    Serial.println("Valor do UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
    Serial.print("0x"); Serial.print(uid[i], HEX);
    }
        Serial.println("");

    Serial.println("UID Reservado: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (i == 0) {
        hex_num = uid[i] << 24;
      } else if (i == 1) {
        hex_num += uid[i] << 16;
      } else if (i == 2) {
        hex_num += uid[i] << 8;
      } else {
        hex_num += uid[i];
      }
    }

    Serial.println(hex_num);
    Serial.println("");

    if (hex_num == 31772 || hex_num == 2076 || hex_num == 56611) {
        solenoidUnlock();

    Serial.println("Acesso Autorizado!");
    Serial.println();

    digitalWrite(led_green, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(led_green, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(led_green, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(led_green, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    
    Serial.println("Por favor, entre e Seja Bem-vindo!");
    Serial.println();
    Serial.println("Aguardando a porta ser fechada.");
    Serial.println();
    Serial.println();

    //Mantém acesso liberado até acionar o sensor de porta
    while (digitalRead(door_sens)) digitalWrite(SOLENOID_PIN, LOW);
    digitalWrite(SOLENOID_PIN, HIGH);
    while (digitalRead(door_sens)) digitalWrite(led_green, LOW);
    digitalWrite(led_green, HIGH);

    //while (digitalRead(door_sens)) digitalWrite(led_red, LOW);
    //digitalWrite(led_red, HIGH);
    }
  } 
}
