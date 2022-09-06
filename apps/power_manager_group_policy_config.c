

/*================================================================
 *   
 *   
 *   文件名称：power_manager_group_policy_config.c
 *   创 建 者：肖飞
 *   创建日期：2022年07月22日 星期五 12时30分44秒
 *   修改日期：2022年09月06日 星期二 08时51分49秒
 *   描    述：
 *
 *================================================================*/
#include "power_manager_group_policy_chain.h"

#include "power_manager.h"
#include "main.h"

#include "log.h"

static relay_node_info_t relay_node_info_0_0 = {
	.relay_id = 0,
	.bind_node_id = {0, 1},
	.gpio_port = ka_1_GPIO_Port,//02km1/02km2
	.gpio_pin = ka_1_Pin,
	.gpio_port_fb = in_6_GPIO_Port,//02km1/02km2
	.gpio_pin_fb = in_6_Pin,
};

static relay_node_info_t *relay_node_info_0_sz[] = {
	&relay_node_info_0_0,
};

static power_manager_group_relay_info_t power_manager_group_relay_info_0 = {
	.power_manager_group_id = 0,
	.size = ARRAY_SIZE(relay_node_info_0_sz),
	.relay_node_info = relay_node_info_0_sz,
};

static power_manager_group_relay_info_t *power_manager_relay_info_sz[] = {
	&power_manager_group_relay_info_0,
};

relay_info_t relay_info = {
	.size = ARRAY_SIZE(power_manager_relay_info_sz),
	.power_manager_group_relay_info = power_manager_relay_info_sz,
};

static uint8_t power_module_group_id_0_0[] = {0};

static power_module_group_bind_node_t power_module_group_bind_node_0_0 = {
	.id = 0,
	.channel_id = 0,
	.power_module_group_id_sz = power_module_group_id_0_0,
	.power_module_group_id_size = ARRAY_SIZE(power_module_group_id_0_0),
};

static uint8_t power_module_group_id_0_1[] = {1};

static power_module_group_bind_node_t power_module_group_bind_node_0_1 = {
	.id = 1,
	.channel_id = 1,
	.power_module_group_id_sz = power_module_group_id_0_1,
	.power_module_group_id_size = ARRAY_SIZE(power_module_group_id_0_1),
};

static power_module_group_bind_node_t *power_module_group_bind_node_sz[] = {
	&power_module_group_bind_node_0_0,
	&power_module_group_bind_node_0_1,
};

static group_power_module_group_bind_info_t group_power_module_group_bind_info_0 = {
	.power_manager_group_id = 0,
	.size = ARRAY_SIZE(power_module_group_bind_node_sz),
	.power_module_group_bind_node = power_module_group_bind_node_sz,
};

static group_power_module_group_bind_info_t *group_power_module_group_bind_info_sz[] = {
	&group_power_module_group_bind_info_0,
};

groups_power_module_group_bind_info_t groups_power_module_group_bind_info = {
	.size = ARRAY_SIZE(group_power_module_group_bind_info_sz),
	.group_power_module_group_bind_info = group_power_module_group_bind_info_sz,
};

static channel_relay_fb_node_info_t channel_relay_fb_node_info_0 = {
	.channel_id = 0,
	//.gpio_port_fb = in_5_GPIO_Port,
	//.gpio_pin_fb = in_5_Pin,
	.hadc = NULL,
	.rank = 0,
};

static channel_relay_fb_node_info_t channel_relay_fb_node_info_1 = {
	.channel_id = 1,
	//.gpio_port_fb = in_5_GPIO_Port,
	//.gpio_pin_fb = in_5_Pin,
	.hadc = NULL,
	.rank = 1,
};

static channel_relay_fb_node_info_t *channel_relay_fb_node_info_sz[] = {
	&channel_relay_fb_node_info_0,
	&channel_relay_fb_node_info_1,
};

static power_manager_group_channel_relay_fb_info_t power_manager_group_channel_relay_fb_info_0 = {
	.power_manager_group_id = 0,
	.size = ARRAY_SIZE(channel_relay_fb_node_info_sz),
	.channel_relay_fb_node_info = channel_relay_fb_node_info_sz,
};

static power_manager_group_channel_relay_fb_info_t *power_manager_group_channel_relay_fb_info_sz[] = {
	&power_manager_group_channel_relay_fb_info_0,
};

channel_relay_fb_info_t channel_relay_fb_info = {
	.size = ARRAY_SIZE(power_manager_group_channel_relay_fb_info_sz),
	.power_manager_group_channel_relay_fb_info = power_manager_group_channel_relay_fb_info_sz,
};

void power_manager_restore_config(channels_info_t *channels_info)
{
	int i;
	int j;

	channels_config_t *channels_config = channels_info->channels_config;
	channels_settings_t *channels_settings = &channels_info->channels_settings;
	power_manager_settings_t *power_manager_settings = &channels_settings->power_manager_settings;

	channels_settings->power_module_settings.power_module_type = channels_config->power_module_config.power_module_default_type;
	channels_settings->power_module_settings.rate_current = 21;
	channels_settings->power_manager_settings.type = channels_config->power_manager_config.power_manager_default_type;
	channels_settings->module_max_output_voltage = 10000;
	channels_settings->module_min_output_voltage = 2000;
	channels_settings->module_max_output_current = 1000;
	channels_settings->module_min_output_current = 3;
	channels_settings->module_max_input_voltage = 0;//2530;
	channels_settings->module_min_input_voltage = 0;//1870;
	channels_settings->module_max_output_power = 20000;
	channels_settings->channels_max_output_power = 240000;
	channels_settings->power_manager_group_policy = POWER_MANAGER_GROUP_POLICY_PRIORITY;

	power_manager_settings->power_manager_group_number = 1;

	debug("power_manager_group_number:%d", power_manager_settings->power_manager_group_number);

	for(i = 0; i < power_manager_settings->power_manager_group_number; i++) {
		power_manager_group_settings_t *power_manager_group_settings = &power_manager_settings->power_manager_group_settings[i];

		power_manager_group_settings->channel_number = 2;
		power_manager_group_settings->relay_board_number_per_channel = 0;

		for(j = 0; j < power_manager_group_settings->relay_board_number_per_channel; j++) {
			power_manager_group_settings->slot_per_relay_board[j] = 6;
		}

		power_manager_group_settings->power_module_group_number = 2;

		for(j = 0; j < power_manager_group_settings->power_module_group_number; j++) {
			power_module_group_settings_t *power_module_group_settings = &power_manager_group_settings->power_module_group_settings[j];
			power_module_group_settings->power_module_number = 6;
		}
	}
}

void channel_info_reset_default_config(channel_info_t *channel_info)
{
	channel_config_t *channel_config = channel_info->channel_config;
	channel_settings_t *channel_settings = &channel_info->channel_settings;

	memset(channel_settings, 0, sizeof(channel_settings_t));

	channel_settings->channel_type = channel_config->channel_type;
	channel_settings->charger_settings.charger_type = channel_config->charger_config.charger_type;
	channel_settings->energy_meter_settings.type = channel_config->energy_meter_config.default_type;
	channel_settings->energy_meter_settings.request_addr = channel_config->energy_meter_config.request_addr;
	channel_settings->energy_meter_settings.slot = channel_config->energy_meter_config.slot;
	channel_settings->energy_meter_settings.energy_accuracy = channel_config->energy_meter_config.energy_accuracy;
	channel_settings->energy_meter_settings.voltage_accuracy = channel_config->energy_meter_config.voltage_accuracy;
	channel_settings->energy_meter_settings.curent_accuracy = channel_config->energy_meter_config.curent_accuracy;
	channel_settings->function_board_type = channel_config->function_board_config.default_type;

	channel_settings->max_output_power = 6000000;

	channel_settings->max_output_voltage = 7500;
	channel_settings->min_output_voltage = 2000;
	channel_settings->max_output_current = 5000;
	channel_settings->min_output_current = 3;

	channel_settings->multi_charge_settings.group = 0;
	channel_settings->multi_charge_settings.id = channel_info->channel_id;
}
