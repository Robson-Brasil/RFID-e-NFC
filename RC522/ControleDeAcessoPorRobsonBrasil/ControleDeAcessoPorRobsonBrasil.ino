//Controle de Acesso com TAG via RFID
//Autor : Robson Brasil
//Versão : 5
//Última Modificação : 08/02/2023

// --- Bibliotecas Auxiliares ---
#include <EEPROM.h>  // We are going to read and write PICC's UIDs from/to EEPROM
#include <SPI.h>
#include <MFRC522.h>
#include <locale.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// --- Mapeamento de Hardware ---
#define SS_PIN 5
#define RST_PIN 2
#define LedVerde 26
#define LedVermelho 27
#define Buzzer 17
#define Rele 13  //Ligado em um LED para testes, toque-o pelo sensor que vc preferir
#define PortaAberta 33  //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Cria instância com MFRC522

// --- Protótipo das Funções Auxiliares ---
void rfid_func();  //Função para identificação das tags RFID

// --- Variáveis Globais ---
char st[20];

// set the LCD number of columns and rows
int ColunasLCD = 20;
int LinhasLCD = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, ColunasLCD, LinhasLCD);

String RFIDouNFC1 = "62 92 7C 1C";
String RFIDouNFC2 = "2D CA 08 1C";
String RFIDouNFC3 = "CA 75 DD 23";
String RFIDouNFC4 = "XX XX XX XX";

String messageStatic0 = "POR FAVOR";
String messageStatic1 = "APROXIME SEU CARTAO!";
String messageStatic2 = "USUARIO 1";
String messageStatic3 = "ENTRADA  AUTORIZADA!";
String messageStatic4 = "USUARIO 2";
String messageStatic5 = "USUARIO 3";
String messageStatic6 = "PORTA ABERTA";
String messageStatic7 = "SEM ATORIZACAO";
String messageStatic8 = "IDENTIFIQUE-SE";
String messageStatic9 = "FECHE A PORTA!";

// Function to scroll text
// The function acepts the following arguments:
// row: row number where the text will be displayed
// message: message to scroll
// delayTime: delay between each character shifting
// ColunasLCD: number of columns of your LCD
void scrollText(int row, String message, int delayTime, int ColunasLCD) {
  for (int i = 0; i < ColunasLCD; i++) {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + ColunasLCD));
    delay(delayTime);
  }
}

// --- Configurações Iniciais ---
void setup() {
  pinMode(LedVerde, OUTPUT);
  pinMode(LedVermelho, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Rele, OUTPUT);        //Ligado em um LED para testes, toque-o pelo sensor que vc preferir
  pinMode(PortaAberta, INPUT_PULLUP);  //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada
  Serial.begin(115200);              // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();                       // Inicia comunicação SPI bus
  mfrc522.PCD_Init();                // Inicia MFRC522

  setlocale(LC_ALL, "Portuguese");
  Serial.print(F("Aproxime o seu cartão do leitor..."));
  Serial.println("Se Autorizado, a porta se abrirá!!");

  //saídas iniciam desligadas
  digitalWrite(Rele, HIGH);
  digitalWrite(LedVerde, HIGH);
  digitalWrite(LedVermelho, HIGH);
  digitalWrite(Buzzer, LOW);

  // initialize LCD
  lcd.begin();
  // turn on LCD backlight
  lcd.backlight();

}  //end setup

// --- Loop Infinito ---
void loop() {

  rfid_func();  //chama função para identificação de tags RFID
  // set cursor to first column, first row
  lcd.setCursor(5, 0);
  // print static message
  lcd.print(messageStatic0);
  // set cursor to second column, first row
  lcd.setCursor(0, 1);
  // print static message
  lcd.print(messageStatic1);

  setlocale(LC_ALL, "Portuguese");

}  //end loop

// === Funções Auxiliares ===
void rfid_func()  //Função para identificação das tags RFID
{
  // -- Verifica novas tags --
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Seleciona um dos cartões
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Mostra UID na serial
  setlocale(LC_ALL, "Portuguese");
  Serial.print(F("Número de Identificação da TAG lida!!!!  ---->"));
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  conteudo.toUpperCase();

  //Cadastro das TAG Novas
  if (conteudo.substring(1) == RFIDouNFC1)  //Cartão 1 para liberar acesso
  {
    digitalWrite(LedVerde, HIGH);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(LedVerde, LOW);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVerde, HIGH);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(LedVerde, LOW);
    delay(100);
    digitalWrite(Buzzer, LOW);
    // print static message
    lcd.setCursor(5, 2);
    lcd.print(messageStatic2);
    lcd.setCursor(0, 3);
    // print scrolling message
    lcd.print(messageStatic3);
    delay(500);
    lcd.clear();  // limpa a tela

    //Mantém acesso liberado até acionar o sensor de porta
    while (digitalRead(PortaAberta)) digitalWrite(Rele, LOW);
    digitalWrite(Rele, HIGH);
    while (digitalRead(PortaAberta)) digitalWrite(LedVerde, LOW);
    digitalWrite(LedVerde, HIGH);
    
  }

  //Cadastro das TAG Novas
  if (conteudo.substring(1) == RFIDouNFC2)  //Cartão 2 para liberar acesso
  {
    digitalWrite(LedVerde, HIGH);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(LedVerde, LOW);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVerde, HIGH);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(LedVerde, LOW);
    delay(100);
    digitalWrite(Buzzer, LOW);
    lcd.setCursor(5, 2);
    lcd.print(messageStatic4);
    lcd.setCursor(0, 3);
    // print scrolling message
    lcd.print(messageStatic3);
    delay(500);
    lcd.clear();  // limpa a tela
    
    //Mantém acesso liberado até acionar o sensor de porta
    while (digitalRead(PortaAberta)) digitalWrite(Rele, LOW);
    digitalWrite(Rele, HIGH);
    while (digitalRead(PortaAberta)) digitalWrite(LedVerde, LOW);
    digitalWrite(LedVerde, HIGH);
  }

  if (conteudo.substring(1) == RFIDouNFC3)  //Cartão 3 para liberar acesso
  {
    digitalWrite(LedVerde, HIGH);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(LedVerde, LOW);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVerde, HIGH);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(LedVerde, LOW);
    delay(100);
    digitalWrite(Buzzer, LOW);
    lcd.setCursor(5, 2);
    lcd.print(messageStatic5);
    lcd.setCursor(0, 3);
    // print scrolling message
    lcd.print(messageStatic3);
    delay(500);
    lcd.clear();  // limpa a tela

    //Mantém acesso liberado até acionar o sensor de porta
    while (digitalRead(PortaAberta)) digitalWrite(Rele, LOW);
    digitalWrite(Rele, HIGH);
    while (digitalRead(PortaAberta)) digitalWrite(LedVerde, LOW);
    digitalWrite(LedVerde, HIGH);
  }

  if (conteudo.substring(1) != RFIDouNFC1)        //Cartão 1
    if (conteudo.substring(1) != RFIDouNFC2)      //Cartão 2
      if (conteudo.substring(1) != RFIDouNFC3)    //Cartão 3
        if (conteudo.substring(1) != RFIDouNFC4)  //outras tags

        {
          // Aciona LED vermelho e Buzzer
          digitalWrite(LedVermelho, HIGH);
          delay(155);
          digitalWrite(Buzzer, HIGH);
          delay(155);
          digitalWrite(LedVermelho, LOW);
          digitalWrite(Buzzer, LOW);
          delay(155);
          digitalWrite(LedVermelho, HIGH);
          delay(155);
          digitalWrite(Buzzer, HIGH);
          delay(155);
          digitalWrite(LedVermelho, LOW);
          digitalWrite(Buzzer, LOW);
          delay(155);
          digitalWrite(LedVermelho, HIGH);
          delay(155);
          digitalWrite(Buzzer, HIGH);
          delay(155);
          digitalWrite(LedVermelho, LOW);
          digitalWrite(Buzzer, LOW);
          delay(155);
          digitalWrite(LedVermelho, HIGH);
          delay(155);
          digitalWrite(Buzzer, HIGH);
          delay(155);
          digitalWrite(LedVermelho, LOW);
          digitalWrite(Buzzer, LOW);
          delay(155);
          lcd.setCursor(3, 2);
          lcd.print(messageStatic7);
          lcd.setCursor(3, 3);
          // print scrolling message
          lcd.print(messageStatic8);
          delay(5000);
          lcd.clear();  // limpa a tela

          //while (digitalRead(PortaAberta)) digitalWrite(LedVermelho, LOW);
          digitalWrite(LedVermelho, LOW);
          delay(100);
          digitalWrite(LedVermelho, HIGH);
          delay(100);
        }
}  //end rfid_func