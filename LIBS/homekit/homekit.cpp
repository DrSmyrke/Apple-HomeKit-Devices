//------------ Includes -----------------------------------------------------------------
#include "homekit.h"
#include <my_defines.h>

//------------ Functions -----------------------------------------------------------------
HomeKit::HomeKit()
{
	m_pAccessories								= m_accessories;
	m_config.accessories						= &m_pAccessories;
	m_config.password							= m_password;

	for( uint16_t i = 0; i < MAX_ACCESSORIES; i++ ){
		m_accessories[ i ].id					= 0;
		m_accessories[ i ].config_number		= 0;
		m_accessories[ i ].category				= (homekit_accessory_category_t)0;
		m_accessories[ i ].services				= nullptr;
	}
}

//----------------------------------------------------------------------------------------
void HomeKit::fillInfoCharacteristics(homekit_service_t* service, const char *name, const char *serial, const char *modelName, const char *version, void (*identifyCallback)(const homekit_value_t) )
{
	homekit_characteristic_t* info[ 6 ];

	service->type								= HOMEKIT_SERVICE_ACCESSORY_INFORMATION;
	service->characteristics					= info;

	initCharacteristic_Name( info[ 0 ], name );

	info[ 1 ]->type								= HOMEKIT_CHARACTERISTIC_MANUFACTURER;
	info[ 1 ]->description						= "Manufacturer";
	info[ 1 ]->format							= homekit_format_string;
	info[ 1 ]->permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( info[ 1 ]->value.string_value, MY_MANUFACTURER_NAME );

	info[ 2 ]->type								= HOMEKIT_CHARACTERISTIC_SERIAL_NUMBER;
	info[ 2 ]->description						= "Serial Number";
	info[ 2 ]->format							= homekit_format_string;
	info[ 2 ]->permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( info[ 2 ]->value.string_value, serial );

	info[ 3 ]->type								= HOMEKIT_CHARACTERISTIC_MODEL;
	info[ 3 ]->description						= "Model";
	info[ 3 ]->format							= homekit_format_string;
	info[ 3 ]->permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( info[ 3 ]->value.string_value, modelName );

	info[ 4 ]->type								= HOMEKIT_CHARACTERISTIC_FIRMWARE_REVISION;
	info[ 4 ]->description						= "Firmware Revision";
	info[ 4 ]->format							= homekit_format_string;
	info[ 4 ]->permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( info[ 4 ]->value.string_value, version );

	info[ 5 ]->type								= HOMEKIT_CHARACTERISTIC_IDENTIFY;
	info[ 5 ]->description						= "Identify";
	info[ 5 ]->format							= homekit_format_bool;
	info[ 5 ]->setter							= identifyCallback;
	info[ 5 ]->permissions						= (homekit_permissions_t)homekit_permissions_paired_write;
	// strcpy( info[ 5 ]->value.string_value, version );
}

//----------------------------------------------------------------------------------------
void HomeKit::initService(homekit_service_t* service, const char *type, const uint8_t characteristicsCount)
{
	homekit_characteristic_t* characteristics[ characteristicsCount ];

	service->type								= type;
	service->characteristics					= characteristics;
}

//----------------------------------------------------------------------------------------
void HomeKit::initCharacteristic(homekit_characteristic_t* characteristic, const char *type, const char *description, const homekit_format_t format, const homekit_permissions_t permissions, homekit_unit_t unit, float *min_value, float *max_value, float *min_step, const homekit_value_t value)
{
	characteristic->type						= type;
	characteristic->description					= description;
	characteristic->format						= format;
	characteristic->permissions					= permissions;
	characteristic->unit						= unit;
	characteristic->min_value					= min_value;
	characteristic->max_value					= max_value;
	characteristic->min_step					= min_step;
	characteristic->value						= value;
}

//----------------------------------------------------------------------------------------
void HomeKit::initCharacteristic_Name(homekit_characteristic_t* characteristic, const char *name)
{
	characteristic->type						= HOMEKIT_CHARACTERISTIC_NAME;
	characteristic->description					= "Name";
	characteristic->format						= homekit_format_string;
	characteristic->permissions					= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( characteristic->value.string_value, name );
}

//----------------------------------------------------------------------------------------
void HomeKit::initCharacteristic_ON(homekit_characteristic_t* characteristic, const homekit_value_t startValue)
{
	uint32_t permissions						= homekit_permissions_paired_read | homekit_permissions_paired_write | homekit_permissions_notify;
	float min									= 0;
	float max									= 1;
	float step									= 1;
	initCharacteristic( characteristic, HOMEKIT_CHARACTERISTIC_ON, "On", homekit_format_bool, (homekit_permissions_t)permissions, homekit_unit_percentage, &min, &max, &step, startValue );
}

//----------------------------------------------------------------------------------------
void HomeKit::initCharacteristic_Brightness(homekit_characteristic_t* characteristic, const homekit_value_t startValue)
{
	uint32_t permissions						= homekit_permissions_paired_read | homekit_permissions_paired_write | homekit_permissions_notify;
	float min									= 0;
	float max									= 100;
	float step									= 1;
	initCharacteristic( characteristic, HOMEKIT_CHARACTERISTIC_BRIGHTNESS, "Brightness", homekit_format_string, (homekit_permissions_t)permissions, homekit_unit_percentage, &min, &max, &step, startValue );
}

//----------------------------------------------------------------------------------------
void HomeKit::initCharacteristic_HUE(homekit_characteristic_t* characteristic, const homekit_value_t startValue)
{
	uint32_t permissions						= homekit_permissions_paired_read | homekit_permissions_paired_write | homekit_permissions_notify;
	float min									= 0;
	float max									= 360;
	float step									= 1;
	initCharacteristic( characteristic, HOMEKIT_CHARACTERISTIC_HUE, "Hue", homekit_format_float, (homekit_permissions_t)permissions, homekit_unit_arcdegrees, &min, &max, &step, startValue );
}

//----------------------------------------------------------------------------------------
void HomeKit::initCharacteristic_Saturation(homekit_characteristic_t* characteristic, const homekit_value_t startValue)
{
	uint32_t permissions						= homekit_permissions_paired_read | homekit_permissions_paired_write | homekit_permissions_notify;
	float min									= 0;
	float max									= 100;
	float step									= 1;
	initCharacteristic( characteristic, HOMEKIT_CHARACTERISTIC_SATURATION, "Saturation", homekit_format_float, (homekit_permissions_t)permissions, homekit_unit_percentage, &min, &max, &step, startValue );
}

//----------------------------------------------------------------------------------------
void HomeKit::setCharacteristicGetter(homekit_characteristic_t* characteristic, homekit_value_t (*getter)())
{
	characteristic->getter						= getter;
}

//----------------------------------------------------------------------------------------
void HomeKit::setCharacteristicSetter(homekit_characteristic_t* characteristic, void (*setter)(const homekit_value_t))
{
	characteristic->setter						= setter;
}

//----------------------------------------------------------------------------------------
homekit_accessory_t* HomeKit::newAccessory(const homekit_accessory_category_t category, const uint8_t servicesCount)
{
	homekit_accessory_t* pointer = nullptr;

	for( uint8_t i = 0; i < MAX_ACCESSORIES; i++ ){
		if( m_accessories[ i ].id == 0 && m_accessories[ i ].config_number == 0 ){
			homekit_service_t* services[ servicesCount ];

			m_accessories[ i ].id				= i + 1;
			m_accessories[ i ].category			= category;
			m_accessories[ i ].config_number	= 1;
			m_accessories[ i ].services			= services;

			pointer = m_accessories + i;
			break;
		}
	}

	return pointer;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

