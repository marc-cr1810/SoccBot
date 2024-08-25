#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "core/platform_detection.h"
#include "core/robot_state.h"

#include "hardware/digital/button.h"
#include "hardware/digital/digital.h"
#include "hardware/digital/digital_pwm.h"
#include "hardware/digital/led.h"
#include "hardware/digital/led_array.h"

#include "utils/timer.h"