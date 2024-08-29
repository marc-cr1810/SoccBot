#pragma once

#include <Arduino.h>

#include "../../core/platform_detection.h"

class switch_array_t
{
public:
	switch_array_t(uint8_t pins[], ssize_t count);
	~switch_array_t();

	bool all_on() const;
	bool all_off() const;

	bool is_on(ssize_t index) const;

	bool operator[](ssize_t index) const;
	operator bool() const;
private:
	uint8_t* m_pins;
	ssize_t m_count;
};