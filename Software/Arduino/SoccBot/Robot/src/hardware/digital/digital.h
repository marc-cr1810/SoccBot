#pragma once

#include <Arduino.h>

class digital_t
{
public:
	enum state_e : uint8_t
	{
		low = LOW,
		high = HIGH
	};

	enum mode_e : uint8_t
	{
		input = INPUT,
		output = OUTPUT,
		input_pullup = INPUT_PULLUP
	};

	digital_t(const uint8_t pin, mode_e mode = input);
	digital_t(const uint8_t pin, state_e state, mode_e mode = input);

	virtual void write(state_e state);
	virtual state_e read();

	const uint8_t get_pin() const;
	void set_mode(mode_e mode);
	mode_e get_mode() const;
protected:
	const uint8_t m_pin;
	state_e m_state;
	mode_e m_mode;
};