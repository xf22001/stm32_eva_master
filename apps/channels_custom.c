

/*================================================================
 *
 *
 *   文件名称：channels_custom.c
 *   创 建 者：肖飞
 *   创建日期：2022年08月31日 星期三 08时53分08秒
 *   修改日期：2022年08月31日 星期三 15时28分47秒
 *   描    述：
 *
 *================================================================*/
#include "channel.h"
#include "main.h"

#include "log.h"

static void handle_channels_idle_led(channels_info_t *channels_info)
{
	int i;
	uint8_t idle = 1;
	GPIO_PinState state;

	for(i = 0; i < channels_info->channel_number; i++) {
		channel_info_t *channel_info = channels_info->channel_info + i;

		if(channel_info->state != CHANNEL_STATE_IDLE) {
			idle = 0;
			break;
		}
	}

	state = (idle == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(out_4_GPIO_Port, out_4_Pin, state);
}

static GPIO_PinState get_fault_led_state(void)
{
	uint32_t ticks = osKernelSysTick();
	static uint32_t stamp = 0;
	static GPIO_PinState state = GPIO_PIN_RESET;

	if(ticks_duration(ticks, stamp) <= 500) {
		return state;
	}

	stamp = ticks;

	if(state == GPIO_PIN_RESET) {
		state = GPIO_PIN_SET;
	} else {
		state = GPIO_PIN_RESET;
	}

	return state;
}

static void handle_channels_fault_led(channels_info_t *channels_info)
{
	int i;
	uint8_t channels_faults = 0;
	GPIO_PinState state;

	for(i = 0; i < channels_info->channel_number; i++) {
		channel_info_t *channel_info = channels_info->channel_info + i;

		if(channel_info->channel_faults != 0) {
			channels_faults = 1;
			break;
		}
	}

	state = (channels_faults == 1) ? get_fault_led_state() : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(out_2_GPIO_Port, out_2_Pin, state);
}

static void handle_channels_adhesion(channels_info_t *channels_info)
{
	static uint32_t adhesion_alive_stamp = 0;
	int i;
	uint8_t idle = 1;
	GPIO_PinState state;
	uint32_t ticks = osKernelSysTick();
	uint8_t adhesion_fault = 0;

	for(i = 0; i < channels_info->channel_number; i++) {
		channel_info_t *channel_info = channels_info->channel_info + i;

		if(channel_info->state != CHANNEL_STATE_IDLE) {
			idle = 0;
			break;
		}
	}

	if(idle == 1) {
		state = HAL_GPIO_ReadPin(in_5_GPIO_Port, in_5_Pin);

		if(state == GPIO_PIN_RESET) {
			adhesion_alive_stamp = ticks;
		}
	} else {
		adhesion_alive_stamp = ticks;
	}

	if(ticks_duration(ticks, adhesion_alive_stamp) >= 5000) {
		adhesion_fault = 1;
	}

	if(get_fault(channels_info->faults, CHANNELS_FAULT_RELAY_ADHESION) != adhesion_fault) {
		set_fault(channels_info->faults, CHANNELS_FAULT_RELAY_ADHESION, adhesion_fault);
	}
}

void handle_channels_common_periodic_custom(channels_info_t *channels_info)
{
	handle_channels_idle_led(channels_info);
	handle_channels_fault_led(channels_info);
	handle_channels_adhesion(channels_info);
}
