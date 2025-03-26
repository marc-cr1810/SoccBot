#include "tssp_cluster.h"

#define TSSP_ANGLE_0	0
#define TSSP_ANGLE_1	14
#define TSSP_ANGLE_2	27
#define TSSP_ANGLE_3	39
#define TSSP_ANGLE_4	53
#define TSSP_ANGLE_5	67
#define TSSP_ANGLE_6	82
#define TSSP_ANGLE_7	98
#define TSSP_ANGLE_8	113
#define TSSP_ANGLE_9	127
#define TSSP_ANGLE_10	145
#define TSSP_ANGLE_11	162
#define TSSP_ANGLE_12	180
#define TSSP_ANGLE_13	198
#define TSSP_ANGLE_14	215
#define TSSP_ANGLE_15	233
#define TSSP_ANGLE_16	247
#define TSSP_ANGLE_17	262
#define TSSP_ANGLE_18	278
#define TSSP_ANGLE_19	292
#define TSSP_ANGLE_20	307
#define TSSP_ANGLE_21	321
#define TSSP_ANGLE_22	333
#define TSSP_ANGLE_23	346

uint16_t tssp_angles[TSSP_COUNT] = {
	TSSP_ANGLE_0,
	TSSP_ANGLE_1,
	TSSP_ANGLE_2,
	TSSP_ANGLE_3,
	TSSP_ANGLE_4,
	TSSP_ANGLE_5,
	TSSP_ANGLE_6,
	TSSP_ANGLE_7,
	TSSP_ANGLE_8,
	TSSP_ANGLE_9,
	TSSP_ANGLE_10,
	TSSP_ANGLE_11,
	TSSP_ANGLE_12,
	TSSP_ANGLE_13,
	TSSP_ANGLE_14,
	TSSP_ANGLE_15,
	TSSP_ANGLE_16,
	TSSP_ANGLE_17,
	TSSP_ANGLE_18,
	TSSP_ANGLE_19,
	TSSP_ANGLE_20,
	TSSP_ANGLE_21,
	TSSP_ANGLE_22,
	TSSP_ANGLE_23
};

tssp_cluster_t::tssp_cluster_t()
	: m_min(0), m_max(0)
{ }

tssp_cluster_t::tssp_cluster_t(uint8_t min, uint8_t max)
	: m_min(min % TSSP_COUNT), m_max(max % TSSP_COUNT)
{ }

float tssp_cluster_t::angle() const
{
	int i = m_min;
	float sum = 0;
	while (i != m_max + 1)
	{
		float angle = tssp_angles[i];
		if (i > m_max)
		{
			angle = angle - 360;
		}
		sum += angle;
		i++;
		i %= TSSP_COUNT;
	}
	float result = sum / count();
	if (result < 0)
	{
		result = result + 360;
	}
	return result;
}

ssize_t tssp_cluster_t::count() const
{
	int min = m_min;
	int max = m_max;
	if (min > max)
	{
		min = -(TSSP_COUNT - min);
	}
	return (ssize_t)(max - min + 1);
}

uint8_t tssp_cluster_t::get_min() const
{
	return m_min;
}

void tssp_cluster_t::set_min(uint8_t min)
{
	m_min = min % TSSP_COUNT;
}

uint8_t tssp_cluster_t::get_max() const
{
	return m_max;
}

void tssp_cluster_t::set_max(uint8_t max)
{
	m_max = max % TSSP_COUNT;
}
