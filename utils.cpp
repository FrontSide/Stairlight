#include "utils.h"
#include <stdlib.h>

int get_step_time(int start_level, int target_level, int transition_time) {
    return transition_time / abs(target_level - start_level);
}

int get_step_time_hfc(int red_step_time, int green_step_time, int blue_step_time) {

    //First get the smallest of the three step times
    int smallest_step_time_mills = blue_step_time;
	
	if ((red_step_time < green_step_time) && (red_step_time < blue_step_time)) {
		smallest_step_time_mills = red_step_time;
	} else if (green_step_time < blue_step_time) {
		smallest_step_time_mills = green_step_time;
	}

	int hfc_step_time = 1;

	//Start with a random step time from above and decrease the number 
	//until the result of all step times modulo the iteration step time
	//equals 0. This iteration step time will then be highest common factor.
	for (int hfc_iter=smallest_step_time_mills;hfc_iter>1;hfc_iter--) {
	
		if ((red_step_time % hfc_iter == 0) &&
		    (green_step_time % hfc_iter == 0) &&
	        (blue_step_time % hfc_iter == 0)) {
		    
			hfc_step_time = hfc_iter;
			break;

		}

	}

	return hfc_step_time;

}

int get_steps_per_change(int individual_step_time, int hfc_step_time) {
    return individual_step_time / hfc_step_time;
}

int get_total_step_count(int total_transition_time, int hfc_step_time) {
    return total_transition_time / hfc_step_time;
}
