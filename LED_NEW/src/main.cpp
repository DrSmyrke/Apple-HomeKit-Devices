#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <DNS_Server.h>
#include <timer.h>
#include <esp_functions.h>
#include <Adafruit_NeoPixel.h>

//----------- DEFINES ----------------------------------------------------------------------


//----------- VARIABLES --------------------------------------------------------------------
#ifdef _ESP01
	Adafruit_NeoPixel pixels( LEDS_COUNT, 1, NEO_GRB + NEO_KHZ800 );
#else
	Adafruit_NeoPixel pixels( LEDS_COUNT, D5, NEO_GRB + NEO_KHZ800 );
#endif
uint8_t fadeValue = 0;
uint8_t fadeMode = 1;
HomeKit homeKit;
uint8_t msecondCounter = 0;
uint8_t secondCounter = 0;
bool m_saveSettingsFlag = false;

Timer timer0( 0, 25 );
DNS_Server dnsServer;
uint8_t rgb[ 3 ];

homekit_characteristic_t lamp_on;
homekit_characteristic_t brightness;
homekit_characteristic_t saturation;
homekit_characteristic_t hue;


#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer webServer;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer webServer;
#endif
char pageBuff[ WEB_PAGE_BUFF_SIZE ];

//----------- FUNCTIONS--------------------------------------------------------------------
void setLed(const uint8_t ledNum, uint8_t r, uint8_t g, uint8_t b)
{
	if( ledNum >= LEDS_COUNT ) return;
	pixels.setPixelColor( ledNum, pixels.Color( r, g, b ) );
}

//----------- FUNCTIONS--------------------------------------------------------------------
void setup()
{
	ESP.wdtEnable( 1000 );

// #ifdef __DEV
	Serial.begin( 115200 );
// #endif

	pixels.begin();
	pixels.clear();
	pixels.show();
	setLed( 0, 0xFF, 0, 0 ); pixels.show(); delay( 100 );
	setLed( 1, 0xFF, 0x35, 0 ); pixels.show(); delay( 100 );
	setLed( 2, 0xFF, 0xFF, 0 ); pixels.show(); delay( 100 );
	setLed( 3, 0, 0x80, 0 ); pixels.show(); delay( 100 );
	setLed( 4, 0, 0, 0xFF ); pixels.show(); delay( 100 );
	setLed( 5, 0x4B, 0, 0x82 ); pixels.show(); delay( 100 );
	setLed( 6, 0xEE, 0x82, 0xEE ); pixels.show(); delay( 100 );
	setLed( 7, 0xFE, 0xFE, 0xFE ); pixels.show(); delay( 100 );
	setLed( 8, 0xFF, 0, 0 ); pixels.show(); delay( 100 );
	setLed( 9, 0xFF, 0x35, 0 ); pixels.show(); delay( 100 );
	setLed( 10, 0xFF, 0xFF, 0 ); pixels.show(); delay( 100 );
	setLed( 11, 0, 0x80, 0 ); pixels.show(); delay( 100 );
	setLed( 12, 0, 0, 0xFF ); pixels.show(); delay( 100 );
	setLed( 13, 0x4B, 0, 0x82 ); pixels.show(); delay( 100 );
	setLed( 14, 0xEE, 0x82, 0xEE ); pixels.show(); delay( 100 );
	setLed( 15, 0xFE, 0xFE, 0xFE ); pixels.show(); delay( 100 );
	setLed( 16, 0xFF, 0, 0 ); pixels.show(); delay( 100 );
	delay( 500 );
	// pixels.clear(); pixels.show();

	loadSettings();
	setBrightness( START_BRIGHT );
	hsbToRgb();
	setRGB( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
	setLamp( lamp_on.value.bool_value );



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
		/*
		homeKit.setPassword( "123-45-678" );
		
		auto assessory = homeKit.newAccessory( homekit_accessory_category_lightbulb, 2 );
		homeKit.fillInfoCharacteristics( assessory->services[ 0 ], DEVICE_NAME, "0123456", MODEL_NAME, FIRMWARE_VERSION );
		homeKit.initService( assessory->services[ 1 ], HOMEKIT_SERVICE_LIGHTBULB, 5 );
		homeKit.initCharacteristic_Name( assessory->services[ 1 ]->characteristics[ 0 ], DEVICE_NAME );

		// homeKit.initCharacteristic_ON( assessory->services[ 1 ]->characteristics[ 1 ], HOMEKIT_BOOL( false ) );
		// homeKit.setCharacteristicGetter( assessory->services[ 1 ]->characteristics[ 1 ], lamp_on_getter );
		// homeKit.setCharacteristicSetter( assessory->services[ 1 ]->characteristics[ 1 ], lamp_on_setter );

		homeKit.initCharacteristic_ON( &lamp_on, lamp_on.value );
		homeKit.initCharacteristic_Brightness( &brightness, brightness.value );
		homeKit.initCharacteristic_HUE( &hue, brightness.value );
		homeKit.initCharacteristic_Saturation( &saturation, saturation.value );
		lamp_on.getter											= lamp_on_getter;
		lamp_on.setter											= lamp_on_setter;
		brightness.getter										= lamp_bright_getter;
		brightness.setter										= lamp_bright_setter;
		hue.getter												= lamp_hue_getter;
		hue.setter												= lamp_hue_setter;
		saturation.getter										= lamp_sat_getter;
		saturation.setter										= lamp_sat_setter;
		assessory->services[ 1 ]->characteristics[ 1 ]			= &lamp_on;
		assessory->services[ 1 ]->characteristics[ 2 ]			= &brightness;
		assessory->services[ 1 ]->characteristics[ 3 ]			= &hue;
		assessory->services[ 1 ]->characteristics[ 4 ]			= &saturation;
		// homeKit.setCharacteristicGetter( brightness, lamp_bright_getter );
		// homeKit.setCharacteristicSetter( assessory->services[ 1 ]->characteristics[ 2 ], lamp_bright_setter );

		// homeKit.initCharacteristic_HUE( assessory->services[ 1 ]->characteristics[ 3 ], HOMEKIT_INT( START_HUE ) );
		// homeKit.setCharacteristicGetter( assessory->services[ 1 ]->characteristics[ 3 ], lamp_hue_getter );
		// homeKit.setCharacteristicSetter( assessory->services[ 1 ]->characteristics[ 3 ], lamp_hue_setter );

		// homeKit.initCharacteristic_Saturation( assessory->services[ 1 ]->characteristics[ 4 ], HOMEKIT_INT( START_SATURATION ) );
		// homeKit.setCharacteristicGetter( assessory->services[ 1 ]->characteristics[ 4 ], lamp_sat_getter );
		// homeKit.setCharacteristicSetter( assessory->services[ 1 ]->characteristics[ 4 ], lamp_sat_setter );

		arduino_homekit_setup( homeKit.getConfig() );

		homekit_characteristic_notify( &lamp_on, lamp_on.value );
		homekit_characteristic_notify( &brightness, brightness.value );
		homekit_characteristic_notify( &hue, saturation.value );
		homekit_characteristic_notify( &saturation, saturation.value );
		*/
	}
}

//-----------------------------------------------------------------------------------------
void loop()
{
	ESP.wdtFeed();

	if( timer0.isInterrupt() ){
		timer0.confirmInerrupt();
// #ifdef __DEV
// 		ESP_DEBUG( "Free heap: %d, HomeKit clients: %d [%d]\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count(), fadeValue );
// #endif
		if( brightness.value.int_value == 73 ){
			if( fadeMode ){
				fadeValue -= 3;
				if( fadeValue <= 5 ){
					setRGB( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
					fadeMode = 0;
				}
			}else{
				fadeValue += 3;
				if( fadeValue >= 250 ) fadeMode = 1;
			}
			pixels.setBrightness( fadeValue ); pixels.show();
		}

		if( ++msecondCounter >= 40 ){
			msecondCounter = 0;
			if( ++secondCounter >= 15 ){
				secondCounter = 0;
				//TODO: save settings
				if( m_saveSettingsFlag ){
					m_saveSettingsFlag = false;
					saveSettings();
				}
			}
		}
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
	}else{
		// arduino_homekit_loop();
	}

	webServer.handleClient();
}

//-----------------------------------------------------------------------------------------
void setRGB(const uint8_t r, const uint8_t g, const uint8_t b)
{
	for( uint8_t i = 0; i < LEDS_COUNT; i++ ){
		setLed( i, r, g, b );
	}

	pixels.show();
}

//-----------------------------------------------------------------------------------------
void setBrightness(const uint8_t value)
{
	if( value == 37 ){
		ESP.wdtFeed();
		pixels.setBrightness( 0xFF );
		setRGB( 0xFF, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0xFF, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0xFF, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0xFF, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0xFF, 0, 0 ); delay( 500 ); ESP.wdtFeed();
		setRGB( 0, 0, 0 ); delay( 500 ); ESP.wdtFeed();
	}else if( value == 73 ){
		fadeValue = value;
	}

	pixels.setBrightness( map( value, 0, 100, 0, 255 ) );
	pixels.show();
}

//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
void setLamp(const uint8_t value)
{
	printf( "LAMP [%s][%d]\n", ( value )? "true" : "false", brightness.value.int_value );

	// //HUE at RGB
	// uint8_t rgb[ 3 ];
	// uint8_t shift = 0;
	// if( hue > 240 ){
	// 	shift = ( hue - 240 ) * 3;
	// 	rgb[ 0 ] = shift;
	// 	rgb[ 1 ] = 0;
	// 	rgb[ 2 ] = ~shift;
	// }else if( hue > 120 ){
	// 	shift = ( hue - 120 ) * 3;
	// 	rgb[ 0 ] = 0;
	// 	rgb[ 1 ] = ~shift;
	// 	rgb[ 2 ] = shift;
	// }else{
	// 	shift = ( hue ) * 3;
	// 	rgb[ 0 ] = ~shift;
	// 	rgb[ 1 ] = shift;
	// 	rgb[ 2 ] = 0;
	// }

	if( value ){
		// for( uint8_t i = 0; i < LEDS_COUNT; i++ ){
		// 	setLed( i, rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
		// }
		
		setBrightness( brightness.value.int_value );
	}else{
		// pixels.clear();
		pixels.setBrightness( 0 );
	}

	hsbToRgb();
}

//-----------------------------------------------------------------------------------------
void hsbToRgb(void)
{
	uint16 s = saturation.value.int_value * 255 / 100;
	uint16 v = brightness.value.int_value * 255 / MAX_BRIGHT;
	uint16 h = (uint16)hue.value.float_value;
	if (s == 0) {
		rgb[ 0 ] = rgb[ 1 ] = rgb[ 2 ] = v;
	} else {
		uint16 t1 = v;
		uint16 t2 = (255 - s) * v / 255;
		uint16 t3 = (t1 - t2) * (h % 60) / 60;
		if (h == 360) h = 0;
		if (h < 60) {
			rgb[ 0 ] = t1;
			rgb[ 2 ] = t2;
			rgb[ 1 ] = t2 + t3;
		}else if (h < 120) {
			rgb[ 1 ] = t1;
			rgb[ 2 ] = t2;
			rgb[ 0 ] = t1 - t3;
		}else if (h < 180) {
			rgb[ 1 ] = t1;
			rgb[ 0 ] = t2;
			rgb[ 2 ] = t2 + t3;
		}else if (h < 240) {
			rgb[ 2 ] = t1;
			rgb[ 0 ] = t2;
			rgb[ 1 ] = t1 - t3;
		}else if (h < 300) {
			rgb[ 2 ] = t1;
			rgb[ 1 ] = t2;
			rgb[ 0 ] = t2 + t3;
		}else if (h < 360) {
			rgb[ 0 ] = t1;
			rgb[ 1 ] = t2;
			rgb[ 2 ] = t1 - t3;
		}else{
			rgb[ 0 ] = 0;
			rgb[ 1 ] = 0;
			rgb[ 2 ] = 0;
		}
	}

	setRGB( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
	// return {r: Math.round(rgb.r), g: Math.round(rgb.g), b: Math.round(rgb.b)};
}

//-----------------------------------------------------------------------------------------
void lamp_bright_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_int ) return;
	brightness.value = value;
	setBrightness( brightness.value.int_value );
	m_saveSettingsFlag = true;
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_bright_getter(void)
{
	return brightness.value;
}

//-----------------------------------------------------------------------------------------
void lamp_on_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_bool ) return;
	setLamp( value.bool_value );
	lamp_on.value = value;
	m_saveSettingsFlag = true;
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_on_getter(void)
{
	return lamp_on.value;
}

//-----------------------------------------------------------------------------------------
void lamp_hue_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_float ) return;
	hue.value = value;
	hsbToRgb();

	// //HUE at RGB
	// uint8_t shift = 0;
	// if( hue > 240 ){
	// 	shift = ( hue - 240 ) * 3;
	// 	rgb[ 0 ] = shift;
	// 	rgb[ 1 ] = 0;
	// 	rgb[ 2 ] = ~shift;
	// }else if( hue > 120 ){
	// 	shift = ( hue - 120 ) * 3;
	// 	rgb[ 0 ] = 0;
	// 	rgb[ 1 ] = ~shift;
	// 	rgb[ 2 ] = shift;
	// }else{
	// 	shift = ( hue ) * 3;
	// 	rgb[ 0 ] = ~shift;
	// 	rgb[ 1 ] = shift;
	// 	rgb[ 2 ] = 0;
	// }
	m_saveSettingsFlag = true;
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_hue_getter(void)
{
	// return HOMEKIT_FLOAT( hue );
	return hue.value;
}

//-----------------------------------------------------------------------------------------
void lamp_sat_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_float ) return;
	saturation.value = value;
	hsbToRgb();
	m_saveSettingsFlag = true;
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_sat_getter(void)
{
	return saturation.value;
}

//-----------------------------------------------------------------------------------------
void loadSettings(void)
{
	if( esp::isFileExists( CONFIG_FILE ) ){
#if defined(ARDUINO_ARCH_ESP8266)
		File f = LittleFS.open( CONFIG_FILE, "r");
#elif defined(ARDUINO_ARCH_ESP32)
		File f = SPIFFS.open( CONFIG_FILE, "r");
#endif
		if( f ){
			uint8_t data[ 5 ];
			if( f.available() == 5 ){
				f.readBytes( (char*)data, sizeof( data ) );
			}
			f.close();

			lamp_on.value.bool_value				= ( data[ 0 ] ) ? true : false;
			brightness.value.int_value				= data[ 1 ];
			saturation.value.int_value				= data[ 2 ];
			hue.value.float_value					= ( data[ 3 ] << 8 ) | ( data[ 4 ] );
		}
	}
}

//-----------------------------------------------------------------------------------------
void saveSettings(void)
{
	if( esp::isFileExists( CONFIG_FILE ) ){
#if defined(ARDUINO_ARCH_ESP8266)
		File f = LittleFS.open( CONFIG_FILE, "w");
#elif defined(ARDUINO_ARCH_ESP32)
		File f = SPIFFS.open( CONFIG_FILE, "w");
#endif
		if( f ){
			uint8_t data[ 5 ];
			data[ 0 ]								= ( lamp_on.value.bool_value ) ? 1 : 0;
			data[ 1 ]								= brightness.value.int_value;
			data[ 2 ]								= saturation.value.int_value;
			data[ 3 ]								= (uint8_t)hue.value.float_value >> 8;
			data[ 4 ]								= (uint8_t)hue.value.float_value;
			f.write( (char*)data, sizeof( data ) );
		}
	}
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
