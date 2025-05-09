#pragma once

#include <robot.h>

class motor_controller_t
{
public:
	motor_controller_t(const uint8_t address);

	bool check_connection() const;

	void move(float direction, float rotation, int16_t speed);
	void rotation(int16_t speed);
	void stop();
private:
	const uint8_t m_address;
};