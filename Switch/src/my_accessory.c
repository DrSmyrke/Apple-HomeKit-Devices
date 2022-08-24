#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <LIBS/my_defines.h>
#include "main.h"

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click the "Identify Accessory" button in Home APP.
void my_accessory_identify(homekit_value_t _value)
{
	printf("accessory identify\n");
}



bool led_power = false;
int led_bri = 100; //[0, 100]
homekit_value_t light_bri_get() {
	return HOMEKIT_INT(led_bri);
}
void led_bri_set(homekit_value_t value) {
	if (value.format != homekit_format_int) {
		return;
	}
	led_bri = value.int_value;
	// led_update();
}
homekit_value_t led_on_get() {
	return HOMEKIT_BOOL(led_power);
}

void led_on_set(homekit_value_t value) {
	if (value.format != homekit_format_bool) {
		printf("Invalid on-value format: %d\n", value.format);
		return;
	}
	led_power = value.bool_value;
	if (led_power) {
		if (led_bri < 1) {
			led_bri = 100;
		}
	}
	// led_update();
}
homekit_characteristic_t led_on = HOMEKIT_CHARACTERISTIC_(ON, false,
		//.callback=HOMEKIT_CHARACTERISTIC_CALLBACK(switch_on_callback),
		.getter=led_on_get,
		.setter=led_on_set
);

#ifdef __MOTION_SENSOR
	homekit_characteristic_t motion_detected = HOMEKIT_CHARACTERISTIC_( MOTION_DETECTED, 0 );
#endif

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_switch_on = HOMEKIT_CHARACTERISTIC_( ON, false );

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_( NAME, DEVICE_NAME );

homekit_accessory_t *accessories[] = {
	HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
		HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
			HOMEKIT_CHARACTERISTIC(NAME, DEVICE_NAME),
			HOMEKIT_CHARACTERISTIC(MANUFACTURER, MY_MANUFACTURER_NAME),
			HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
			HOMEKIT_CHARACTERISTIC(MODEL, MODEL_NAME),
			HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, FIRMWARE_VERSION),
			HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
			NULL
		}),
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_switch_on,
			&cha_name,
			NULL
		}),
		// HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]){
		// 	&led_on,
		// 	HOMEKIT_CHARACTERISTIC( NAME, "Ch1" ),
		// 	HOMEKIT_CHARACTERISTIC( BRIGHTNESS, 100, .getter=light_bri_get, .setter=led_bri_set ),
		// 	// HOMEKIT_CHARACTERISTIC( HUE, 100, .getter=light_bri_get, .setter=led_bri_set ),
		// 	NULL
		// }),
#ifdef __MOTION_SENSOR
		HOMEKIT_SERVICE(MOTION_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			HOMEKIT_CHARACTERISTIC( NAME, "Motion" ),
			&motion_detected,
			NULL
		}),
		NULL
	}),
#endif
	// HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
	// 	HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
	// 		HOMEKIT_CHARACTERISTIC(NAME, "LED 1"),
	// 		HOMEKIT_CHARACTERISTIC(MANUFACTURER, MY_MANUFACTURER_NAME),
	// 		HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
	// 		HOMEKIT_CHARACTERISTIC(MODEL, MODEL_NAME),
	// 		HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, FIRMWARE_VERSION),
	// 		HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
	// 		NULL
	// 	}),
	// 	HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]){
	// 		&led_on,
	// 		HOMEKIT_CHARACTERISTIC( NAME, "Ch2" ),
	// 		HOMEKIT_CHARACTERISTIC( BRIGHTNESS, 100, .getter=light_bri_get, .setter=led_bri_set ),
	// 		// HOMEKIT_CHARACTERISTIC( HUE, 100, .getter=light_bri_get, .setter=led_bri_set ),
	// 		NULL
	// 	}),
	// 	NULL
	// }),
	NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};


