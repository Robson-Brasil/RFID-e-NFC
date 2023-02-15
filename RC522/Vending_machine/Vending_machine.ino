
#include<SPI.h>
#include<MFRC522.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);


int a, b, c, d;
int led=4;
int valor1=5;
int valor2=3;
int valor3=4;
int valor4=7;
int cred1=5;
int cred2=6;
int cred3=7;
int cred4=8;
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;

void setup(){
  Serial.begin(9600);
  SPI.begin();
  lcd.begin(20, 4);
   pinMode(led, OUTPUT); 
   pinMode(5,INPUT_PULLUP);
   pinMode(6,INPUT_PULLUP);
   pinMode(7,INPUT_PULLUP);
   pinMode(8,INPUT_PULLUP);
  mfrc522.PCD_Init();
  
  Serial.print(a);
  Serial.print(b);
  Serial.print(c);
  Serial.println(d);
  

}
void loop(){
  cred1=digitalRead(5);
  if(cred1==LOW){
    flag1++;
    delay(150);
  }
  cred2=digitalRead(6);
    if(cred2==LOW){
    flag2++;
    delay(150);
  }
  cred3=digitalRead(7);
    if(cred3==LOW){
    flag3++;
    delay(150);
  }
    cred4=digitalRead(8);
    if(cred4==LOW){
    flag4++;
    delay(150);
  }
inicio();
  
      if(! mfrc522.PICC_IsNewCardPresent()){
        return;
      }
        if(! mfrc522.PICC_ReadCardSerial()){
        return;
      } 
          
      Serial.print("UID tag: ");
      String content = "";
      byte letter;
      for(byte i=0; i<mfrc522.uid.size; i++){
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      Serial.print("Mensagem : ");
      content.toUpperCase();
      if(content.substring(1) == "53 24 E4 D5"){
        a=1;
      }
            if(content.substring(1) == "35 53 0E 2D"){
        b=1;
      }
            if(content.substring(1) == "55 06 67 2D"){
        c=1;
      }
            if(content.substring(1) == "73 54 0A 27"){
        d=1;
      }
  //a
  
  if(a==1){
     
      
      
      if(content.substring(1) == "53 24 E4 D5"&&flag1>0){
        Serial.println("Produto 1");
        lcd.setCursor(0,2);
        lcd.print("      Produto 1     ");
       Serial.print(flag1);
       Serial.print(" Crédito(s) no valor de R$");
        lcd.setCursor(0,3);
        lcd.print("CREDITO: ");
        lcd.print(flag1);
        lcd.print("  R$");
        lcd.print(valor1*flag1);
        lcd.print(",00");
        Serial.print(valor1*flag1);
        Serial.println(",00");
        delay(4000);
        Serial.println("Retire o produto 1");
        lcd.setCursor(0,3);
        flag1--;
        lcd.print(" Retire o produto 1 ");
        digitalWrite(4, HIGH); 
        delay(3000);    
        digitalWrite(4, LOW);
        lcd.setCursor(0,3);
        lcd.print("RESTANTE: ");
        lcd.print(flag1);
        lcd.print("  R$");
        lcd.print(valor1*flag1);
        lcd.print(",00");
        delay(3000);
      }
      if(content.substring(1) == "53 24 E4 D5"&&flag1==0){
         Serial.println("Crédito zerado. Recarregue.");
         lcd.setCursor(0,2);
         lcd.print("     RECARREGUE     "); 
         lcd.setCursor(0,3); 
         lcd.print("    Sem  Credito    "); 
         delay(3000);
         a=0;
      }
      
  }
  ///////////////////////////
    if(b==1){
     
      
      
      if(content.substring(1) == "35 53 0E 2D"&&flag2>0){
        Serial.println("Produto 2");
        lcd.setCursor(0,2);
        lcd.print("      Produto 2     ");
       Serial.print(flag2);
       Serial.print(" Crédito(s) no valor de R$");
        lcd.setCursor(0,3);
        lcd.print("CREDITO: ");
        lcd.print(flag2);
        lcd.print("  R$");
        lcd.print(valor2*flag2);
        lcd.print(",00");
        Serial.print(valor2*flag2);
        Serial.println(",00");
        delay(4000);
        Serial.println("Retire o produto 2");
        lcd.setCursor(0,3);
        lcd.print(" Retire o produto 2 ");
        flag2--;
        digitalWrite(4, HIGH); 
        delay(3000);      
        digitalWrite(4, LOW);
        lcd.setCursor(0,3);
        lcd.print("RESTANTE: ");
        lcd.print(flag2);
        lcd.print("  R$");
        lcd.print(valor2*flag2);
        lcd.print(",00");
        delay(3000);
      }
      if(content.substring(1) == "35 53 0E 2D"&&flag2==0){
         Serial.println("Crédito zerado. Recarregue.");
         lcd.setCursor(0,2);
         lcd.print("     RECARREGUE     "); 
         lcd.setCursor(0,3); 
         lcd.print("    Sem  Credito    "); 
         delay(3000);
         b=0;
      }
      
  }
 ////////////////////////
   if(c==1){
     
      
      
      if(content.substring(1) == "55 06 67 2D"&&flag3>0){
        Serial.println("Produto 3");
        lcd.setCursor(0,2);
        lcd.print("      Produto 3     ");
       Serial.print(flag3);
       Serial.print(" Crédito(s) no valor de R$");
        lcd.setCursor(0,3);
        lcd.print("CREDITO: ");
        lcd.print(flag3);
        lcd.print("  R$");
        lcd.print(valor3*flag3);
        lcd.print(",00");
        Serial.print(valor3*flag3);
        Serial.println(",00");
        delay(4000);
        Serial.println("Retire o produto 3");
        lcd.setCursor(0,3);
        lcd.print(" Retire o produto 3 "); 
        flag3--;
        digitalWrite(4, HIGH);
        delay(3000);        
        digitalWrite(4, LOW);
        lcd.setCursor(0,3);
        lcd.print("RESTANTE: ");
        lcd.print(flag3);
        lcd.print("  R$");
        lcd.print(valor3*flag3);
        lcd.print(",00");
        delay(3000);
      }
      if(content.substring(1) == "55 06 67 2D"&&flag3==0){
         Serial.println("Crédito zerado. Recarregue.");
         lcd.setCursor(0,2);
         lcd.print("     RECARREGUE     "); 
         lcd.setCursor(0,3); 
         lcd.print("    Sem  Credito    "); 
         delay(3000);
         c=0;
      }
      
  }
  /////////////////////////
    if(d==1){
     
      
      
      if(content.substring(1) == "73 54 0A 27"&&flag4>0){
        Serial.println("Produto 4");
        lcd.setCursor(0,2);
        lcd.print("      Produto 4     ");
       Serial.print(flag4);
       Serial.print(" Crédito(s) no valor de R$");
        lcd.setCursor(0,3);
        lcd.print("CREDITO: ");
        lcd.print(flag4);
        lcd.print("  R$");
        lcd.print(valor4*flag4);
        lcd.print(",00");
        Serial.print(valor4*flag4);
        Serial.println(",00");
        delay(4000);
        Serial.println("Retire o produto 4");
        lcd.setCursor(0,3);
        lcd.print(" Retire o produto 4 ");
        flag4--;
        digitalWrite(4, HIGH); 
        delay(3000);
        digitalWrite(4, LOW);
        lcd.setCursor(0,3);
        lcd.print("RESTANTE: ");
        lcd.print(flag4);
        lcd.print("  R$");
        lcd.print(valor4*flag4);
        lcd.print(",00");
        delay(3000);
      }
      if(content.substring(1) == "73 54 0A 27"&&flag4==0){
         Serial.println("Crédito zerado. Recarregue.");
         lcd.setCursor(0,2);
         lcd.print("     RECARREGUE     "); 
         lcd.setCursor(0,3); 
         lcd.print("    Sem  Credito    "); 
         delay(3000);
         d=0;
      }
      
  }
  delay(500);
}

void inicio(){
   //lcd.clear();   
   lcd.setCursor(1,0);
   lcd.print("P1");
   lcd.setCursor(6,0);
   lcd.print("P2");
   lcd.setCursor(11,0);
   lcd.print("P3");
   lcd.setCursor(16,0);
   lcd.print("P4");

   lcd.setCursor(1,1);
   if(flag1<10){lcd.print("0");}
   lcd.print(flag1);
   lcd.setCursor(6,1);
   if(flag2<10){lcd.print("0");}
   lcd.print(flag2);
   lcd.setCursor(11,1);
   if(flag3<10){lcd.print("0");}
   lcd.print(flag3);
   lcd.setCursor(16,1);
   if(flag4<10){lcd.print("0");}
   lcd.print(flag4);
   
  Serial.print(flag1);
  Serial.print(flag2);
  Serial.print(flag3);
  Serial.print(flag4);
  
   lcd.setCursor(1,2);
   lcd.print("Aproxime o cartao");
   Serial.println("  - Aproxime o cartão");
      lcd.setCursor(0,3);
      lcd.print("                    ");
}
 
