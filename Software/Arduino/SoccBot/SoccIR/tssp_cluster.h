#pragma once

#include <Arduino.h>
#include <core/platform_detection.h>

#define TSSP_COUNT		24

class tssp_cluster_t
{
public:
	tssp_cluster_t();
	tssp_cluster_t(uint8_t min, uint8_t max = 0);

	float angle() const;
	ssize_t count() const;

	uint8_t get_min() const;
	void set_min(uint8_t min);
	uint8_t get_max() const;
	void set_max(uint8_t max);
private:
	uint8_t m_min;
	uint8_t m_max;
};