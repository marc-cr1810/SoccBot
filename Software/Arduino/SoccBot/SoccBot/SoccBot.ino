#include <Wire.h>
#include <robot.h>

#include "hardware/camera.h"
#include "hardware/ir_controller.h"
#include "hardware/light_sensor.h"
#include "hardware/motor_controller.h"

robot_state_t state(13, 9);

button_t button_start(10);
button_t button_calibrate(11);

uint8_t led_pins[] = { 12, 24, 20, 25, 15, 26, 21, 27, 16, 28, 22, 29, 17, 30, 23, 31 };
led_array_t led_array(led_pins, 16);

uint8_t opt_switch_pins[] = { 2, 3, 4, 5, 6, 32 };
switch_array_t opt_switch(opt_switch_pins, 6);

camera_t camera;
analog_t lightgate(14);
ir_controller_t ir_controller(IR_I2C_ADDRESS);
light_sensor_t light_sensor(LIGHT_I2C_ADDRESS);
motor_controller_t motor_controller(MOTOR_I2C_ADDRESS);

bool running = false;
int current_led = 0;
robot_timer_t led_timer;

void setup()
{
	state.set_state(robot_state_t::booted);
	
	Wire.begin();
	camera.init();

	bool error = false;
	if (!ir_controller.check_connection())
	{
		error = true;
	}

	if (light_sensor.check_connection())
	{
		error = true;
	}

	if (motor_controller.check_connection())
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

void move_towards_ball()
{
	float ball_angle = ir_controller.ball_angle();
	float move_direction = 0.0f;

	float rotation = 0.0;
	if (camera.has_goal())
	{
		auto image = camera.get_image();
		auto center = camera.get_goal().center;
		auto adj = center.x - (image.width / 2);
		rotation = ((float)adj / (image.width / 2)) * -1;
	}

	if ((ball_angle >= 0.0f && ball_angle < 11.25f) || (ball_angle >= 348.75f && ball_angle <= 360.0f))
	{
		move_direction = 270.0f;
	}
	else if (ball_angle >= 11.25f && ball_angle < 22.5f)
	{
		move_direction = 315.0f;
	}
	else if (ball_angle >= 337.5f && ball_angle < 348.75f)
	{
		move_direction = 225.0f;
	}
	else if (ball_angle >= 22.5f && ball_angle < 67.5f)
	{
		move_direction = 45.0f;
	}
	else if (ball_angle >= 67.5f && ball_angle < 112.5f)
	{
		move_direction = 90.0f;
	}
	else if (ball_angle >= 112.5f && ball_angle < 157.5f)
	{
		move_direction = 135.0f;
	}
	else if (ball_angle >= 157.5f && ball_angle < 202.5f)
	{
		move_direction = 180.0f;
	}
	else if (ball_angle >= 202.5f && ball_angle < 247.5f)
	{
		move_direction = 135.0f;
	}
	else if (ball_angle >= 247.5f && ball_angle < 292.5f)
	{
		move_direction = 90.0f;
	}
	else if (ball_angle >= 292.5f && ball_angle < 337.5f)
	{
		move_direction = 135.0f;
	}

	motor_controller.move(move_direction, rotation, 220);
}

void loop()
{
	camera.update();
	ir_controller.update();
	light_sensor.update();
	state.update();

	Serial.printf("image %ix%i, goal %i %i\n", camera.get_image().width, camera.get_image().height, camera.get_goal().center.x, camera.get_goal().center.y);
	//Serial.printf("Ball detected: %s  Ball Angle: %.02f\n", ir_controller.ball_detected() ? "true" : "false", ir_controller.ball_angle());

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
			if (ir_controller.ball_detected())
			{
				move_towards_ball();
			}
			else
			{
				motor_controller.stop();
			}
		}
	}
	else
	{
		if (camera.has_goal())
		{
			auto image = camera.get_image();
			auto center = camera.get_goal().center;
			auto adj = center.x - (image.width / 2);
			float speed = round((abs((float)adj) / (image.width / 2)) * 60);
			Serial.println(speed);

			if (adj < 0)
			{
				motor_controller.rotation(speed);
				Serial.printf("left");
			}
			else
			{
				motor_controller.rotation(-speed);
				Serial.printf("right");
			}
		}
		else
		{
			motor_controller.stop();
		}
	}

	delay(1);
}
