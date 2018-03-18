//Microcontroller and peripheral constants
#define MOTION_PIN 2

//times in milliseconds
#define TRANSITION_UP_TIME 3000 
#define TRANSITION_DOWN_TIME 3000 
#define STAY_ON_TIME 17000 
#define AFTER_CYCLE_REST 5000

//colour levels
#define RED_ON_LEVEL 255
#define GREEN_ON_LEVEL 180
#define BLUE_ON_LEVEL 180
#define MAX_RGB_LEVEL 255
#define MIN_RGB_LEVEL 0 

/* Set RGB levels for all LEDs on the strip
 * all LEDs will be set to the same level */
void set_global_rgb(int red_level, int green_level, int blue_level);

/* Descrease RGB levels globally 
 * over a given time period. 
 * 
 * total_step_count: 
 *  Number of transition loop iteration to perform for changing from start to target RBG levels
 *  this number is calculated from the transition time and the loop delay time in the transition_global function
 * red_wait_steps_count: 
 *  The amount of loop iterations the red channel will wait until its level is decreased by 1.
 *  the lower this number the more often the level will be decreased meaning that it will transition over a 
 *  higher quantity of light intensitiy level (wait_step_count is inverse peroportional to light level change quantity)
 * green_wait_steps_count: 
 *  Same as above but for the green light channel
 * blue_wait_steps_count: 
 *  Same as above but for the blue light channel
 * 
 * Having different wait step counts for each channel means that the colour of the light can change while its transitioning
 * thus meaning that different colour channels can transition from and to different levels of intensity over the same
 * period of time (full transition_time)*/
void step_decrease_global(int total_step_count, int red_wait_steps_count, int green_wait_steps_count, int blue_wait_steps_count, int step_delay_mills);

/* Increase RGB levels globally 
 * over a given time period.
 * 
 * See step_decrease_global docstring */
void step_increase_global(int total_step_count, int red_wait_steps_count, int green_wait_steps_count, int blue_wait_steps_count, int step_delay_mills);

/*Transition RGB levels globally 
 *from given start to given finish levels*/
void transition_global(int start_red_level, int start_green_level, int start_blue_level, int target_red_level, int target_green_level, int target_blue_level, int transition_time_mills);

/* Turn on the lights over a given period of time 
 * 
 * transition_time_mills:
 *  total time in milliseconds the transition from off to on should take
 */
void light_up(int transition_time_mills);

/* Turn off the lights over a given period of time 
 * 
 * transition_time_mills:
 *  total time in milliseconds the transition from on to off should take*/
void light_down(int transition_time_mills);
