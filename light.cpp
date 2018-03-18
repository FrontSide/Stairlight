#include "light.h"
#include "utils.h"
#include "debugout.h"

#ifndef TEST
    #include "strip.h"
    #include "Arduino.h"
#endif

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

	if (red_level < MIN_RGB_LEVEL) {
		red_level = MIN_RGB_LEVEL;
	}

	if (green_level < MIN_RGB_LEVEL) {
		green_level = MIN_RGB_LEVEL;
	}

	if (blue_level < MIN_RGB_LEVEL) {
		blue_level = MIN_RGB_LEVEL;
	}

    #ifndef TEST
        //Set the RGB levels for all LEDs in the array (strip)
        for (int led_idx=0; led_idx<strip_led_count(); led_idx++) {
            strip_set_led(led_idx, red_level, green_level, blue_level);
        }
        apply_strip();
    #endif

    debug("set_global_rbg:: set red to %d\n", red_level);
    debug("set_global_rbg:: set green to %d\n", green_level);
    debug("set_global_rbg:: set blue to %d\n", blue_level);

}

void step_decrease_global(int total_step_count, int red_wait_steps_count, int green_wait_steps_count, int blue_wait_steps_count, int step_delay_mills) {

    #ifndef TEST
        //Get the start RGB levels by reading the current first LED levels
        int red_level = strip_get_global_red_level();
        int green_level = strip_get_global_green_level();
        int blue_level = strip_get_global_blue_level();
    #else
        //Get the start RGB levels by reading the current first LED levels
        int red_level = 255;
        int green_level = 180;
        int blue_level = 180;
     #endif

    debug("step_decrease_global :: total_step_count = %d\n", total_step_count);
    debug("step_decrease_global :: red_wait_steps_count = %d\n", red_wait_steps_count);
    debug("step_decrease_global :: green_wait_steps_count = %d\n", green_wait_steps_count);
    debug("step_decrease_global :: blue_wait_steps_count = %d\n", blue_wait_steps_count);
    debug("step_decrease_global :: step_delay_mills = %d\n", step_delay_mills);
   

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
    bool levels_changed;
	for (int step_iter=0; step_iter<total_step_count; step_iter++) {
	
        debug("step_decrease_global:: step_iter is %d\n", step_iter);
        levels_changed = false;

		if (step_iter % red_wait_steps_count == 0) {
			red_level--;
            levels_changed = true;
            debug("step_decrease_global :: decrease red_level :: red_level = %d\n", red_level);
		}

		if (step_iter % green_wait_steps_count == 0) {
			green_level--; 
            levels_changed = true;
            debug("step_decrease_global :: decrease green_level :: green_level = %d\n", green_level);
		}
		
		if (step_iter % blue_wait_steps_count == 0) {
			blue_level--;
            levels_changed = true;
            debug("step_decrease_global :: decrease blue_level :: blue_level = %d\n", blue_level);
		}

		if (levels_changed) {
		    set_global_rgb(red_level, green_level, blue_level);
        }
        
        #ifndef TEST
		    delay(step_delay_mills);
        #endif

	}
}	

void step_increase_global(int total_step_count, int red_wait_steps_count, int green_wait_steps_count, int blue_wait_steps_count, int step_delay_mills) {

    #ifndef TEST
        //Get the start RGB levels by reading the current first LED levels
        int red_level = strip_get_global_red_level();
        int green_level = strip_get_global_green_level();
        int blue_level = strip_get_global_blue_level();
    #else
        int red_level = 0;
        int green_level = 0;
        int blue_level = 0;
    #endif

    debug("step_increase_global :: total_step_count = %d\n", total_step_count);
    debug("step_increase_global :: red_wait_steps_count = %d\n", red_wait_steps_count);
    debug("step_increase_global :: green_wait_steps_count = %d\n", green_wait_steps_count);
    debug("step_increase_global :: blue_wait_steps_count = %d\n", blue_wait_steps_count);
    debug("step_increase_global :: step_delay_mills = %d\n", step_delay_mills);

    bool levels_changed;
	for (int step_iter=0; step_iter<total_step_count; step_iter++) {
	
        debug("step_increase_global:: step_iter is %d\n", step_iter);
        levels_changed = false;

		if (step_iter % red_wait_steps_count == 0) {
			red_level++;
            levels_changed = true;
            debug("step_increase_global :: increase red_level :: red_level = %d\n", red_level);
		}

		if (step_iter % green_wait_steps_count == 0) {
			green_level++; 
            levels_changed = true;
            debug("step_increase_global :: increase green_level :: green_level = %d\n", green_level);
		}
		
		if (step_iter % blue_wait_steps_count == 0) {
			blue_level++;
            levels_changed = true;
            debug("step_increase_global :: increase blue_level :: blue_level = %d\n", blue_level);
		}
		
        if (levels_changed) {
		    set_global_rgb(red_level, green_level, blue_level);
        }

        #ifndef TEST
		    delay(step_delay_mills);
        #endif
	
	}

}

void transition_global(int start_red_level, int start_green_level, int start_blue_level, int target_red_level, int target_green_level, int target_blue_level, int transition_time_mills) {

	//Set the global RGB levels to their start values
	//This should no actually change anything on the lights
	//as start levels should always refelect the current status of the 
	//LED levels.
	set_global_rgb(start_red_level, start_green_level, start_blue_level);

	//Calculate the times one channel should spend on each value 
	//while transitioning from start to target level over the time period
	//of transition_time_mills 	
	int red_level_step_time_mills = get_step_time(start_red_level, target_red_level, transition_time_mills);
	int green_level_step_time_mills = get_step_time(start_green_level, target_green_level, transition_time_mills);
	int blue_level_step_time_mills = get_step_time(start_blue_level, target_blue_level, transition_time_mills);

	//Find the highest common factor (HFC) of the three step times.
	//This will allow us to use a common delay time for the light transition
	//loop for all colours. Indicidual colour levels are then changed
	//after a calculated number of steps (transition loop iterations).
	int hfc_step_time = get_step_time_hfc(red_level_step_time_mills, green_level_step_time_mills, blue_level_step_time_mills);

	//Now that we have a common step time, we need to find out
	//after how many wait before we change the level of each colour channel
	int red_steps_till_change = get_steps_per_change(red_level_step_time_mills, hfc_step_time);
	int green_steps_till_change = get_steps_per_change(green_level_step_time_mills, hfc_step_time);
	int blue_steps_till_change = get_steps_per_change(blue_level_step_time_mills, hfc_step_time);

	//This is the total number of iterations we need to make in the light transition
	//loop until we reach transition_time_mills as which point all colour channel levels
	//are in their end state
	int total_step_count = get_total_step_count(transition_time_mills, hfc_step_time);

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
