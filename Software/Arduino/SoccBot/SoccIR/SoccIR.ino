#include <Wire.h>
#include <robot.h>

robot_state_t state(11, 10);

analog_multi_t tssp_cluster_1(14, 0, 1, 2);
analog_multi_t tssp_cluster_2(15, 3, 4, 5);
analog_multi_t tssp_cluster_3(16, 6, 7, 8);

bool has_ball = false;
uint8_t angle = 0;

void setup()
{
	state.set_state(robot_state_t::booted);

	Wire.begin(IR_I2C_ADDRESS);
	Wire.onRequest(i2c_request_event);
	Wire.onReceive(i2c_recieve_event);

	delay(100);
	state.set_state(robot_state_t::standby);
}

uint16_t read_ir_sensor(uint8_t index)
{
	uint8_t cluster_index = floor((float)index / 8.0f);
	analog_multi_t* cluster = nullptr;
	switch (cluster_index)
	{
	case 0: cluster = &tssp_cluster_1; break;
	case 1: cluster = &tssp_cluster_2; break;
	case 2: cluster = &tssp_cluster_3; break;
	default: state.set_state(robot_state_t::error); break;
	}

	uint8_t cluster_tssp_index = index - (cluster_index * 8);
	return cluster->read(cluster_tssp_index);
}

void loop()
{
	state.update();
	int min = 24;
	int max = -1;

	for (int i = 0; i < 24; i++)
	{
		bool value = read_ir_sensor(i) < 10;
		if (value)
		{
			has_ball = true;
			angle = i;
			if (min > i)
			{
				min = i;
			}
			if (max < i)
			{
				max = i;
			}
		}
	}

	angle = (max + min) / 2;
}

void i2c_request_event()
{
	Wire.write((uint8_t)0x00);
	Wire.write((uint8_t)angle);
	Wire.write((uint8_t)has_ball);

	has_ball = false;
	angle = 0;
}

void i2c_recieve_event(int bytes)
{
}
