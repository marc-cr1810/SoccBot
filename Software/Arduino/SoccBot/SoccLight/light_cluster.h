#pragma once

#include <Arduino.h>
#include <core/platform_detection.h>

#define LIGHT_SENSOR_COUNT	24

struct point_t
{
	float x;
	float y;
};

class light_cluster_t
{
public:
	light_cluster_t();
	light_cluster_t(uint8_t min, uint8_t max = 0);

	float angle() const;
	ssize_t count() const;
	point_t point() const;

	uint8_t get_min() const;
	void set_min(uint8_t min);
	uint8_t get_max() const;
	void set_max(uint8_t max);
private:
	uint8_t m_min;
	uint8_t m_max;
};