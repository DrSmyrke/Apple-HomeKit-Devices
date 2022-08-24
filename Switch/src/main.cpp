#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <DNS_Server.h>
#include <esp_functions.h>
#include <functions.h>
#include <timer.h>
#include <ArduinoOTA.h>

//----------- DEFINES ----------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
Timer timer0( 0, 1000 );
DNS_Server dnsServer;
HomeKit homeKit;
char pageBuff[ WEB_PAGE_BUFF_SIZE ];

extern "C" homekit_server_config_t config;
// homekit_characteristic_t* cha_name;
// homekit_characteristic_t* cha_switch_on;
// homekit_service_t services[ 2 ];
// homekit_service_t* pServices = services;

// homekit_characteristic_t swithCharacteristic[ 2 ];
// homekit_characteristic_t* pSwithCharacteristic;
// extern "C" homekit_characteristic_t occupancy_detected;
extern "C" homekit_characteristic_t cha_name;
extern "C" homekit_characteristic_t cha_switch_on;
#ifdef __MOTION_SENSOR
	extern "C" homekit_characteristic_t motion_detected;
#endif

// homekit_accessory_t accessories[ 1 ];
// homekit_accessory_t *pAccessories = accessories;
#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer webServer;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer webServer;
#endif

//----------- FUNCTIONS--------------------------------------------------------------------
// void occupancy_toggle() {
// 	const uint8_t state = occupancy_detected.value.uint8_value;
// 	occupancy_detected.value = HOMEKIT_UINT8((!state) ? 1 : 0);
// 	homekit_characteristic_notify(&occupancy_detected, occupancy_detected.value);
// }

void DNS_request(const char* request, const uint8_t* ip)
{
#ifdef __DEV
	Serial.print( "DNS: " );
	Serial.print( request );
	if( ip != nullptr ){
		Serial.print( " [" );
		for( uint8_t i = 0; i < 4; i++ ){
			if( i > 0 ) Serial.print( '.' );
			Serial.print( ip[ i ] );
		}
		Serial.print( "]" );
	}
	Serial.println();
#endif
}

#ifdef __MOTION_SENSOR
void handleInterrupt(void)
{
	motion_detected.value = HOMEKIT_UINT8( ( digitalRead( MOTION_PIN ) ) ? 1 : 0 );
	homekit_characteristic_notify( &motion_detected, motion_detected.value );
}
#endif

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
#ifndef __DEV
	pinMode( LED_BUILTIN, OUTPUT );
	digitalWrite( LED_BUILTIN, LOW );
#else
	Serial.begin( 115200 );
#endif

#ifdef __MOTION_SENSOR
	pinMode( MOTION_PIN, INPUT );
	attachInterrupt( MOTION_PIN, handleInterrupt, CHANGE );
#endif
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

		dnsServer.newRequest( DNS_request );
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

		// homeKit.setAccessories( *config.accessories );
		arduino_homekit_setup( &config );

		// uint32_t id = homeKit.newAccessorie( homekit_accessory_category_switch );

		// homeKit.setPassword( "880-05-553" );
		// arduino_homekit_setup( homeKit.getConfig() );
	}

	ArduinoOTA.begin();
}

//-----------------------------------------------------------------------------------------
void loop()
{
	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();

		if( !esp::flags.ap_mode ){
			ESP_DEBUG( "Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count()) ;
		}
#ifndef __DEV
		digitalWrite( LED_BUILTIN, ( esp::isWiFiConnection() ) ? LOW : HIGH );
#endif
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
		webServer.handleClient();
	}else{
		arduino_homekit_loop();
	}

	ArduinoOTA.handle();
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
void my_accessory_identify(const homekit_value_t _value)
{
	ESP_DEBUG( "accessory identify\n" );
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
