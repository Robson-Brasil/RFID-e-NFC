#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define os pinos do RFID
#define SS_PIN 15
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define os pinos dos LEDs e do buzzer
#define PINO_LED_VERMELHO 27
#define PINO_LED_VERDE 14
#define PINO_BUZZER 26
#define PINO_RELE 26

// Define as credenciais de WiFi e MQTT
const char* ssid = "SSID_DA_REDE";
const char* password = "SENHA_DA_REDE";
const char* mqtt_server = "IP_DO_BROKER_MQTT";
const char* mqtt_user = "USUARIO_MQTT";
const char* mqtt_password = "SENHA_MQTT";
const char* mqtt_topic = "cartao";
const char* lcd_topic = "lcd";

// Define o cliente WiFi e o cliente MQTT
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Define o LCD I2C
#define LCD_ADDR 0x3F
#define LCD_COLS 20
#define LCD_ROWS 4
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// Variável para armazenar o UID do cartão lido
String cartaoUID = "";

const int NUMERO_CARTOES = 4;
String cartoes[NUMERO_CARTOES] = {
  "23D27A23",
  "A18A47D3",
  "D3A47DA1",
  "5B5E2A8A"
};

void setup() {
  // Inicializa o serial e os pinos
  Serial.begin(115200);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);
  
  // Inicializa o RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Conecta-se à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede WiFi...");
  }
  Serial.println("Conectado à rede WiFi");
  
  // Conecta-se ao broker MQTT
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password )) {
      Serial.println("Conectado ao broker MQTT");
    } else {
      Serial.print("Falha na conexão ao broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
  
  // Inicializa o LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cartao: ");
}

void loop() {
  // Verifica se há novos cartões
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Lê o UID do cartão
    cartaoUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cartaoUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      cartaoUID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println("Cartao lido: " + cartaoUID);
    
    // Publica o UID do cartão no tópico "cartao"
    client.publish("cartao", cartaoUID.c_str());
    
    // Verifica se o cartão lido está na lista de cartões cadastrados
    boolean cartaoCadastrado = false;
    for (int i = 0; i < NUMERO_CARTOES; i++) {
      if (cartaoUID == cartoes[i]) {
        cartaoCadastrado = true;
        break;
      }
    }
    
    if (cartaoCadastrado) {
      // Se o cartão está cadastrado, liga o relé e pisca o LED verde 5 vezes
      digitalWrite(PINO_RELE, HIGH);
      for (int i = 0; i < 5; i++) {
        digitalWrite(PINO_LED_VERDE, HIGH);
        delay(100);
        digitalWrite(PINO_LED_VERDE, LOW);
        delay(100);
      }
      digitalWrite(PINO_LED_VERMELHO, LOW);
      digitalWrite(PINO_BUZZER, LOW);
    } else {
      // Se o cartão não está cadastrado, acende o LED vermelho e toca o buzzer 5 vezes
      digitalWrite(PINO_LED_VERMELHO, HIGH);
      for (int i = 0; i < 5; i++) {
        digitalWrite(PINO_BUZZER, HIGH);
        delay(100);
        digitalWrite(PINO_BUZZER, LOW);
        delay(100);
      }
      digitalWrite(PINO_RELE, LOW);
      digitalWrite(PINO_LED_VERDE, LOW);
    }
    
    // Escreve no display LCD a mensagem correspondente
    if (cartaoCadastrado) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Acesso permitido!");
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Acesso negado!");
    }
  }
  
  // Verifica se há mensagens recebidas do broker MQTT
  client.loop();
}
