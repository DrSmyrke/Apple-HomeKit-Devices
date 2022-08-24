#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <DNS_Server.h>
#include <esp_functions.h>
#include <functions.h>
#include <timer.h>
#include <AUXHeatpumpIR.h>

//----------- DEFINES ----------------------------------------------------------------------

//----------- VARIABLES --------------------------------------------------------------------
Timer timer0( 0, 1000 );
DNS_Server dnsServer;
// HomeKit homeKit;
char pageBuff[ WEB_PAGE_BUFF_SIZE ];


uint8_t activeFlag;

IRSenderESP8266 irSender( CONDITION1_PIN );
// IRSenderESP8266 irSender2( CONDITION2_PIN );
AUXHeatpumpIR *heatpumpIR;


extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t active_characteristic;
extern "C" homekit_characteristic_t currentTemp_characteristic;
extern "C" homekit_characteristic_t currentState_characteristic;
extern "C" homekit_characteristic_t targetState_characteristic;
extern "C" homekit_characteristic_t targetCoolingTemp_characteristic;
extern "C" homekit_characteristic_t targetHeatingTemp_characteristic;



#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer webServer;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer webServer;
#endif

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
#ifdef __DEV
	Serial.begin( 115200 );
#else
	pinMode( LED_BUILTIN, OUTPUT );
	digitalWrite( LED_BUILTIN, LOW );
#endif

	activeFlag							= 2;
	heatpumpIR							= new AUXHeatpumpIR();

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

		// active_characteristic.setter				= setState;

		arduino_homekit_setup( &config );

		// uint32_t id = homeKit.newAccessorie( homekit_accessory_category_switch );

		// homeKit.setPassword( "880-05-553" );
		// arduino_homekit_setup( homeKit.getConfig() );
	}

	ESP_DEBUG( "INIT OK\n" );
}

//-----------------------------------------------------------------------------------------
void loop()
{
	static float currentTemp = 0.0;
	static uint8_t state = 255;
	static uint8_t clients = 0;
	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();
#ifdef __DEV
		if( !esp::flags.ap_mode ){
			uint8_t clientsNew = arduino_homekit_connected_clients_count();
			if( clientsNew != clients ){
				ESP_DEBUG( "Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), clientsNew) ;
				clients = clientsNew;
			}
			
		}
#endif
#ifndef __DEV
		// digitalWrite( LED_BUILTIN, ( esp::isWiFiConnection() ) ? HIGH : LOW );
#endif
		if( activeFlag ){
			activeFlag--;
			condei();
		}

		if( currentTemp != currentTemp_characteristic.value.float_value ){
			homekit_characteristic_notify( &currentTemp_characteristic, currentTemp_characteristic.value );
			currentTemp = currentTemp_characteristic.value.float_value;
		}
		if( state != currentState_characteristic.value.uint8_value ){
			homekit_characteristic_notify( &currentState_characteristic, currentState_characteristic.value );
			state = currentState_characteristic.value.uint8_value;
		}
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
		webServer.handleClient();
	}else{
		arduino_homekit_loop();
	}
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void condei(void)
{
#ifndef __DEV
	digitalWrite( LED_BUILTIN, ( active_characteristic.value.uint8_value > 0 ) ? HIGH : LOW );
#endif
	uint8_t power = ( active_characteristic.value.uint8_value > 0 ) ? POWER_ON : POWER_OFF;
	uint8_t acmode = MODE_COOL;
	uint8_t fan = FAN_2;
	uint8_t temp = (uint8_t)currentTemp_characteristic.value.float_value;
	switch( targetState_characteristic.value.uint8_value ){
		case 1:
			acmode = MODE_HEAT;
			temp = (uint8_t)targetHeatingTemp_characteristic.value.float_value;
		break;
		case 2:
			acmode = MODE_COOL;
			temp = (uint8_t)targetCoolingTemp_characteristic.value.float_value;
		break;
		case 0:
		default:
			acmode = MODE_AUTO;
			temp = 24;
		break;
	}
	
	heatpumpIR->send( irSender, power, acmode, fan, temp, VDIR_AUTO, 0 );
}

//-----------------------------------------------------------------------------------------
void setTargetHeatingTemp(homekit_value_t value)
{
	if( value.format != homekit_format_float ) {
		ESP_DEBUG( "Invalid on-value format: %d\n", value.format );
		return;
	}

	currentTemp_characteristic.value = value;
	ESP_DEBUG( "Set heating temp: %f\n", value.float_value );

	activeFlag = RETRY_COUNT;
}

//-----------------------------------------------------------------------------------------
void setTargetCoolingTemp(homekit_value_t value)
{
	if( value.format != homekit_format_float ) {
		ESP_DEBUG( "Invalid on-value format: %d\n", value.format );
		return;
	}

	currentTemp_characteristic.value = value;
	ESP_DEBUG( "Set cooling temp: %f\n", value.float_value );

	activeFlag = RETRY_COUNT;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void setTargetState(homekit_value_t value)
{
	if( value.format != homekit_format_uint8 ) {
		ESP_DEBUG( "Invalid on-value format: %d\n", value.format );
		return;
	}

	targetState_characteristic.value = value;
	ESP_DEBUG( "Set target state: %d\n", value.uint8_value );

	switch( targetState_characteristic.value.uint8_value ){
		case 0:
			//OFF
			active_characteristic.value = HOMEKIT_UINT8( 0 );
			currentState_characteristic.value  = HOMEKIT_UINT8( 1 );
		break;
		case 1:
			//HEAT
			active_characteristic.value = HOMEKIT_UINT8( 1 );
			currentTemp_characteristic.value  = HOMEKIT_FLOAT( 22.0 );
			targetHeatingTemp_characteristic.value  = HOMEKIT_FLOAT( 23.0 );
			currentState_characteristic.value  = HOMEKIT_UINT8( 2 );
		break;
		case 2:
			//COOL
			active_characteristic.value = HOMEKIT_UINT8( 1 );
			currentTemp_characteristic.value  = HOMEKIT_FLOAT( 26.0 );
			targetCoolingTemp_characteristic.value  = HOMEKIT_FLOAT( 25.0 );
			currentState_characteristic.value  = HOMEKIT_UINT8( 3 );
		break;
		case 3:
			//AUTO
			active_characteristic.value = HOMEKIT_UINT8( 1 );
			currentTemp_characteristic.value  = HOMEKIT_FLOAT( 14.0 );
			currentState_characteristic.value  = HOMEKIT_UINT8( 2 );
		break;
	}

	activeFlag = RETRY_COUNT;
}

//-----------------------------------------------------------------------------------------
void setState(homekit_value_t value)
{
	if( value.format != homekit_format_uint8 ) {
		ESP_DEBUG( "Invalid on-value format: %d\n", value.format );
		return;
	}

	// if( active_characteristic.value.uint8_value != value.uint8_value ){
	// 	if( value.uint8_value && targetState_characteristic.value.uint8_value == 0 ) setTargetState( HOMEKIT_UINT8( 2 ) );
	// 	activeFlag = 2;
	// }

	active_characteristic.value = value;
	ESP_DEBUG( "Switch: %s\n", value.uint8_value ? "ON" : "OFF" );

	setTargetState( HOMEKIT_UINT8( ( value.uint8_value ) ? 2 : 0 ) );
}

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
