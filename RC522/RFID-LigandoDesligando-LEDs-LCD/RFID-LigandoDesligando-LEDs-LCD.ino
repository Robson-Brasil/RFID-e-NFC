//This code was Written by Oskar Polanski, known as oskarplayer27 or TheSurrenderPenguin, he is a 13 year old programmer
//Some bits are included in Polish (NONE IN POLISH, NOW IN ENGLISH), the native language of Oskar, so please change it, i have shown you where with the 2 slashes
#include <SPI.h> //Basically all the libraries, initializing, other stuff
#include <MFRC522.h>
int onoffstate = 0; // its for the "latch" of the relay

#define SS_PIN 5
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LedAzul 25
#define LedVerde 26
#define LedVermelho 27
#define Buzzer 17

// --- Variáveis Globais ---
char st[20];

// set the LCD number of columns and rows
int ColunasLCD = 20;
int LinhasLCD = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, ColunasLCD, LinhasLCD);  

void setup() {
  // put your setup code here, to run once:
lcd.begin();                      // initialize the lcd 
lcd.backlight();
Serial.begin(115200);
pinMode (LedVerde, OUTPUT ); //green led  - Relay signal - PIN 8
pinMode (17, OUTPUT ); //Buzzer - PIN 7
pinMode(27, OUTPUT); //red led
pinMode(25, OUTPUT); //blue led
SPI.begin();
mfrc522.PCD_Init();   // Initiate MFRC522
lcd.print("Coloque cartao >"); //Change to ur language, means put the card to the reader
Serial.println();
}

void loop() {
if ( ! mfrc522.PICC_IsNewCardPresent())
{
return;
}
if ( ! mfrc522.PICC_ReadCardSerial())
{
  return;
}
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();

  if (content.substring(1) == "62 92 7C 1C")// Change to your card's UID
  {
   if (onoffstate == 0)
   {
    lcd.print("A ler...");// Means:Reading..., change if u want
    lcd.clear();
    lcd.print("Ligado");// Means:ON, change to ur language
    digitalWrite(8, HIGH);
    onoffstate++;
    i();
    Serial.println(onoffstate);
    delay(2000);
    lcd.clear();
   }
else
   {
    lcd.print("A ler...");// Means:Reading..., change if u want
    lcd.clear();
    lcd.print("Desligado");// Means:OFF, change to ur language
    digitalWrite(LedVerde, LOW);
    onoffstate--;
    e();
    Serial.println(onoffstate);
    delay(2000);
    lcd.clear();
   }
  }
  else
   {
    lcd.clear();
    lcd.print(" Nao autorizado ");// Means:Bad card, change to ur language
    delay(500);
    v();
    v();
    v();
    lcd.clear();
   }
   lcd.print("Coloque cartao >");// Means:Put the card to the reader, change to ur language
   delay(2000);

  if (content.substring(2) == "2D CA 08 1C")// Change to your card's UID
  {
   if (onoffstate == 0)
   {
    lcd.print("A ler...");// Means:Reading..., change if u want
    lcd.clear();
    lcd.print("Ligado");// Means:ON, change to ur language
    digitalWrite(LedVerde, HIGH);
    onoffstate++;
    i();
    Serial.println(onoffstate);
    delay(2000);
    lcd.clear();
   }
else
   {
    lcd.print("A ler...");// Means:Reading..., change if u want
    lcd.clear();
    lcd.print("Desligado");// Means:OFF, change to ur language
    digitalWrite(LedVerde, LOW);
    onoffstate--;
    e();
    Serial.println(onoffstate);
    delay(2000);
    lcd.clear();
   }
  }
  else
   {
    lcd.clear();
    lcd.print(" Nao autorizado ");// Means:Bad card, change to ur language
    delay(500);
    v();
    v();
    v();
    lcd.clear();
   }
   lcd.print("Coloque cartao >");// Means:Put the card to the reader, change to ur language
   delay(2000);
   
    }


void v() {
  digitalWrite(Buzzer, HIGH);
  digitalWrite(LedVermelho, HIGH);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVermelho, LOW);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(LedVermelho, HIGH);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVermelho, LOW);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(LedVermelho, HIGH);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVermelho, LOW);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(LedVermelho, HIGH);
    delay(300);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedVermelho, LOW);
    delay(500);
 
}

void e() {
  digitalWrite(Buzzer, HIGH);
  digitalWrite(LedAzul, HIGH);
    delay(100);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LedAzul, LOW);
    delay(100);
 
}

void i() {
  e();
  e();
}
