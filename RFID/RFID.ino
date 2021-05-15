/**
   @author: John Melody Me
   @contact: <JOhnmelody@dingtalk.com>
*/
#include "SPI.h"
#include "MFRC522.h"

/**
   +-----------------+-------------+
   | RFID RC522 PIN  | ARDUINO UNO |
   +-----------------+-------------+
   | SDA             | 10          |
   | SCK             | 13          |
   | MOSI            | 11          |
   | MISO            | 12          |
   | IRQ             | UNUSED      |
   | GND             | GND         |
   | RST             | 9           |
   | 3.3V            | 3.3V        |
   +-----------------+-------------+
*/

#define PIN_RST 9
#define PIN_SDA 10
#define BAUD_RATE 9600
#define RFID_TONE 1000
#define BUZZER 3
#define DURATION 500
#define PIEZO_PIN 8

#define MASTER_TAG_PROHIBITED "6FB077CC"
#define MASTER_TAG "D743AF7B"
#define MASTER_TAG_ALT "5527112A"

int tones[] = {
  1915,
  1700,
  1519,
  1432,
  1275,
  1136,
  1014,
  956
};

String tagID = "";

MFRC522 mfrc522(PIN_SDA, PIN_RST);

void setup()
{
  // Initiate SPI connection
  SPI.begin();

  // Initiate Proximity Coupling Device
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  // Initiate Serial Connection
  Serial.begin(BAUD_RATE);
  while (!Serial);

  if (!mfrc522.PICC_IsNewCardPresent())
  {
    Serial.println(" No Card Presented.");
  }
}

void loop()
{
  // (TRUE, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
  if (mfrc522.PICC_IsNewCardPresent())
  {
    // if RFID tag/card was read return TRUE
    if (mfrc522.PICC_ReadCardSerial())
    {
      // Serial.print("RFID TAG ID -> ");

      playTone(RFID_TONE, DURATION);

      for (byte i = 0; i < mfrc522.uid.size; ++i)
      {
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
        // Serial.print(" ");

        verification();
      }
    }
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
//   mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

void playTone(int rfidTone, int duration)
{
  tone(PIEZO_PIN , rfidTone, duration);
}

boolean getCardIdentification()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    //Since a PICC placed get Serial and continue
    return false;
  }

  tagID = "";

  for (uint8_t i = 0; i < 4; i++)
  {
    // The MIFARE PICCs that we use have 4 byte UID
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading

  return true;
}

void verification()
{
  while (getCardIdentification())
  {
    if (tagID == MASTER_TAG)
    {
      log("Access Granted");
      log("Card ID: " + tagID + "\n");
    }
    else if (tagID == MASTER_TAG_PROHIBITED)
    {
      log("Access Denied [PLEASE DO NOT USE A CITIZENSHIP CARD]");
      log("Identity: CHENG TZE KEONG");
      log("Card ID: " + tagID + "\n");
    }
    else
    {
      log("Access Denied");
      log("Card ID: " + tagID + "\n");
    }
  }
}

void log(String input)
{
  Serial.println(input);
}
