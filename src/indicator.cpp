#include "config.hpp"
#include "indicator.hpp"

CRGB leds[LED_COUNT];

DEFINE_GRADIENT_PALETTE(black_to_white_p){
    0,   0,   0,   0,    // black
    127, 255, 255, 255,  // white
    255, 0,   0,   0     // black
};

DEFINE_GRADIENT_PALETTE(black_to_blue_p){
    0,   0, 0, 0,    // black
    255, 0, 0, 255,  // blue
};

const CRGBPalette16 white_palette = black_to_white_p;
const CRGBPalette16 blue_palette = black_to_blue_p;

void init_indicator() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_BRIGHTNESS);
}

void play_effect(const CRGBPalette16& palette, bool reverse) {
    for (int t = -256; t < 256 + (256 / LEDS_PER_PALETTE); t++) {
        for (int i = 0; i < LED_COUNT; i++) {
            leds[i] =
                ColorFromPalette(palette, constrain(i * (256 / LEDS_PER_PALETTE) + t, 0, 255));
        }
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
}
