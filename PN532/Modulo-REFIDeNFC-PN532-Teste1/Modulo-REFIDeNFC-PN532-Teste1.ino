#include <Wire.h>
 
#include <PN532_I2C.h>
 
#include <PN532.h>
 
#include <NfcAdapter.h>
 
PN532_I2C pn532_i2c(Wire);
 
int ledpin1=5;
 
int ledpin2=4;
 
NfcAdapternfc = NfcAdaptador(pn532_i2c);
 
  String tagId1 = "FA 5F 99 1A";
 
   String tagId2= "39 0B B6 B0";
 
String tagId = "None";
 
bytenuidPICC[4];
 
void setup(void) {
 
Serial.begin(115200);
 
pinMode(ledpin1,OUTPUT);
 
pinMode(ledpin2,OUTPUT);
 
Serial.println("System initialized");
 
nfc.begin();
 
digitalWrite(ledpin1, LOW);
 
digitalWrite(ledpin2, LOW);
 
}
 
void loop() {
 
readNFC();
 
if(tagId==tagId1)
 
  {
 
if(digitalRead(ledpin1) == 0)
 
    {
 
digitalWrite(ledpin1, HIGH);
 
tagId = "";
 
delay(1000);
 
    }
 
  }
 
if(tagId==tagId1)
 
  {
 
if(digitalRead(ledpin1) == 1)
 
    {
 
digitalWrite(ledpin1, LOW);
 
tagId = "";
 
delay(1000);
 
    }
 
  }
 
if(tagId==tagId2)
 
  {
 
if(digitalRead(ledpin2) == 0)
 
    {
 
digitalWrite(ledpin2, HIGH);
 
tagId = "";
 
delay(1000);
 
    }
 
  }
 
if(tagId==tagId2)
 
  {
 
if(digitalRead(ledpin2) == 1)
 
    {
 
digitalWrite(ledpin2, LOW);
 
tagId = "";
 
delay(1000);
 
    }
 
  }
 
}
 
voidreadNFC() {
 
if (nfc.tagPresent())
 
  {
 
NfcTag tag = nfc.read();
 
tag.print();
 
tagId = tag.getUidString();
 
Serial.println("Tag id");
 
Serial.println(tagId);
 
  }
 
delay(1000);
 
}