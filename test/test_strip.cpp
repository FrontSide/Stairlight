#include "utils.h"
#include "light.h"
#include <iostream>

int main() {

	int total_transition_time = 3000;

	int red_step_time = get_step_time(0, 255, total_transition_time);
	int green_step_time = get_step_time(0, 170, total_transition_time);
	int blue_step_time = get_step_time(0, 170, total_transition_time);

	std::cout << "Red Step Time: " << red_step_time << '\n';
	std::cout << "Green Step Time: " <<  green_step_time << '\n';
	std::cout << "Blue Step Time: " <<  blue_step_time << '\n';

	int hfc_step_time = get_step_time_hfc(red_step_time, green_step_time, blue_step_time);
	std::cout << "HFC Step Time: " << hfc_step_time << '\n';
	
	int red_steps_till_change = get_steps_per_change(red_step_time, hfc_step_time);
	int green_steps_till_change = get_steps_per_change(green_step_time, hfc_step_time);
	int blue_steps_till_change = get_steps_per_change(blue_step_time, hfc_step_time);

	std::cout << "Red Steps Till Change: " << red_steps_till_change << '\n';
	std::cout << "Green Steps Till Change: " <<  green_steps_till_change << '\n';
	std::cout << "Blue Steps Till Change: " <<  blue_steps_till_change << '\n';
	
	int total_step_count = get_total_step_count(total_transition_time, hfc_step_time);
	std::cout << "Total Step Count: " <<  total_step_count << '\n';

	set_global_rgb(255,255,255);
	step_increase_global(total_step_count, red_steps_till_change, green_steps_till_change, blue_steps_till_change, hfc_step_time);
	set_global_rgb(0,0,0);

	std::cout << "Light up 3000\n";
	light_up(3000);
	std::cout << "Light down 3000\n";
	light_down(3000);

	return 0;

}
