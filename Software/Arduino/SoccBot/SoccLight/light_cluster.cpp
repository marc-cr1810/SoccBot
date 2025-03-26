#include "light_cluster.h"

light_cluster_t::light_cluster_t()
	: m_min(0), m_max(0)
{ }

light_cluster_t::light_cluster_t(uint8_t min, uint8_t max)
	: m_min(min), m_max(max)
{ }

float light_cluster_t::angle() const
{
	int i = m_min;
	int sum = 0;
	while (i != m_max + 1)
	{
		float angle = i * (360.0f / LIGHT_SENSOR_COUNT);
		if (i > m_max)
		{
			angle = angle - 360;
		}
		sum += angle;
		i++;
		i %= LIGHT_SENSOR_COUNT;
	}
	float result = sum / count();
	if (result < 0)
	{
		result = result + 360;
	}
	return result;
}

ssize_t light_cluster_t::count() const
{
	int min = m_min;
	int max = m_max;
	if (min > max)
	{
		min = -(LIGHT_SENSOR_COUNT - min);
	}
	return (ssize_t)(max - min + 1);
}

point_t light_cluster_t::point() const
{
	float a = angle() * DEG_TO_RAD;
	return { (float)cos(a), (float)sin(a) };
}

uint8_t light_cluster_t::get_min() const
{
	return m_min;
}

void light_cluster_t::set_min(uint8_t min)
{
	m_min = min % LIGHT_SENSOR_COUNT;
}

uint8_t light_cluster_t::get_max() const
{
	return m_max;
}

void light_cluster_t::set_max(uint8_t max)
{
	m_max = max % LIGHT_SENSOR_COUNT;
}
