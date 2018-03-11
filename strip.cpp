#include "FastLED.h"
#include "strip.h"
#include <stdlib.h>

CRGB leds[NUM_LEDS];

/* This is the Arduino startup function
 * which will be executed before the loop starts
 * at startup */
void setup() {

	Serial.begin(9600);
	while (! Serial); // Wait until Serial is ready - Leonardo
	Serial.println("Serial is ready");

	//initialise LEDs	
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
	
	//initialise motion sensor input pin
	pinMode(MOTION_PIN, INPUT);

}

/* This is the Arduino loop function
 * which will be executed continuously
 * (same as while(true){}) */
void loop() {
  
	// If there's input on the motion sensor
	// start the light sequence.
	if (digitalRead(MOTION_PIN) > 0) {

		//turn the light on
		light_up(TRANSITION_UP_TIME);
		//Stay on 
		delay(STAY_ON_TIME);
		//turn light off
		light_down(TRANSITION_DOWN_TIME);

		//To Avoid getting switched on immediately after (accidentally)
		//wait before going back to reading from the sensor
		delay(AFTER_CYCLE_REST);

	}
  
}

void set_global_rgb(int red_level, int green_level, int blue_level) {

	if (red_level > MAX_RGB_LEVEL) {
		red_level = MAX_RGB_LEVEL;
	}

	if (green_level > MAX_RGB_LEVEL) {
		green_level = MAX_RGB_LEVEL;
	}

	if (blue_level > MAX_RGB_LEVEL) {
		blue_level = MAX_RGB_LEVEL;
	}

	//Set the RGB levels for all LEDs in the array (strip)
	for (int led_idx=0; led_idx<NUM_LEDS; led_idx++) {
		leds[led_idx].r = red_level; 
		leds[led_idx].g = green_level; 
		leds[led_idx].b = blue_level; 
	}

	//All RGB levels for all LEDs are defined,
	//now we can actually let the microcontroller
	//apply the new levels to the lights
	FastLED.show();

}

void step_decrease_global(int total_step_count, int red_wait_steps_count, int green_wait_steps_count, int blue_wait_steps_count, int step_delay_mills) {

	//Get the start RGB levels by reading the current first LED levels
	int red_level = leds[0].r;
	int green_level = leds[0].g;
	int blue_level = leds[0].b;

	//We iterate for a given number of times and decrease 
	//colour levels in respect of their wait step count
	//
	// Colours that transition between a higher amount of levels
	// will need to change their value more often than colours
	// that perform smaller level changed.
	// The wait step count will be smaller the larger the quantity
	// of level change as the level will need to change more often
	// over the transition time period in order to reach its target value.
	//
	// Colour channels that only peform small level changes will have a high
	// step wait count meaning their level will be changed fewer times.
	//
	// After each iteration we wait for step delay mills which is calculated
	// step_delay_mills = full_transition_time / total_step_count
	for (int step_iter=0; step_iter<total_step_count; step_iter++) {
	
		if (step_iter % red_wait_steps_count == 0) {
			red_level--;
		}

		if (step_iter % green_wait_steps_count == 0) {
			green_level--; 
		}
		
		if (step_iter % blue_wait_steps_count == 0) {
			blue_level--; 
		}

		
		set_global_rgb(red_level, green_level, blue_level);
		delay(step_delay_mills);

	}
}	

void step_increase_global(int total_step_count, int red_wait_steps_count, int green_wait_steps_count, int blue_wait_steps_count, int step_delay_mills) {

	//Get the start RGB levels by reading the current first LED levels
	int red_level = leds[0].r;
	int green_level = leds[0].g;
	int blue_level = leds[0].b;

	for (int step_iter=0; step_iter<total_step_count; step_iter++) {
	
		if (step_iter % red_wait_steps_count == 0) {
			red_level++;
		}

		if (step_iter % green_wait_steps_count == 0) {
			green_level++; 
		}
		
		if (step_iter % blue_wait_steps_count == 0) {
			blue_level++; 
		}
		
		set_global_rgb(red_level, green_level, blue_level);
		delay(step_delay_mills);
	
	}

}

void transition_global(int start_red_level, int start_green_level, int start_blue_level, int target_red_level, int target_green_level, int target_blue_level, int transition_time_mills) {

	//Set the global RGB levels to their start values
	//This should no actually change anything on the lights
	//as start levels should always refelect the current status of the 
	//LED levels.
	set_global_rgb(start_red_level, start_green_level, start_blue_level);
	
	int red_level_step_time_mills = 
		transition_time_mills / abs(target_red_level - start_red_level);
	int green_level_step_time_mills = 
		transition_time_mills / abs(target_green_level - start_green_level);
	int blue_level_step_time_mills = 
		transition_time_mills / abs(target_blue_level - start_blue_level);

	// Find the highest common factor (HFC) of the three step times.
	// This will allow us to use a common delay time for the light transition
	// loop for all colours. Indicidual colour levels are then changed
	// after a calculated number of steps (transition loop iterations).
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
	//after how many wait before we change the level of each colour channel
	int red_steps_till_change = red_level_step_time_mills / hfc_step_time;
	int green_steps_till_change = green_level_step_time_mills / hfc_step_time;
	int blue_steps_till_change = blue_level_step_time_mills / hfc_step_time;

	//This is the total number of iterations we need to make in the light transition
	//loop until we reach transition_time_mills as which point all colour channel levels
	//are in their end state
	int total_step_count = transition_time_mills / hfc_step_time;

	//We assume that if the red target level is higher than the red start level,
	//the same goes for all colour channels i.e we perform a light level increase transition
	if (target_red_level > start_red_level) {
		step_increase_global(total_step_count, red_steps_till_change, green_steps_till_change, blue_steps_till_change, hfc_step_time);
	}

	//We assume that if the red target level is lower than the red start level,
	//the same goes for all colour channels i.e we perform a light level decrease transition
	if (target_red_level < start_red_level) {
		step_decrease_global(total_step_count, red_steps_till_change, green_steps_till_change, blue_steps_till_change, hfc_step_time);
	}

}

void light_up(int transition_time_mills) {
	transition_global(0, 0, 0, RED_ON_LEVEL, GREEN_ON_LEVEL, BLUE_ON_LEVEL, transition_time_mills);
}

void light_down(int transition_time_mills) {
	transition_global(RED_ON_LEVEL, GREEN_ON_LEVEL, BLUE_ON_LEVEL, 0, 0, 0, transition_time_mills);
}
