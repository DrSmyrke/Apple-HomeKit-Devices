#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <LIBS/my_defines.h>
#include "main.h"



void my_accessory_identify(homekit_value_t _value)
{
	printf("accessory identify\n");
}



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
	setBrightness( value.int_value );
}

//-----------------------------------------------------------------------------------------
homekit_value_t lamp_bright_getter(void)
{
	return HOMEKIT_INT( getBrightness() );
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
			HOMEKIT_CHARACTERISTIC( BRIGHTNESS, 100, .getter=lamp_bright_getter, .setter=lamp_bright_setter ),
			// HOMEKIT_CHARACTERISTIC( HUE, 100, .getter=light_bri_get, .setter=led_bri_set ),
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


