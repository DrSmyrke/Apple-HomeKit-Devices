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
#include <homekit.h>
#if defined(ARDUINO_ARCH_ESP8266)
	#include <LittleFS.h>
#elif defined(ARDUINO_ARCH_ESP32)
	#include <SPIFFS.h>
#endif

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
extern HomeKit homeKit;
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];


//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------
void setRGB(const uint8_t r, const uint8_t g, const uint8_t b);
void setBrightness(const uint8_t value);
void setLamp(const uint8_t value);
void setLed(const uint8_t ledNum, uint8_t r, uint8_t g, uint8_t b);
void hsbToRgb(void);
homekit_value_t lamp_on_getter(void);
void lamp_on_setter(const homekit_value_t value);
homekit_value_t lamp_bright_getter(void);
void lamp_bright_setter(const homekit_value_t value);
homekit_value_t lamp_hue_getter(void);
void lamp_hue_setter(const homekit_value_t value);
homekit_value_t lamp_sat_getter(void);
void lamp_sat_setter(const homekit_value_t value);
void loadSettings(void);
void saveSettings(void);


#endif //_MAIN_H_
