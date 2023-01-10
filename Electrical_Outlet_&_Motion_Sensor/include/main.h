#ifndef _MAIN_H_
#define _MAIN_H_


//----------- DEFINES --------------------------------------------------------------------
#define DEVICE_NAME							"ElectricalOutletMotionSensor"
#if defined(ARDUINO_ARCH_ESP8266)
	#define MODEL_NAME						"ESP8266"
#elif defined(ARDUINO_ARCH_ESP32)
	#define MODEL_NAME						"ESP32"
#endif
#define FIRMWARE_VERSION					"1.1."FIRMWARE_REVISION
#define MOTION_PIN							3
#define SWITCH_PIN							2
#define WEB_PAGE_BUFF_SIZE					2048

//----------- INCLUDES --------------------------------------------------------------------
#include <homekit/types.h>

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
// extern HomeKit homeKit;
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];

//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------
void my_accessory_identify(const homekit_value_t _value);
homekit_value_t outletPhysicalState(void);
homekit_value_t outletGet(void);
void outletSet(homekit_value_t value);





#endif //_MAIN_H_
