#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <LIBS/my_defines.h>
#include "main.h"



void my_accessory_identify(homekit_value_t _value)
{
	printf("accessory identify\n");
}

uint8_t rgb[ 3 ];
uint16_t hue = START_HUE;
uint8_t saturation = START_SATURATION;
bright = START_BRIGHT;
void lamp_on_setter(const homekit_value_t value);
homekit_value_t lamp_on_getter(void);

homekit_characteristic_t lamp_on = HOMEKIT_CHARACTERISTIC_(ON, false,
		//.callback=HOMEKIT_CHARACTERISTIC_CALLBACK(switch_on_callback),
		.getter=lamp_on_getter,
		.setter=lamp_on_setter
);

//-----------------------------------------------------------------------------------------
void lamp_on_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_bool ) return;
	setLamp( value.bool_value );
	lamp_on.value = value;
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_on_getter(void)
{
	return lamp_on.value;
}

//-----------------------------------------------------------------------------------------
void lamp_bright_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_int ) return;
	bright = value.int_value;
	setBrightness( bright );
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_bright_getter(void)
{
	return HOMEKIT_INT( bright );
}

//-----------------------------------------------------------------------------------------
void lamp_hue_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_float ) return;
	hue = value.float_value;
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
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_hue_getter(void)
{
	return HOMEKIT_FLOAT( hue );
}

//-----------------------------------------------------------------------------------------
void lamp_sat_setter(const homekit_value_t value)
{
	if( value.format != homekit_format_float ) return;
	saturation = value.float_value;
	hsbToRgb();
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_sat_getter(void)
{
	return HOMEKIT_FLOAT( saturation );
}


homekit_accessory_t *accessories[] = {
	HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
		HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
			HOMEKIT_CHARACTERISTIC(NAME, DEVICE_NAME),
			HOMEKIT_CHARACTERISTIC(MANUFACTURER, MY_MANUFACTURER_NAME),
			HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
			HOMEKIT_CHARACTERISTIC(MODEL, MODEL_NAME),
			HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, FIRMWARE_VERSION),
			HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
			NULL
		}),
		HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&lamp_on,
			HOMEKIT_CHARACTERISTIC( NAME, DEVICE_NAME ),
			HOMEKIT_CHARACTERISTIC( BRIGHTNESS, START_BRIGHT, .getter=lamp_bright_getter, .setter=lamp_bright_setter ),
			HOMEKIT_CHARACTERISTIC( HUE, START_HUE, .getter=lamp_hue_getter, .setter=lamp_hue_setter ),
			HOMEKIT_CHARACTERISTIC( SATURATION, START_SATURATION, .getter=lamp_sat_getter, .setter=lamp_sat_setter ),
			NULL
		}),
		NULL
	}),
	NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};


