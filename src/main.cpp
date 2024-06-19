#include <Arduino.h>

#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define BOT_TOKEN "123:456"
#define CHAT_ID "123"

#define TAKE_PHOTO "take a photo"

#include "esp_camera.h"
// #include "FS.h"                // SD Card ESP32
// #include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"

// Pin definition for CAMERA_MODEL_AI_THINKER
// Change pin definition if you're using another ESP32 with camera module
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// Keep track of number of pictures
// unsigned int pictureNumber = 0;

//Stores the camera configuration parameters
camera_config_t config;

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

#include <FastLED.h>
#include <EncButton.h>

#define LED_PIN 13
#define BTN_PIN 12
#define NUM_LEDS 8
#define BRIGHTNESS 64
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

  bot.sendFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", CHAT_ID);

  Serial.printf("Image sent!");

  //return the frame buffer back to the driver for reuse
  esp_camera_fb_return(frame);

  SetupBlackAndCyanStripedPalette();
  doEffect();
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  Serial.println(msg.text);
  if (msg.text == TAKE_PHOTO) takeSavePhoto();
  //bot.sendMessage("taking!", msg.chatID);
}

void configInitCamera() {
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 24000000;
  config.pixel_format = PIXFORMAT_JPEG;  //YUV422,GRAYSCALE,RGB565,JPEG
  config.grab_mode = CAMERA_GRAB_LATEST;

  // Select lower framesize if the camera doesn't support PSRAM
  if (psramFound()) {
    Serial.println("psram Found!!");
    config.frame_size = FRAMESIZE_UXGA;  // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;            //10-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Initialize the Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t* s = esp_camera_sensor_get();
  // s->set_framesize(s, FRAMESIZE_UXGA);      // FRAMESIZE_[QQVGA|HQVGA|QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA|QXGA(ov3660)]);
  // s->set_quality(s, 10);                    // 10 to 63
  s->set_brightness(s, 0);                  // -2 to 2
  s->set_contrast(s, 0);                    // -2 to 2
  s->set_saturation(s, 0);                  // -2 to 2
  s->set_special_effect(s, 0);              // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);                    // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);                    // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);                     // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);               // 0 = disable , 1 = enable
  s->set_aec2(s, 0);                        // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);                    // -2 to 2
  s->set_aec_value(s, 300);                 // 0 to 1200
  s->set_gain_ctrl(s, 1);                   // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);                    // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);                         // 0 = disable , 1 = enable
  s->set_wpc(s, 1);                         // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);                     // 0 = disable , 1 = enable
  s->set_lenc(s, 1);                        // 0 = disable , 1 = enable
  s->set_hmirror(s, 1);                     // 0 = disable , 1 = enable
  s->set_vflip(s, 1);                       // 0 = disable , 1 = enable
  s->set_dcw(s, 1);                         // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);                    // 0 = disable , 1 = enable
}

// --- ///

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //disable brownout detector

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  SetupBlackAndWhiteStripedPalette();
  currentBlending = LINEARBLEND;
  
  Serial.begin(115200);
  connectWiFi();

  //Initialize the camera
  Serial.print("Initializing the camera module... ");
  configInitCamera();
  Serial.println("Ok!");

  bot.setChatID(CHAT_ID);
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
