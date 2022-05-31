

/*================================================================
 *
 *
 *   文件名称：power_manager_group_policy_handler.c
 *   创 建 者：肖飞
 *   创建日期：2021年11月30日 星期二 15时07分16秒
 *   修改日期：2022年05月31日 星期二 14时15分13秒
 *   描    述：
 *
 *================================================================*/
#include "power_manager.h"

#include "main.h"

#include "log.h"

void power_manager_restore_config(channels_info_t *channels_info)
{
	int i;
	int j;

	channels_config_t *channels_config = channels_info->channels_config;
	channels_settings_t *channels_settings = &channels_info->channels_settings;
	power_manager_settings_t *power_manager_settings = &channels_settings->power_manager_settings;

	power_manager_settings->power_manager_group_number = 1;

	for(i = 0; i < power_manager_settings->power_manager_group_number; i++) {
		power_manager_group_settings_t *power_manager_group_settings = &power_manager_settings->power_manager_group_settings[i];
		power_manager_group_settings->channel_number = channels_config->channel_number;
		power_manager_group_settings->power_module_group_number = 1;

		for(j = 0; j < power_manager_group_settings->power_module_group_number; j++) {
			power_module_group_settings_t *power_module_group_settings = &power_manager_group_settings->power_module_group_settings[j];
			power_module_group_settings->power_module_number = 1;
		}
	}
}

typedef struct {
	uint8_t relay_id;
	uint8_t channel_id[2];
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	GPIO_TypeDef *gpio_port_fb;
	uint16_t gpio_pin_fb;
} relay_node_info_t;

typedef struct {
	uint8_t power_manager_group_id;
	uint8_t size;
	relay_node_info_t **relay_node_info;
} power_manager_group_relay_info_t;

typedef struct {
	uint8_t size;
	power_manager_group_relay_info_t **power_manager_group_relay_info;
} relay_info_t;

static relay_node_info_t relay_node_info_0_0 = {
	.relay_id = 0,
	.channel_id = {0, 1},
	.gpio_port = relay_4_GPIO_Port,//02km9/02km10
	.gpio_pin = relay_4_Pin,
	.gpio_port_fb = in_6_GPIO_Port,//02km9/02km10/02km11/02km12/02km13/02km14/02km14/02km15/02km16/
	.gpio_pin_fb = in_6_Pin,
};

static relay_node_info_t relay_node_info_0_1 = {
	.relay_id = 1,
	.channel_id = {1, 2},
	.gpio_port = out_7_GPIO_Port,//02km13/02km14
	.gpio_pin = out_7_Pin,
	.gpio_port_fb = in_6_GPIO_Port,//02km9/02km10/02km11/02km12/02km13/02km14/02km14/02km15/02km16/
	.gpio_pin_fb = in_6_Pin,
};

static relay_node_info_t relay_node_info_0_2 = {
	.relay_id = 2,
	.channel_id = {2, 3},
	.gpio_port = out_6_GPIO_Port,//02km13/02km14
	.gpio_pin = out_6_Pin,
	.gpio_port_fb = in_6_GPIO_Port,//02km9/02km10/02km11/02km12/02km13/02km14/02km14/02km15/02km16/
	.gpio_pin_fb = in_6_Pin,
};

static relay_node_info_t relay_node_info_0_3 = {
	.relay_id = 3,
	.channel_id = {3, 0},
	.gpio_port = out_5_GPIO_Port,//02km15/02km16
	.gpio_pin = out_5_Pin,
	.gpio_port_fb = in_6_GPIO_Port,//02km9/02km10/02km11/02km12/02km13/02km14/02km14/02km15/02km16/
	.gpio_pin_fb = in_6_Pin,
};

static relay_node_info_t *relay_node_info_0_sz[] = {
	&relay_node_info_0_0,
	&relay_node_info_0_1,
	&relay_node_info_0_2,
	&relay_node_info_0_3,
};

static power_manager_group_relay_info_t power_manager_group_relay_info_0 = {
	.power_manager_group_id = 0,
	.size = ARRAY_SIZE(relay_node_info_0_sz),
	.relay_node_info = relay_node_info_0_sz,
};

static power_manager_group_relay_info_t *power_manager_relay_info_sz[] = {
	&power_manager_group_relay_info_0,
};

static relay_info_t relay_info = {
	.size = ARRAY_SIZE(power_manager_relay_info_sz),
	.power_manager_group_relay_info = power_manager_relay_info_sz,
};

static relay_node_info_t *get_relay_node_info_by_channel_id(uint8_t power_manager_group_id, uint8_t channel_id_a, uint8_t channel_id_b)
{
	relay_node_info_t *relay_node_info = NULL;
	power_manager_group_relay_info_t *power_manager_group_relay_info = NULL;
	int i;

	OS_ASSERT(channel_id_a != channel_id_b);

	for(i = 0; i < relay_info.size; i++) {
		power_manager_group_relay_info_t *power_manager_group_relay_info_item = relay_info.power_manager_group_relay_info[i];

		if(power_manager_group_relay_info_item->power_manager_group_id == power_manager_group_id) {
			power_manager_group_relay_info = power_manager_group_relay_info_item;
			break;
		}
	}

	if(power_manager_group_relay_info == NULL) {
		return relay_node_info;
	}

	for(i = 0; i < power_manager_group_relay_info->size; i++) {
		relay_node_info_t *relay_node_info_item = power_manager_group_relay_info->relay_node_info[i];
		int j;
		uint8_t found = 1;

		for(j = 0; j < 2; j++) {
			if(channel_id_a == relay_node_info_item->channel_id[j]) {
				break;
			}
		}

		if(j == 2) {
			found = 0;
			continue;
		}

		for(j = 0; j < 2; j++) {
			if(channel_id_b == relay_node_info_item->channel_id[j]) {
				break;
			}
		}

		if(j == 2) {
			found = 0;
			continue;
		}

		if(found != 0) {
			relay_node_info = relay_node_info_item;
			break;
		}
	}

	return relay_node_info;
}

static relay_node_info_t *get_relay_node_info_by_relay_id(uint8_t power_manager_group_id, uint8_t relay_id)
{
	relay_node_info_t *relay_node_info = NULL;
	power_manager_group_relay_info_t *power_manager_group_relay_info = NULL;
	int i;

	for(i = 0; i < relay_info.size; i++) {
		power_manager_group_relay_info_t *power_manager_group_relay_info_item = relay_info.power_manager_group_relay_info[i];

		if(power_manager_group_relay_info_item->power_manager_group_id == power_manager_group_id) {
			power_manager_group_relay_info = power_manager_group_relay_info_item;
			break;
		}
	}

	if(power_manager_group_relay_info == NULL) {
		return relay_node_info;
	}

	for(i = 0; i < power_manager_group_relay_info->size; i++) {
		relay_node_info_t *relay_node_info_item = power_manager_group_relay_info->relay_node_info[i];

		if(relay_id == relay_node_info_item->relay_id) {
			relay_node_info = relay_node_info_item;
			break;
		}
	}

	return relay_node_info;
}

//一个通道绑定的电源模块组
typedef struct {
	uint8_t channel_id;
	uint8_t group_id[2];
} channel_power_module_group_bind_item_info_t;

//一个电源管理组内的通道绑定信息
typedef struct {
	uint8_t group_id;
	uint8_t size;
	channel_power_module_group_bind_item_info_t **channel_power_module_group_bind_item_info;
} group_channel_power_module_group_bind_info_t;

//所有电源管理组信息
typedef struct {
	uint8_t size;
	group_channel_power_module_group_bind_info_t **group_channel_power_module_group_bind_info;
} groups_channel_power_module_group_bind_info_t;

static channel_power_module_group_bind_item_info_t channel_power_module_group_bind_item_info_0_0 = {
	.channel_id = 0,
	.group_id = {0, 1},
};

static channel_power_module_group_bind_item_info_t channel_power_module_group_bind_item_info_0_1 = {
	.channel_id = 1,
	.group_id = {2, 3},
};

static channel_power_module_group_bind_item_info_t channel_power_module_group_bind_item_info_0_2 = {
	.channel_id = 2,
	.group_id = {4, 5},
};

static channel_power_module_group_bind_item_info_t channel_power_module_group_bind_item_info_0_3 = {
	.channel_id = 3,
	.group_id = {6, 7},
};

static channel_power_module_group_bind_item_info_t *channel_power_module_group_bind_item_info_sz[] = {
	&channel_power_module_group_bind_item_info_0_0,
	&channel_power_module_group_bind_item_info_0_1,
	&channel_power_module_group_bind_item_info_0_2,
	&channel_power_module_group_bind_item_info_0_3,
};

static group_channel_power_module_group_bind_info_t group_channel_power_module_group_bind_info_0 = {
	.group_id = 0,
	.size = ARRAY_SIZE(channel_power_module_group_bind_item_info_sz),
	.channel_power_module_group_bind_item_info = channel_power_module_group_bind_item_info_sz,
};

static group_channel_power_module_group_bind_info_t *group_channel_power_module_group_bind_info_sz[] = {
	&group_channel_power_module_group_bind_info_0,
};

static groups_channel_power_module_group_bind_info_t groups_channel_power_module_group_bind_info = {
	.size = ARRAY_SIZE(group_channel_power_module_group_bind_info_sz),
	.group_channel_power_module_group_bind_info = group_channel_power_module_group_bind_info_sz,
};

static channel_power_module_group_bind_item_info_t *get_channel_power_module_group_bind_item_info(uint8_t group_id, uint8_t channel_id)
{
	int i;
	uint8_t found = 0;
	channel_power_module_group_bind_item_info_t *channel_power_module_group_bind_item_info = NULL;
	group_channel_power_module_group_bind_info_t *group_channel_power_module_group_bind_info = NULL;

	for(i = 0; i < groups_channel_power_module_group_bind_info.size; i++) {
		group_channel_power_module_group_bind_info_t *group_channel_power_module_group_bind_info_item = groups_channel_power_module_group_bind_info.group_channel_power_module_group_bind_info[i];

		if(group_channel_power_module_group_bind_info_item->group_id == group_id) {
			group_channel_power_module_group_bind_info = group_channel_power_module_group_bind_info_item;
			found = 1;
			break;
		}
	}

	if(found == 0) {
		return channel_power_module_group_bind_item_info;
	}

	found = 0;

	for(i = 0; i < group_channel_power_module_group_bind_info->size; i++) {
		channel_power_module_group_bind_item_info_t *channel_power_module_group_bind_item_info_item = group_channel_power_module_group_bind_info->channel_power_module_group_bind_item_info[i];

		if(channel_power_module_group_bind_item_info_item->channel_id == channel_id) {
			channel_power_module_group_bind_item_info = channel_power_module_group_bind_item_info_item;
			found = 1;
			break;
		}
	}

	return channel_power_module_group_bind_item_info;
}

typedef struct {
	bitmap_t *relay_map;//链式搭接开关状态位
} power_manager_group_policy_ctx_t;

static int _init(void *_power_manager_info)
{
	int ret = 0;
	int i;
	power_manager_info_t *power_manager_info = (power_manager_info_t *)_power_manager_info;
	//channels_info_t *channels_info = power_manager_info->channels_info;
	power_manager_info->power_manager_group_policy_ctx = (power_manager_group_policy_ctx_t *)os_calloc(power_manager_info->power_manager_group_number, sizeof(power_manager_group_policy_ctx_t));
	OS_ASSERT(power_manager_info->power_manager_group_policy_ctx != NULL);

	for(i = 0; i < power_manager_info->power_manager_group_number; i++) {
		power_manager_group_relay_info_t *power_manager_group_relay_info = relay_info.power_manager_group_relay_info[i];
		power_manager_group_policy_ctx_t *power_manager_group_policy_ctx = (power_manager_group_policy_ctx_t *)power_manager_info->power_manager_group_policy_ctx + i;
		power_manager_group_policy_ctx->relay_map = alloc_bitmap(power_manager_group_relay_info->size);
		OS_ASSERT(power_manager_group_policy_ctx->relay_map != NULL);
	}

	return ret;
}

static int _deinit(void *_power_manager_info)
{
	int ret = 0;
	int i;
	power_manager_info_t *power_manager_info = (power_manager_info_t *)_power_manager_info;
	//channels_info_t *channels_info = power_manager_info->channels_info;

	OS_ASSERT(power_manager_info->power_manager_group_policy_ctx != NULL);

	for(i = 0; i < power_manager_info->power_manager_group_number; i++) {
		power_manager_group_policy_ctx_t *power_manager_group_policy_ctx = (power_manager_group_policy_ctx_t *)power_manager_info->power_manager_group_policy_ctx + i;
		OS_ASSERT(power_manager_group_policy_ctx->relay_map != NULL);
		free_bitmap(power_manager_group_policy_ctx->relay_map);
	}

	return ret;
}

static int _channel_start(void *_power_manager_channel_info)
{
	int ret = 0;
	power_manager_channel_info_t *power_manager_channel_info = (power_manager_channel_info_t *)_power_manager_channel_info;
	debug("power manager channel %d start", power_manager_channel_info->id);
	return ret;
}

static int _channel_charging(void *_power_manager_channel_info)
{
	int ret = 0;
	//power_manager_channel_info_t *power_manager_channel_info = (power_manager_channel_info_t *)_power_manager_channel_info;
	//debug("power manager channel %d charging", power_manager_channel_info->id);
	return ret;
}

static void clean_up_relay_map(power_manager_group_info_t *power_manager_group_info)
{
	power_manager_info_t *power_manager_info = (power_manager_info_t *)power_manager_group_info->power_manager_info;
	//channels_info_t *channels_info = power_manager_info->channels_info;
	power_manager_group_policy_ctx_t *power_manager_group_policy_ctx = (power_manager_group_policy_ctx_t *)power_manager_info->power_manager_group_policy_ctx + power_manager_group_info->id;
	int i;

	for(i = 0; i < power_manager_group_info->channel_number; i++) {
		set_bitmap_value(power_manager_group_policy_ctx->relay_map, i, 0);
	}
}

static void channel_info_deactive_power_module_group(power_manager_channel_info_t *power_manager_channel_info)
{
	struct list_head *pos;
	struct list_head *n;
	struct list_head *head;
	power_manager_group_info_t *power_manager_group_info = power_manager_channel_info->power_manager_group_info;

	head = &power_manager_channel_info->power_module_group_list;

	list_for_each_safe(pos, n, head) {
		power_module_group_info_t *power_module_group_info = list_entry(pos, power_module_group_info_t, list);
		power_module_item_info_t *power_module_item_info;
		struct list_head *head1 = &power_module_group_info->power_module_item_list;
		list_for_each_entry(power_module_item_info, head1, power_module_item_info_t, list) {
			power_module_item_info->status.state = POWER_MODULE_ITEM_STATE_PREPARE_DEACTIVE;
		}
		list_move_tail(&power_module_group_info->list, &power_manager_group_info->power_module_group_deactive_list);
	}
}

static void free_power_module_group_for_stop_channel(power_manager_group_info_t *power_manager_group_info)
{
	power_manager_channel_info_t *power_manager_channel_info;
	struct list_head *head;

	head = &power_manager_group_info->channel_deactive_list;

	list_for_each_entry(power_manager_channel_info, head, power_manager_channel_info_t, list) {
		channel_info_deactive_power_module_group(power_manager_channel_info);
	}
}

static void channel_info_deactive_unneeded_power_module_group_average(power_manager_channel_info_t *power_manager_channel_info)//POWER_MODULE_POLICY_PRIORITY
{
	struct list_head *pos;
	struct list_head *n;
	struct list_head *head;
	struct list_head list_unneeded_power_module_group = LIST_HEAD_INIT(list_unneeded_power_module_group);
	power_manager_group_info_t *power_manager_group_info = power_manager_channel_info->power_manager_group_info;
	channel_power_module_group_bind_item_info_t *channel_power_module_group_bind_item_info;
	int i;

	head = &power_manager_channel_info->power_module_group_list;

	list_for_each_safe(pos, n, head) {
		power_module_group_info_t *power_module_group_info = list_entry(pos, power_module_group_info_t, list);
		list_move_tail(&power_module_group_info->list, &list_unneeded_power_module_group);
	}

	//恢复该枪独占的模块组
	channel_power_module_group_bind_item_info = get_channel_power_module_group_bind_item_info(power_manager_group_info->id, power_manager_channel_info->id);

	for(i = 0; i < ARRAY_SIZE(channel_power_module_group_bind_item_info->group_id); i++) {
		power_module_group_info_t *power_module_group_info = power_manager_group_info->power_module_group_info + channel_power_module_group_bind_item_info->group_id[i];

		if(list_contain(&power_module_group_info->list, &list_unneeded_power_module_group) == 0) {
			//恢复模块归属
			list_move_tail(&power_module_group_info->list, &power_manager_channel_info->power_module_group_list);
			debug("add power module group %d to channel %d", power_module_group_info->id, power_manager_channel_info->id);
		}
	}

	head = &list_unneeded_power_module_group;

	//清理需要关闭的模块
	list_for_each_safe(pos, n, head) {
		power_module_group_info_t *power_module_group_info = list_entry(pos, power_module_group_info_t, list);
		power_module_item_info_t *power_module_item_info;
		struct list_head *head1 = &power_module_group_info->power_module_item_list;
		list_for_each_entry(power_module_item_info, head1, power_module_item_info_t, list) {
			power_module_item_info->status.state = POWER_MODULE_ITEM_STATE_PREPARE_DEACTIVE;
		}
		list_move_tail(&power_module_group_info->list, &power_manager_group_info->power_module_group_deactive_list);
		debug("remove power module group %d from channel %d", power_module_group_info->id, power_manager_channel_info->id);
	}
}

static void free_power_module_group_for_active_channel_average(power_manager_group_info_t *power_manager_group_info)//POWER_MODULE_POLICY_PRIORITY
{
	power_manager_channel_info_t *power_manager_channel_info;
	struct list_head *head;

	head = &power_manager_group_info->channel_active_list;

	list_for_each_entry(power_manager_channel_info, head, power_manager_channel_info_t, list) {
		channel_info_deactive_unneeded_power_module_group_average(power_manager_channel_info);
	}
}

static int _free(void *_power_manager_group_info)
{
	int ret = 0;
	power_manager_group_info_t *power_manager_group_info = (power_manager_group_info_t *)_power_manager_group_info;

	debug("power manager group %d free", power_manager_group_info->id);

	//清理继电器
	clean_up_relay_map(power_manager_group_info);

	//链式分配模块间会相互牵扯,释放所有模块,重新计算
	//释放要停机通道的模块
	free_power_module_group_for_stop_channel(power_manager_group_info);
	//释放多余模块,并恢复需要的继电器
	free_power_module_group_for_active_channel_average(power_manager_group_info);

	return ret;
}

static void power_manager_channel_info_assign_one_power_module_group_agerage(power_manager_channel_info_t *power_manager_channel_info)
{
	power_manager_group_info_t *power_manager_group_info = (power_manager_group_info_t *)power_manager_channel_info->power_manager_group_info;
	power_manager_info_t *power_manager_info = (power_manager_info_t *)power_manager_group_info->power_manager_info;
	power_manager_group_policy_ctx_t *power_manager_group_policy_ctx = (power_manager_group_policy_ctx_t *)power_manager_info->power_manager_group_policy_ctx + power_manager_group_info->id;
	channels_info_t *channels_info = power_manager_info->channels_info;
	struct list_head *head;
	power_manager_channel_info_t *power_manager_channel_info_item;
	power_manager_channel_info_t *power_manager_channel_info_item_prev;
	channel_power_module_group_bind_item_info_t *channel_power_module_group_bind_item_info;
	int i;

	//恢复该枪独占的模块组
	channel_power_module_group_bind_item_info = get_channel_power_module_group_bind_item_info(power_manager_group_info->id, power_manager_channel_info->id);

	for(i = 0; i < ARRAY_SIZE(channel_power_module_group_bind_item_info->group_id); i++) {
		power_module_group_info_t *power_module_group_info = power_manager_group_info->power_module_group_info + channel_power_module_group_bind_item_info->group_id[i];

		if(list_contain(&power_module_group_info->list, &power_manager_group_info->power_module_group_idle_list) == 0) {
			power_module_item_info_t *power_module_item_info;
			head = &power_module_group_info->power_module_item_list;
			list_for_each_entry(power_module_item_info, head, power_module_item_info_t, list) {
				if(power_module_item_info->status.state != POWER_MODULE_ITEM_STATE_IDLE) {
					debug("power module state is not idle:%s!!!", get_power_module_item_state_des(power_module_item_info->status.state));
				}

				power_module_item_info->status.state = POWER_MODULE_ITEM_STATE_PREPARE_ACTIVE;
			}
			power_module_group_info->power_manager_channel_info = power_manager_channel_info;
			list_move_tail(&power_module_group_info->list, &power_manager_channel_info->power_module_group_list);
			debug("assign module group %d to channel %d", power_module_group_info->id, power_manager_channel_info->id);
		}

	}

	//left search
	power_manager_channel_info_item_prev = power_manager_channel_info;
	power_manager_channel_info_item = power_manager_channel_info;

	while(power_manager_channel_info_item != NULL) {
		uint8_t next_channel_id;
		uint8_t find_power_module_group = 0;
		uint8_t assign_power_module_group = 0;
		relay_node_info_t *relay_node_info;
		power_manager_group_info_t *power_manager_group_info_item;

		if(power_manager_channel_info_item->id == 0) {
			next_channel_id = channels_info->channel_number - 1;
		} else {
			next_channel_id = power_manager_channel_info_item->id - 1;
		}

		power_manager_channel_info_item = power_manager_info->power_manager_channel_info + next_channel_id;
		power_manager_group_info_item = (power_manager_group_info_t *)power_manager_channel_info_item->power_manager_group_info;

		if(power_manager_group_info_item->id != power_manager_group_info->id) {
			debug("power_manager_group_info_item->id:%d, power_manager_group_info->id:%d", power_manager_group_info_item->id, power_manager_group_info->id);
			continue;
		}

		OS_ASSERT(power_manager_group_info_item == power_manager_group_info);

		if(list_contain(&power_manager_channel_info_item->list, &power_manager_group_info->channel_active_list) == 0) {
			debug("power_manager_channel_info_item->id:%d", power_manager_channel_info_item->id);
			break;
		}

		channel_power_module_group_bind_item_info = get_channel_power_module_group_bind_item_info(power_manager_group_info->id, next_channel_id);

		for(i = 0; i < ARRAY_SIZE(channel_power_module_group_bind_item_info->group_id); i++) {
			power_module_group_info_t *power_module_group_info = power_manager_group_info->power_module_group_info + channel_power_module_group_bind_item_info->group_id[i];

			//当前模块组已存在于该枪
			if(list_contain(&power_module_group_info->list, &power_manager_channel_info->power_module_group_list) == 0) {
				find_power_module_group = 1;
			}

			//当前模块组在空闲列表
			if(list_contain(&power_module_group_info->list, &power_manager_group_info->power_module_group_idle_list) == 0) {
				power_module_item_info_t *power_module_item_info;
				find_power_module_group = 1;
				assign_power_module_group = 1;
				//set relay power_manager_channel_info_item_prev---power_manager_channel_info_item, by id
				relay_node_info = get_relay_node_info_by_channel_id(power_manager_group_info->id,
				                  power_manager_channel_info_item_prev->id,
				                  power_manager_channel_info_item->id);
				OS_ASSERT(relay_node_info != NULL);
				set_bitmap_value(power_manager_group_policy_ctx->relay_map, relay_node_info->relay_id, 1);

				head = &power_module_group_info->power_module_item_list;
				list_for_each_entry(power_module_item_info, head, power_module_item_info_t, list) {
					if(power_module_item_info->status.state != POWER_MODULE_ITEM_STATE_IDLE) {
						debug("power module state is not idle:%s!!!", get_power_module_item_state_des(power_module_item_info->status.state));
					}

					power_module_item_info->status.state = POWER_MODULE_ITEM_STATE_PREPARE_ACTIVE;
				}
				power_module_group_info->power_manager_channel_info = power_manager_channel_info;
				list_move_tail(&power_module_group_info->list, &power_manager_channel_info->power_module_group_list);
				debug("assign module group %d to channel %d", power_module_group_info->id, power_manager_channel_info->id);
			}
		}

		if(find_power_module_group == 0) {
			debug("");
			break;
		}

		if(assign_power_module_group != 0) {
			debug("");
			return;
		}

		power_manager_channel_info_item_prev = power_manager_channel_info_item;
	}

	//right search
	power_manager_channel_info_item_prev = power_manager_channel_info;
	power_manager_channel_info_item = power_manager_channel_info;

	while(power_manager_channel_info_item != NULL) {
		uint8_t next_channel_id;
		uint8_t find_power_module_group = 0;
		uint8_t assign_power_module_group = 0;
		relay_node_info_t *relay_node_info;
		power_manager_group_info_t *power_manager_group_info_item;

		if(power_manager_channel_info_item->id == (channels_info->channel_number - 1)) {
			next_channel_id = 0;
		} else {
			next_channel_id = power_manager_channel_info_item->id + 1;
		}

		power_manager_channel_info_item = power_manager_info->power_manager_channel_info + next_channel_id;
		power_manager_group_info_item = (power_manager_group_info_t *)power_manager_channel_info_item->power_manager_group_info;

		if(power_manager_group_info_item->id != power_manager_group_info->id) {
			debug("power_manager_group_info_item->id:%d, power_manager_group_info->id:%d", power_manager_group_info_item->id, power_manager_group_info->id);
			continue;
		}

		OS_ASSERT(power_manager_group_info_item == power_manager_group_info);

		if(list_contain(&power_manager_channel_info_item->list, &power_manager_group_info->channel_active_list) == 0) {
			debug("power_manager_channel_info_item->id:%d", power_manager_channel_info_item->id);
			break;
		}

		channel_power_module_group_bind_item_info = get_channel_power_module_group_bind_item_info(power_manager_group_info->id, next_channel_id);

		for(i = 0; i < ARRAY_SIZE(channel_power_module_group_bind_item_info->group_id); i++) {
			power_module_group_info_t *power_module_group_info = power_manager_group_info->power_module_group_info + channel_power_module_group_bind_item_info->group_id[i];

			//当前模块组已存在于该枪
			if(list_contain(&power_module_group_info->list, &power_manager_channel_info->power_module_group_list) == 0) {
				find_power_module_group = 1;
			}

			//当前模块组在空闲列表
			if(list_contain(&power_module_group_info->list, &power_manager_group_info->power_module_group_idle_list) == 0) {
				power_module_item_info_t *power_module_item_info;
				find_power_module_group = 1;
				assign_power_module_group = 1;
				//set relay power_manager_channel_info_item_prev---power_manager_channel_info_item, by id
				relay_node_info = get_relay_node_info_by_channel_id(power_manager_group_info->id,
				                  power_manager_channel_info_item_prev->id,
				                  power_manager_channel_info_item->id);
				OS_ASSERT(relay_node_info != NULL);
				set_bitmap_value(power_manager_group_policy_ctx->relay_map, relay_node_info->relay_id, 1);

				head = &power_module_group_info->power_module_item_list;
				list_for_each_entry(power_module_item_info, head, power_module_item_info_t, list) {
					if(power_module_item_info->status.state != POWER_MODULE_ITEM_STATE_IDLE) {
						debug("power module state is not idle:%s!!!", get_power_module_item_state_des(power_module_item_info->status.state));
					}

					power_module_item_info->status.state = POWER_MODULE_ITEM_STATE_PREPARE_ACTIVE;
				}
				power_module_group_info->power_manager_channel_info = power_manager_channel_info;
				list_move_tail(&power_module_group_info->list, &power_manager_channel_info->power_module_group_list);
				debug("assign module group %d to channel %d", power_module_group_info->id, power_manager_channel_info->id);
			}
		}

		if(find_power_module_group == 0) {
			debug("");
			break;
		}

		if(assign_power_module_group != 0) {
			debug("");
			return;
		}

		power_manager_channel_info_item_prev = power_manager_channel_info_item;
		debug("");
	}
}

static void active_power_manager_group_info_power_module_group_assign_average(power_manager_group_info_t *power_manager_group_info)
{
	power_manager_channel_info_t *power_manager_channel_info;
	struct list_head *head;

	while(list_size(&power_manager_group_info->power_module_group_idle_list) > 0) {//没有多余的模块需要分配了,退出
		head = &power_manager_group_info->channel_active_list;
		debug("power manager group %d power module group number:%d", power_manager_group_info->id, list_size(&power_manager_group_info->power_module_group_idle_list));

		list_for_each_entry(power_manager_channel_info, head, power_manager_channel_info_t, list) {
			power_manager_channel_info_assign_one_power_module_group_agerage(power_manager_channel_info);
		}
	}
}

static int _assign(void *_power_manager_group_info)
{
	int ret = 0;
	power_manager_group_info_t *power_manager_group_info = (power_manager_group_info_t *)_power_manager_group_info;
	debug("power manager group %d assign", power_manager_group_info->id);
	//充电中的枪数
	uint8_t active_channel_count;

	//获取需要充电的枪数
	active_channel_count = list_size(&power_manager_group_info->channel_active_list);
	debug("active_channel_count:%d", active_channel_count);

	if(active_channel_count == 0) {//如果没有枪需要充电,不分配
		return ret;
	}

	active_power_manager_group_info_power_module_group_assign_average(power_manager_group_info);
	return ret;
}

static void action_relay_map(power_manager_group_info_t *power_manager_group_info)
{
	int i;
	power_manager_info_t *power_manager_info = (power_manager_info_t *)power_manager_group_info->power_manager_info;
	power_manager_group_policy_ctx_t *power_manager_group_policy_ctx = (power_manager_group_policy_ctx_t *)power_manager_info->power_manager_group_policy_ctx + power_manager_group_info->id;
	bitmap_t *relay_map = power_manager_group_policy_ctx->relay_map;

	for(i = 0; i < relay_map->size; i++) {
		relay_node_info_t *relay_node_info = get_relay_node_info_by_relay_id(power_manager_group_info->id, i);
		GPIO_PinState state = GPIO_PIN_RESET;

		if(get_bitmap_value(relay_map, i) != 0) {
			state = GPIO_PIN_SET;
		}

		HAL_GPIO_WritePin(relay_node_info->gpio_port, relay_node_info->gpio_pin, state);
	}
}

static int _config(void *_power_manager_group_info)
{
	int ret = 0;
	power_manager_group_info_t *power_manager_group_info = (power_manager_group_info_t *)_power_manager_group_info;
	debug("power manager group %d config", power_manager_group_info->id);
	action_relay_map(power_manager_group_info);
	return ret;
}

static int _sync(void *_power_manager_group_info)
{
	int ret = 0;
	power_manager_group_info_t *power_manager_group_info = (power_manager_group_info_t *)_power_manager_group_info;
	debug("power manager group %d sync", power_manager_group_info->id);
	return ret;
}

static power_manager_group_policy_handler_t power_manager_group_policy_handler_0 = {
	.policy = 0,
	.init = _init,
	.deinit = _deinit,
	.channel_start = _channel_start,
	.channel_charging = _channel_charging,
	.free = _free,
	.assign = _assign,
	.config = _config,
	.sync = _sync,
};

static power_manager_group_policy_handler_t *power_manager_group_policy_handler_sz[] = {
	&power_manager_group_policy_handler_0,
};

power_manager_group_policy_handler_t *get_power_manager_group_policy_handler(uint8_t policy)
{
	int i;
	power_manager_group_policy_handler_t *power_manager_group_policy_handler = NULL;

	for(i = 0; i < ARRAY_SIZE(power_manager_group_policy_handler_sz); i++) {
		power_manager_group_policy_handler_t *power_manager_group_policy_handler_item = power_manager_group_policy_handler_sz[i];

		if(power_manager_group_policy_handler_item->policy == policy) {
			power_manager_group_policy_handler = power_manager_group_policy_handler_item;
		}
	}

	return power_manager_group_policy_handler;
}
