#include <MFRC522.h> //biblioteca responsável pela comunicação com o módulo RFID-RC522
#include <SPI.h> //biblioteca para comunicação do barramento SPI
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

//Define os pinos do RF522
#define SS_PIN    21
#define RST_PIN   22

//Define os pinos para o display
#define DISPLAY_CS    5
#define DISPLAY_DC    2
#define DISPLAY_SDA   17
#define DISPLAY_SCK   16
#define DISPLAY_RST   4

//Define os pinos dos LEDs
#define pinVerde     12
#define pinVermelho  32

//Define tamanhos de blocos para manipular dados do RFID
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

// For 1.44" and 1.8" TFT with ST7735 use:
//Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, SDA, SCK, RST);
Adafruit_ST7735 tft = Adafruit_ST7735(DISPLAY_CS, DISPLAY_DC, DISPLAY_SDA, DISPLAY_SCK, DISPLAY_RST);

//esse objeto 'chave' é utilizado para autenticação
MFRC522::MIFARE_Key key;
//código de status de retorno da autenticação
MFRC522::StatusCode status;

// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

int LINHA_DO_DISPLAY = 0;

void setup() {
  // Inicia a serial
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  pinMode(pinVerde, OUTPUT);
  pinMode(pinVermelho, OUTPUT);

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  tft.setRotation(1); //Ajusta a direção do display

  Serial.println(F("Display iniciado."));


  // Inicia MFRC522
  mfrc522.PCD_Init();
  mensagensIniciais();
}

void loop()
{
  // Aguarda a aproximacao do cartao
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  //chama o menu e recupera a opção desejada
  int opcao = menu();
  // verifica se ainda está com o cartão/tag
  //  if ( ! mfrc522.PICC_IsNewCardPresent())
  //  {
  //    return;
  //  }

  if (opcao == 0)
    leituraDados();
  else if (opcao == 1)
    gravarDados();
  else {
    Serial.println(F("Opção Incorreta!"));
    displayPrint("Opcao incorreta!", ST77XX_WHITE);
    return;
  }

  // instrui o PICC quando no estado ACTIVE a ir para um estado de "parada"
  mfrc522.PICC_HaltA();
  // "stop" a encriptação do PCD, deve ser chamado após a comunicação com autenticação, caso contrário novas comunicações não poderão ser iniciadas
  mfrc522.PCD_StopCrypto1();
}

//faz a leitura dos dados do cartão/tag
void leituraDados()
{
  //imprime os detalhes tecnicos do cartão/tag
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

  //Prepara a chave - todas as chaves estão configuradas para FFFFFFFFFFFFh (Padrão de fábrica).
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //buffer para colocar os dados ligos
  byte buffer[SIZE_BUFFER] = {0};

  //bloco que faremos a operação
  byte bloco = 1;
  byte tamanho = SIZE_BUFFER;


  //faz a autenticação do bloco que vamos operar
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    tft.fillScreen(ST77XX_RED); //Pinta a tela de VERMELHO
    LINHA_DO_DISPLAY = 5;
    displayPrint("  FALHA NA AUTENTICACAO!", ST77XX_WHITE);
    digitalWrite(pinVermelho, HIGH);
    delay(1000);
    digitalWrite(pinVermelho, LOW);
    delay(1000);
    mensagensIniciais();
    return;
  }

  //faz a leitura dos dados do bloco
  status = mfrc522.MIFARE_Read(bloco, buffer, &tamanho);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    tft.fillScreen(ST77XX_RED); //Pinta a tela de VERMELHO
    LINHA_DO_DISPLAY = 5;
    displayPrint("  FALHA NA LEITURA!", ST77XX_WHITE);
    digitalWrite(pinVermelho, HIGH);
    delay(1000);
    digitalWrite(pinVermelho, LOW);
    mensagensIniciais();
    return;
  }
  else {
    tft.fillScreen(ST77XX_GREEN); //Pinta a tela de VERDE
    LINHA_DO_DISPLAY = 5;
    displayPrint("  SUCESSO NA LEITURA!", ST77XX_BLACK);
    digitalWrite(pinVerde, HIGH);
    delay(1000);
    digitalWrite(pinVerde, LOW);
    mensagensIniciais();
  }

  Serial.print(F("\nDados bloco ["));
  Serial.print(bloco); Serial.print(F("]: "));

  //imprime os dados lidos
  for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++)
  {
    Serial.write(buffer[i]);
  }
  Serial.println(" ");
}

//faz a gravação dos dados no cartão/tag
void gravarDados()
{
  //imprime os detalhes tecnicos do cartão/tag
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
  // aguarda 30 segundos para entrada de dados via Serial
  Serial.setTimeout(30000L) ;
  Serial.println(F("Insira os dados a serem gravados com o caractere '#' ao final\n[máximo de 16 caracteres]:"));
  tft.fillScreen(ST77XX_BLACK); //Pinta a tela de PRETO
  LINHA_DO_DISPLAY = 1;
  displayPrint("FERNANDO K TECNOLOGIA", ST77XX_WHITE);
  displayPrint("1 - Digite os dados.", ST77XX_WHITE);
  LINHA_DO_DISPLAY = 3;
  displayPrint("2 - Insira o caractere #.", ST77XX_WHITE);
  LINHA_DO_DISPLAY = 5;
  displayPrint("[maximo de 16 caracteres]", ST77XX_WHITE);

  //Prepara a chave - todas as chaves estão configuradas para FFFFFFFFFFFFh (Padrão de fábrica).
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //buffer para armazenamento dos dados que iremos gravar
  byte buffer[MAX_SIZE_BLOCK] = "";
  byte bloco; //bloco que desejamos realizar a operação
  byte tamanhoDados; //tamanho dos dados que vamos operar (em bytes)

  //recupera no buffer os dados que o usuário inserir pela serial
  //serão todos os dados anteriores ao caractere '#'
  tamanhoDados = Serial.readBytesUntil('#', (char*)buffer, MAX_SIZE_BLOCK);
  //espaços que sobrarem do buffer são preenchidos com espaço em branco
  for (byte i = tamanhoDados; i < MAX_SIZE_BLOCK; i++)
  {
    buffer[i] = ' ';
  }

  bloco = 1; //bloco definido para operação
  String str = (char*)buffer; //transforma os dados em string para imprimir
  Serial.println(str);

  //Authenticate é um comando para autenticação para habilitar uma comuinicação segura
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    bloco, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    tft.fillScreen(ST77XX_RED); //Pinta a tela de VERMELHO
    LINHA_DO_DISPLAY = 5;
    displayPrint("  FALHA NA AUTENTICACAO!", ST77XX_WHITE);
    digitalWrite(pinVermelho, HIGH);
    delay(1000);
    digitalWrite(pinVermelho, LOW);
    mensagensIniciais();
    return;
  }
  //else Serial.println(F("PCD_Authenticate() success: "));

  //Grava no bloco
  status = mfrc522.MIFARE_Write(bloco, buffer, MAX_SIZE_BLOCK);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    tft.fillScreen(ST77XX_RED); //Pinta a tela de VERMELHO
    LINHA_DO_DISPLAY = 5;
    displayPrint("  FALHA NA GRAVACAO!", ST77XX_WHITE);
    digitalWrite(pinVermelho, HIGH);
    delay(1000);
    digitalWrite(pinVermelho, LOW);
    mensagensIniciais();
    return;
  }
  else {
    Serial.println(F("MIFARE_Write() success: "));
    tft.fillScreen(ST77XX_GREEN); //Pinta a tela de VERDE
    LINHA_DO_DISPLAY = 5;
    displayPrint("  GRAVADO COM SUCESSO!", ST77XX_BLACK);
    digitalWrite(pinVerde, HIGH);
    delay(1000);
    digitalWrite(pinVerde, LOW);
  }
  mensagensIniciais();
}

//menu para escolha da operação
int menu()
{
  Serial.println(F("\nEscolha uma opção:"));
  Serial.println(F("0 - Leitura de Dados"));
  Serial.println(F("1 - Gravação de Dados\n"));

  tft.fillScreen(ST77XX_BLACK); //Pinta a tela de preto
  LINHA_DO_DISPLAY = 1;
  displayPrint("FERNANDO K TECNOLOGIA", ST77XX_WHITE);
  LINHA_DO_DISPLAY = 3;
  displayPrint("Escolha uma opcao:", ST77XX_WHITE);
  displayPrint("0 - Leitura de Dados", ST77XX_WHITE);
  displayPrint("1 - Gravacao de Dados", ST77XX_WHITE);

  //fica aguardando enquanto o usuário nao enviar algum dado
  while (!Serial.available()) {};

  //recupera a opção escolhida
  int op = (int)Serial.read();
  //remove os proximos dados (como o 'enter ou \n' por exemplo) que vão por acidente
  while (Serial.available()) {
    if (Serial.read() == '\n') break;
    Serial.read();
  }
  return (op - 48); //do valor lido, subtraimos o 48 que é o ZERO da tabela ascii
}

void mensagensIniciais()
{
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  // Mensagens iniciais no display
  tft.fillScreen(ST77XX_BLACK); //Pinta a tela de preto
  LINHA_DO_DISPLAY = 1;
  displayPrint("FERNANDO K TECNOLOGIA", ST77XX_WHITE);
  LINHA_DO_DISPLAY = 3;
  displayPrint("Aproxime seu cartao", ST77XX_WHITE);
  displayPrint("do leitor...", ST77XX_WHITE);
  delay(1000);
}

//função para controlar linhas e automatizar impressão no display
void displayPrint(char *text, uint16_t color) {
  tft.setCursor(0, LINHA_DO_DISPLAY * 10);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
  LINHA_DO_DISPLAY++;
  if (LINHA_DO_DISPLAY > 12)
  {
    tft.fillScreen(ST77XX_BLACK); //Pinta a tela de preto
    LINHA_DO_DISPLAY = 0;
  }
}
