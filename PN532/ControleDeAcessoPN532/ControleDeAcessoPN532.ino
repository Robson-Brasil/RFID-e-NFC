//exam tag1 = 11111, tag2 = 22222, tag3 = 33333

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ 6
#define PN532_RESET 3
#define BUZZER_PIN 4
#define SOLENOID_PIN 5

bool success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;
unsigned int hex_num = 0;

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
  Serial.begin(115200); Serial.println("Hello!");

  attachInterrupt(0, manualButton, FALLING);
  pinMode(SOLENOID_PIN, OUTPUT); digitalWrite(SOLENOID_PIN, HIGH);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, HIGH), delay(100), digitalWrite(BUZZER_PIN, LOW);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.println("Waiting for Tag");
  solenoidLock();
}

void loop(void) {

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");

    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");

    Serial.print("Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (i == 0) {
        hex_num = uid[i] << 24;
      } else if (i == 1) {
        hex_num += uid[i] << 16;
      } else if (i == 2) {
        hex_num += uid[i] << 8;
      } else {
        hex_num += uid[i];
      }
    }

    Serial.println(hex_num);
    Serial.println("");

    if (hex_num == 31772 || hex_num == 2076 || hex_num == 33333) {
      digitalWrite(BUZZER_PIN, HIGH), delay(200), digitalWrite(BUZZER_PIN, LOW);
      solenoidUnlock();
    } else {
      for (int i = 0; i <= 2; i++)delay(100), digitalWrite(BUZZER_PIN, HIGH), delay(100), digitalWrite(BUZZER_PIN, LOW);
      delay(300);
      solenoidLock();
    }

  } else {
    Serial.println("Timed out waiting for a card");
    solenoidLock();
  }
}

void manualButton() {
  Serial.println("button");
  int i = 0;
  while (i <= 750) {
    Serial.println(i);
    digitalWrite(SOLENOID_PIN, HIGH);
    if (i < 100) {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
    Serial.println("UNLOCK");
    i++;
  }
  solenoidLock();
}

void solenoidLock() {
  hex_num = 0;
  digitalWrite(SOLENOID_PIN, HIGH);
  Serial.println("LOCK");
}

void solenoidUnlock() {
  hex_num = 0;
  Serial.println("UNLOCK");
  digitalWrite(SOLENOID_PIN, LOW);
  delay(2000);
  solenoidLock();
}
