#ifndef _MAIN_H_
#define _MAIN_H_


//----------- DEFINES --------------------------------------------------------------------
#if defined( DS18B20_SENSOR )
	#define SENSOR_TYPE						"DS18B20"
#elif defined( BMP180_SENSOR )
	#define SENSOR_TYPE						"BMP180"
#else
	#define SENSOR_TYPE						"NONE"
#endif
#define DEVICE_NAME							SENSOR_TYPE"_Temperature_Sensor"
#if defined(ARDUINO_ARCH_ESP8266)
	#define MODEL_NAME						"ESP8266/"SENSOR_TYPE
#elif defined(ARDUINO_ARCH_ESP32)
	#define MODEL_NAME						"ESP32/"SENSOR_TYPE
#endif
#define SLEEP_SECONDS						90000
#define WEB_PAGE_BUFF_SIZE					2048
#define FIRMWARE_VERSION					"1.0."FIRMWARE_REVISION

//----------- INCLUDES --------------------------------------------------------------------

//----------- STRUCTIRES -------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
extern char pageBuff[ WEB_PAGE_BUFF_SIZE ];
//----------- INCLUDES ---------------------------------------------------------------------

//----------- FUNCTIONS---------------------------------------------------------------------
void my_homekit_report(const double temperature);




#endif //_MAIN_H_
