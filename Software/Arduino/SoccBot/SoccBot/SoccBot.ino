#include <Wire.h>
#include <robot.h>

#include "hardware/ir_controller.h"

robot_state_t state(13, 9);

button_t button_start(10);
button_t button_calibrate(11);

uint8_t led_pins[] = { 12, 24, 20, 25, 15, 26, 21, 27, 16, 28, 22, 29, 17, 30, 23, 31 };
led_array_t led_array(led_pins, 16);

uint8_t opt_switch_pins[] = { 2, 3, 4, 5, 6, 32 };
switch_array_t opt_switch(opt_switch_pins, 6);

ir_controller_t ir_controller(IR_I2C_ADDRESS);

bool running = false;
int current_led = 0;
robot_timer_t led_timer;

void setup()
{
	state.set_state(robot_state_t::booted);
	
	Wire.begin();
	Serial.begin(115200);

	if (ir_controller.check_connection())
	{
		state.set_state(robot_state_t::standby);
	}
	else
	{
		state.set_state(robot_state_t::error);
	}
}

void loop()
{
	ir_controller.update();
	state.update();

	Serial.printf("Ball detected: %s  Ball Angle: %f\n", ir_controller.ball_detected() ? "true" : "false", ir_controller.ball_angle());

	if (button_start.released())
	{
		running = !running;
		state.set_state(running ? robot_state_t::ok : robot_state_t::standby);
		led_array.all_off();
	}

	if (running)
	{
		if ((uint32_t)led_timer >= 50)
		{
			for (int i = 0; i < 16; i++)
			{
				if (i == current_led)
				{
					led_array.on(i);
				}
				else
				{
					led_array.off(i);
				}
			}
			current_led++;
			current_led %= 16;
			led_timer.restart();
		}
	}

	delay(1);
}
