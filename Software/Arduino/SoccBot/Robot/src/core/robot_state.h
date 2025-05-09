#pragma once

#include <Arduino.h>

#include "../hardware/digital/led.h"
#include "../utils/timer.h"

class robot_state_t
{
public:
	enum state_e
	{
		unknown = -1,
		booted = 0,
		ok,
		error,
		warning,
		standby
	};

	robot_state_t(uint8_t red_led_pin, uint8_t green_led_pin);

	void update();

	void set_state(state_e state);
private:
	led_t m_red_led;
	led_t m_green_led;
	robot_timer_t m_timer;

	bool m_red_blink = false;
	bool m_green_blink = false;

	state_e m_state;
};