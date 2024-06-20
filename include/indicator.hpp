#pragma once

#include <FastLED.h>

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LEDS_PER_PALETTE 32
#define UPDATES_PER_SECOND 200

// variables

extern const CRGBPalette16 white_palette;
extern const CRGBPalette16 blue_palette;

// functions

void init_indicator();

void play_effect(const CRGBPalette16& palette, bool reverse = false);
