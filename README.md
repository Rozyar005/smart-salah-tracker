# 📿 Smart Prayer (Salah) Tracker 

![Project Status](https://img.shields.io/badge/status-active-success?style=for-the-badge)
![Platform](https://img.shields.io/badge/platform-Arduino%20Nano-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

## 📖 Project Breakdown
**A smart companion for your daily prayers.** 🌙

Imagine a device that quietly counts your prayer movements so you never lose track of which Rakat you are on. This project uses an ultrasonic "eye" to detect when you go down for Sujud (prostration). When your head gets close to the sensor, it updates a tiny OLED screen. You just press a button to choose a 2-Rakat or 4-Rakat prayer, and it handles the rest automatically!

## 🛠️ Technical Specs

This project relies on standard Arduino libraries to handle the hardware communication and display graphics.

| Dependency | Purpose |
| :--- | :--- |
| **`Wire.h`** | Built-in Arduino library used for I2C communication (how the Arduino talks to the OLED screen). |
| **`Adafruit_GFX.h`** | The core graphics engine. It provides the math and functions to draw text, pixels, and custom bitmaps (like the standing/sujud icons). |
| **`Adafruit_SSD1306.h`** | The specific hardware driver for the 128x64 OLED display, interpreting the GFX commands for this exact screen model. |

## 🔌 Hardware Wiring

| Component | Arduino Nano Pin | Notes |
| :--- | :--- | :--- |
| **Ultrasonic Trig** | `D2` | Sends the sound wave. |
| **Ultrasonic Echo** | `D3` | Receives the sound wave. |
| **Button (2 Rakat)** | `D4` | Connect other leg to `GND` (uses `INPUT_PULLUP`). |
| **Button (4 Rakat)** | `D5` | Connect other leg to `GND` (uses `INPUT_PULLUP`). |
| **Vibration Motor** | `D6` | Haptic feedback for Sujud detection. |
| **OLED SDA** | `A4` | I2C Data pin. |
| **OLED SCL** | `A5` | I2C Clock pin. |

## 🚀 Step-by-Step Setup

Follow these steps to flash the code to your Arduino Nano.

### 1. Install Required Libraries
Open the Arduino IDE, go to **Sketch > Include Library > Manage Libraries**, and install:
* `Adafruit GFX Library` by Adafruit
* `Adafruit SSD1306` by Adafruit

### 2. Upload the Code
1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/YourUsername/SmartPrayerTracker.git](https://github.com/YourUsername/SmartPrayerTracker.git)
