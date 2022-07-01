#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>

#include <timer.h>
#include <esp_functions.h>

//----------- DEFINES ----------------------------------------------------------------------


//----------- VARIABLES --------------------------------------------------------------------
Timer timer0( 0, 1000 );

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
	Serial.begin( 115200 );

	pinMode( SWITCH_PIN, OUTPUT );
	digitalWrite( SWITCH_PIN, LOW );

	esp::init();

	esp::wifi_STA_init( DEVICE_NAME );

	cha_switch_on.value.bool_value = true;
	cha_switch_on.setter = cha_switch_on_setter;
	arduino_homekit_setup( &config );
}

//-----------------------------------------------------------------------------------------
void loop()
{
	arduino_homekit_loop();

	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();

		ESP_DEBUG("Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
}

//-----------------------------------------------------------------------------------------
void cha_switch_on_setter(const homekit_value_t value)
{
	bool on = value.bool_value;
	cha_switch_on.value.bool_value = on; //sync the value
	ESP_DEBUG( "Switch: %s", on ? "ON" : "OFF" );
	digitalWrite( SWITCH_PIN, on ? LOW : HIGH );
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
