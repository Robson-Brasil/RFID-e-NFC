// Programa : RFID - Teste de Conceito, ativando Servo-Motor com Buzzer
// Autor : Robson Brasil
// Versão : 2
// Última Modificação : 01/01/2020 

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
 
Servo servo;

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 7 //define o pino do LED VERDE
#define LED_R 8 //define o pino do LED VERMELHO
#define LED_B 6 //define o pono do LED AZUL
#define BUZZER 4 //define o pino do BUZZER
// Definições pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 

// Leds indicadores acesso liberado ou negado
int led_liberado = 7; 
int led_negado = 8;

char st[50];

void setup() 
{
  pinMode(led_liberado, OUTPUT);
  pinMode(led_negado, OUTPUT);
  // Define que o servo esta ligado a porta digital 5
  servo.attach(5);
  // Move o servo para a posição inicial (Ex.: Porta Fechada)
  servo.write(0);
  // Inicia a serial
  Serial.begin(9600);
  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Por Favor, Aproxime o seu cartão no leitor...");
  Serial.println(" Aguarde a Liberação o Cartão está sendo analisado! ");
}

void loop() 
{
  // Aguarda a aproximação do cartão
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
// TAG Liberada para acesso - Chaveiro Azul
  if (conteudo.substring(1)== "39 BF F3 5C")
    {
    // Acesso Liberado - Ex.: Porta Aberta e Acende o LED Verde
    servo.write(180);
    digitalWrite(led_liberado, HIGH);
    tone(BUZZER, 1500);
    delay(100);
    noTone(BUZZER);
    Serial.println(F("Chaveiro Azul - Acesso Autorizado !"));
    Serial.println("Por favor, entre e Seja Bem-vindo");
    delay(2000);
    servo.write(0);
    digitalWrite(led_liberado, LOW);
    }

// TAG Liberada para acesso - Cartão Branco
  if (conteudo.substring(1)== "62 92 7C 1C")
  {
    // Acesso Liberado - Ex.: Porta Aberta e Acende o LED Verde
    servo.write(180);
    digitalWrite(led_liberado, HIGH);
    tone(BUZZER, 1500);
    delay(800);
    noTone(BUZZER);
    Serial.println("Cartão Branco - Acesso Autorizado !");
    Serial.println("Por favor, entre e Seja Bem-vindo");
    delay(2500);
    servo.write(0);
    digitalWrite(led_liberado, LOW);
    }

// TAG Liberada para acesso - Cartão Estudantil
  //if (conteudo.substring(1)== "2D CA 08 1C")
    {
   // Acesso Liberado - Ex.: Porta Aberta e Acende o LED Verde
   //servo.write(180);
   //digitalWrite(led_liberado, HIGH);
   //tone(BUZZER, 1500);
   //delay(800);
   //noTone(BUZZER);
   //Serial.println(F("Cartão Estudantil - Acesso Autorizado !"));
   //Serial.println("Por favor, entre e Seja Bem-vindo");
    //delay(3500);
    //servo.write(0);
    //digitalWrite(led_liberado, LOW);
    }

// TAG Liberada para acesso - Galaxy Note 4
  if (conteudo.substring(1)== "08 BB AD A7")
    {
    // Acesso Liberado - Ex.: Porta Aberta e Acende o LED Verde
    servo.write(-360);
    digitalWrite(led_liberado, HIGH);
    tone(BUZZER, 1500);
    delay(800);
    noTone(BUZZER);
    Serial.println(F("Ual, usando um Celular para destravar a porta !"));
    Serial.println("Celular Com Acesso Autorizado!!!!");
    delay(3500);
    servo.write(360);
    digitalWrite(led_liberado, LOW);
    }

else   {
    // Acesso Negado - Ex.: Acesso Negado e Acende o LED Vermelho
    Serial.println("Acesso Negado");
    digitalWrite(led_negado, HIGH);
    //delay(1000);
    tone(BUZZER, 440);
    delay(1000);
    digitalWrite(led_negado, LOW);
    noTone(BUZZER);
  }
  
}
