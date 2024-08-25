#pragma once

#include "digital.h"

class led_t : private digital_t
{
public:
	led_t(uint8_t pin, state_e state = state_e::low);

	void on();
	void off();
	void toggle();
};