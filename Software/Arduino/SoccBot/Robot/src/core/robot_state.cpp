#include "robot_state.h"

robot_state_t::robot_state_t(uint8_t red_led_pin, uint8_t green_led_pin)
	: m_red_led(red_led_pin), m_green_led(green_led_pin)
{ }

void robot_state_t::update()
{
	if ((uint32_t)m_timer >= 500)
	{
		if (m_red_blink)
		{
			m_red_led.toggle();
		}
		if (m_green_blink)
		{
			m_green_led.toggle();
		}
		m_timer.restart();
	}
}

void robot_state_t::set_state(state_e state)
{
	m_red_blink = false;
	m_green_blink = false;

	m_state = state;
	switch (m_state)
	{
	case unknown:
	{
		m_red_led.on();
		m_red_blink = true;
		m_green_led.on();
		m_green_blink = true;
	}
	case booted:
	{
		m_red_led.on();
		m_green_led.off();
		break;
	}
	case ok:
	{
		m_red_led.off();
		m_green_led.on();
		break;
	}
	case error:
	{
		m_red_led.on();
		m_red_blink = true;
		m_green_led.off();
		break;
	}
	case warning:
	{
		m_red_led.on();
		m_red_blink = true;
		m_green_led.on();
		break;
	}
	case standby:
	{
		m_red_led.off();
		m_green_led.on();
		m_green_blink = true;
		break;
	}
	}
}
