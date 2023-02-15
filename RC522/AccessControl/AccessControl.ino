#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define RST_PIN 9
#define SS_PIN 10
#define buzz 8
#define resetAlarm 7
#define redLed 6
#define blueLed 5
#define doorOpen 80
#define doorClose 180

//byte readCard[4];
char *accessID[] = {"938B34B", "Fuzzi Boy"}; // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";
String unknownID = "Unknown";
bool ledBlink = false;

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); //Parameters: (addr,columns,rows)
Servo doorServo;

void setup()
{
  // Initiating
  Serial.begin(9600);
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  doorServo.attach(3); //Servo attached pin 3
  doorServo.write(doorClose); //Ensure the door is closed
  lcd.init(); // LCD screen
  lcd.backlight();

  lcd.clear();
  lcd.print(" Access Control ");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card >");
  pinMode(resetAlarm, INPUT);
  digitalWrite(resetAlarm, HIGH);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
}

void loop()
{
  int rsAlarm = HIGH;
  //Wait until new tag is available
  while (getID())
  {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (tagID == accessID[0])
    {

      lcd.print("Access Granted");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print(accessID[1]);
      welcomeSound();
      openDoor();
      countDown(10);
      closeDoor();
    }
    else
    {
      lcd.print("Keep Out!");
      tone(buzz, 900, 100);
      if (unknownID != tagID) {
        lcd.setCursor(0, 1);
        lcd.print("Unknown Person");
        unknownID = tagID;
      }
      else
      { lcd.setCursor(0, 1);
        lcd.print("Alarm On!!!");
        while (rsAlarm == HIGH) {
          alarmSound();
          rsAlarm = digitalRead(resetAlarm);
        }
        lcd.clear();
        lcd.print("Alarm Off!");
        digitalWrite(redLed, LOW);
        digitalWrite(blueLed, LOW);
      }
    }

    delay(3000);

    lcd.clear();
    lcd.print(" Access Control ");
    lcd.setCursor(0, 1);
    lcd.print("Scan Your Card >");
  }
}

//Read new tag if available
boolean getID()
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new Card placed to RFID reader continue
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a Card placed get Serial and continue
    return false;
  }
  tagID = "";
  for (byte i = 0; i < 4; i++) { // We use have 4 byte UID
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}

void welcomeSound() {
  tone(buzz, 660, 100);
  delay(150);
  tone(buzz, 660, 100);
  delay(300);
  tone(buzz, 660, 100);
  delay(300);
  tone(buzz, 510, 100);
  delay(100);
  tone(buzz, 660, 100);
  delay(300);
  tone(buzz, 770, 100);
  delay(550);
}

void alarmSound() {
  static byte times = 10;
  byte longSiren = random(0, 3);
  if (times > 0) {
    digitalWrite(redLed, ledBlink);
    digitalWrite(blueLed, !ledBlink);
    shortHorn();
    times --;
    ledBlink = !ledBlink;
  }
  else {
    for (int l = 0; l < longSiren; l++) {
      longHorn();
      ledBlink = !ledBlink;
    }
    times = 10;
  }
}

void longHorn() {                            
  for (int hz = 440; hz < 1000; hz++) {
    tone(buzz, hz, 80);
    delay(5);
  }
  digitalWrite(redLed, ledBlink);
  digitalWrite(blueLed, !ledBlink);
  for (int hz = 1000; hz > 440; hz--) {
    tone(buzz, hz, 80);
    delay(5);
  }
  digitalWrite(redLed, ledBlink);
  digitalWrite(blueLed, !ledBlink);
}

void shortHorn() {              

  // Whoop up
  for (int hz = 440; hz < 1000; hz += 25) {
    tone(buzz, hz, 50);
    delay(5);
  }
  // Whoop down
  for (int hz = 1000; hz > 440; hz -= 25) {
    tone(buzz, hz, 50);
    delay(5);
  }
}

void openDoor() {
  lcd.clear();
  lcd.print("Door Open");
  for (byte i = doorClose; i >= doorOpen; i--) {
    doorServo.write(i);
    delay(20);
  }
}

void closeDoor() {
  for (byte i = doorOpen; i <= doorClose; i++) {
    doorServo.write(i);
    delay(15);
  }
  lcd.clear();
  tone(buzz, 700, 100);
  lcd.print("Door closed");
}

void countDown(int count) {
  lcd.setCursor(0,1);
  lcd.print("Close after: ");
  for (int i = count; i >= 0; i--) {
    lcd.setCursor(13, 1);
    lcd.print(i);
    tone(buzz, 1000, 100);
    delay(900);
    lcd.setCursor(13, 1);
    lcd.print("  ");
  }
}
