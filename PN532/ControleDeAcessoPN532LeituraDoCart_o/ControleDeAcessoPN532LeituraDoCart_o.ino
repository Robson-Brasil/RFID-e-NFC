/* Connect
GND ----- GND
VCC ----- 5V
SDA ----- SDA
SCL ----- SCL
- Key Code
*/

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

//Define a interface de comunicação
/*#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

#elif 1
#include <PN532_HSU.h>
#include <PN532.h>
PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

#else
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
#endif
*/
void setup(void) {
Serial.begin(115200);
nfc.begin();

uint32_t versiondata = nfc.getFirmwareVersion();
if (! versiondata) {
Serial.print("Não encontrou placa PN53x");
while (1); // parada
}

// Obteve dados ok
Serial.print("Chip encontrado PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
Serial.println("");
// Definir o número máximo de tentativas de repetição para ler um cartão
// Isso nos impede de esperar eternamente por um cartão, que é
// o comportamento padrão do PN532.
nfc.setPassiveActivationRetries(0xFF);

// configurar placa para ler etiquetas RFID
nfc.SAMConfig();

Serial.println("Esperando por um cartão ISO14443A");
}

void loop(void) {
boolean success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer para armazenar o UID retornado
uint8_t uidLength; // Comprimento do UID (4 ou 7 bytes, dependendo do tipo de cartão ISO14443A)

// Aguarde por um tipo de cartões ISO14443A (Mifare, etc.). Quando um é encontrado
// 'uid' será preenchido com o UID e uidLength indicará
// se o uid tiver 4 bytes (Mifare Classic) ou 7 bytes (Mifare Ultralight)
success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

if (success) {
Serial.println("Encontrei um cartão!");
Serial.print("Comprimento de UID: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
Serial.print("UID Valor: ");
String hex_value = "";
for (uint8_t i=0; i < uidLength; i++)
{
//Serial.print(F(" 0x");Serial.print(uid, HEX));
Serial.print(" ");Serial.print(uid, HEX);
hex_value += (String)uid;
}

Serial.println(", value="+hex_value);

if(hex_value == "13816044131") {
Serial.println("Esta é a tag chave. ");
}
else if(hex_value == "11818312831") {
Serial.println("Esta é a etiqueta do cartão. ");
}
else if(hex_value == "63156295") {
Serial.println("Esta é a etiqueta do telefone. ");
}
else
Serial.println("Eu não encontrei.");


Serial.println("");
// Espere 1 segundo antes de continuar
delay(1000);
}
else
{
// PN532 provavelmente expirou esperando por um cartão
//Serial.println("Esperando por um cartão ...");
}
}
Voltar ao topo