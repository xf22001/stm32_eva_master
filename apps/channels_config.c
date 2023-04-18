

/*================================================================
 *
 *
 *   文件名称：channels_config.c
 *   创 建 者：肖飞
 *   创建日期：2021年01月18日 星期一 09时26分44秒
 *   修改日期：2023年04月18日 星期二 09时08分42秒
 *   描    述：
 *
 *================================================================*/
#include "channels_config.h"
#include "os_utils.h"
#include "power_modules.h"
#include "ntc_temperature.h"
#include "hw_adc.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi3;

static energy_meter_config_item_t energy_meter_config_item_0_0 = {
	.type = ENERGY_METER_TYPE_DC,
	.huart = &huart3,
};

static energy_meter_config_item_t energy_meter_config_item_0_1 = {
	.type = ENERGY_METER_TYPE_AC,
	.huart = &huart3,
};

static energy_meter_config_item_t *energy_meter_config_item_0_sz[] = {
	&energy_meter_config_item_0_0,
	&energy_meter_config_item_0_1,
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
		.output_relay_gpio_p = run_a_GPIO_Port,
		.output_relay_pin_p = run_a_Pin,
		.output_relay_gpio_n = run_a_GPIO_Port,
		.output_relay_pin_n = run_a_Pin,
		.charger_lock_state_gpio = in_1_GPIO_Port,
		.charger_lock_state_pin = in_1_Pin,
		.relay_charger_lock_gpio = lock_a_GPIO_Port,
		.relay_charger_lock_pin = lock_a_Pin,
		.charger_auxiliary_power_choose_gpio = ch_a_GPIO_Port,
		.charger_auxiliary_power_choose_pin = ch_a_Pin,
		.charger_auxiliary_power_onoff_gpio = pw_a_GPIO_Port,
		.charger_auxiliary_power_onoff_pin = pw_a_Pin,
	},
	.energy_meter_config = {
		.default_type = ENERGY_METER_TYPE_DC,
		//.request_addr = 1,
		//.slot = 0,
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
	.cp_gpio = in_a_cc1_GPIO_Port,
	.cp_pin = in_a_cc1_Pin,
	.cp_connect_state = GPIO_PIN_SET,
};

static channel_config_t *channel_config_sz[] = {
	&channel0_config,
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
		.power_module_default_type = POWER_MODULE_TYPE_INFY_V2G,
	},
	.power_manager_config = {
		.power_manager_default_type = POWER_MANAGER_TYPE_NATIVE,
	},
	.voice_config = {
	},
	.card_reader_config = {
		.default_type = CARD_READER_TYPE_ZLG,
		.size = ARRAY_SIZE(card_reader_config_item_sz),
		.items = card_reader_config_item_sz,
	},
	.display_config = {
		.huart = &huart1,
		.station = 2,
	},
	.proxy_channel_info = {
		.hcan = &hcan1,
	},
	.multi_charge_info = {
		.hcan = &hcan1,
	},
	.board_temperature_adc = &hadc1,
	.board_temperature_adc_rank = 0,
	.force_stop_port = in_4_GPIO_Port,
	.force_stop_pin = in_4_Pin,
	.force_stop_normal_state = GPIO_PIN_RESET,
	.door_port = in_2_GPIO_Port,
	.door_pin = in_2_Pin,
	.door_normal_state = GPIO_PIN_RESET,
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

int adc_value_helper(adc_value_type_t adc_value_type, void *ctx)
{
	int value = 0;

	switch(adc_value_type) {
		case ADC_VALUE_TYPE_BOARD_TEMPERATURE: {
			channels_info_t *channels_info = (channels_info_t *)ctx;
			adc_info_t *adc_info = NULL;
			uint16_t temperature_ad = 0;
			adc_info = get_or_alloc_adc_info(channels_info->channels_config->board_temperature_adc);
			OS_ASSERT(adc_info != NULL);
			temperature_ad = get_adc_value(adc_info, channels_info->channels_config->board_temperature_adc_rank);
			//debug("board temperature ad %d", channels_info->temperature_ad);
			value = get_ntc_temperature(10000, temperature_ad, 4095);
		}
		break;

		case ADC_VALUE_TYPE_CP_AD_VOLTAGE: {
			channel_info_t *channel_info = (channel_info_t *)ctx;
			adc_info_t *adc_info = NULL;
			uint16_t cp_ad = 0;
			adc_info = get_or_alloc_adc_info(channel_info->channel_config->cp_ad_adc);
			OS_ASSERT(adc_info != NULL);
			cp_ad = get_adc_value(adc_info, channel_info->channel_config->cp_ad_adc_rank);
			//debug("channel %d cp ad:%d", channel_info->channel_id, cp_ad);
			value = cp_ad * 3300 / 4096;//0v-1.2v 采样 0v-12v

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
