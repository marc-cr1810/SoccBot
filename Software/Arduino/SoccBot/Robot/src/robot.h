#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "core/platform_detection.h"
#include "core/robot_state.h"

#include "hardware/analog/analog.h"
#include "hardware/analog/analog_multi.h"
#include "hardware/digital/button.h"
#include "hardware/digital/digital.h"
#include "hardware/digital/digital_multi.h"
#include "hardware/digital/digital_pwm.h"
#include "hardware/digital/led.h"
#include "hardware/digital/led_array.h"
#include "hardware/digital/switch.h"
#include "hardware/digital/switch_array.h"

#include "utils/timer.h"

#define IR_I2C_ADDRESS		0b10101000
#define MOTOR_I2C_ADDRESS	0b10101001
#define LIGHT_I2C_ADDRESS	0b10101010

#define I2C_DELAY_MS		1