#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#ifdef BMP180_SENSOR
	#include <BMP180.h>
	#include <Wire.h>
#endif
#include <timer.h>
#include <esp_functions.h>

//----------- DEFINES ----------------------------------------------------------------------


//----------- VARIABLES --------------------------------------------------------------------
char firmvare_version[ NAME_BUFF_SIZE ];
char model_name[ NAME_BUFF_SIZE ];
#ifdef BMP180_SENSOR
	BMP180 bmp180( BMP180_ULTRAHIGHRES );
#endif
Timer timer0( 0, 1000 );

#ifdef SLEEP_MODE
	uint8_t sec_count = 0;
#endif

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
	Serial.begin( 115200 );
#ifdef BMP180_SENSOR
	bmp180.begin();
#endif

	strcpy( firmvare_version, FIRMWARE_VERSION );
	strcat( firmvare_version, FIRMWARE_REVISION );

	strcpy( model_name, MODEL_NAME );
#ifdef BMP180_SENSOR
	strcat( model_name, "/BMP180" );
#else if DS18B20_SENSOR
	strcat( model_name, "/DS18B20" );
#endif

	esp::init();

	esp::wifi_STA_init( DEVICE_NAME );

	arduino_homekit_setup( &config );
#ifdef SLEEP_MODE
	system_deep_sleep_set_option( 1 );
#endif
}

//-----------------------------------------------------------------------------------------
void loop()
{
	arduino_homekit_loop();

	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();

		if( arduino_homekit_connected_clients_count() ){
#ifdef BMP180_SENSOR
			my_homekit_report( bmp180.getTemperature() );
#endif
#ifdef SLEEP_MODE
			if( sec_count++ >= 10 ){
				system_deep_sleep_instant( SLEEP_SECONDS * 1000 );
				sec_count = 0;
			}
#endif
		}

		ESP_DEBUG("Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
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
