#ifndef _MAIN_H_
#define _MAIN_H_


//----------- DEFINES --------------------------------------------------------------------
#define DEVICE_NAME							"RGB LED Panel"
#if defined(ARDUINO_ARCH_ESP8266)
	#define MODEL_NAME						"ESP8266"
#elif defined(ARDUINO_ARCH_ESP32)
	#define MODEL_NAME						"ESP32"
#endif
#define FIRMWARE_VERSION					"1.0."FIRMWARE_REVISION
#define WEB_PAGE_BUFF_SIZE					2048
#define LEDS_COUNT							17
#define START_BRIGHT						15
#define MAX_BRIGHT							100
#define START_HUE							100
#define START_SATURATION					50

//----------- INCLUDES --------------------------------------------------------------------
#include <homekit/types.h>

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];


//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------




#endif //_MAIN_H_
