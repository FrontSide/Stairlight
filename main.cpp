#include <Arduino.h>
#include "strip.h"
#include "light.h"
#include "FastLED.h"

/* This is the Arduino startup function
which will be executed before the loop starts
at startup */
void setup() {

	//Initialise LED Strip
	init_strip();

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(7, OUTPUT); 

	digitalWrite(LED_BUILTIN, HIGH);
	
	//initialise motion sensor input pin
	//pinMode(MOTION_PIN, INPUT);

	/*play a startup sequence*/
	set_global_rgb(0,0,0);
	set_global_rgb(50,0,0);
	delay(2000);
	set_global_rgb(50,50,0);
	delay(2000);
	set_global_rgb(50,50,50);
	delay(2000);
	set_global_rgb(0,0,0);

}

void blink_controller_led() {
	for (int waitcount = 0; waitcount < 200; waitcount++) {
		delay(50);
		digitalWrite(CONTROLLER_LED, HIGH);
		delay(50);
		digitalWrite(CONTROLLER_LED, LOW);
	}
}

void run_top_to_bottom_sequence() {
	for(int dot = 0; dot < strip_led_count(); dot++) { 
            	strip_set_led(dot, 50, 50, 100);
	    	apply_strip();
            	delay(10);
        }
	blink_controller_led();
	delay(LIGHTS_ON_DELAY);
	for(int dot = 0; dot < strip_led_count(); dot++) { 
            	strip_set_led(dot, 0, 0, 0);
	    	apply_strip();
            	delay(10);
        }
	delay(BLOCK_MOTION_SENSOR_DELAY);
}

void run_bottom_to_top_sequence() {
	for(int dot = strip_led_count(); dot > 0; dot--) { 
            	strip_set_led(dot, 50, 50, 100);
	    	apply_strip();
            	delay(10);
        }
	blink_controller_led();
	delay(LIGHTS_ON_DELAY);
	for(int dot = strip_led_count(); dot > 0; dot--) { 
            	strip_set_led(dot, 0, 0, 0);
	    	apply_strip();
            	delay(10);
        }
	delay(BLOCK_MOTION_SENSOR_DELAY);
}


/*This is the Arduino loop function
which will be executed continuously
(same as while(true){})*/
void loop() {

	if (millis() % 4000 == 0) {
		digitalWrite(CONTROLLER_LED, LOW);
	} else if (millis() % 2000 == 0) {
		digitalWrite(CONTROLLER_LED, HIGH);
	}

        /* Illuminate LEDs starting from bottom (from LED 0 to end, from power source)
           Remark: Flickering, Colour gets warmer the farther along strip we go */
	if (digitalRead(MOTION_PIN_UPSTAIRS) < digitalRead(MOTION_PIN_DOWNSTAIRS)) {
		run_top_to_bottom_sequence();	
	/* Illuminate LEDs starting from the top
           Remark: Consistent colour across strip, no flickering */
	} else if (digitalRead(MOTION_PIN_UPSTAIRS) > digitalRead(MOTION_PIN_DOWNSTAIRS)) {
		run_bottom_to_top_sequence();	
	}	

}
