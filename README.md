# *silly* camera 

— ESP32-CAM *No Preview* Camera Project

This project demonstrates how to use an ESP32-CAM module to create a Wi-Fi-enabled camera that can be controlled remotely via a Telegram bot or a physical button. The project allows you to take photos with the ESP32-CAM and send them directly to a Telegram chat. The LED strip attached to the ESP32-CAM also plays visual effects, enhancing user interaction.

## Introduction

The main goal of this project is to create a super simple camera that intentionally lacks a preview screen. By omitting the preview, users are encouraged to focus on the moment rather than perfecting a selfie or framing the shot. This minimalist approach is ideal for capturing spontaneous and genuine moments.

The *silly* camera project allows you to remotely capture images using the ESP32-CAM module and interact with it through a Telegram bot or a physical button. The project utilizes the [FastLED](https://github.com/FastLED/FastLED) library for LED effects and integrates it with the ESP32-CAM’s camera module to create a visually appealing and functional IoT device.

## Features

- **Remote Photo Capture**: Capture photos with the ESP32-CAM via Telegram commands.
- **Manual Photo Capture**: Take photos by pressing a physical button connected to the ESP32-CAM, providing an alternative way to interact with the camera.
- **LED Visual Effects**: The project includes LED visual effects using the [FastLED](https://github.com/FastLED/FastLED) library, providing feedback during photo capture.
- **Wi-Fi Connectivity**: The ESP32-CAM connects to your local Wi-Fi network, allowing for remote operations.
- **Telegram Bot Integration**: Interact with the ESP32-CAM through a Telegram bot interface.

## Hardware Requirements

- **ESP32-CAM Module**: A microcontroller with a built-in camera.
- **LED Strip**: [WS2811](https://www.aliexpress.com/w/wholesale-ws2811-led-strip.html) or compatible, with at least 8 LEDs.
- **Button**: For manual photo capture.
- **Power Supply**: 5V power supply for the ESP32-CAM and LED strip.

## Software Requirements

- **[PlatformIO](https://platformio.org/)**: A development environment for IoT projects.
- **[Arduino Framework](https://www.arduino.cc/en/Guide/ArduinoIDE)**: The ESP32-CAM is programmed using the Arduino framework.
- **Telegram Bot API**: Used to communicate with the ESP32-CAM through Telegram.

## Installation

### Step 1: Clone the Repository

```sh
git clone https://github.com/potat-dev/silly-cam
```

### Step 2: Set Up PlatformIO

Ensure you have [PlatformIO](https://platformio.org/install/ide?install=vscode) installed in your IDE (e.g., VSCode). Open the project folder in your IDE.

### Step 3: Configure Wi-Fi and Telegram Bot

Edit the `config.hpp` file to set your Wi-Fi credentials and Telegram bot token.

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASS"

#define BOT_TOKEN "YOUR_BOT_TOKEN"
#define CLIENT_ID "YOUR_CLIENT_ID"
```

### Step 4: Build and Upload

Connect your ESP32-CAM module to your computer, and upload the code using PlatformIO:

```sh
platformio run --target upload
```

## Usage

### Connecting to Wi-Fi

When powered on, the ESP32-CAM will attempt to connect to the configured Wi-Fi network. If the connection is successful, it will notify you via the serial monitor.

### Using the Telegram Bot

After setting up the bot, you can send the command `take a photo` to capture an image with the ESP32-CAM. The image will be sent back to the Telegram chat as a JPEG file.

### Manual Photo Capture

You can also manually capture a photo by pressing the button connected to the ESP32-CAM. This provides an easy and straightforward way to take pictures without needing to use the Telegram interface.

## Configuration

- **LED Effects**: Customize the LED effects in `indicator.cpp` to suit your needs.
- **Camera Settings**: Adjust the camera configuration in `camera.cpp` if you need different image quality or camera behavior.

### Example Configuration

The following example shows how to configure the Wi-Fi and Telegram bot credentials:

```cpp
#define WIFI_SSID "HomeNetwork"
#define WIFI_PASS "SecurePassword123"

#define BOT_TOKEN "123456789:ABCDEFGHIJKLMNOP"
#define CLIENT_ID "987654321"
```

## Code Structure

- **`platformio.ini`**: Configuration file for PlatformIO.
- **`camera.hpp` / `camera.cpp`**: Handles camera initialization and configuration.
- **`config.hpp`**: Stores configuration parameters such as Wi-Fi credentials and bot tokens.
- **`indicator.hpp` / `indicator.cpp`**: Manages LED effects using the [FastLED](https://github.com/FastLED/FastLED) library.
- **`main.cpp`**: The main entry point of the project, handling setup and loop functions.

## Acknowledgements

- **[GyverLibs](https://github.com/GyverLibs)**: Libraries for [FastBot](https://github.com/GyverLibs/FastBot) and [EncButton](https://github.com/GyverLibs/EncButton) used for Telegram bot integration and button handling.
- **[FastLED](https://github.com/FastLED/FastLED)**: Library used for controlling the LED strip.
- **[PlatformIO](https://platformio.org/)**: A powerful development environment that simplifies the development and deployment of embedded systems.
