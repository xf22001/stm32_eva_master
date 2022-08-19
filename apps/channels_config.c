

/*================================================================
 *
 *
 *   文件名称：channels_config.c
 *   创 建 者：肖飞
 *   创建日期：2021年01月18日 星期一 09时26分44秒
 *   修改日期：2022年08月19日 星期五 16时52分58秒
 *   描    述：
 *
 *================================================================*/
#include "channels_config.h"
#include "os_utils.h"
#include "power_modules.h"
#include "ntc_temperature.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi3;

static energy_meter_config_item_t energy_meter_config_item_0_0 = {
	.type = ENERGY_METER_TYPE_DC,
	.huart = &huart3,
};

static energy_meter_config_item_t *energy_meter_config_item_0_sz[] = {
	&energy_meter_config_item_0_0,
};

static function_board_config_item_t function_board_config_item_0_0 = {
	.type = FUNCTION_BOARD_TYPE_485,
	.huart = &huart3,
};

static function_board_config_item_t *function_board_config_item_0_sz[] = {
	&function_board_config_item_0_0,
};

static channel_config_t channel0_config = {
	.channel_type = CHANNEL_TYPE_NATIVE,
	.charger_config = {
		.charger_type = CHANNEL_CHARGER_BMS_TYPE_GB,
		.hcan_bms = &hcan2,
	},
	.energy_meter_config = {
		.default_type = ENERGY_METER_TYPE_DC,
		.energy_accuracy = VALUE_ACCURACY_2,
		.voltage_accuracy = VALUE_ACCURACY_1,
		.curent_accuracy = VALUE_ACCURACY_3,
		.size = ARRAY_SIZE(energy_meter_config_item_0_sz),
		.items = energy_meter_config_item_0_sz,
	},
	.function_board_config = {
		.default_type = FUNCTION_BOARD_TYPE_485,
		.size = ARRAY_SIZE(function_board_config_item_0_sz),
		.items = function_board_config_item_0_sz,
	},
};

static channel_config_t channel1_config = {
	.channel_type = CHANNEL_TYPE_PROXY_REMOTE,
};

static channel_config_t channel2_config = {
	.channel_type = CHANNEL_TYPE_PROXY_REMOTE,
};

static channel_config_t channel3_config = {
	.channel_type = CHANNEL_TYPE_PROXY_REMOTE,
};

static channel_config_t *channel_config_sz[] = {
	&channel0_config,
	&channel1_config,
	&channel2_config,
	&channel3_config,
};

static card_reader_config_item_t card_reader_config_item_0 = {
	.type = CARD_READER_TYPE_PSEUDO,
};

static card_reader_config_item_t card_reader_config_item_1 = {
	.type = CARD_READER_TYPE_ZLG,
	.huart = &huart2,
};

static card_reader_config_item_t *card_reader_config_item_sz[] = {
	&card_reader_config_item_0,
	&card_reader_config_item_1,
};

static channels_config_t channels_config_0 = {
	.id = 0,
	.channel_number = ARRAY_SIZE(channel_config_sz),
	.channel_config = channel_config_sz,
	.power_module_config = {
		.hcan = &hcan1,
		.power_module_default_type = POWER_MODULE_TYPE_WINLINE,
	},
	.power_manager_config = {
		.power_manager_default_type = POWER_MANAGER_TYPE_NATIVE,
	},
	.voice_config = {
	},
	.card_reader_config = {
		.default_type = CARD_READER_TYPE_PSEUDO,
		.size = ARRAY_SIZE(card_reader_config_item_sz),
		.items = card_reader_config_item_sz,
	},
	.display_config = {
		.huart = &huart6,
	},
	.proxy_channel_info = {
		.hcan = &hcan1,
	},
	.multi_charge_info = {
		.hcan = &hcan1,
	},
	.board_temperature_adc = &hadc1,
	.board_temperature_adc_rank = 0,
};

static channels_config_t *channels_config_sz[] = {
	&channels_config_0,
};

channels_config_t *get_channels_config(uint8_t id)
{
	int i;
	channels_config_t *channels_config = NULL;
	channels_config_t *channels_config_item = NULL;

	for(i = 0; i < ARRAY_SIZE(channels_config_sz); i++) {
		channels_config_item = channels_config_sz[i];

		if(channels_config_item->id == id) {
			channels_config = channels_config_item;
			break;
		}
	}

	return channels_config;
}

int adc_value_helper(adc_value_type_t adc_value_type, uint16_t adc_value)
{
	int value = 0;

	switch(adc_value_type) {
		case ADC_VALUE_TYPE_BOARD_TEMPERATURE: {
			value = get_ntc_temperature(10000, adc_value, 4095);
		}
		break;

		case ADC_VALUE_TYPE_CP_AD_VOLTAGE: {
			value = adc_value * 3300 / 4096;//0v-1.2v 采样 0v-12v

			//(V - 0.5) * 2 / 102 * 8 * 4 / 3 = u
			//V - 0.5 = u / (2 / 102 * 8 * 4 / 3)
			//修正前
			//V = u / (2 / 102 * 8 * 4 / 3) + 0.5
			//修正后
			//V = u / (1.8667 / 101.8667 * 8 * 4 / 3) + 0.5

			value = value * 5.1159817458616805 / 10 + 50;
		}
		break;

		default: {
			app_panic();
		}
		break;
	}

	return value;
}
