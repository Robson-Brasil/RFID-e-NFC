#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Definir os pinos do módulo RFID RC522
#define RST_PIN         2
#define SS_PIN          5
#define MOSI            23
#define MISO            19
#define SCK             18

// Definir o pino do relé
#define RELE            13

// Definir as credenciais da rede WiFi
const char* ssid = "RVR 2,4GHz";
const char* password = "RodrigoValRobson2021";

// Definir as credenciais do servidor MQTT
const char* mqttServer = "192.168.15.10";
const int mqttPort = 1883;
const char* mqttUser = "RobsonBrasil";
const char* mqttPassword = "loboalfa";
const char* subtopic = "RFID/Acesso/Estado";
const char* pubtopic = "RFID/Acesso/Comando";

// Inicializar o módulo RFID RC522
MFRC522 rfid(SS_PIN, RST_PIN);

// Inicializar o cliente WiFi
WiFiClient wifiClient;

// Inicializar o cliente MQTT
PubSubClient client(mqttServer, mqttPort, wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // Inicializar a comunicação serial
  Serial.begin(115200);
  while (!Serial);
  
  // Inicializar o pino do relé
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);

  // Inicializar o módulo RFID RC522
  SPI.begin();
  rfid.PCD_Init();

  // Conectar-se à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede WiFi...");
  }
  Serial.println("Conectado à rede WiFi!");

  // Conectar-se ao servidor MQTT
  client.connect("ESP32Client", mqttUser, mqttPassword);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão ao servidor MQTT, código de erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  client.setCallback(callback);

  // Inscrever-se no tópico
  client.subscribe(subtopic);
}

void reconnectMQTT() {
  // Enquanto não estiver conectado, tente se reconectar
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão ao servidor MQTT, código de erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  // Inscrever-se no tópico
  client.subscribe(subtopic);
}

void loop() {
  // Verifica se há novos cartões RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Lê o número do cartão
    String cardID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      cardID += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.println("Cartão lido: " + cardID);
    
    // Verifica se o cartão é válido
    if (cardID == "62927c1c") {
      // Ativando o relé
      digitalWrite(RELE, LOW);
      Serial.println("Cartão válido. Autorizado.");
      
      // Publicando mensagem no tópico
      client.publish("RFID/Acesso/Comando", "AUTORIZADO");

      delay(5000);    
      digitalWrite(RELE, HIGH);      
        
    } else {
      // Desativando o relé
      digitalWrite(RELE, HIGH);
      Serial.println("Cartão inválido. NAO AUTORIZADO.");
      
      // Publicando mensagem no tópico
      client.publish("RFID/Acesso/Comando", "NAO AUTORIZADO!");
   
    }
  }
  
  // Verifica a conexão com o servidor MQTT
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  // Aguarda um segundo antes de verificar os cartões RFID novamente
  delay(1000);
}
