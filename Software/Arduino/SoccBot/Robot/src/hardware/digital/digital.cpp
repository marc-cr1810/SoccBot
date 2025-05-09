#include "digital.h"

digital_t::digital_t(const uint8_t pin, mode_e mode)
	: m_pin(pin), m_state(low), m_mode(mode)
{
	pinMode(m_pin, m_mode);
}

digital_t::digital_t(const uint8_t pin, state_e state, mode_e mode)
	: m_pin(pin), m_state(state), m_mode(mode)
{
	pinMode(m_pin, m_mode);
	digitalWrite(m_pin, m_state);
}

void digital_t::write(state_e state)
{
	m_state = state;
	digitalWrite(m_pin, m_state);
}

digital_t::state_e digital_t::read()
{
	m_state = static_cast<state_e>(digitalRead(m_pin));
	return m_state;
}

const uint8_t digital_t::get_pin() const
{
	return m_pin;
}

void digital_t::set_mode(mode_e mode)
{
	m_mode = mode;
	pinMode(m_pin, m_mode);
}

digital_t::mode_e digital_t::get_mode() const
{
	return m_mode;
}