#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#ifdef BMP180_SENSOR
	#include <BMP180.h>
	#include <Wire.h>
#elif DS18B20_SENSOR
	#include <OneWire.h>
	#include <DallasTemperature.h>
#endif
#include <timer.h>
#include <esp_functions.h>

//----------- DEFINES ----------------------------------------------------------------------


//----------- VARIABLES --------------------------------------------------------------------
#ifdef BMP180_SENSOR
	BMP180 bmp180( BMP180_ULTRAHIGHRES );
#elif DS18B20_SENSOR
	OneWire oneWire( 2 );
	DallasTemperature sensors( &oneWire );
#endif
Timer timer0( 0, 1000 );
float temperature = 0.0;
#ifdef SLEEP_MODE
	uint8_t sec_count = 0;
#endif

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;



#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer webServer;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer webServer;
#endif

char pageBuff[ WEB_PAGE_BUFF_SIZE ];

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
	temperature				= 0.0;

	Serial.begin( 115200 );
#ifdef BMP180_SENSOR
	bmp180.begin();
#elif DS18B20_SENSOR
	sensors.begin();
#endif

	esp::init();

	esp::wifi_STA_init( DEVICE_NAME );

	config.password								= "123-45-678";

	arduino_homekit_setup( &config );
#ifdef SLEEP_MODE
	system_deep_sleep_set_option( 1 );
#endif

// #ifdef __DEV
// 		int reason = ESP.getResetInfoPtr()->reason;

// 		ESP_DEBUG( "RESET [%d]\n", reason );

// 		if( reason == REASON_EXT_SYS_RST ){
// 			homekit_storage_reset();
// 		}
// #endif

	esp::pageBuff = pageBuff;
	esp::addWebServerPages( &webServer, true, true, true );
	webServer.on( "/storageReset", [ webServer ](void){
		homekit_storage_reset();
		ESP.reset();
		webServer.send ( 200, "text/html", "OK" );
	} );
	webServer.begin();
}

//-----------------------------------------------------------------------------------------
void loop()
{
	static uint8_t clients = 0;

	arduino_homekit_loop();

	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();

#ifdef BMP180_SENSOR
		temperature = bmp180.getTemperature();
#elif DS18B20_SENSOR
		sensors.requestTemperatures();
		temperature = sensors.getTempCByIndex( 0 );
#endif
		my_homekit_report( temperature );
		
		if( arduino_homekit_connected_clients_count() ){
			
#ifdef SLEEP_MODE
			if( sec_count++ >= 3 ){
				system_deep_sleep_instant( SLEEP_SECONDS * 1000 );
				sec_count = 0;
			}
#endif
		}

		uint8_t clientsNew = arduino_homekit_connected_clients_count();
		if( clientsNew != clients ){
			ESP_DEBUG( "Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), clientsNew) ;
			clients = clientsNew;
		}
	}

	webServer.handleClient();
}

//-----------------------------------------------------------------------------------------
void my_homekit_report( const double temperature )
{
	cha_current_temperature.value.float_value = temperature;
	ESP_DEBUG( "Temperature: %.2f\n", temperature );
	homekit_characteristic_notify( &cha_current_temperature, cha_current_temperature.value );
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
