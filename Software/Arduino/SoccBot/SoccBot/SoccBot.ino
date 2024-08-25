#include <robot.h>

robot_state_t state(13, 9);

button_t button_start(10);
button_t button_calibrate(11);

uint8_t led_pins[] = { 12, 24, 20, 25, 15, 26, 21, 27, 16, 28, 22, 29, 17, 30, 23, 31 };
led_array_t led_array(led_pins, 16);

bool running = false;
int current_led = 0;
robot_timer_t led_timer;

void setup()
{
	state.set_state(robot_state_t::booted);
	delay(100);
	state.set_state(robot_state_t::standby);
}

void loop()
{
	state.update();

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
