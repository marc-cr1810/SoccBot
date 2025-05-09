#include "digital_multi.h"

digital_multi_t::digital_multi_t(const uint8_t pin, const uint8_t addr_pin_a, const uint8_t addr_pin_b, const uint8_t addr_pin_c, mode_e mode)
	: digital_t(pin, mode), m_addr_pin_a(addr_pin_a), m_addr_pin_b(addr_pin_b), m_addr_pin_c(addr_pin_c)
{
	pinMode(m_addr_pin_a, OUTPUT);
	pinMode(m_addr_pin_b, OUTPUT);
	pinMode(m_addr_pin_c, OUTPUT);

	digitalWrite(m_addr_pin_a, LOW);
	digitalWrite(m_addr_pin_b, LOW);
	digitalWrite(m_addr_pin_c, LOW);
}

digital_multi_t::digital_multi_t(const uint8_t pin, const uint8_t addr_pin_a, const uint8_t addr_pin_b, const uint8_t addr_pin_c, state_e state, mode_e mode)
	: digital_t(pin, state, mode), m_addr_pin_a(addr_pin_a), m_addr_pin_b(addr_pin_b), m_addr_pin_c(addr_pin_c)
{
	pinMode(m_addr_pin_a, OUTPUT);
	pinMode(m_addr_pin_b, OUTPUT);
	pinMode(m_addr_pin_c, OUTPUT);

	digitalWrite(m_addr_pin_a, LOW);
	digitalWrite(m_addr_pin_b, LOW);
	digitalWrite(m_addr_pin_c, LOW);
}

void digital_multi_t::write(uint8_t address, state_e state)
{
	set_address(address);
	digital_t::write(state);
}

digital_t::state_e digital_multi_t::read(uint8_t address)
{
	set_address(address);
	return digital_t::read();
}

void digital_multi_t::set_address(uint8_t address)
{
	m_selected_addr = address;

	digitalWrite(m_addr_pin_a, (m_selected_addr & 0b001) ? HIGH : LOW);
	digitalWrite(m_addr_pin_b, (m_selected_addr & 0b010) ? HIGH : LOW);
	digitalWrite(m_addr_pin_c, (m_selected_addr & 0b100) ? HIGH : LOW);
}

uint8_t digital_multi_t::get_address() const
{
	return m_selected_addr;
}
