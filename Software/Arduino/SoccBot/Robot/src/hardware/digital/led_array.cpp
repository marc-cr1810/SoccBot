#include "led_array.h"

led_array_t::led_array_t(uint8_t pins[], ssize_t count)
	: m_count(count)
{
	m_pins = new uint8_t[count];
	for (ssize_t i = 0; i < m_count; i++)
	{
		m_pins[i] = pins[i];
		pinMode(m_pins[i], OUTPUT);
	}
}

led_array_t::~led_array_t()
{
	delete[] m_pins;
}

void led_array_t::all_on()
{
	for (ssize_t i = 0; i < m_count; i++)
	{
		digitalWrite(m_pins[i], HIGH);
	}
}

void led_array_t::all_off()
{
	for (ssize_t i = 0; i < m_count; i++)
	{
		digitalWrite(m_pins[i], LOW);
	}
}

void led_array_t::on(ssize_t index)
{
	if (index >= m_count)
	{
		return;
	}

	digitalWrite(m_pins[index], HIGH);
}

void led_array_t::off(ssize_t index)
{
	if (index >= m_count)
	{
		return;
	}

	digitalWrite(m_pins[index], LOW);
}
