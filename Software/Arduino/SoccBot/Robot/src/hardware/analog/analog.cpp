#include "analog.h"

analog_t::analog_t(const uint8_t pin)
	: m_pin(pin)
{ }

uint16_t analog_t::read() const
{
	return analogRead(m_pin);
}
