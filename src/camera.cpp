#include <Arduino.h>

#include "camera.hpp"

void init_camera() {
    camera_config_t config;

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
    config.pin_href = HREF_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.xclk_freq_hz = 24000000;
    config.pixel_format = PIXFORMAT_JPEG;   // YUV422, GRAYSCALE, RGB565, JPEG
    config.grab_mode = CAMERA_GRAB_LATEST;  // select the latest frame, ignore buffer

    // select lower framesize if the camera doesn't support PSRAM
    if (psramFound()) {
        Serial.println("PSRAM found!");
        config.frame_size = FRAMESIZE_UXGA;  // FRAMESIZE_QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
        config.jpeg_quality = 10;            // 10 to 63 - lower means higher quality
        config.fb_count = 2;
    } else {
        Serial.println("PSRAM not found! Switching to lower quality");
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // initialize camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    // set sensor configs
    sensor_t* s = esp_camera_sensor_get();

    // switches
    s->set_exposure_ctrl(s, 1);
    s->set_gain_ctrl(s, 1);
    s->set_whitebal(s, 1);
    s->set_awb_gain(s, 1);
    s->set_colorbar(s, 0);
    s->set_raw_gma(s, 1);
    s->set_aec2(s, 0);
    s->set_lenc(s, 1);
    s->set_bpc(s, 0);
    s->set_wpc(s, 1);
    s->set_dcw(s, 1);

    // image flip
    s->set_hmirror(s, 1);
    s->set_vflip(s, 1);

    // values: -2 to 2
    s->set_brightness(s, 0);
    s->set_saturation(s, 0);
    s->set_contrast(s, 0);
    s->set_ae_level(s, 0);

    // image correction values
    s->set_gainceiling(s, GAINCEILING_2X);  // 2X to 128X
    s->set_aec_value(s, 300);               // 0 to 1200
    s->set_agc_gain(s, 0);                  // 0 to 30

    // effects: 0 to 6
    // None, Negative, Gray, Red Tint, Green Tint, Blue Tint, Sepia
    s->set_special_effect(s, 0);

    // white balance: 0 to 4
    // Auto, Sunny, Cloudy, Office, Home
    s->set_wb_mode(s, 0);
}
