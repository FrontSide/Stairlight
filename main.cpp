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
	set_global_rgb(255,0,0);
	delay(2000);
	set_global_rgb(255,255,0);
	delay(2000);
	set_global_rgb(255,255,255);
	delay(2000);
	set_global_rgb(0,0,0);

}

/*This is the Arduino loop function
which will be executed continuously
(same as while(true){})*/
void loop() {

	if (millis() % 4000 == 0) {
		digitalWrite(7, LOW);
	}
	else if (millis() % 2000 == 0) {
		digitalWrite(7, HIGH);
	}
		
	if (digitalRead(MOTION_PIN) > 0) {

		for(int dot = 0; dot < strip_led_count(); dot++) { 
            		strip_set_led(dot, 100, 100, 150);
	    		apply_strip();
            		delay(50);
        	}
  
		for (int waitcount = 0; waitcount < 200; waitcount++) {
			delay(50);
			digitalWrite(7, HIGH);
			delay(50);
			digitalWrite(7, LOW);
		}

		delay(20000);

		for(int dot = 0; dot < strip_led_count(); dot++) { 
            		strip_set_led(dot, 0, 0, 0);
	    		apply_strip();
            		delay(50);
        	}

		delay(5000);
	
	}	


	// If there's input on the motion sensor
	// start the light sequence.
	/*
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

	}*/
  
}
