#include "FastLED.h"
#include "strip.h"

#define NUM_LEDS 64
#define DATA_PIN 5

CRGB leds[NUM_LEDS];

void setup() {

        //initialise LEDs	
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 

}

void loop() {

	light_up(3000);
	delay(2000);
	light_down();
	delay(2000);

}

void set_global_rgb(int red_level, int green_level, int blue_level) {

	if (red_level > 255) {
		red_level = 255;
	}

	if (green_level > 255) {
		green_level = 255;
	}

	if (blue_level > 255) {
		blue_level = 255;
	}

	for (int led_idx=0; led_idx<NUM_LEDS; led_idx++) {
		leds[led_idx].r = red_level; 
		leds[led_idx].g = green_level; 
		leds[led_idx].b = blue_level; 
	}

	FastLED.show();

}


void light_up(int transition_time_mills) {

	int start_red_level = 0;
	int start_green_level = 0;
	int start_blue_level = 0;

	int target_red_level = 255;
	int target_green_level = 255;
	int target_blue_level = 255;

	int red_level_step_time_mills = 
		transition_time_mills / (target_red_level - start_red_level);
	int green_level_step_time_mills = 
		transition_time_mills / (target_green_level - start_green_level);
	int blue_level_step_time_mills = 
		transition_time_mills / (target_blue_level - start_blue_level);

	// Find the highest common factor of the three step times
	int hfc_step_time = 1;

	// Start with a random step time from above and decrease the number 
	// until the result of all step times modulo the iteration step time
	// equals 0. This iteration step time will then be highest common factor.
	for (int hfc_iter=red_level_step_time_mills;hfc_iter>1;hfc_iter--) {
	
		if ((red_level_step_time_mills % hfc_iter == 0) &&
		    (green_level_step_time_mills % hfc_iter == 0) &&
	            (blue_level_step_time_mills % hfc_iter == 0)) {
		    
			hfc_step_time = hfc_iter;
			break;

		    }

	}

	//Now that we have a common step time, we need to find out
	//after how many steps we need to increase the level of each colour channel
	int red_steps_till_increase = red_level_step_time_mills / hfc_step_time;
	int green_steps_till_increase = green_level_step_time_mills / hfc_step_time;
	int blue_steps_till_increase = blue_level_step_time_mills / hfc_step_time;

	int total_step_count = transition_time_mills / hfc_step_time;

	int red_level = start_red_level;
	int green_level = start_green_level;
	int blue_level = start_blue_level;
	for (int step_iter=0; step_iter>total_step_count; step_iter++) {
	
		if (step_iter % red_steps_till_increase == 0) {
			red_level++;
		}

		if (step_iter % green_steps_till_increase == 0) {
			green_level++; 
		}
		
		if (step_iter % blue_steps_till_increase == 0) {
			blue_level++; 
		}

		
		set_global_rgb(red_level, green_level, blue_level);
	
	}


}

void light_down() {

	for (int led_idx=0; led_idx<NUM_LEDS; led_idx++) {
	
		leds[led_idx] = CRGB::Black; 

	}

	FastLED.show();

}
