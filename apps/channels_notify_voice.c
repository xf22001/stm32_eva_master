

/*================================================================
 *
 *
 *   文件名称：channels_notify_voice.c
 *   创 建 者：肖飞
 *   创建日期：2021年08月03日 星期二 11时23分04秒
 *   修改日期：2022年05月12日 星期四 11时52分59秒
 *   描    述：
 *
 *================================================================*/
#include "channels_notify_voice.h"
#include "channels.h"
#include "charger.h"
#include "voice.h"

#include "log.h"

typedef enum {
	VOICE_DATA_WELCOM = 0,
	VOICE_DATA_CHARGER_CONNECTED,
	VOICE_DATA_VEHICLE_READY,
	VOICE_DATA_CHARGER_DISCONNECT,
	VOICE_DATA_CHARGE_START,
	VOICE_DATA_CHARGE_END,
	VOICE_DATA_CHARGER1_CONNECTED,
	VOICE_DATA_VEHICLE1_READY,
	VOICE_DATA_CHARGER1_DISCONNECT,
	VOICE_DATA_CHARGER1_START,
	VOICE_DATA_CHARGER1_END,
	VOICE_DATA_CHARGER2_CONNECTED,
	VOICE_DATA_VEHICLE2_READY,
	VOICE_DATA_CHARGER2_DISCONNECT,
	VOICE_DATA_CHARGER2_START,
	VOICE_DATA_CHARGER2_END,
	VOICE_DATA_VEHICLE_S2_DISCONNECT,
	VOICE_DATA_VEHICLE1_S2_DISCONNECT,
	VOICE_DATA_VEHICLE2_S2_DISCONNECT,
	VOICE_DATA_CHARGER_PAUSE,
	VOICE_DATA_CHARGER1_PAUSE,
	VOICE_DATA_CHARGER2_PAUSE,
	VOICE_DATA_CARDREADER_START,
	VOICE_DATA_CARDREADER_GET_CARD,
	VOICE_DATA_CARD_INVALID,
	VOICE_DATA_WELCOM_NEXT_VISIT,
	VOICE_DATA_CARD_MARK_VALID,
	VOICE_DATA_CARD_PASSWORD_INVALID,
	VOICE_DATA_CARDREADER_CARD_ID_VALID,
	VOICE_DATA_CARDREADER_CARD_ID_INVALID,
	VOICE_DATA_SELECT_CHARGE_TYPE,
} voice_data_t;

static void channels_notify_voice(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	channels_notify_ctx_t *channels_notify_ctx = (channels_notify_ctx_t *)chain_ctx;
	voice_info_t *voice_info = (voice_info_t *)channels_info->voice_info;

	switch(channels_notify_ctx->notify) {
		case CHANNELS_NOTIFY_CHANNEL_STATE_CHANGE: {
			channel_info_t *channel_info = (channel_info_t *)channels_notify_ctx->ctx;

			switch(channel_info->state) {
				case CHANNEL_STATE_IDLE: {
					request_voice(voice_info, VOICE_DATA_WELCOM_NEXT_VISIT);
				}
				break;

				case CHANNEL_STATE_START: {
					switch(channel_info->channel_id) {
						case 0: {
							request_voice(voice_info, VOICE_DATA_CHARGER1_START);
						}
						break;

						case 1: {
							request_voice(voice_info, VOICE_DATA_CHARGER2_START);
						}
						break;

						default: {
							request_voice(voice_info, VOICE_DATA_CHARGE_START);
						}
						break;
					}
				}
				break;

				case CHANNEL_STATE_STOP: {
					switch(channel_info->channel_id) {
						case 0: {
							request_voice(voice_info, VOICE_DATA_CHARGER1_END);
						}
						break;

						case 1: {
							request_voice(voice_info, VOICE_DATA_CHARGER2_END);
						}
						break;

						default: {
							request_voice(voice_info, VOICE_DATA_CHARGE_END);
						}
						break;
					}
				}
				break;

				default: {
				}
				break;
			}
		}
		break;

		case CHANNELS_NOTIFY_CHANNEL_CHARGER_STATE_EVENT: {
			charger_state_event_ctx_t *charger_state_event_ctx = (charger_state_event_ctx_t *)channels_notify_ctx->ctx;
			charger_info_t *charger_info = (charger_info_t *)charger_state_event_ctx->ctx;
			channel_info_t *channel_info = charger_info->channel_info;

			switch(charger_state_event_ctx->event) {
				case CHARGER_STATE_EVENT_CHARGER_CONNECTOR: {
					switch(channel_info->channel_id) {
						case 0: {
							if(channel_info->charger_connect_state == 0) {
								request_voice(voice_info, VOICE_DATA_CHARGER1_DISCONNECT);
							} else {
								request_voice(voice_info, VOICE_DATA_CHARGER1_CONNECTED);
							}
						}
						break;

						case 1: {
							if(channel_info->charger_connect_state == 0) {
								request_voice(voice_info, VOICE_DATA_CHARGER2_DISCONNECT);
							} else {
								request_voice(voice_info, VOICE_DATA_CHARGER2_CONNECTED);
							}
						}
						break;

						default: {
							if(channel_info->charger_connect_state == 0) {
								request_voice(voice_info, VOICE_DATA_CHARGER_DISCONNECT);
							} else {
								request_voice(voice_info, VOICE_DATA_CHARGER_CONNECTED);
							}
						}
						break;
					}
				}
				break;

				case CHARGER_STATE_EVENT_VEHICLE_CONNECTOR: {
					switch(channel_info->channel_id) {
						case 0: {
							if(channel_info->vehicle_relay_state == 0) {
								if(charger_info->charger_bms_work_state == CHARGER_BMS_WORK_STATE_RUNNING) {
									request_voice(voice_info, VOICE_DATA_CHARGER1_PAUSE);
								} else {
									request_voice(voice_info, VOICE_DATA_VEHICLE1_S2_DISCONNECT);
								}
							} else {
								request_voice(voice_info, VOICE_DATA_VEHICLE1_READY);
							}
						}
						break;

						case 1: {
							if(channel_info->vehicle_relay_state == 0) {
								if(charger_info->charger_bms_work_state == CHARGER_BMS_WORK_STATE_RUNNING) {
									request_voice(voice_info, VOICE_DATA_CHARGER2_PAUSE);
								} else {
									request_voice(voice_info, VOICE_DATA_VEHICLE2_S2_DISCONNECT);
								}
							} else {
								request_voice(voice_info, VOICE_DATA_VEHICLE2_READY);
							}
						}
						break;

						default: {
							if(channel_info->vehicle_relay_state == 0) {
								if(charger_info->charger_bms_work_state == CHARGER_BMS_WORK_STATE_RUNNING) {
									request_voice(voice_info, VOICE_DATA_CHARGER_PAUSE);
								} else {
									request_voice(voice_info, VOICE_DATA_VEHICLE_S2_DISCONNECT);
								}
							} else {
								request_voice(voice_info, VOICE_DATA_VEHICLE_READY);
							}
						}
						break;
					}
				}
				break;

				default: {
				}
				break;
			}
		}
		break;

		case CHANNELS_NOTIFY_CARD_READER_START: {
			request_voice(voice_info, VOICE_DATA_CARDREADER_START);
		}
		break;

		case CHANNELS_NOTIFY_CARD_READER_GET_CARD: {
			request_voice(voice_info, VOICE_DATA_CARDREADER_GET_CARD);
		}
		break;

		case CHANNELS_NOTIFY_CARD_READER_MARK_INVALID: {
			request_voice(voice_info, VOICE_DATA_CARD_MARK_VALID);
		}
		break;

		case CHANNELS_NOTIFY_CARD_VERIFY_RESULT: {
		}
		break;

		case CHANNELS_NOTIFY_SELECT_CHARGE_TYPE: {
			request_voice(voice_info, VOICE_DATA_SELECT_CHARGE_TYPE);
		}
		break;

		default: {
			debug("");
		}
		break;
	}
}

typedef struct {
	callback_item_t channels_notify_callback_item;
} channels_notify_voice_ctx_t;

channels_notify_voice_ctx_t *channels_notify_voice_ctx = NULL;

int init_channels_notify_voice(channels_info_t *channels_info)
{
	int ret = -1;
	voice_info_t *voice_info = (voice_info_t *)channels_info->voice_info;

	if(channels_notify_voice_ctx != NULL) {
		return ret;
	}

	channels_notify_voice_ctx = os_calloc(1, sizeof(channels_notify_voice_ctx_t));
	OS_ASSERT(channels_notify_voice_ctx != NULL);

	channels_notify_voice_ctx->channels_notify_callback_item.fn = channels_notify_voice;
	channels_notify_voice_ctx->channels_notify_callback_item.fn_ctx = channels_info;
	OS_ASSERT(register_callback(channels_info->channels_notify_chain, &channels_notify_voice_ctx->channels_notify_callback_item) == 0);

	request_voice(voice_info, VOICE_DATA_WELCOM);

	ret = 0;
	return ret;
}
