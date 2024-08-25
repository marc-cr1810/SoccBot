#pragma once

#ifdef ARDUINO
	#ifdef ARDUINO_ARCH_AVR
		#ifdef ARDUINO_AVR_MEGA2560
			#define ARDUINO_MEGA2560
		#endif
	#elif TEENSYDUINO
		#ifdef ARDUINO_TEENSY40
			#define TEENSY_4_0
		#endif
	#endif
#endif