#ifndef _MAIN_H_
#define _MAIN_H_


//----------- DEFINES --------------------------------------------------------------------
#define DEVICE_NAME							"Air Conditioner"
#if defined(ARDUINO_ARCH_ESP8266)
	#define MODEL_NAME						"ESP8266"
#elif defined(ARDUINO_ARCH_ESP32)
	#define MODEL_NAME						"ESP32"
#endif
#define FIRMWARE_VERSION					"1.0."FIRMWARE_REVISION
#define CONDITION1_PIN						D2
#define WEB_PAGE_BUFF_SIZE					2048
#define RETRY_COUNT							3

//----------- INCLUDES --------------------------------------------------------------------
#include <homekit/types.h>

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
// extern HomeKit homeKit;
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];

//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------
void my_accessory_identify(const homekit_value_t _value);

#ifdef __cplusplus
extern "C" {
#endif

homekit_value_t getState(void);
homekit_value_t getCurrentState(void);
homekit_value_t getTargetState(void);
homekit_value_t getCurrentTemp(void);
void setTargetHeatingTemp(homekit_value_t value);
void setTargetCoolingTemp(homekit_value_t value);
void setTargetState(homekit_value_t value);
void setState(homekit_value_t value);
void condei(void);

#ifdef __cplusplus
}
#endif




#endif //_MAIN_H_
