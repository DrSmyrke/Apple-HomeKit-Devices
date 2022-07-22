#ifndef __HK_FUNCTIONS_H__
#define __HK_FUNCTIONS_H__


#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <../my_defines.h>

#ifndef MAX_ACCESSORIES
	#define MAX_ACCESSORIES					32
#endif


class HomeKit{
	public:
		HomeKit();
		~HomeKit(){}
		void fillInfoCharacteristics(const char *name, const char *serial, const char *modelName, const char *version, void (*identifyCallback)(const homekit_value_t) = nullptr );
		uint32_t newAccessorie(const homekit_accessory_category_t category);

		void setPassword(char *password){ m_config.password = password; }
		void setAccessories(homekit_accessory_t *accessories){ m_config.accessories = &accessories; }

		homekit_characteristic_t* getInfoCharacteristics(void){ return m_info; }
		homekit_server_config_t* getConfig(void){ return &m_config; }
	private:
		homekit_accessory_t m_accessories[ MAX_ACCESSORIES ];
		homekit_accessory_t* m_pAccessories;
		homekit_server_config_t m_config;
		homekit_characteristic_t m_info[ 6 ];
};

#endif //__HK_FUNCTIONS_H__
