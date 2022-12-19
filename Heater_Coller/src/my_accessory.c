#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <my_defines.h>
#include "main.h"


homekit_characteristic_t active_characteristic = HOMEKIT_CHARACTERISTIC_(ACTIVE, 0,
	.getter = getState,
	.setter = setState,
);
homekit_characteristic_t currentTemp_characteristic = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 1.0,
	.getter = getCurrentTemp,
);
homekit_characteristic_t currentState_characteristic = HOMEKIT_CHARACTERISTIC_(CURRENT_HEATER_COOLER_STATE, 1,
	.getter = getCurrentState,
);
homekit_characteristic_t targetState_characteristic = HOMEKIT_CHARACTERISTIC_(TARGET_HEATER_COOLER_STATE, 2,
	.getter = getTargetState,
	.setter = setTargetState,
);
homekit_characteristic_t targetCoolingTemp_characteristic = HOMEKIT_CHARACTERISTIC_(COOLING_THRESHOLD_TEMPERATURE, 25.0,
	.setter = setTargetCoolingTemp,
);
homekit_characteristic_t targetHeatingTemp_characteristic = HOMEKIT_CHARACTERISTIC_(HEATING_THRESHOLD_TEMPERATURE, 23.0,
	.setter = setTargetHeatingTemp,
);


//-----------------------------------------------------------------------------------------
void my_accessory_identify(const homekit_value_t _value)
{
	printf( "accessory identify\n" );
}

//-----------------------------------------------------------------------------------------
homekit_value_t getState(void)
{
	return active_characteristic.value;
}

//-----------------------------------------------------------------------------------------
homekit_value_t getCurrentTemp(void)
{
	return currentTemp_characteristic.value;
}

//-----------------------------------------------------------------------------------------
homekit_value_t getCurrentState(void)
{
	return currentState_characteristic.value;
}

//-----------------------------------------------------------------------------------------
homekit_value_t getTargetState(void)
{
	return targetState_characteristic.value;
}

//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------







homekit_accessory_t *accessories[] = {
	HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_air_conditioner, .services=(homekit_service_t*[]) {
		HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
			HOMEKIT_CHARACTERISTIC( NAME, DEVICE_NAME ),
			HOMEKIT_CHARACTERISTIC( MANUFACTURER, MY_MANUFACTURER_NAME ),
			HOMEKIT_CHARACTERISTIC( SERIAL_NUMBER, "0123456" ),
			HOMEKIT_CHARACTERISTIC( MODEL, MODEL_NAME ),
			HOMEKIT_CHARACTERISTIC( FIRMWARE_REVISION, FIRMWARE_VERSION ),
			HOMEKIT_CHARACTERISTIC( IDENTIFY, my_accessory_identify ),
			NULL
		}),
		HOMEKIT_SERVICE(HEATER_COOLER, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			HOMEKIT_CHARACTERISTIC( NAME, "Cooler" ),
			&active_characteristic,
			&currentTemp_characteristic,
			&currentState_characteristic,
			&targetState_characteristic,
			&targetCoolingTemp_characteristic,
			&targetHeatingTemp_characteristic,
			NULL
		}),
		NULL
	}),
	NULL
};

//  Required Characteristics:
//  - ACTIVE
//  - CURRENT_TEMPERATURE
//  - CURRENT_HEATER_COOLER_STATE
//  - TARGET_HEATER_COOLER_STATE
 
//  Optional Characteristics:
//  - NAME
//  - ROTATION_SPEED
//  - TEMPERATURE_DISPLAY_UNITS
//  - SWING_MODE
//  - COOLING_THRESHOLD_TEMPERATURE
//  - HEATING_THRESHOLD_TEMPERATURE
//  - LOCK_PHYSICAL_CONTROLS
//  */
// #define HOMEKIT_SERVICE_ HOMEKIT_APPLE_UUID2("BC")
homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};


