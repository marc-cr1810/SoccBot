#include "camera.h"

void camera_t::init()
{
	Serial2.begin(115200);
}

void camera_t::update()
{
	if (Serial2.available() == 9)
	{
		uint8_t color = Serial2.read();
		uint16_t width_1 = Serial2.read();
		uint16_t width_2 = Serial2.read();
		uint16_t height_1 = Serial2.read();
		uint16_t height_2 = Serial2.read();
		uint16_t center_x_1 = Serial2.read();
		uint16_t center_x_2 = Serial2.read();
		uint16_t center_y_1 = Serial2.read();
		uint16_t center_y_2 = Serial2.read();

		m_image.width = (width_2 << 8) | width_1;
		m_image.height = (height_2 << 8) | height_1;

		m_goal.center.x = (center_x_2 << 8) | center_x_1;
		m_goal.center.y = (center_y_2 << 8) | center_y_1;
	}
}

image_t camera_t::get_image() const
{
	return m_image;
}

goal_t camera_t::get_goal() const
{
	return m_goal;
}

bool camera_t::has_goal() const
{
	return m_goal.center.x != -1 || m_goal.center.y != -1;
}
