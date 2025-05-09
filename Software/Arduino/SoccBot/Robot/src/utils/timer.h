#pragma once

#include <Arduino.h>

class robot_timer_t
{
public:
	robot_timer_t();

	void start();
	void stop();
	void reset();
	void restart();

	uint32_t elapsed() const;
	double elapsed_seconds() const;

	operator uint32_t() const;
	operator double() const;
private:
	uint32_t m_start = 0;
	uint32_t m_stop = UINT32_MAX;
};