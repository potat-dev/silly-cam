#include <Arduino.h>

#include "camera.hpp"
#include "config.hpp"
#include "indicator.hpp"

#define TAKE_PHOTO "take a photo"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

#include <EncButton.h>

Button butt(BTN_PIN);

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

void takeSavePhoto() {
    play_effect(white_palette);

    camera_fb_t* frame = esp_camera_fb_get();

    if (!frame) {
        Serial.println("Camera capture failed");
        return;
    }

    play_effect(white_palette, true);

    bot.sendFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", CLIENT_ID);
    Serial.println("Image sent!");

    // return the frame buffer back to the driver for reuse
    esp_camera_fb_return(frame);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
    Serial.println(msg.text);
    if (msg.text == TAKE_PHOTO) takeSavePhoto();
    // bot.sendMessage("taking!", msg.chatID);
}

void setup() {
    init_indicator();
    play_effect(white_palette);

    Serial.begin(115200);
    connectWiFi();

    Serial.print("Initializing the camera module... ");
    init_camera();
    Serial.println("Ok!");

    bot.setChatID(CLIENT_ID);
    bot.attach(newMsg);
    bot.showMenuText("camera restarted", TAKE_PHOTO);

    play_effect(blue_palette);
}

void loop() {
    butt.tick();
    if (butt.click()) takeSavePhoto();
    FastLED.show();
    bot.tick();
}
