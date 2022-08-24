#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <DNS_Server.h>
#include <esp_functions.h>
#include <functions.h>
#include <timer.h>

//----------- DEFINES ----------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
Timer timer0( 0, 1000 );
DNS_Server dnsServer;
HomeKit homeKit;
char pageBuff[ WEB_PAGE_BUFF_SIZE ];



extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t outlet_on;
extern "C" homekit_characteristic_t outlet_in_use;
// extern "C" homekit_characteristic_t motion_detected;



#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer webServer;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer webServer;
#endif

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
#ifndef __DEV
	// pinMode( LED_BUILTIN, OUTPUT );
	// digitalWrite( LED_BUILTIN, LOW );
// #else
// 	Serial.begin( 115200 );
#endif
	Serial.begin( 115200 );

	// pinMode( MOTION_PIN, INPUT );
	// attachInterrupt( MOTION_PIN, handleInterrupt, CHANGE );
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
		// homeKit.fillInfoCharacteristics( DEVICE_NAME, "0123456", MODEL_NAME, FIRMWARE_VERSION );

		// cha_name								= swithCharacteristic;
		// cha_switch_on							= swithCharacteristic + 1;

		// cha_switch_on->type						= HOMEKIT_CHARACTERISTIC_ON;
		// cha_switch_on->description				= "On";
		// cha_switch_on->format					= homekit_format_bool;
		// cha_switch_on->value.format				= homekit_format_bool;
		// cha_switch_on->value.bool_value			= false;
		// cha_switch_on->permissions				= (homekit_permissions_t)( homekit_permissions_paired_read | homekit_permissions_paired_write | homekit_permissions_notify );
		// cha_switch_on->setter					= cha_switch_on_setter;

		
		

		// services[ 0 ].id						= 1;
		// services[ 0 ].type						= HOMEKIT_SERVICE_ACCESSORY_INFORMATION;
		// homekit_characteristic_t* pInfoCharacteristics = homeKit.getInfoCharacteristics();
		// services[ 0 ].characteristics			= &pInfoCharacteristics;

		// services[ 1 ].id						= 2;
		// services[ 1 ].type						= HOMEKIT_SERVICE_SWITCH;
		// services[ 1 ].primary					= true;
		// services[ 1 ].characteristics			= &pSwithCharacteristic;

		// accessories[ 0 ].id						= 1;
		// accessories[ 0 ].config_number			= 1;
		// accessories[ 0 ].category				= homekit_accessory_category_switch;
		// accessories[ 0 ].services				= &pServices;

		// config.accessories						= &pAccessories;
		config.password								= "123-45-678";

		// homeKit.setAccessories( *config.accessories );
		arduino_homekit_setup( &config );

		// uint32_t id = homeKit.newAccessorie( homekit_accessory_category_switch );

		// homeKit.setPassword( "880-05-553" );
		// arduino_homekit_setup( homeKit.getConfig() );
	}
}

//-----------------------------------------------------------------------------------------
void loop()
{
	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();
#ifdef __DEV
		if( !esp::flags.ap_mode ){
			ESP_DEBUG( "Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count()) ;
		}
#endif
#ifndef __DEV
		// digitalWrite( LED_BUILTIN, ( esp::isWiFiConnection() ) ? HIGH : LOW );
#endif
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
		webServer.handleClient();
	}else{
		arduino_homekit_loop();
	}

	// bool state = ( digitalRead( MOTION_PIN ) ) ? true : false;
	// if( motion_detected.value.bool_value != state ){
	// 	motion_detected.value = HOMEKIT_BOOL( state );
	// 	homekit_characteristic_notify( &motion_detected, motion_detected.value );
	// }
}

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
