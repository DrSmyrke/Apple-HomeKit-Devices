#ifndef _MAIN_H_
#define _MAIN_H_


//----------- DEFINES --------------------------------------------------------------------
#define DEVICE_NAME							"RGB_LED_Panel"
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
#define CONFIG_FILE							"settings.conf"

//----------- INCLUDES --------------------------------------------------------------------
#include <homekit/types.h>

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];


//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------
void indexPageHeadler(void);
void getPageHeadler(void);
void timer0Interrupt(void*);
void saveSettings(void);
void loadSettings(void);

#endif //_MAIN_H_
