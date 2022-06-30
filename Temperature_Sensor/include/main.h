#ifndef _MAIN_H_
#define _MAIN_H_


//----------- DEFINES --------------------------------------------------------------------
#define DEVICE_NAME							"BMP180_Temperature_Sensor"
// #define MODEL_NAME							"ESP8266/ESP32"
#if defined(ARDUINO_ARCH_ESP8266)
	#define MODEL_NAME						"ESP8266"
#elif defined(ARDUINO_ARCH_ESP32)
	#define MODEL_NAME						"ESP32"
#endif
#define SLEEP_SECONDS						900
#define FIRMWARE_VERSION					"1.0."
#define NAME_BUFF_SIZE						14

//----------- INCLUDES --------------------------------------------------------------------
// #include <esp_functions.h>

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
extern char firmvare_version[ NAME_BUFF_SIZE ];
extern char model_name[ NAME_BUFF_SIZE ];

//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------
void my_homekit_report(const double temperature);




#endif //_MAIN_H_
