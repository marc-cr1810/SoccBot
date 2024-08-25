#include "button.h"

button_t::button_t(uint8_t pin)
	: digital_t(pin)
{ }

bool button_t::pressed()
{
	read();
	bool result = m_state == state_e::high && m_state != m_prev_state;
	m_prev_state = m_state;
	return result;
}

bool button_t::released()
{
	read();
	bool result = m_state == state_e::low && m_state != m_prev_state;
	m_prev_state = m_state;
	return result;
}

bool button_t::is_pressed()
{
	return read() == state_e::high;
}
