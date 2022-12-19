#include "main.h"
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <DNS_Server.h>
#include <timer.h>
#include <esp_functions.h>
#include <Adafruit_NeoPixel.h>

//----------- DEFINES ----------------------------------------------------------------------


//----------- VARIABLES --------------------------------------------------------------------
Adafruit_NeoPixel pixels( LEDS_COUNT, D5, NEO_GRB + NEO_KHZ800 );
uint8_t bright = 4;

Timer timer0( 0, 1000 );
DNS_Server dnsServer;
// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t lamp_on;

extern "C" void setBrightness(const uint8_t value);
extern "C" uint8_t getBrightness(void);
extern "C" void setLamp(const uint8_t value);
extern "C" void setLed(const uint8_t ledNum, uint8_t r, uint8_t g, uint8_t b);

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

#ifdef __DEV
	Serial.begin( 115200 );
#endif

	pixels.begin();
	pixels.clear();
	pixels.show();
	pixels.setBrightness( bright );
	delay( 1000 );
	setLed( 0, 0xFF, 0, 0 ); pixels.show(); delay( 250 );
	setLed( 1, 0xFF, 0x35, 0 ); pixels.show(); delay( 250 );
	setLed( 2, 0xFF, 0xFF, 0 ); pixels.show(); delay( 250 );
	setLed( 3, 0, 0x80, 0 ); pixels.show(); delay( 250 );
	setLed( 4, 0, 0, 0xFF ); pixels.show(); delay( 250 );
	setLed( 5, 0x4B, 0, 0x82 ); pixels.show(); delay( 250 );
	setLed( 6, 0xEE, 0x82, 0xEE ); pixels.show(); delay( 250 );
	setLed( 7, 0xFE, 0xFE, 0xFE ); pixels.show(); delay( 250 );
	setLed( 8, 0xFF, 0, 0 ); pixels.show(); delay( 250 );
	setLed( 9, 0xFF, 0x35, 0 ); pixels.show(); delay( 250 );
	setLed( 10, 0xFF, 0xFF, 0 ); pixels.show(); delay( 250 );
	setLed( 11, 0, 0x80, 0 ); pixels.show(); delay( 250 );
	setLed( 12, 0, 0, 0xFF ); pixels.show(); delay( 250 );
	setLed( 13, 0x4B, 0, 0x82 ); pixels.show(); delay( 250 );
	setLed( 14, 0xEE, 0x82, 0xEE ); pixels.show(); delay( 250 );
	setLed( 15, 0xFE, 0xFE, 0xFE ); pixels.show(); delay( 250 );
	setLed( 16, 0xFF, 0, 0 ); pixels.show(); delay( 250 );
	delay( 500 );
	pixels.clear();
	pixels.show();



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
#ifdef __DEV
		ESP_DEBUG( "Free heap: %d, HomeKit clients: %d\n", ESP.getFreeHeap(), arduino_homekit_connected_clients_count() );
#endif
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
	}else{
		arduino_homekit_loop();
	}
}

//-----------------------------------------------------------------------------------------
void setBrightness(const uint8_t value)
{
	bright = (uint8_t)map( value, 0, 100, 0, 255 );
	pixels.setBrightness( bright );
	pixels.show();
}

//-----------------------------------------------------------------------------------------
uint8_t getBrightness(void)
{
	return map( bright, 0, 255, 0, 100 );
}

//-----------------------------------------------------------------------------------------
void setLamp(const uint8_t value)
{
	printf( "LAMP [%s]\n", ( value )?"true":"false" );

	if( value ){
		for( uint8_t i = 0; i < LEDS_COUNT; i++ ){
			setLed( i, 0xFF, 0x35, 0 );
		}
	}else{
		pixels.clear();
	}
	
	pixels.show();
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
