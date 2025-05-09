#pragma once

#include <robot.h>

class ir_controller_t
{
public:
	ir_controller_t(const uint8_t address);

	void update();
	bool check_connection() const;

	bool ball_detected() const;
	float ball_angle() const;
private:
	const uint8_t m_address;
	bool m_ball_detected = false;
	float m_ball_angle = 0.0f;
	robot_timer_t m_timer;
};