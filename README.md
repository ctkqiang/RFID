# RFID RC522 卡片读取器 - Arduino 项目

该项目演示了如何使用 RFID RC522 模块读取和验证 RFID 卡片。系统旨在允许访问特定的 RFID 标签（卡片），同时根据预定义的标签 ID 拒绝其他卡片的访问。

## 作者
- **姓名**: 钟智强

## 功能特性
- **RFID 标签识别**: 使用 MFRC522 模块读取并识别 RFID 标签。
- **标签验证**: 基于预定义的标签 ID 允许或拒绝访问。
- **蜂鸣器反馈**: 通过蜂鸣器提供成功或失败的访问反馈。
- **禁止标签**: 阻止某些 RFID 标签的访问。
- **串口输出**: 通过串口监视器显示 RFID 标签 ID 及其他相关信息。

## 硬件组件
- **RFID RC522 模块**
- **Arduino UNO**
- **蜂鸣器 / 压电扬声器**
- **跳线**
- **面包板**

## 硬件配置

以下是 Arduino UNO 与 RFID RC522 模块之间的连接配置：

| RFID RC522 引脚 | Arduino UNO 引脚 |
|-----------------|-----------------|
| SDA             | 10              |
| SCK             | 13              |
| MOSI            | 11              |
| MISO            | 12              |
| IRQ             | 未使用          |
| GND             | GND             |
| RST             | 9               |
| 3.3V            | 3.3V            |

## 引脚定义

- **PIN_RST**: RFID 模块的复位引脚。
- **PIN_SDA**: SPI 数据引脚。
- **BUZZER**: 连接蜂鸣器的引脚，用于反馈。
- **PIEZO_PIN**: 连接压电扬声器的引脚，用于播放音调。
- **BAUD_RATE**: 串口通信的波特率（9600）。
- **RFID_TONE**: RFID 卡片检测时播放的音调。
- **DURATION**: 播放音调的持续时间。
- **MASTER_TAG**: 授权 RFID 卡片的 ID（主卡）。
- **MASTER_TAG_ALT**: 备用授权卡片 ID。
- **MASTER_TAG_PROHIBITED**: 禁用的 RFID 卡片 ID（如身份证等）。

## 代码概述

1. **库文件引用**:
   - 使用 `MFRC522` 库与 RFID 模块进行交互。
   - 使用 `SPI` 库进行 Arduino 与 RFID 模块之间的 SPI 通信。

2. **初始化设置**:
   - 使用 `SPI.begin()` 初始化 SPI 通信。
   - 使用 `mfrc522.PCD_Init()` 初始化 RFID 模块。
   - 使用 `Serial.begin(BAUD_RATE)` 初始化串口通信，以便输出调试信息。

3. **卡片检测**:
   - 程序不断检测是否有新卡片通过 `mfrc522.PICC_IsNewCardPresent()`。
   - 检测到新卡片后，读取卡片的序列号并输出。

4. **标签验证**:
   - 每个检测到的 RFID 卡片会与预定义的标签 ID 进行比较。
   - 如果卡片与 **MASTER_TAG** 匹配，则允许访问，输出成功信息。
   - 如果卡片与 **MASTER_TAG_PROHIBITED** 匹配，则拒绝访问，输出警告信息。
   - 其他卡片则直接拒绝访问。

5. **蜂鸣器反馈**:
   - 通过 `tone(PIEZO_PIN, RFID_TONE, DURATION)` 播放音调，给出音频反馈。

6. **日志输出**:
   - 使用 `log()` 函数将相关信息输出到串口监视器。

## 使用方法

1. 按照“硬件配置”中的接线图，将 RFID RC522 模块连接到 Arduino UNO。
2. 将提供的代码上传到 Arduino UNO。
3. 打开串口监视器查看 RFID 标签 ID。
4. 将 RFID 标签靠近读取器，系统会根据预定义的标签 ID 验证卡片并提供访问权限。
5. 通过蜂鸣器听取反馈：
   - **滴**：成功（允许访问）。
   - **哔**：拒绝访问。

## 函数说明

- **playTone(int rfidTone, int duration)**: 播放指定频率和持续时间的音调。
- **getCardIdentification()**: 读取 RFID 卡片的唯一 ID，并将其存储在 `tagID` 变量中。
- **verification()**: 比较读取的 RFID 标签 ID 与允许和禁止的标签 ID。
- **log(String input)**: 将日志信息输出到串口监视器，用于调试或信息显示。
- **getCurrentState()**: 如果有新的卡片读取，则返回 `true`，否则返回 `false`。

## 示例输出

```plaintext
请放置卡片
RFID 标签 ID -> D7 43 AF 7B
访问允许
卡片 ID: D743AF7B
```

### 其他注意事项
- 确保正确为 RFID 模块供电（3.3V），并检查接线是否正确。
- **MASTER_TAG** 和 **MASTER_TAG_PROHIBITED** 在代码中已定义，可以根据需要修改为实际使用的卡片 ID。


### 个人捐赠支持
如果您认为该项目对您有所帮助，并且愿意个人捐赠以支持其持续发展和维护，🥰我非常感激您的慷慨。
您的捐赠将帮助我继续改进和添加新功能到该项目中。 通过财务捐赠，您将有助于确保该项目保持免
费和对所有人开放。即使是一小笔捐款也能产生巨大的影响，也是对我个人的鼓励。

以下是我的支付宝二维码，您可以扫描二维码进行个人捐赠：

<br />

| 微信支付 | 支付宝支付 |
| --- | --- |
| <img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9863.jpg?raw=true" height="500" /> | <img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9859.JPG?raw=true" height="500" /> |

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/F1F5VCZJU)



## 爱心捐款
<a href="https://qr.alipay.com/fkx19369scgxdrkv8mxso92"><img src="https://img.shields.io/badge/alipay-00A1E9?style=for-the-badge&logo=alipay&logoColor=white"></a> <a href="https://ko-fi.com/F1F5VCZJU"><img src="https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white"></a> <a href="https://www.paypal.com/paypalme/ctkqiang"><img src="https://img.shields.io/badge/PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white"></a> <a href="https://donate.stripe.com/00gg2nefu6TK1LqeUY"><img src="https://img.shields.io/badge/Stripe-626CD9?style=for-the-badge&logo=Stripe&logoColor=white"></a>

## 关注我
<a href="https://twitch.tv/ctkqiang"><img src="https://img.shields.io/badge/Twitch-9146FF?style=for-the-badge&logo=twitch&logoColor=white"></a> <a href="https://open.spotify.com/user/22sblyn4dsymya3xinw3umhai"><img src="https://img.shields.io/badge/Spotify-1ED760?&style=for-the-badge&logo=spotify&logoColor=white"></a> <a href="https://www.tiktok.com/@ctkqiang"><img src="https://img.shields.io/badge/TikTok-000000?style=for-the-badge&logo=tiktok&logoColor=white"></a> <a href="https://stackoverflow.com/users/10758321/%e9%92%9f%e6%99%ba%e5%bc%ba"><img src="https://img.shields.io/badge/Stack_Overflow-FE7A16?style=for-the-badge&logo=stack-overflow&logoColor=white"></a> <a href="https://www.facebook.com/JohnMelodyme/"><img src="https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white"></a> <a href="https://github.com/ctkqiang"><img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white"></a> <a href="https://www.instagram.com/ctkqiang"><img src="https://img.shields.io/badge/Instagram-E4405F?style=for-the-badge&logo=instagram&logoColor=white"></a> <a href="https://www.linkedin.com/in/ctkqiang/"><img src="https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white"></a> <a href="https://linktr.ee/ctkqiang.official"><img src="https://img.shields.io/badge/linktree-39E09B?style=for-the-badge&logo=linktree&logoColor=white"></a> <a href="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9245.JPG?raw=true"><img src="https://img.shields.io/badge/WeChat-07C160?style=for-the-badge&logo=wechat&logoColor=white"></a>

