#include "FastLED.h"
#include "strip.h"
#include "utils.h"

CRGB leds[STRIP_NUM_LEDS];

void init_strip() {
	FastLED.addLeds<NEOPIXEL, STRIP_DATA_PIN>(leds, STRIP_NUM_LEDS); 
}

void apply_strip() {
    FastLED.show();
}

int strip_get_global_red_level() {
    return leds[0].r;
}

int strip_get_global_green_level() {
    return leds[0].g;
}

int strip_get_global_blue_level() {
    return leds[0].b;
}

void strip_set_led(int led_idx, int red_level, int green_level, int blue_level) {
    leds[led_idx].r = red_level; 
    leds[led_idx].g = green_level; 
    leds[led_idx].b = blue_level;
}

int strip_led_count() {
    return STRIP_NUM_LEDS;
}
