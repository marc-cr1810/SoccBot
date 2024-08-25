#include "robot_state.h"

robot_state_t::robot_state_t(uint8_t red_led_pin, uint8_t green_led_pin)
	: red_led(red_led_pin), green_led(green_led_pin)
{ }

void robot_state_t::update()
{

}

void robot_state_t::set_state(state_e state)
{
}
