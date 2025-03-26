#include <Wire.h>
#include <robot.h>

#include "maths/vector2.h"
#include "maths/vector3.h"
#include "maths/vector4.h"
#include "maths/matrix_4x3.h"
#include "maths/matrix_3x4.h"
#include "maths/matrix_3x3.h"

struct motor_t
{
	uint8_t pin_speed;
	uint8_t pin_dir;
	uint8_t pin_brake;

	void setup()
	{
		pinMode(pin_speed, OUTPUT);
		pinMode(pin_dir, OUTPUT);
		pinMode(pin_brake, OUTPUT);

		analogWrite(pin_speed, 0);
		digitalWrite(pin_dir, LOW);
		digitalWrite(pin_brake, LOW);
	}

	void set_speed(int16_t speed)
	{
		uint16_t s = abs(speed) % 256;
		uint8_t dir = speed < 0 ? LOW : HIGH;

		analogWrite(pin_speed, s);
		digitalWrite(pin_dir, dir);
	}
};

motor_t motor_1 = { PIN_PB1, PIN_PD0, PIN_PC0 };
motor_t motor_2 = { PIN_PD3, PIN_PD1, PIN_PC1 };
motor_t motor_3 = { PIN_PD5, PIN_PD2, PIN_PC2 };
motor_t motor_4 = { PIN_PD6, PIN_PD4, PIN_PC3 };

#define MOTOR_ANGLE_1	45
#define MOTOR_ANGLE_2	135
#define MOTOR_ANGLE_3	225
#define MOTOR_ANGLE_4	315

const float motor_drive_angles[] = {
	((MOTOR_ANGLE_1 + 90) % 360) * DEG_TO_RAD,
	((MOTOR_ANGLE_2 + 90) % 360) * DEG_TO_RAD,
	((MOTOR_ANGLE_3 + 90) % 360) * DEG_TO_RAD,
	((MOTOR_ANGLE_4 + 90) % 360) * DEG_TO_RAD
};

void i2c_request_event();
void i2c_recieve_event(int bytes);
void move_motors(float direction, float rotation, int16_t speed);
void rotate_motors(int16_t speed);
void stop_motors();

robot_state_t state(11, 10);

matrix_4x3 mat_wheels;

void setup()
{
	state.set_state(robot_state_t::booted);

	Wire.begin(MOTOR_I2C_ADDRESS);
	Wire.onRequest(i2c_request_event);
	Wire.onReceive(i2c_recieve_event);

	motor_1.setup();
	motor_2.setup();
	motor_3.setup();
	motor_4.setup();

	matrix_type mat[3][4] = {
		{ cosf(motor_drive_angles[0]), cosf(motor_drive_angles[1]), cosf(motor_drive_angles[2]), cosf(motor_drive_angles[3]) },
		{ sinf(motor_drive_angles[0]), sinf(motor_drive_angles[1]), sinf(motor_drive_angles[2]), sinf(motor_drive_angles[3]) },
		{ 1, 1, 1, 1}
	};
	matrix_3x4 m(mat);
	matrix_4x3 m_t = matrix_3x4::transpose(m);
	matrix_3x3 mat3x3 = m_t * m;
	matrix_3x3 m_inv = matrix_3x3::inverse(mat3x3);
	auto m_near_final = m * m_inv;
	mat_wheels = matrix_3x4::transpose(m_near_final);

	state.set_state(robot_state_t::standby);
}

void loop()
{
	state.update();
}

void i2c_request_event()
{

}

void i2c_recieve_event(int bytes)
{
	uint8_t* buffer = new uint8_t[bytes];
	auto bytes_read = Wire.readBytes(buffer, bytes);

	if (bytes_read == 0)
	{
		delete[] buffer;
		return;
	}

	uint8_t cmd = buffer[0];
	switch (cmd)
	{
	case CMD_MOTOR_MOVE:
	{
		if (bytes_read < 7)
		{
			break;
		}

		int16_t direction = ((buffer[1] & 0xFF) << 8) | (buffer[2] & 0xFF);
		int16_t rotation = ((buffer[3] & 0xFF) << 8) | (buffer[4] & 0xFF);
		int16_t speed = ((buffer[5] & 0xFF) << 8) | (buffer[6] & 0xFF);

		move_motors((float)direction / 100.0f, (float)rotation / 100.0f, speed);
		break;
	}
	case CMD_MOTOR_ROTATE:
	{
		if (bytes_read < 3)
		{
			break;
		}

		int16_t speed = ((buffer[1] & 0xFF) << 8) | (buffer[2] & 0xFF);
		rotate_motors((float)speed);
		break;
	}
	case CMD_MOTOR_STOP:
	{
		stop_motors();
		break;
	}
	}

	delete[] buffer;
}

void move_motors(float direction, float rotation, int16_t speed)
{
	Vector2f dir(direction);
	vector3 vec_dir(dir.GetX(), dir.GetY(), rotation);
	auto motor_speeds = mat_wheels * vec_dir;

	motor_1.set_speed(motor_speeds[0] * speed);
	motor_2.set_speed(motor_speeds[1] * speed);
	motor_3.set_speed(motor_speeds[2] * speed);
	motor_4.set_speed(motor_speeds[3] * speed);
}

void rotate_motors(int16_t speed)
{
	motor_1.set_speed(speed);
	motor_2.set_speed(speed);
	motor_3.set_speed(speed);
	motor_4.set_speed(speed);
}

void stop_motors()
{
	motor_1.set_speed(0);
	motor_2.set_speed(0);
	motor_3.set_speed(0);
	motor_4.set_speed(0);
}
