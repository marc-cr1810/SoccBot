#include "timer.h"

robot_timer_t::robot_timer_t()
{
	restart();
}

void robot_timer_t::start()
{
	m_stop = UINT32_MAX;
}

void robot_timer_t::stop()
{
	if (m_stop == UINT32_MAX)
	{
		m_stop == millis();
	}
}

void robot_timer_t::reset()
{
	m_stop = m_start = millis();
}

void robot_timer_t::restart()
{
	m_start = millis();
	start();
}

uint32_t robot_timer_t::elapsed() const
{
	return m_stop == UINT32_MAX ? millis() - m_start : m_stop - m_start;
}

double robot_timer_t::elapsed_seconds() const
{
	return (double)elapsed() / 1000;
}

robot_timer_t::operator uint32_t() const
{
	return elapsed();
}

robot_timer_t::operator double() const
{
	return elapsed_seconds();
}
