#include <robot.h>

robot_state_t state(11, 10);

void setup()
{
	state.set_state(robot_state_t::booted);
	delay(100);
	state.set_state(robot_state_t::standby);
}

void loop()
{
	state.update();
}
