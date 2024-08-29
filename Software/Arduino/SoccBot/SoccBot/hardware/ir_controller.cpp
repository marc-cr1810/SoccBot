#include "ir_controller.h"

#include <Wire.h>
#include <WireIMXRT.h>

ir_controller_t::ir_controller_t(const uint8_t address)
	: m_address(address)
{
}

void ir_controller_t::update()
{
	if ((uint32_t)m_timer >= I2C_DELAY_MS)
	{
		// u16 : angle
		// u8  : has_ball
		Wire.requestFrom((uint8_t)m_address, (uint8_t)3);
		size_t count = 0;
		if (Wire.available())
		{
			uint8_t bytes[3];
			count = Wire.readBytes(bytes, 3);
			if (count == 3)
			{
				uint16_t angle = ((bytes[0] & 0xFF) << 8) | (bytes[1] & 0xFF);
				m_ball_angle = (float)angle / 100.0f;
				m_ball_detected = bytes[2] != 0;
			}
		}
		m_timer.restart();
	}
}

bool ir_controller_t::check_connection() const
{
	Wire.beginTransmission(m_address);
	uint8_t result = Wire.endTransmission();
	return result == 0;
}

bool ir_controller_t::ball_detected() const
{
	return m_ball_detected;
}

float ir_controller_t::ball_angle() const
{
	return m_ball_angle;
}
