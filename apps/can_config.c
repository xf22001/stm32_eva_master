

/*================================================================
 *
 *
 *   文件名称：can_config.c
 *   创 建 者：肖飞
 *   创建日期：2020年04月17日 星期五 09时16分53秒
 *   修改日期：2020年04月29日 星期三 11时02分29秒
 *   描    述：
 *
 *================================================================*/
#include "can_config.h"
#include "os_utils.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

can_config_t can_config_can1 = {
	.type = CAN_TYPE_HAL,
	.hcan = &hcan1,
	.config_can = &hcan1,
	.filter_number = 0,
	.filter_fifo = CAN_FILTER_FIFO0,
	.filter_id = 0,
	.filter_mask_id = 0,
	.filter_rtr = 0,
	.filter_mask_rtr = 0,
	.filter_ext = 0,
	.filter_mask_ext = 0,
};

can_config_t can_config_can2 = {
	.type = CAN_TYPE_HAL,
	.hcan = &hcan2,
	.config_can = &hcan1,
	.filter_number = 14,
	.filter_fifo = CAN_FILTER_FIFO1,
	.filter_id = 0,
	.filter_mask_id = 0,
	.filter_rtr = 0,
	.filter_mask_rtr = 0,
	.filter_ext = 0,
	.filter_mask_ext = 0,
};

static can_config_t *can_config_sz[] = {
	&can_config_can1,
	&can_config_can2,
};

can_config_t *get_can_config(void *hcan)
{
	uint8_t i;
	can_config_t *can_config = NULL;
	can_config_t *can_config_item = NULL;

	for(i = 0; i < ARRAY_SIZE(can_config_sz); i++) {
		can_config_item = can_config_sz[i];

		if(hcan == can_config_item->hcan) {
			can_config = can_config_item;
			break;
		}
	}

	return can_config;
}
