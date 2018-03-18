/*Get the amount of transition time assigned to each value
of the value range from start_level to target_level*/
int get_step_time(int start_level, int target_level, int transition_time);

/*Find the highest common factor (HFC) of three step times.*/
int get_step_time_hfc(int red_step_time, int green_step_time, int blue_step_time);

/*Get the amount of steps a value will wait within a transition loop
before its value is changed to accommodate the transition 
over the total transition time period*/
int get_steps_per_change(int individual_step_time, int hfc_step_time);

/*Get the amount of total transition steps based on transition time 
and hfc step time as calculated by get_step_time_hfc*/
int get_total_step_count(int total_transition_time, int hfc_step_time);