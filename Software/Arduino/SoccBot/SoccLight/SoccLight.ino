#include <Wire.h>
#include <robot.h>

#include "light_cluster.h"

#define THRESHOLD	800

robot_state_t state(11, 10);

analog_multi_t light_cluster_1(PIN_PC0, PIN_PD0, PIN_PD1, PIN_PD2);
analog_multi_t light_cluster_2(PIN_PC1, PIN_PD3, PIN_PD4, PIN_PD5);
analog_multi_t light_cluster_3(PIN_PC2, PIN_PD6, PIN_PD7, PIN_PB0);

light_cluster_t light_clusters[LIGHT_SENSOR_COUNT - 1];

bool on_white = false;
uint16_t angle = 0;
uint32_t lightsensors = 0;

void setup()
{
	state.set_state(robot_state_t::booted);

	Wire.begin(LIGHT_I2C_ADDRESS);
	Wire.onRequest(i2c_request_event);
	Wire.onReceive(i2c_recieve_event);

	delay(100);
	state.set_state(robot_state_t::standby);
}

uint16_t read_light_sensor(uint8_t index)
{
	uint8_t cluster_index = floor((float)index / 8.0f);
	analog_multi_t* cluster = nullptr;
	switch (cluster_index)
	{
	case 0: cluster = &light_cluster_1; break;
	case 1: cluster = &light_cluster_2; break;
	case 2: cluster = &light_cluster_3; break;
	default: state.set_state(robot_state_t::error); return 0;
	}

	uint8_t cluster_light_index = index - (cluster_index * 8);
	if (cluster_light_index > 0b111)
	{
		state.set_state(robot_state_t::error);
		return 0;
	}
	return cluster->read(cluster_light_index);
}

void loop()
{
	state.update();

	int count = 0;
	int8_t min = -1;
	int8_t max = -1;
	uint32_t values = 0;
	bool is_cluster = false;

	for (ssize_t i = 0; i < LIGHT_SENSOR_COUNT; i++)
	{
		bool value = read_light_sensor(i) > THRESHOLD;
		values |= (uint32_t)(value ? 1 : 0) << (23 - i);

		if (is_cluster)
		{
			if (!value)
			{
				max = i - 1;
				light_clusters[count].set_min(min);
				light_clusters[count].set_max(max);
				count++;
				min = -1;
				max = -1;
				is_cluster = false;
			}
			else if (i == (LIGHT_SENSOR_COUNT - 1))
			{
				max = i;
				if (count > 0 && light_clusters[0].get_min() == 0)
				{
					light_clusters[0].set_min(max);
					min = -1;
					max = -1;
					is_cluster = false;
				}
				else
				{
					max = i - 1;
					light_clusters[count].set_min(min);
					light_clusters[count].set_max(max);
					count++;
					min = -1;
					max = -1;
					is_cluster = false;
				}
			}
		}
		else if (value)
		{
			min = i;
			is_cluster = true;
		}
	}

	if (count > 0)
	{
		light_cluster_t& cluster = light_clusters[0];
		for (int i = 1; i < count; i++)
		{
			if (light_clusters[i].count() > cluster.count())
			{
				cluster = light_clusters[i];
			}
		}
		angle = round(cluster.angle() * 100);
	}
	else
	{
		angle = 0;
	}
	lightsensors = values;
	on_white = count > 0;
}

void i2c_request_event()
{
	Wire.write((uint8_t)((angle >> 8) & 0xFF));
	Wire.write((uint8_t)(angle & 0xFF));
	Wire.write((uint8_t)on_white);
	Wire.write((uint8_t)(lightsensors >> 16) & 0xFF);
	Wire.write((uint8_t)(lightsensors >> 8) & 0xFF);
	Wire.write((uint8_t)lightsensors & 0xFF);
}

void i2c_recieve_event(int bytes)
{

}
