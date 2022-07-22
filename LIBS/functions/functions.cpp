//------------ Includes -----------------------------------------------------------------
#include "functions.h"

//------------ Functions -----------------------------------------------------------------
HomeKit::HomeKit()
{
	m_pAccessories							= m_accessories;
	m_config.accessories					= &m_pAccessories;

	for( uint16_t i = 0; i < MAX_ACCESSORIES; i++ ){
		m_accessories[ i ].id				= 0;
		m_accessories[ i ].config_number	= 0;
		m_accessories[ i ].category			= (homekit_accessory_category_t)0;
		m_accessories[ i ].services			= nullptr;
	}
}

//----------------------------------------------------------------------------------------
void HomeKit::fillInfoCharacteristics(const char *name, const char *serial, const char *modelName, const char *version, void (*identifyCallback)(const homekit_value_t) )
{
	m_info[ 0 ].type							= HOMEKIT_CHARACTERISTIC_NAME;
	m_info[ 0 ].description						= "Name";
	m_info[ 0 ].format							= homekit_format_string;
	m_info[ 0 ].value.format					= homekit_format_string;
	m_info[ 0 ].permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( m_info[ 0 ].value.string_value, name );

	m_info[ 1 ].type							= HOMEKIT_CHARACTERISTIC_MANUFACTURER;
	m_info[ 1 ].description						= "Manufacturer";
	m_info[ 1 ].format							= homekit_format_string;
	m_info[ 1 ].value.format					= homekit_format_string;
	m_info[ 1 ].permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( m_info[ 1 ].value.string_value, MY_MANUFACTURER_NAME );

	m_info[ 2 ].type							= HOMEKIT_CHARACTERISTIC_SERIAL_NUMBER;
	m_info[ 2 ].description						= "Serial Number";
	m_info[ 2 ].format							= homekit_format_string;
	m_info[ 2 ].value.format					= homekit_format_string;
	m_info[ 2 ].permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( m_info[ 2 ].value.string_value, serial );

	m_info[ 3 ].type							= HOMEKIT_CHARACTERISTIC_MODEL;
	m_info[ 3 ].description						= "Model";
	m_info[ 3 ].format							= homekit_format_string;
	m_info[ 3 ].value.format					= homekit_format_string;
	m_info[ 3 ].permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( m_info[ 3 ].value.string_value, modelName );

	m_info[ 4 ].type							= HOMEKIT_CHARACTERISTIC_FIRMWARE_REVISION;
	m_info[ 4 ].description						= "Firmware Revision";
	m_info[ 4 ].format							= homekit_format_string;
	m_info[ 4 ].value.format					= homekit_format_string;
	m_info[ 4 ].permissions						= (homekit_permissions_t)homekit_permissions_paired_read;
	strcpy( m_info[ 4 ].value.string_value, version );

	m_info[ 5 ].type							= HOMEKIT_CHARACTERISTIC_IDENTIFY;
	m_info[ 5 ].description						= "Identify";
	m_info[ 5 ].format							= homekit_format_bool;
	m_info[ 5 ].setter							= identifyCallback;
	m_info[ 5 ].permissions						= (homekit_permissions_t)homekit_permissions_paired_write;
	strcpy( m_info[ 5 ].value.string_value, version );
}

//----------------------------------------------------------------------------------------
uint32_t HomeKit::newAccessorie(const homekit_accessory_category_t category)
{
	uint32_t id = 0;

	for( uint16_t i = 0; i < MAX_ACCESSORIES; i++ ){
		if( m_accessories[ i ].id == 0 && m_accessories[ i ].config_number == 0 ){
			id = i + 1;
			m_accessories[ i ].id				= id;
			m_accessories[ i ].category			= category;
			m_accessories[ i ].config_number	= 1;
			break;
		}
	}

	return id;
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

