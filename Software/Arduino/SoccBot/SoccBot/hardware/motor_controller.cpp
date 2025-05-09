#include "motor_controller.h"

#include <Wire.h>
#include <WireIMXRT.h>

motor_controller_t::motor_controller_t(const uint8_t address)
	: m_address(address)
{ }

bool motor_controller_t::check_connection() const
{
	Wire.beginTransmission(m_address);
	uint8_t result = Wire.endTransmission();
	return result == 0;
}

void motor_controller_t::move(float direction, float rotation, int16_t speed)
{
	int16_t send_dir = round(direction * 100);
	int16_t send_rot = round(rotation * 100);

	Wire.beginTransmission(m_address);

	Wire.write(CMD_MOTOR_MOVE);
	Wire.write((uint8_t)((send_dir >> 8) & 0xFF));
	Wire.write((uint8_t)(send_dir & 0xFF));
	Wire.write((uint8_t)((send_rot >> 8) & 0xFF));
	Wire.write((uint8_t)(send_rot & 0xFF));
	Wire.write((uint8_t)((speed >> 8) & 0xFF));
	Wire.write((uint8_t)(speed & 0xFF));

	Wire.endTransmission();
}

void motor_controller_t::rotation(int16_t speed)
{
	Wire.beginTransmission(m_address);

	Wire.write(CMD_MOTOR_ROTATE);
	Wire.write((uint8_t)((speed >> 8) & 0xFF));
	Wire.write((uint8_t)(speed & 0xFF));

	Wire.endTransmission();
}

void motor_controller_t::stop()
{
	Wire.beginTransmission(m_address);
	Wire.write(CMD_MOTOR_STOP);
	Wire.endTransmission();
}
