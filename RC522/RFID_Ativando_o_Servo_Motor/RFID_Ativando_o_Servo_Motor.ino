// Programa : RFID - Teste de Conceito, ativando Servo-Motor
// Autor : Robson Brasil
// Versão : 2
// Última Modificação : 01/01/2020 

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
 
Servo microservo9g;

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 6 //define green LED pin
#define LED_R 7 //define red LED
#define BUZZER 4 //buzzer pin
// Definições pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 

// Leds indicadores acesso liberado ou negado
int led_liberado = 6;
int led_negado = 7;

char st[20];

void setup() 
{
  pinMode(led_liberado, OUTPUT);
  pinMode(led_negado, OUTPUT);
  // Define que o servo esta ligado a porta digital 3
  microservo9g.attach(5);
  // Move o servo para a posição inicial (Ex.: Porta Fechada)
  microservo9g.write(180);
  // Inicia a serial
  Serial.begin(9600);
  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartão no leitor...");
  Serial.println();
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
  
// TAG Liberada para acesso - Caveiro Azul
  if (conteudo.substring(1) == "39 BF F3 5C" || conteudo.substring(1) == "62 92 7C 1C")
  {
    // Acesso Liberado - Ex.: Porta Aberta e Acende o LED Verde
    microservo9g.write(-180);
    digitalWrite(led_liberado, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    Serial.println("Acesso Autorizado !");
    Serial.println();
    delay(2500);
    microservo9g.write(180);
    digitalWrite(led_liberado, LOW);
    }

   else   {
    Serial.println(" Acesso Negado");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }

}
