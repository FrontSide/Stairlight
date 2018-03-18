#include <Arduino.h>
#include "strip.h"
#include "light.h"

/* This is the Arduino startup function
which will be executed before the loop starts
at startup */
void setup() {

	//Initialise LED Strip
	init_strip();
	
	//initialise motion sensor input pin
	pinMode(MOTION_PIN, INPUT);

	//play a startup sequence
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