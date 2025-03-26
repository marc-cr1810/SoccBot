#pragma once

#include <robot.h>

struct image_t
{
	uint16_t width;
	uint16_t height;
};

struct center_t
{
	int16_t x;
	int16_t y;
};

struct goal_t
{
	enum color_e
	{
		yellow,
		blue
	};

	center_t center;
};

class camera_t
{
public:
	void init();
	void update();

	image_t get_image() const;
	goal_t get_goal() const;
	bool has_goal() const;
private:
	image_t m_image;
	goal_t m_goal;
};