#include <Wire.h>
#include <SPI.h>
//--download dari web--//
#include <Adafruit_PN532.h>
#include <LiquidCrystal_I2C.h>

//--relay--//
#define rly 9

//--LCD--//
LiquidCrystal_I2C lcd(0x27, 20, 4);

//--RFID--//
//Define a interface de comunicação
#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

#else
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
#endif

int tukimin[4]={0x62,0x92,0x7C,0x1C};
int sukijan[8]={0x2D,0xCA,0x08,0x1C};
void setup(void) {
  Serial.begin(115200);
  pinMode(rly,OUTPUT);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CNC Store Bandung");

  while (!Serial) delay(10); // for Leonardo/Micro/Zero
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID Value: ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CNC Store Bandung");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
      lcd.setCursor(0,1);
      lcd.print("HEX: ");
      lcd.setCursor(5+(i * 2), 1);
      lcd.print(uid[i], HEX);
      //-- cek name--//
      lcd.setCursor(0,2);
      if (uid[i]==tukimin[i]){
        lcd.print("Cecep");
        digitalWrite(rly,LOW);
      }
      else if(uid[i]==sukijan[i]){
        lcd.print("Ujang");
        digitalWrite(rly,LOW);
      }
      else{
        lcd.print("tidak diketahui");
        digitalWrite(rly,HIGH);
      }
    }
    Serial.println("");
    // Wait 1 second before continuing
    delay(1000);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
}
