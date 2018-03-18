#define STRIP_NUM_LEDS 150
#define STRIP_DATA_PIN 5

/* Initialitze the LED Strip */
void init_strip();

/* Apply changes to the actual LED strip through the microcontroller */
void apply_strip();

/* Get gurrent global red channel level 
 * (This is done by simply reading the red level of the first LED) */
int strip_get_global_red_level();

/* As strip_get_global_red_level for for the green colour channel */
int strip_get_global_green_level();

/* As strip_get_global_red_level for for the blue colour channel */
int strip_get_global_blue_level();

/* Set the colour levels for a specific LED in the array.
   This merely changes the levels in the in-memory array but doesn't write
   the actual value to the LED strip. Use apply_strip() to write to the strip. */
void strip_set_led(int led_idx, int red_level, int green_level, int blue_level);

/* Return the number of LEDs in the array */
int strip_led_count();