#include <Wire.h>
#include <robot.h>

robot_state_t state(11, 10);

void setup()
{
	state.set_state(robot_state_t::booted);

	Wire.begin(LIGHT_I2C_ADDRESS);
	Wire.onRequest(i2c_request_event);
	Wire.onReceive(i2c_recieve_event);

	delay(100);
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

}
