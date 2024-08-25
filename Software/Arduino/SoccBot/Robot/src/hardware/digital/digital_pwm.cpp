#include "digital_pwm.h"


digital_pwm_t::digital_pwm_t(uint8_t pin)
	: digital_t(pin, output)
{ }

digital_pwm_t::digital_pwm_t(uint8_t pin, uint8_t value)
	: digital_t(pin, output)
{
	analogWrite(m_pin, value);
}

void digital_pwm_t::write(uint8_t value)
{
	analogWrite(m_pin, value);
}

digital_pwm_t::state_e digital_pwm_t::read()
{
	return low;
}