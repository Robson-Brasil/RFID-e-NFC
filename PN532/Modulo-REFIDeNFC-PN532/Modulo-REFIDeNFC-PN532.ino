//Programa: Teste modulo PN532 NFC RFID Arduino
//Autor: Arduino e Cia
//Baseado no programa exemplo da biblioteca PN532

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

void setup(void)
{
  Serial.begin(115200);
  Serial.println("*** Teste Modulo PN532 NFC RFID ***");

  nfc.begin();

  //Verifica a conexao do modulo PN532
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.print("Placa PN53x nao encontrada...");
    while (1); // halt
  }

  //Conexao ok, mostra informacoes do firmware
  Serial.print("Encontrado chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware versao: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Aguardando cartao ISO14443A...");
  Serial.println("");
}

void loop(void)
{
  boolean success;
  //Buffer para armazenar a UID lida
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  //Tamanho da UID (4 ou 7 bytes dependendo do tipo do cartao)
  uint8_t uidLength;

  //Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  //'uid' will be populated with the UID, and uidLength will indicate
  //if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  //Caso o cartao seja detectado, imprime o UID do cartao
  if (success)
  {
    Serial.println("Cartao detectado!");
    Serial.print("Tamanho do UID: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");
    Serial.println("");
    //Aguarda 1 segundo para continuar
    delay(1000);
  }
  else
  {
    //PN532 probably timed out waiting for a card
    //Serial.println("Timed out waiting for a card");
  }
}