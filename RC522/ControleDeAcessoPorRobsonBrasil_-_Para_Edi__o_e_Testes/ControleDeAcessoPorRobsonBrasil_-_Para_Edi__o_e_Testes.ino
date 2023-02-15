//Controle de Acesso com TAG via RFID
//Autor : Robson Brasil
//Versão : 25
//Última Modificação : 01/01/2019

// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// --- Mapeamento de Hardware ---
#define   SS_PIN    10
#define   RST_PIN    9
#define   led_green  7
#define   led_red    8
#define   buzzer     4
#define   door_lock  6    //Ligado em um LED para testes, toque-o pelo sensor que vc preferir
#define   door_sens  3    //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada
//#define   servo      5    //Servo Motor
Servo servo;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância com MFRC522

// --- Protótipo das Funções Auxiliares ---
void rfid_func();                            //Função para identificação das tags RFID
 
// --- Variáveis Globais --- 
char st[20];

// --- Configurações Iniciais ---
void setup() 
{

  pinMode(led_green, OUTPUT);
  pinMode(led_red,   OUTPUT);
  pinMode(buzzer,    OUTPUT);
  pinMode(door_lock, OUTPUT);       //Ligado em um LED para testes, toque-o pelo sensor que vc preferir
  pinMode(door_sens, INPUT_PULLUP); //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada
  servo.attach(5);                  // Define que o servo esta ligado a porta digital 5
  servo.write(0);                   // Move o servo para a posição inicial (Ex.: Porta Fechada)
  Serial.begin(9600);               // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();                      // Inicia comunicação SPI bus
  mfrc522.PCD_Init();               // Inicia MFRC522
  
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println("Se Autorizado, a porta se abrirá!!");

  //saídas iniciam desligadas
  digitalWrite(door_lock, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_red,   LOW);
  digitalWrite(buzzer,    LOW);
  
} //end setup

// --- Loop Infinito ---
void loop() 
{

    rfid_func();   //chama função para identificação de tags RFID

} //end loop

// === Funções Auxiliares ===
void rfid_func()                            //Função para identificação das tags RFID
{
      // -- Verifica novas tags --
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
      Serial.println();
      Serial.println();
      Serial.print("Número de Identificação da TAG lida!!!!  ---->");
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


      //Cadastro das TAG Novas
      if (conteudo.substring(1) == "62 92 7C 1C") //Cartão 1 para liberar acesso
      {
        Serial.println("Acesso Autorizado!");
        Serial.println();
        
        // Aciona LED Verde, buzzer e Servo Motor
        
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(buzzer,    HIGH);
        delay(100);
        digitalWrite(led_green,  LOW);
        delay(100);
        digitalWrite(buzzer,     LOW);
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(buzzer,    HIGH);
        delay(100);
        digitalWrite(led_green,  LOW);
        delay(100);
        digitalWrite(buzzer,     LOW);
                
        Serial.println(F("Cartão 1 !"));
        Serial.println("Por favor, entre e Seja Bem-vindo");
        servo.write(180);
        delay(500);
        servo.write(0);
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println("Aguardando a porta ser fechada");
        Serial.println();
        Serial.println();
      
        //Mantém acesso liberado até acionar o sensor de porta
        while(digitalRead(door_sens)) digitalWrite(door_lock, HIGH);
        digitalWrite(door_lock, LOW);
                         
      }

      //Cadastro das TAG Novas
      if (conteudo.substring(1) == "2D CA 08 1C") //Cartão 2 para liberar acesso
      {
        Serial.println("Acesso Autorizado!");
        Serial.println();
        
        // Aciona LED Verde, buzzer e Servo Motor
        
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(buzzer,    HIGH);
        delay(100);
        digitalWrite(led_green,  LOW);
        delay(100);
        digitalWrite(buzzer,     LOW);
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(buzzer,    HIGH);
        delay(100);
        digitalWrite(led_green,  LOW);
        delay(100);
        digitalWrite(buzzer,     LOW);
                
        Serial.println(F("Cartão 2 !"));
        Serial.println("Por favor, entre e Seja Bem-vindo");
        servo.write(180);
        delay(500);
        servo.write(0);
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println("Aguardando a porta ser fechada");
        Serial.println();
        Serial.println();
        
        //Mantém acesso liberado até acionar o sensor de porta
        while(digitalRead(door_sens)) digitalWrite(door_lock, HIGH);
        digitalWrite(door_lock, LOW);
      }

        if (conteudo.substring(1) == "CA 75 DD 23") //Cartão 3 para liberar acesso
      {
        Serial.println("Acesso Autorizado!");
        Serial.println();
        
        // Aciona LED Verde, buzzer e Servo Motor
        
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(buzzer,    HIGH);
        delay(100);
        digitalWrite(led_green,  LOW);
        delay(100);
        digitalWrite(buzzer,     LOW);
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(buzzer,    HIGH);
        delay(100);
        digitalWrite(led_green,  LOW);
        delay(100);
        digitalWrite(buzzer,     LOW);
                
        Serial.println(F("Cartão 3 !"));
        Serial.println("Por favor, entre e Seja Bem-vindo");
        servo.write(180);
        delay(500);
        servo.write(0);
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println("Aguardando a porta ser fechada");
        Serial.println();
        Serial.println();

        //Mantém acesso liberado até acionar o sensor de porta
        while(digitalRead(door_sens)) digitalWrite(door_lock, HIGH);
        digitalWrite(door_lock, LOW);
      }
     
      if (conteudo.substring(1) != "62 92 7C 1C")//Cartão 1
      if (conteudo.substring(1) != "2D CA 08 1C")//Cartão 2
      if (conteudo.substring(1) != "CA 75 DD 23")//Cartão 3
      if (conteudo.substring(1) != "XX XX XX XX") //outras tags
      
      {
        Serial.println();
        Serial.println();
        Serial.println("Acesso Negado");
        Serial.println("Por Favor, Identifique-se!!!!");
        Serial.println();
        Serial.println();
        
        // Aciona LED vermelho e buzzer
        digitalWrite(led_red,   HIGH);
        delay(155);
        digitalWrite(buzzer,    HIGH);
        delay(155);
        digitalWrite(led_red,    LOW);
        digitalWrite(buzzer,     LOW);
        delay(155);
        digitalWrite(led_red,   HIGH);
        delay(155);
        digitalWrite(buzzer,    HIGH);
        delay(155);
        digitalWrite(led_red,    LOW);
        digitalWrite(buzzer,     LOW);
        delay(155);
        digitalWrite(led_red,   HIGH);
        delay(155);
        digitalWrite(buzzer,    HIGH);
        delay(155);
        digitalWrite(led_red,    LOW);
        digitalWrite(buzzer,     LOW);
        delay(155);
        digitalWrite(led_red,   HIGH);
        delay(155);
        digitalWrite(buzzer,    HIGH);
        delay(155);
        digitalWrite(led_red,    LOW);
        digitalWrite(buzzer,     LOW);
        delay(155);
    }
} //end rfid_func
