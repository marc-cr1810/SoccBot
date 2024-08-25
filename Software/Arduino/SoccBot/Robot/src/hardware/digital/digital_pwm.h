#pragma once

#include "digital.h"

class digital_pwm_t : public digital_t
{
public:
	digital_pwm_t(uint8_t pin);
	digital_pwm_t(uint8_t pin, uint8_t value);

	void write(uint8_t value);
	virtual state_e read() override;
};