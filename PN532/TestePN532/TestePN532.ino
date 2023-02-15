#include <Wire.h> //
#include <PN532_I2C.h> //Set PN532 i2C Library can be downloaded below.
#include <PN532.h>   //Library can be dowloaded below
#include <NfcAdapter.h>

#define PN532_IRQ   (17)
#define PN532_RESET (16)
#define BUZZER_PIN 33
#define SOLENOID_PIN 4

PN532_I2C pn532(Wire); // Set PN532 runs on i2C Procotol
NfcAdapter nfc = NfcAdapter(pn532);

bool success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;
unsigned int hex_num = 0;

void setup(void) {
  Serial.begin(115200); Serial.println("Hello!");

  pinMode(SOLENOID_PIN, OUTPUT); digitalWrite(SOLENOID_PIN, HIGH);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, HIGH), delay(100), digitalWrite(BUZZER_PIN, LOW);

  nfc.begin();
}
//**********************************************************************************************//

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
 
void loop(void) {

   Serial.println("Please wait while scanning any NFC device");  
  delay(2000);
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString()); 
 
    if (tag.hasNdefMessage()) //set if there is message in the tags
    {
      NdefMessage message = tag.getNdefMessage();
      Serial.print("NFC TAG MSG: ");
      Serial.print(message.getRecordCount());
      Serial.print("NFC Tag Data");
 
      if (message.getRecordCount() != 1) { 
        Serial.print(".");
      }
 
      Serial.println("Failed No NFC Device Found");
      delay(1000);

      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
       {
        Serial.print("NDEFRecord ");
        Serial.println(i+1);   
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
 
        String valString = "";
        for (int c = 0; c < payloadLength; c++) {
          valString += (char)payload[c];
        }
        Serial.print("INFO (as String): ");
        Serial.println(valString);
        

    String uid = record.getId(); 

    if (uid != "62:92:7C:1C") {
      digitalWrite(BUZZER_PIN, HIGH), delay(200), digitalWrite(BUZZER_PIN, LOW);
      solenoidUnlock();
      Serial.println("");
      Serial.println(uid);

    } else {
      for (int i = 0; i <= 2; i++)delay(100), digitalWrite(BUZZER_PIN, HIGH), delay(100), digitalWrite(BUZZER_PIN, LOW);
      delay(300);
      solenoidLock();
    }
}
  }
}
}
