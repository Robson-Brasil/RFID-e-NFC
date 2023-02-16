//This code was Written by Oskar Polanski, known as oskarplayer27 or TheSurrenderPenguin, he is a 13 year old programmer
//Some bits are included in Polish (NONE IN POLISH, NOW IN ENGLISH), the native language of Oskar, so please change it, i have shown you where with the 2 slashes
#include <SPI.h> //Basically all the libraries, initializing, other stuff
#include <MFRC522.h>
int onoffstate = 0; // its for the "latch" of the relay

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4}; //customisation ;D
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = { 0x1,0x1,0x5,0x9,0x1f,0x8,0x4};
  
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:
lcd.init();                      // initialize the lcd 
lcd.backlight();
Serial.begin(9600);
pinMode (8, OUTPUT ); //green led  - Relay signal - PIN 8
pinMode (7, OUTPUT ); //Buzzer - PIN 7
pinMode(5, OUTPUT); //red led
pinMode(6, OUTPUT); //blue led
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
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "F3 F7 F4 2E")// Change to your card's UID
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
    digitalWrite(8, LOW);
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
  digitalWrite(7, HIGH);
  digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);
    delay(300);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    delay(500);
 
}

void e() {
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    delay(100);
 
}

void i() {
  e();
  e();
}
