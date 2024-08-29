#include "switch.h"

switch_t::switch_t(const uint8_t pin)
	: digital_t(pin)
{ }

bool switch_t::is_on() const
{
	return digitalRead(m_pin);
}

switch_t::operator bool() const
{
	return is_on();
}
