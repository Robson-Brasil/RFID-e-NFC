#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         5           // Configuração do pino RST do RC522, o valor padrão é 5
#define SS_PIN          21          // Configuração do pino SDA(SS) do RC522, o valor padrão é 21
#define LED_VERDE       19          // Configuração do pino do LED verde, o valor padrão é 19
#define LED_VERMELHO    18          // Configuração do pino do LED vermelho, o valor padrão é 18
#define BUZZER          23          // Configuração do pino do buzzer, o valor padrão é 23
#define RELE            26

#define WIFI_SSID       "SEU_WIFI_SSID"
#define WIFI_PASSWORD   "SUA_SENHA_DO_WIFI"
#define MQTT_SERVER     "SEU_ENDERECO_DO_BROKER_MQTT"
#define MQTT_PORT       1883
#define MQTT_USER       "SEU_USUARIO_MQTT"
#define MQTT_PASSWORD   "SUA_SENHA_MQTT"

#define MAX_CARTOES     10          // Número máximo de cartões que podem ser cadastrados
#define UID_SIZE        4           // Tamanho do UID do cartão (em bytes)
#define CARTAO_MESTRE   "244BDDB7"  // UID do cartão mestre ou administrador

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Criação do objeto MFRC522
WiFiClient wifiClient;              // Criação do objeto WiFiClient
PubSubClient client(wifiClient);    // Criação do objeto PubSubClient
LiquidCrystal_I2C lcd(0x27, 20, 4); // Criação do objeto LiquidCrystal_I2C

String cartaoUID;                   // Armazena o UID do cartão lido
String cartoes[MAX_CARTOES];        // Array para armazenar os UIDs dos cartões cadastrados
int num_cartoes = 0;                // Número atual de cartões cadastrados

// Função para inicializar a conexão com o WiFi
void connectWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("Conectado ao WiFi com sucesso!");
}

// Função para inicializar a conexão com o MQTT Broker
void connectMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou! Código de erro: ");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

// Função para acionar o relé
void acionarRele() {
  digitalWrite(LED_VERDE, HIGH);
  delay(1000);
  digitalWrite(LED_VERDE, LOW);
}

// Função para cadastrar um novo cartão
void cadastrarCartao() {
  // Verificando se o número máximo de cartões foi atingido
  if (num_cartoes >= MAX_CARTOES) {
    Serial.println("Numero maximo de cartoes cadastrados atingido!");
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Aproxime o");
  lcd.println("cartao a ser");
  lcd.println("cadastrado");
  
  // Aguarda a leitura do cartão
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial());
  
  // Lê o UID do cartão
  String cartaoUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cartaoUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    cartaoUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  // Verifica se o cartão já foi cadastrado
  for (int i = 0; i < num_cartoes; i++) {
    if (cartoes[i] == cartaoUID) {
      Serial.println("Cartao ja cadastrado!");
      return;
    }
  }
  
  // Cadastra o cartão
  cartoes[num_cartoes] = cartaoUID;
  num_cartoes++;
  
  Serial.println("Cartao cadastrado: " + cartaoUID);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Cartao");
  lcd.println("cadastrado:");
  lcd.println(cartaoUID);
  
  // Publica o UID do cartão no tópico "cartao"
  client.publish("cartao", cartaoUID.c_str());
}

// Função para indicar que o cartão não foi cadastrado
void cartaoNaoCadastrado() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
    delay(500);
  }
}
void setup() {
  // Inicia a comunicação serial
  Serial.begin(115200);
  
  // Inicia o módulo RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(RELE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  //Be careful how Rele circuit behave on while resetting or power-cycling your Arduino
  digitalWrite(RELE, HIGH);        // Make sure door is locked
  digitalWrite(LED_VERMELHO, LOW); // Make sure led is off
  digitalWrite(LED_VERDE, LOW);   // Make sure led is off
  digitalWrite(BUZZER, LOW);
  
  // Inicia a comunicação com o display LCD
  lcd.begin();
  lcd.backlight();
}

void loop() {
  // Verifica se há novos cartões
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Lê o UID do cartão
    String cartaoUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cartaoUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      cartaoUID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println("Cartao lido: " + cartaoUID);

    // Verifica se o cartão é o cartão mestre
    if (cartaoUID == CARTAO_MESTRE) {
      cadastrarCartao();
      return;
    }

    // Verifica se o cartão está cadastrado
    bool cartao_cadastrado = false;
    for (int i = 0; i < num_cartoes; i++) {
      if (cartaoUID == cartoes[i]) {
        cartao_cadastrado = true;
        break;
      }
    }

    if (cartao_cadastrado) {
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(RELE, HIGH);
      delay(1000);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(RELE, LOW);
      Serial.println("Acionando rele!");
      client.publish("cartao", cartaoUID.c_str());
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cartao:");
      lcd.setCursor(0,1);
      lcd.print(cartaoUID);
      lcd.setCursor(0,2);
      lcd.print("Acesso Liberado");
      delay(5000);
      lcd.clear();
    } else {
      cartaoNaoCadastrado();
      Serial.println("Cartao nao cadastrado!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cartao:");
      lcd.setCursor(0,1);
      lcd.print(cartaoUID);
      lcd.setCursor(0,2);
      lcd.print("Acesso Negado");
      delay(5000);
      lcd.clear();
    }
  }

  // Verifica se há mensagens recebidas do broker MQTT
  client.loop();
}
