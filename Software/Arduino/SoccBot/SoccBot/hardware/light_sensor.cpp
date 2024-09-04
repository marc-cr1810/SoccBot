#include "light_sensor.h"

#include <Wire.h>
#include <WireIMXRT.h>

light_sensor_t::light_sensor_t(const uint8_t address)
	: m_address(address)
{
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

void light_sensor_t::update()
{
	if ((uint32_t)m_timer >= I2C_DELAY_MS)
	{
		// u16 : angle
		// u8  : has_ball
		Wire.requestFrom((uint8_t)m_address, (uint8_t)6);
		size_t count = 0;
		if (Wire.available())
		{
			uint8_t bytes[3];
			count = Wire.readBytes(bytes, 6);
			if (count == 6)
			{
				uint16_t angle = ((bytes[0] & 0xFF) << 8) | (bytes[1] & 0xFF);
				m_ball_angle = (float)angle / 100.0f;
				m_ball_detected = bytes[2] != 0;

				Serial.printf("light: %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c\n",
					BYTE_TO_BINARY(bytes[3]), BYTE_TO_BINARY(bytes[4]), BYTE_TO_BINARY(bytes[5]));
			}
		}
		m_timer.restart();
	}
}

bool light_sensor_t::check_connection() const
{
	Wire.beginTransmission(m_address);
	uint8_t result = Wire.endTransmission();
	return result == 0;
}

bool light_sensor_t::ball_detected() const
{
	return m_ball_detected;
}

float light_sensor_t::ball_angle() const
{
	return m_ball_angle;
}
