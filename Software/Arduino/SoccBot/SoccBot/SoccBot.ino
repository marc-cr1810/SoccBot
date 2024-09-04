#include <Wire.h>
#include <robot.h>

#include "hardware/ir_controller.h"
#include "hardware/light_sensor.h"

robot_state_t state(13, 9);

button_t button_start(10);
button_t button_calibrate(11);

uint8_t led_pins[] = { 12, 24, 20, 25, 15, 26, 21, 27, 16, 28, 22, 29, 17, 30, 23, 31 };
led_array_t led_array(led_pins, 16);

uint8_t opt_switch_pins[] = { 2, 3, 4, 5, 6, 32 };
switch_array_t opt_switch(opt_switch_pins, 6);

analog_t lightgate(14);
ir_controller_t ir_controller(IR_I2C_ADDRESS);
light_sensor_t light_sensor(LIGHT_I2C_ADDRESS);

bool running = false;
int current_led = 0;
robot_timer_t led_timer;

void setup()
{
	state.set_state(robot_state_t::booted);
	
	Wire.begin();
	Serial.begin(115200);

	bool error = false;
	if (!ir_controller.check_connection())
	{
		error = true;
	}

	if (light_sensor.check_connection())
	{
		error = true;
	}

	if (error)
	{
		state.set_state(robot_state_t::standby);
	}
	else
	{
		state.set_state(robot_state_t::error);
	}
}

void light_ball_direction()
{
	if (!ir_controller.ball_detected())
	{
		led_array.all_off();
		return;
	}

	float angle = ir_controller.ball_angle();
	int led_index = floor(angle / (360.0f / led_array.count()));
	for (ssize_t i = 0; i < led_array.count(); i++)
	{
		if (i == led_index)
		{
			led_array.on(i);
		}
		else
		{
			led_array.off(i);
		}
	}
}

void loop()
{
	ir_controller.update();
	light_sensor.update();
	state.update();

	Serial.printf("Ball detected: %s  Ball Angle: %.02f\n", ir_controller.ball_detected() ? "true" : "false", ir_controller.ball_angle());

	if (button_start.released())
	{
		running = !running;
		state.set_state(running ? robot_state_t::ok : robot_state_t::standby);
		led_array.all_off();
	}

	if (running)
	{
		if (lightgate.read() < 150)
		{
			led_array.all_on();
		}
		else
		{
			light_ball_direction();
		}
	}

	delay(1);
}
