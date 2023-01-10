#ifndef __HOMEKIT_FUNCTIONS_H__
#define __HOMEKIT_FUNCTIONS_H__


#include <homekit/homekit.h>
#include <homekit/characteristics.h>

#ifndef MAX_ACCESSORIES
	#define MAX_ACCESSORIES					1
#endif


class HomeKit{
	public:
		HomeKit();
		~HomeKit(){}
		void fillInfoCharacteristics(homekit_service_t* service, const char *name, const char *serial, const char *modelName, const char *version, void (*identifyCallback)(const homekit_value_t) = nullptr );
		void initService(homekit_service_t* service, const char *type, const uint8_t characteristicsCount);
		void initCharacteristic(homekit_characteristic_t* characteristic, const char *type, const char *description, const homekit_format_t format, const homekit_permissions_t permissions, homekit_unit_t unit, float *min_value, float *max_value, float *min_step, const homekit_value_t value);
		void initCharacteristic_Name(homekit_characteristic_t* characteristic, const char *name);
		void initCharacteristic_ON(homekit_characteristic_t* characteristic, const homekit_value_t startValue);
		void initCharacteristic_Brightness(homekit_characteristic_t* characteristic, const homekit_value_t startValue);
		void initCharacteristic_HUE(homekit_characteristic_t* characteristic, const homekit_value_t startValue);
		void initCharacteristic_Saturation(homekit_characteristic_t* characteristic, const homekit_value_t startValue);
		void setCharacteristicGetter(homekit_characteristic_t* characteristic, homekit_value_t (*getter)() = nullptr);
		void setCharacteristicSetter(homekit_characteristic_t* characteristic, void (*setter)(const homekit_value_t) = nullptr);
		homekit_accessory_t* newAccessory(const homekit_accessory_category_t category, const uint8_t servicesCount);

		void setPassword(const char *password){ strcpy( m_password, password ); }
		void setAccessories(homekit_accessory_t *accessories){ m_config.accessories = &accessories; }

		homekit_server_config_t* getConfig(void){ return &m_config; }
	private:
		homekit_accessory_t m_accessories[ MAX_ACCESSORIES ];
		homekit_accessory_t* m_pAccessories;
		homekit_server_config_t m_config;
		char m_password[ 11 ];
};

#endif //__HOMEKIT_FUNCTIONS_H__
