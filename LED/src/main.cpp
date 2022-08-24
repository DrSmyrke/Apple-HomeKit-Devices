#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <DNS_Server.h>
#include <timer.h>
#include <esp_functions.h>

//----------- DEFINES ----------------------------------------------------------------------


//----------- VARIABLES --------------------------------------------------------------------
Timer timer0( 0, 1000 );
DNS_Server dnsServer;
// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" void occupancy_toggle();
extern "C" void led_toggle();
extern "C" void accessory_init();

#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer webServer;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer webServer;
#endif
char pageBuff[ WEB_PAGE_BUFF_SIZE ];
//----------- FUNCTIONS--------------------------------------------------------------------


//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
	Serial.begin( 115200 );

	pinMode( SWITCH_PIN, OUTPUT );
	digitalWrite( SWITCH_PIN, LOW );

	esp::init();

	IPAddress apIP( 10, 10, 10, 10 );
	esp::wifi_init( DEVICE_NAME, apIP, apIP, IPAddress( 255, 255, 255, 0 ) );
	if( esp::flags.ap_mode ){
		dnsServer.setErrorReplyCode( DNSReplyCode::NoError );
		dnsServer.addRecord( "*", WiFi.softAPIP() );
		dnsServer.start( DNS_PORT );

		esp::pageBuff = pageBuff;
		esp::addWebServerPages( &webServer, true, true, true );
		webServer.begin();
	}else{
		arduino_homekit_setup( &config );
	}
}

//-----------------------------------------------------------------------------------------
void loop()
{
	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();

		ESP_DEBUG("Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
	}else{
		arduino_homekit_loop();
	}
}

//-----------------------------------------------------------------------------------------
void cha_switch_on_setter(const homekit_value_t value)
{
	// bool on = value.bool_value;
	// cha_switch_on.value.bool_value = on; //sync the value
	// ESP_DEBUG( "Switch: %s", on ? "ON" : "OFF" );
	// digitalWrite( SWITCH_PIN, on ? LOW : HIGH );
}

//-----------------------------------------------------------------------------------------
void cha_switch_on_setter2(const homekit_value_t value)
{
	// bool on = value.bool_value;
	// cha_switch_on.value.bool_value = on; //sync the value
	// ESP_DEBUG( "Switch2: %s", on ? "ON" : "OFF" );
	// digitalWrite( SWITCH_PIN, on ? LOW : HIGH );
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
