#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <my_defines.h>
#include "main.h"


homekit_characteristic_t outlet_on = HOMEKIT_CHARACTERISTIC_(ON, false,
	//.callback=HOMEKIT_CHARACTERISTIC_CALLBACK(switch_on_callback),
	.getter=outletGet,
	.setter=outletSet
);

//-----------------------------------------------------------------------------------------
void my_accessory_identify(const homekit_value_t _value)
{
	printf( "accessory identify\n" );
}

//-----------------------------------------------------------------------------------------
homekit_value_t outletPhysicalState(void)
{
	return HOMEKIT_BOOL( true );
}

//-----------------------------------------------------------------------------------------
// homekit_value_t motionReadState(void)
// {
// 	return HOMEKIT_BOOL( motion_detected.value.bool_value );
// }

//-----------------------------------------------------------------------------------------
void outletSet(homekit_value_t value)
{
	if( value.format != homekit_format_bool ) {
		printf( "Invalid on-value format: %d\n", value.format );
		return;
	}

	outlet_on.value.bool_value = value.bool_value;
	printf( "Switch: %s", value.bool_value ? "ON" : "OFF" );
	digitalWrite( SWITCH_PIN, value.bool_value ? 1 : 0 );
}

//-----------------------------------------------------------------------------------------
homekit_value_t outletGet(void)
{
	return HOMEKIT_BOOL( outlet_on.value.bool_value );
}





homekit_characteristic_t outlet_in_use = HOMEKIT_CHARACTERISTIC_( OUTLET_IN_USE, false,
	.getter = outletPhysicalState,
);

// homekit_characteristic_t motion_detected = HOMEKIT_CHARACTERISTIC_( MOTION_DETECTED, false
// 	// ,.getter = motionReadState,
// );






homekit_accessory_t *accessories[] = {
	HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_outlet, .services=(homekit_service_t*[]) {
		HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
			HOMEKIT_CHARACTERISTIC( NAME, DEVICE_NAME ),
			HOMEKIT_CHARACTERISTIC( MANUFACTURER, MY_MANUFACTURER_NAME ),
			HOMEKIT_CHARACTERISTIC( SERIAL_NUMBER, "0123456" ),
			HOMEKIT_CHARACTERISTIC( MODEL, MODEL_NAME ),
			HOMEKIT_CHARACTERISTIC( FIRMWARE_REVISION, FIRMWARE_VERSION ),
			HOMEKIT_CHARACTERISTIC( IDENTIFY, my_accessory_identify ),
			NULL
		}),
		HOMEKIT_SERVICE(OUTLET, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			HOMEKIT_CHARACTERISTIC( NAME, "Outlet" ),
			&outlet_on,
			&outlet_in_use,
			NULL
		}),
		// HOMEKIT_SERVICE(MOTION_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]){
		// 	HOMEKIT_CHARACTERISTIC( NAME, "Motion" ),
		// 	&motion_detected,
		// 	NULL
		// }),
		NULL
	}),
	NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};


