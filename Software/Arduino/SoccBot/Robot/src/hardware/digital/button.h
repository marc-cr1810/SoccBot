#pragma once

#include "digital.h"

class button_t : private digital_t
{
public:
	button_t(uint8_t pin);

	bool pressed();
	bool released();
	bool is_pressed();
private:
	state_e m_prev_state;
};