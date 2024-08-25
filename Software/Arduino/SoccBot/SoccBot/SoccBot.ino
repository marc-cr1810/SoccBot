#include <robot.h>

led_t led_red(13);
led_t led_green(9);

button_t button_start(10);
button_t button_calibrate(11);

robot_timer_t timer;

void setup()
{
	led_red.on();
	led_green.on();

	timer.start();
}

void loop()
{
	if (button_start.released())
	{
		led_green.on();
	}
	else
	{
		led_green.off();
	}

	if ((uint32_t)timer >= 250)
	{
		led_red.toggle();
		timer.restart();
	}

	delay(50);
}
