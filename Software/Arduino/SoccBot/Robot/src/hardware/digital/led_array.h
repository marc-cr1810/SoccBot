#pragma once

#include <Arduino.h>

#include "../../core/platform_detection.h"

class led_array_t
{
public:
	led_array_t(uint8_t pins[], ssize_t count);
	~led_array_t();

	void all_on();
	void all_off();

	void on(ssize_t index);
	void off(ssize_t index);
private:
	uint8_t* m_pins;
	ssize_t m_count;
};