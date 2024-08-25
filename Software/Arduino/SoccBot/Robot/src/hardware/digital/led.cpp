#include "led.h"

led_t::led_t(uint8_t pin, state_e state)
	: digital_t(pin, state, digital_t::output)
{ }

void led_t::on()
{
	write(state_e::high);
}

void led_t::off()
{
	write(state_e::low);
}

void led_t::toggle()
{
	m_state = m_state == state_e::high ? state_e::low : state_e::high;
	write(m_state);
}