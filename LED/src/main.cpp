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
uint8_t fadeValue = 0;
uint8_t fadeMode = 1;

Timer timer0( 0, 25 );
DNS_Server dnsServer;
// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t lamp_on;
extern "C" uint8_t rgb[ 3 ];
extern "C" uint16_t hue;
extern "C" uint8_t saturation;
extern "C" uint8_t bright;

extern "C" void setRGB(const uint8_t r, const uint8_t g, const uint8_t b);
extern "C" void setBrightness(const uint8_t value);
extern "C" void setLamp(const uint8_t value);
extern "C" void setLed(const uint8_t ledNum, uint8_t r, uint8_t g, uint8_t b);
extern "C" void hsbToRgb(void);

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
	}else{
		arduino_homekit_setup( &config );
	}

	esp::pageBuff = pageBuff;
	esp::addWebServerPages( &webServer, true, true, true );
	webServer.on( "/storageReset", [ webServer ](void){
		homekit_storage_reset();
		ESP.reset();
		webServer.send ( 200, "text/html", "OK" );
	} );
	webServer.on( "/", indexPageHeadler );
	webServer.on( "/get", getPageHeadler );
	webServer.begin();

	ESP_DEBUG( "INIT OK\n" );
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
		if( bright == 73 ){
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
	}

	if( esp::flags.ap_mode ){
		dnsServer.processNextRequest();
	}else{
		arduino_homekit_loop();
	}

	webServer.handleClient();
}

//-----------------------------------------------------------------------------------------
void indexPageHeadler(void)
{
	//-------------------------------------------------------------
	if( webServer.hasArg( "onoff" ) ){
		uint8_t value = ( webServer.arg( "onoff" ) == "true" ) ? 1 : 0;
		setLamp( value );
		lamp_on.value = HOMEKIT_BOOL( value );
		homekit_characteristic_notify( &lamp_on, lamp_on.value );
		webServer.send ( 200, "application/json", "{'result': 'ok'}" );
	}else if( webServer.hasArg( "hue" ) && webServer.hasArg( "sat" ) && webServer.hasArg( "bri" ) ){
		hue = webServer.arg( "hue" ).toInt();
		saturation = webServer.arg( "sat" ).toInt();
		bright = webServer.arg( "bri" ).toInt();
		setBrightness( bright );
		hsbToRgb();
		webServer.send ( 200, "application/json", "{'result': 'ok'}" );
	}
	//-------------------------------------------------------------
	esp::webSendFile( &webServer, "/index.html", "text/html" );
}

//-----------------------------------------------------------------------------------------
void getPageHeadler(void)
{
	strcpy( pageBuff, "{\"on\": " );
	strcat( pageBuff, itoa( ( ( lamp_on.value.bool_value ) ? 1 : 0 ), esp::tmpVal, 10 ) );
	strcat( pageBuff, ",\"hue\":" );
	strcat( pageBuff, itoa( hue, esp::tmpVal, 10 ) );
	strcat( pageBuff, ",\"sat\":" );
	strcat( pageBuff, itoa( saturation, esp::tmpVal, 10 ) );
	strcat( pageBuff, ",\"bri\":" );
	strcat( pageBuff, itoa( bright, esp::tmpVal, 10 ) );
	strcat( pageBuff, "}" );
	webServer.send ( 200, "application/json", pageBuff );
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
	printf( "LAMP [%s][%d]\n", ( value )? "true" : "false", bright );

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
		
		setBrightness( bright );
	}else{
		// pixels.clear();
		pixels.setBrightness( 0 );
	}

	hsbToRgb();
}

//-----------------------------------------------------------------------------------------
void hsbToRgb(void)
{
	uint16 s = saturation * 255 / 100;
	uint16 v = bright * 255 / MAX_BRIGHT;
	if (s == 0) {
		rgb[ 0 ] = rgb[ 1 ] = rgb[ 2 ] = v;
	} else {
		uint16 t1 = v;
		uint16 t2 = (255 - s) * v / 255;
		uint16 t3 = (t1 - t2) * (hue % 60) / 60;
		if (hue == 360) hue = 0;
		if (hue < 60) {
			rgb[ 0 ] = t1;
			rgb[ 2 ] = t2;
			rgb[ 1 ] = t2 + t3;
		}else if (hue < 120) {
			rgb[ 1 ] = t1;
			rgb[ 2 ] = t2;
			rgb[ 0 ] = t1 - t3;
		}else if (hue < 180) {
			rgb[ 1 ] = t1;
			rgb[ 0 ] = t2;
			rgb[ 2 ] = t2 + t3;
		}else if (hue < 240) {
			rgb[ 2 ] = t1;
			rgb[ 0 ] = t2;
			rgb[ 1 ] = t1 - t3;
		}else if (hue < 300) {
			rgb[ 2 ] = t1;
			rgb[ 1 ] = t2;
			rgb[ 0 ] = t2 + t3;
		}else if (hue < 360) {
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
