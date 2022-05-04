/**
 * @author: John Melody Me
 * @contact: <Johnmelodyme@yahoo.com>
 * 
 *  Configuation
 * +-----------------+-------------+
 * | RFID RC522 PIN  | ARDUINO UNO |
 * +-----------------+-------------+
 * | SDA             | 10          |
 * | SCK             | 13          |
 * | MOSI            | 11          |
 * | MISO            | 12          |
 * | IRQ             | UNUSED      |
 * | GND             | GND         |
 * | RST             | 9           |
 * | 3.3V            | 3.3V        |
 * +-----------------+-------------+
 */
#include "SPI.h"
#include "MFRC522.h"

#define ENVIRONMENT_MODE 0x0 // Set 0x0 for READ and 0x1 for WRITE 
#define PIN_RST 0x9
#define PIN_SDA 0xA
#define BAUD_RATE 0x2580
#define RFID_TONE 0xB54
#define BUZZER 0x3
#define DURATION 0x2BC
#define PIEZO_PIN 0x8
#define MASTER_TAG_PROHIBITED "6FB077CC"
#define MASTER_TAG "D743AF7B"

/**
 * Set the block to which we want to write data.
 * Be aware of Sector Trailer Blocks
 */
int blockNumber = 2;

String content = "";

/** 
 * Create an array of 16 Bytes and fill it with data
 * This is the actual data which is going to be written 
 * into the card 
 */
byte blockData [16] = { "John Melody" };

/** 
 * Create another array to read data from Block 
 * Legthn of buffer should be 2 Bytes more than the size 
 * of Block (16 Bytes) 
 */
byte bufferLen = 18;
byte readBlockData[18];

/** Create MF522 Instances*/
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
MFRC522 mfrc522(PIN_SDA, PIN_RST);

void setup() 
{
  /** Initiate Serial Instances */
  Serial.begin(BAUD_RATE);

  /** Initiate SPI Bus Instances */
  SPI.begin();

  /** Initiate MFRC522 Instances */
  mfrc522.PCD_Init();

  Serial.print("\nJohn Melody Me Copright 2022\n");
  Serial.print("\n\nPlease Present Card:\n");
}

void loop()
{
  if (mfrc522.PICC_IsNewCardPresent()) 
  {
    if (ENVIRONMENT_MODE == 0x0) 
    {
      if (mfrc522.PICC_ReadCardSerial()) 
      {
        playTone(RFID_TONE, DURATION);

        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          content.toUpperCase();
        }

        readFrom(blockNumber, readBlockData);
      }
    } 
    else 
    {
      playTone(RFID_TONE, DURATION);

      /** 
       * Prepare the ksy for authentication.
       * All keys are set to FFFFFFFFFFFFH at chip delivery 
       * from the factory by default
       */
      for (byte i = 0; i < 0x6; i++) 
      {
        key.keyByte[i] = 0xFF;
      }

      writeIntoCard(blockNumber, blockData);
    }
  }
}

void writeIntoCard(int blockNumber, byte blockData[])
{
  /** Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("\nAuthentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("\nAuthentication success");
  }
  
  /** Write data to the block */
  status = mfrc522.MIFARE_Write(blockNumber, blockData, 16);
  
  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("\nWriting to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("\nData was written into Block successfully");
  }
}

/** Read Data and metadata from RFID / NFC tag */
void readFrom(int blockNumber, byte readBlockData[]) 
{
  /** Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                         blockNumber, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("\nAuthnetication failed.READ_UNAUTHORISED");
    // Serial.println(mfrc522.GetStatusCodeName(status));

    return;
  }
  else
  {
    Serial.println("Authorised READ_AUTHORISED");
  }

  /** Read Data from [block] */
  status = mfrc522.MIFARE_Read(blockNumber, readBlockData, &bufferLen);

  return;
}

/** Play Tone in buzzer */
void playTone(int rfidTone, int duration)
{
  tone(PIEZO_PIN , rfidTone, duration);
}

/** @returns state of card readablitity */
boolean getReadableSate() 
{
  if (mfrc522.PICC_ReadCardSerial()) 
  {
    return true;
  }
  else
  {
    return false;
  }
}

/** @returns state of preseting of the card */
bool getCurrentState()
{
  if (mfrc522.PICC_IsNewCardPresent())
  {
    return true;
  }
  else
  {
    return false;
  }
}

/** @returns picctype of preseted rfid card */
String getPICCType()
{
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  return String(mfrc522.PICC_GetType(piccType));
}