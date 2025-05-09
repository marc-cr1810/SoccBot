#pragma once

#include "digital.h"

class digital_multi_t : public digital_t
{
public:
	digital_multi_t(const uint8_t pin, const uint8_t addr_pin_a, const uint8_t addr_pin_b, const uint8_t addr_pin_c, mode_e mode = input);
	digital_multi_t(const uint8_t pin, const uint8_t addr_pin_a, const uint8_t addr_pin_b, const uint8_t addr_pin_c, state_e state, mode_e mode = input);

	void write(uint8_t address, state_e state);
	state_e read(uint8_t address);

	void set_address(uint8_t address);
	uint8_t get_address() const;
private:
	uint8_t m_selected_addr = 0b000;
	const uint8_t m_addr_pin_a, m_addr_pin_b, m_addr_pin_c;
};