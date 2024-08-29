#include "switch_array.h"

switch_array_t::switch_array_t(uint8_t pins[], ssize_t count)
	: m_count(count)
{
	m_pins = new uint8_t[count];
	for (ssize_t i = 0; i < m_count; i++)
	{
		m_pins[i] = pins[i];
		pinMode(m_pins[i], INPUT);
	}
}

switch_array_t::~switch_array_t()
{
	delete[] m_pins;
}

bool switch_array_t::all_on() const
{
	for (ssize_t i = 0; i < m_count; i++)
	{
		if (digitalRead(m_pins[i]) == LOW)
		{
			return false;
		}
	}

	return true;
}

bool switch_array_t::all_off() const
{
	for (ssize_t i = 0; i < m_count; i++)
	{
		if (digitalRead(m_pins[i]) == HIGH)
		{
			return false;
		}
	}

	return true;
}

bool switch_array_t::is_on(ssize_t index) const
{
	if (index >= m_count)
	{
		return false;
	}

	return digitalRead(m_pins[index]) == HIGH;
}

bool switch_array_t::operator[](ssize_t index) const
{
	return is_on(index);
}

switch_array_t::operator bool() const
{
	return all_on();
}
