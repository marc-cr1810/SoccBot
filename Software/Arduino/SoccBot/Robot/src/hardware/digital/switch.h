#pragma once

#include "digital.h"

class switch_t : private digital_t
{
public:
	switch_t(const uint8_t pin);

	bool is_on() const;
	operator bool() const;
};