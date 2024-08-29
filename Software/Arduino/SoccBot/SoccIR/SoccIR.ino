#include <Wire.h>
#include <robot.h>

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

uint16_t tssp_angles[24] = {
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
}

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
