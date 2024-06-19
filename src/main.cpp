#include <Arduino.h>

#include "camera.hpp"
#include "config.hpp"

#define TAKE_PHOTO "take a photo"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

#define FASTLED_INTERNAL
#include <EncButton.h>
#include <FastLED.h>

#define BRIGHTNESS 200
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
Button butt(BTN_PIN);

#define UPDATES_PER_SECOND 200

CRGBPalette16 currentPalette;
TBlendType currentBlending;

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 255;
    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 4;
    }
}

void SetupBlackAndWhiteStripedPalette() {
    fill_solid(currentPalette, 16, CRGB::Black);
    currentPalette[4] = CRGB::White;
}

void SetupBlackAndCyanStripedPalette() {
    fill_solid(currentPalette, 16, CRGB::Black);
    currentPalette[4] = CRGB::Green;
}

void connectWiFi() {
    delay(200);
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (millis() > 15000) ESP.restart();
    }

    Serial.println("\nConnected");
}

void doEffect() {
    for (uint8_t startIndex = 0; startIndex < 96; startIndex++) {
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
        // Serial.println(startIndex);
    }
}

void takeSavePhoto() {
    SetupBlackAndWhiteStripedPalette();
    doEffect();

    // Take Picture with Camera
    camera_fb_t* frame = esp_camera_fb_get();

    if (!frame) {
        Serial.println("Camera capture failed");
        return;
    }

    bot.sendFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", CLIENT_ID);
    Serial.println("Image sent!");

    // return the frame buffer back to the driver for reuse
    esp_camera_fb_return(frame);

    SetupBlackAndCyanStripedPalette();
    doEffect();
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
    Serial.println(msg.text);
    if (msg.text == TAKE_PHOTO) takeSavePhoto();
    // bot.sendMessage("taking!", msg.chatID);
}

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    SetupBlackAndWhiteStripedPalette();
    currentBlending = LINEARBLEND;

    Serial.begin(115200);
    connectWiFi();

    // Initialize the camera
    Serial.print("Initializing the camera module... ");
    init_camera();
    Serial.println("Ok!");

    bot.setChatID(CLIENT_ID);
    bot.attach(newMsg);
    bot.showMenuText("camera restarted", TAKE_PHOTO);

    doEffect();
}

void loop() {
    butt.tick();
    if (butt.click()) takeSavePhoto();
    FastLED.show();
    bot.tick();
}
