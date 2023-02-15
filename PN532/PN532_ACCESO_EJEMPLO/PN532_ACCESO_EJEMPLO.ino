
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

String tags[] = {"62 92 7C 1C", "2D CA 08 1C" };  
boolean autorizado= false;

void setup(void) {
    pinMode(13,OUTPUT);
    Serial.begin(115200);
    nfc.begin();
}

void loop(void) {
   
  if (nfc.tagPresent()){
    NfcTag tag = nfc.read();
    Serial.println("Tag id: "+tag.getUidString());
     
    for(int i = 0; i < sizeof(tags); i++){  
      if( tag.getUidString() == tags[i] ){
        autorizado=true;
      }
    }
    
    if(autorizado){
      Serial.println("AUTORIZADO!");
      digitalWrite(13,HIGH);
      delay(1500);
      digitalWrite(13,LOW);
      autorizado=false;
    }else{
      Serial.println("NO AUTORIZADO...");
    }
  
  }
  delay(500);

}
