/**
 * @author: 钟智强
 * 本程序用于使用 RFID RC522 读取和验证 RFID 卡片
 */
#include "MFRC522.h"
#include "SPI.h"

/**
 * 配置
 * +-----------------+-------------+
 * | RFID RC522 引脚 | ARDUINO UNO |
 * +-----------------+-------------+
 * | SDA             | 10          |
 * | SCK             | 13          |
 * | MOSI            | 11          |
 * | MISO            | 12          |
 * | IRQ             | 未使用      |
 * | GND             | GND         |
 * | RST             | 9           |
 * | 3.3V            | 3.3V        |
 * +-----------------+-------------+
 */
#define PIN_RST 9                         // 定义复位引脚
#define PIN_SDA 10                        // 定义 SDA 引脚
#define BAUD_RATE 9600                    // 定义串口波特率
#define RFID_TONE 1000                    // 定义 RFID 声音频率
#define BUZZER 3                          // 定义蜂鸣器引脚
#define DURATION 700                      // 定义声音持续时间
#define PIEZO_PIN 8                       // 定义压电蜂鸣器引脚
#define MASTER_TAG_PROHIBITED "6FB077CC"  // 禁用的主卡标签 ID
#define MASTER_TAG "D743AF7B"             // 主卡标签 ID
#define MASTER_TAG_ALT "5527112A"         // 备用主卡标签 ID

// 定义音调频率数组
int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};

String tagID = "";  // 定义标签 ID 字符串

MFRC522 mfrc522(PIN_SDA, PIN_RST);  // 初始化 MFRC522 模块

void setup() {
    // 初始化 SPI 连接
    SPI.begin();

    // 初始化近场耦合设备
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();

    // 初始化串行连接
    Serial.begin(BAUD_RATE);
    while (!Serial);  // 等待串行连接准备好

    if (!mfrc522.PICC_IsNewCardPresent()) {
        log("请放置卡片\n");  // 提示放置卡片
    }

    while (getCurrentState())  // 持续检测卡片状态
    {
        if (true) {
            log("没有检测到卡片。");  // 提示没有检测到卡片
        }
    }
}

void loop() {
    // 如果检测到新的 RFID 标签/卡片
    if (mfrc522.PICC_IsNewCardPresent()) {
        // 如果读取到 RFID 标签/卡片
        if (mfrc522.PICC_ReadCardSerial()) {
            Serial.print("RFID 标签 ID -> ");

            playTone(RFID_TONE, DURATION);  // 播放音调

            for (byte i = 0; i < mfrc522.uid.size; ++i) {
                Serial.print(mfrc522.uid.uidByte[i], HEX);
                Serial.print(" ");

                verification();  // 验证标签
            }
        }
    }

    // 输出卡片的调试信息；PICC_HaltA() 会自动调用
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

void playTone(int rfidTone, int duration) {
    // 播放指定频率和持续时间的音调
    tone(PIEZO_PIN, rfidTone, duration);
}

bool getCardIdentification() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return false;  // 如果没有新的卡片，返回 false
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        // 如果没有读取到卡片，返回 false
        return false;
    }

    tagID = "";  // 清空标签 ID

    for (uint8_t i = 0; i < 4; i++) {
        // 将 4 字节的 UID 添加到标签 ID 字符串中
        tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    tagID.toUpperCase();   // 转换为大写
    mfrc522.PICC_HaltA();  // 停止读取

    return true;  // 返回 true 表示成功读取到卡片
}

void verification() {
    while (getCardIdentification()) {
        if (tagID == MASTER_TAG) {
            log("访问允许");  // 访问允许
            log("卡片 ID: " + tagID + "\n");
        } else if (tagID == MASTER_TAG_PROHIBITED) {
            log("访问拒绝 [请勿使用身份证]");  // 访问拒绝
            log("卡片 ID: " + tagID + "\n");
        } else {
            log("访问拒绝");  // 访问拒绝
            log("卡片 ID: " + tagID + "\n");
        }
    }
}

void log(String input) {
    // 输出日志信息
    Serial.println(input);
}

bool getCurrentState() {
    if (mfrc522.PICC_IsNewCardPresent()) {
        return true;  // 如果有新的卡片，返回 true
    }

    return false;  // 否则返回 false
}
