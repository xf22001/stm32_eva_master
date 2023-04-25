

/*================================================================
 *
 *
 *   文件名称：display_cache.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月17日 星期六 09时42分40秒
 *   修改日期：2023年04月25日 星期二 09时34分22秒
 *   描    述：
 *
 *================================================================*/
#include "display_cache.h"
#include "app.h"
#include "channels.h"
#include "channel.h"
#include "net_client.h"
#include "power_manager.h"
#if !defined(DISABLE_CARDREADER)
#include "card_reader.h"
#endif
#include "modbus_addr_handler.h"

#include "log.h"

void load_app_display_cache(app_info_t *app_info)
{
	switch(app_info->mechine_info.request_type) {
		case REQUEST_TYPE_NONE: {
			app_info->display_cache_app.request_type = 0;
		}
		break;

		case REQUEST_TYPE_SSE: {
			app_info->display_cache_app.request_type = 1;
		}
		break;

		case REQUEST_TYPE_OCPP_1_6: {
			app_info->display_cache_app.request_type = 2;
		}
		break;

		default: {
			app_info->display_cache_app.request_type = 0;
		}
		break;
	}

	{
		time_t ts = get_time();
		struct tm tm = *localtime(&ts);

		app_info->display_cache_app.sys_time[0] = tm.tm_year + 1900;
		app_info->display_cache_app.sys_time[1] = tm.tm_mon + 1;
		app_info->display_cache_app.sys_time[2] = tm.tm_mday;
		app_info->display_cache_app.sys_time[3] = tm.tm_hour;
		app_info->display_cache_app.sys_time[4] = tm.tm_min;
		app_info->display_cache_app.sys_time[5] = tm.tm_sec;
	}

}

void sync_app_display_cache(app_info_t *app_info)
{
	if(app_info->display_cache_app.request_type_sync != 0) {
		net_client_info_t *net_client_info = get_net_client_info();
		app_info->display_cache_app.request_type_sync = 0;
		debug("app_info->display_cache_app.request_type:%d", app_info->display_cache_app.request_type);

		switch(app_info->display_cache_app.request_type) {
			case 0: {
				app_info->mechine_info.request_type = REQUEST_TYPE_NONE;
			}
			break;

			case 1: {
				app_info->mechine_info.request_type = REQUEST_TYPE_SSE;
			}
			break;

			case 2: {
				app_info->mechine_info.request_type = REQUEST_TYPE_OCPP_1_6;
			}
			break;

			default: {
				app_info->mechine_info.request_type = REQUEST_TYPE_NONE;
			}
			break;
		}

		app_info->mechine_info_invalid = 1;

		if(net_client_info != NULL) {
			set_client_state(net_client_info, CLIENT_REINIT);
		}
	}

	if(app_info->display_cache_app.sys_time_sync != 0) {
		app_info->display_cache_app.sys_time_sync = 0;

		switch(app_info->display_cache_app.time_sync) {
			case 0: {
				time_t ts = get_time();
				struct tm tm = *localtime(&ts);

				app_info->display_cache_app.sys_time[0] = tm.tm_year + 1900;
				app_info->display_cache_app.sys_time[1] = tm.tm_mon + 1;
				app_info->display_cache_app.sys_time[2] = tm.tm_mday;
				app_info->display_cache_app.sys_time[3] = tm.tm_hour;
				app_info->display_cache_app.sys_time[4] = tm.tm_min;
				app_info->display_cache_app.sys_time[5] = tm.tm_sec;
			}
			break;

			case 1: {
				time_t ts;
				struct tm tm = {0};

				tm.tm_year = app_info->display_cache_app.sys_time[0] - 1900;
				tm.tm_mon = app_info->display_cache_app.sys_time[1] - 1;
				tm.tm_mday = app_info->display_cache_app.sys_time[2];
				tm.tm_hour = app_info->display_cache_app.sys_time[3];
				tm.tm_min = app_info->display_cache_app.sys_time[4];
				tm.tm_sec = app_info->display_cache_app.sys_time[5];
				ts = mktime(&tm);

				if(set_time(ts) == 0) {
					debug("set time successful!");
				} else {
					debug("set time failed!");
				}

			}
			break;

			default: {
			}
			break;
		}
	}
}

static int mark_price_segs(uint8_t i, uint8_t start_seg, uint8_t stop_seg, uint32_t price, void *_seg)
{
	uint8_t *seg = (uint8_t *)_seg;
	seg[start_seg] = 1;
	return 0;
}

typedef int (*price_item_seg_cb_t)(uint8_t i, uint8_t start_seg, uint8_t stop_seg, channels_settings_t *channels_settings, void *ctx);

static int parse_price_info_by_segs(channels_settings_t *channels_settings, uint8_t *seg, price_item_seg_cb_t price_item_seg_cb, void *ctx)
{
	int i;
	int j = 0;
	uint8_t start_seg = 0;

	for(i = 1; i < PRICE_SEGMENT_SIZE; i++) {
		if(seg[i] != 1) {
			continue;
		}

		if(price_item_seg_cb != NULL) {
			if(price_item_seg_cb(j, start_seg, i, channels_settings, ctx) == 0) {
				j++;
			}
		}

		start_seg = i;
	}

	if(price_item_seg_cb != NULL) {
		if(price_item_seg_cb(j, start_seg, i, channels_settings, ctx) == 0) {
			j++;
		}
	}

	return j;
}


int price_item_seg_cb(uint8_t i, uint8_t start_seg, uint8_t stop_seg, channels_settings_t *channels_settings, void *_price_item_cache)
{
	int ret = -1;
	price_item_cache_t *price_item_cache = (price_item_cache_t *)_price_item_cache;
	time_t stop;
	struct tm *tm;
	price_info_t *price_info_energy = &channels_settings->price_info_energy;
	price_info_t *price_info_service = &channels_settings->price_info_service;

	if(i >= 48) {
		debug("");
		return ret;
	}

	stop = get_ts_by_seg_index(stop_seg);

	debug("[%d] start:%d, stop:%d, price:%d, service price:%d", i, start_seg, stop_seg, price_info_energy->price[start_seg], price_info_service->price[start_seg]);

	price_item_cache += i;

	tm = localtime(&stop);
	price_item_cache->hour = get_bcd_from_u8((tm->tm_hour != 0) ? tm->tm_hour : 24);
	price_item_cache->min = get_bcd_from_u8(tm->tm_min);

	price_item_cache->price_h = get_u16_1_from_u32(price_info_energy->price[start_seg]);
	price_item_cache->price_l = get_u16_0_from_u32(price_info_energy->price[start_seg]);
	price_item_cache->service_price_h = get_u16_1_from_u32(price_info_service->price[start_seg]);
	price_item_cache->service_price_l = get_u16_0_from_u32(price_info_service->price[start_seg]);

	ret = 0;
	return ret;
}

static callback_item_t channels_notify_callback_item;

static void start_popup(channels_info_t *channels_info, uint8_t popup_type, uint8_t popup_value)
{
	channels_info->display_cache_channels.popup_type = popup_type;
	channels_info->display_cache_channels.popup_value = popup_value;
}

static void channels_notify_callback(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	channels_notify_ctx_t *channels_notify_ctx = (channels_notify_ctx_t *)chain_ctx;

	switch(channels_notify_ctx->notify) {
		case CHANNELS_NOTIFY_CARD_VERIFY_RESULT:
		case CHANNELS_NOTIFY_VIN_VERIFY_RESULT: {
			account_response_info_t *account_response_info = (account_response_info_t *)channels_notify_ctx->ctx;
			channel_info_t *channel_info = (channel_info_t *)account_response_info->channel_info;

			if(account_response_info->code != ACCOUNT_STATE_CODE_OK) {
				start_popup(channels_info, MODBUS_POPUP_TYPE_AUTH_0 + channel_info->channel_id, account_response_info->code);
			}
		}
		break;

		default: {
		}
		break;
	}
}

void load_channels_display_cache(channels_info_t *channels_info)
{
	channels_settings_t *channels_settings = &channels_info->channels_settings;
	uint8_t seg[PRICE_SEGMENT_SIZE] = {0};
	parse_price_info(&channels_settings->price_info_energy, mark_price_segs, &seg[0]);
	parse_price_info(&channels_settings->price_info_service, mark_price_segs, &seg[0]);
	parse_price_info_by_segs(channels_settings, &seg[0], price_item_seg_cb, &channels_info->display_cache_channels.price_item_cache[0]);

	channels_notify_callback_item.fn = channels_notify_callback;
	channels_notify_callback_item.fn_ctx = channels_info;
	OS_ASSERT(register_callback(channels_info->channels_notify_chain, &channels_notify_callback_item) == 0);
}

static void price_seg_to_price_info(channels_settings_t *channels_settings, price_item_cache_t *price_item_cache, uint8_t max_price_seg)
{
	int i;
	int j;
	time_t start = 0;
	time_t stop = 0;

	price_info_t *price_info_energy = &channels_settings->price_info_energy;
	price_info_t *price_info_service = &channels_settings->price_info_service;

	for(i = 0; i < max_price_seg; i++) {
		price_item_cache_t *item = price_item_cache + i;

		stop = get_u8_from_bcd(get_u8_l_from_u16(item->hour)) * 3600 + get_u8_from_bcd(get_u8_l_from_u16(item->min)) * 60;

		if(stop == 0) {
			debug("skip invalid seg %d, hout:%x, min:%x", i, item->hour, item->min);
			continue;
		}

		start = get_seg_index_by_ts(start);
		stop = get_seg_index_by_ts(stop);

		if(start > stop) {
			stop += PRICE_SEGMENT_SIZE;
		}

		debug("start:%d, stop:%d", (uint32_t)start, (uint32_t)stop);

		for(j = start; j < stop ; j++) {
			int k = j % PRICE_SEGMENT_SIZE;
			uint32_t price_energy = get_u32_from_u16_01(item->price_l, item->price_h);
			uint32_t price_service = get_u32_from_u16_01(item->service_price_l, item->service_price_h);
			debug("energy price[%d]:%d", k, price_energy);
			debug("service price[%d]:%d", k, price_service);
			price_info_energy->price[k] = price_energy;
			price_info_service->price[k] = price_service;
		}

		start = stop;
	}
}

void sync_channels_display_cache(channels_info_t *channels_info)
{
	channels_settings_t *channels_settings = &channels_info->channels_settings;

	if(channels_info->display_cache_channels.price_sync != 0) {
		channels_info->display_cache_channels.price_sync = 0;
		price_seg_to_price_info(channels_settings, &channels_info->display_cache_channels.price_item_cache[0], 48);

		channels_info->channels_settings_invalid = 1;
	}

	if(channels_info->display_cache_channels.withholding_sync != 0) {
		channels_info->display_cache_channels.withholding_sync = 0;
		channels_settings->withholding = get_u32_from_u16_01(channels_info->display_cache_channels.withholding_l, channels_info->display_cache_channels.withholding_h);
		channels_info->channels_settings_invalid = 1;
	}

	if(channels_info->display_cache_channels.record_sync == 1) {
		channel_record_task_info_t *channel_record_task_info = get_or_alloc_channel_record_task_info(0);

		channels_info->display_cache_channels.record_sync = 0;

		switch(channels_info->display_cache_channels.record_load_cmd) {
			case 0: {
				channel_record_item_page_load_current(channel_record_task_info);
			}
			break;

			case 1: {
				channel_record_item_page_load_prev(channel_record_task_info);
			}
			break;

			case 2: {
				channel_record_item_page_load_next(channel_record_task_info);
			}
			break;

			case 3: {
				struct tm tm = {0};
				uint8_t year_l = get_u8_l_from_u16(channels_info->display_cache_channels.record_dt_cache.year);
				uint8_t year_h = get_u8_h_from_u16(channels_info->display_cache_channels.record_dt_cache.year);

				tm.tm_year = get_u16_from_bcd_b01(year_l, year_h) - 1900;
				tm.tm_mon = get_u8_from_bcd(channels_info->display_cache_channels.record_dt_cache.mon) - 1;
				tm.tm_mday = get_u8_from_bcd(channels_info->display_cache_channels.record_dt_cache.day);
				channel_record_task_info->page_load_time = mktime(&tm);

				channel_record_item_page_load_location(channel_record_task_info);
			}
			break;

			default: {
			}
			break;
		}

		channels_info->display_cache_channels.record_load_cmd = 0;
	}
}

void load_channel_display_cache(channel_info_t *channel_info)
{
}

static void display_start_channel(channel_info_t *channel_info)
{
	channel_request_start(channel_info, &channel_info->channel_event_start_display);
}

static void account_request_cb(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	account_response_info_t *account_response_info = (account_response_info_t *)chain_ctx;
	channel_info_t *channel_info = (channel_info_t *)account_response_info->channel_info;
	channels_notify_ctx_t channels_notify_ctx;

	channels_notify_ctx.notify = CHANNELS_NOTIFY_CARD_VERIFY_RESULT;
	channels_notify_ctx.ctx = account_response_info;
	do_callback_chain(channels_info->channels_notify_chain, &channels_notify_ctx);

	switch(account_response_info->code) {
		case ACCOUNT_STATE_CODE_OK: {
			debug("balance:%d", account_response_info->balance);
			channel_info->channel_event_start_display.account_balance = account_response_info->balance;
			memcpy(channel_info->channel_event_start_display.serial_no, account_response_info->serial_no, sizeof(channel_info->channel_event_start_display.serial_no));
			display_start_channel(channel_info);
			start_popup(channels_info, MODBUS_POPUP_TYPE_NONE, 0);
		}
		break;

		default: {
			uint8_t popup_type = MODBUS_POPUP_TYPE_NONE;
			debug("code:%d", account_response_info->code);

			switch(channel_info->channel_id) {
				case 0: {
					popup_type = MODBUS_POPUP_TYPE_AUTH_0;
				}
				break;

				case 1: {
					popup_type = MODBUS_POPUP_TYPE_AUTH_1;
				}
				break;

				case 2: {
					popup_type = MODBUS_POPUP_TYPE_AUTH_2;
				}
				break;

				case 3: {
					popup_type = MODBUS_POPUP_TYPE_AUTH_3;
				}
				break;

				default: {
				}
				break;
			}

			start_popup(channels_info, popup_type, account_response_info->code);
		}
		break;
	}
}

static void password_confirm_cb_start_fn(channel_info_t *channel_info)
{
	channels_info_t *channels_info = channel_info->channels_info;
	net_client_info_t *net_client_info = get_net_client_info();

	if(net_client_info != NULL) {
		if(ticks_duration(channel_info->display_cache_channel.alive_stamps, osKernelSysTick()) >= 60000) {
			account_response_info_t account_response_info = {0};
			//无后台刷卡
			debug("");
			account_response_info.channel_info = channel_info;
			account_response_info.code = ACCOUNT_STATE_CODE_GET_PASSWORD_TIMEOUT;
			account_response_info.balance = 0;
			account_request_cb(channels_info, &account_response_info);
		} else {
			account_request_info_t account_request_info = {0};
			account_request_info.account_type = channel_info->display_cache_channel.account_type;
			account_request_info.account = (char *)&channel_info->display_cache_channel.account[0];
			account_request_info.password = (char *)&channel_info->display_cache_channel.password[0];
			account_request_info.channel_info = channel_info;
			account_request_info.fn = account_request_cb;
			net_client_net_client_ctrl_cmd(net_client_info, NET_CLIENT_CTRL_CMD_QUERY_ACCOUNT, &account_request_info);
			debug("");
		}
	} else {
		account_response_info_t account_response_info = {0};
		//无后台刷卡
		debug("");
		account_response_info.channel_info = channel_info;
		account_response_info.code = ACCOUNT_STATE_CODE_OFFLINE;
		account_response_info.balance = 0;
		account_request_cb(channels_info, &account_response_info);
	}
}

#if !defined(DISABLE_CARDREADER)
static void card_reader_cb_start_fn(void *fn_ctx, void *chain_ctx)
{
	channel_info_t *channel_info = (channel_info_t *)fn_ctx;
	card_reader_data_t *card_reader_data = (card_reader_data_t *)chain_ctx;
	channels_info_t *channels_info = channel_info->channels_info;

	if(card_reader_data != NULL) {
		get_ascii_from_u64((char *)&channel_info->display_cache_channel.account[0], sizeof(channel_info->display_cache_channel.account), card_reader_data->id);
		channel_info->display_cache_channel.alive_stamps = osKernelSysTick();
		start_popup(channels_info, MODBUS_POPUP_TYPE_PASSWORD_0 + channel_info->channel_id, 1);
	} else {
		account_response_info_t account_response_info = {0};
		account_response_info.channel_info = channel_info;
		account_response_info.code = ACCOUNT_STATE_CODE_GET_CARD_TIMEOUT;
		account_response_info.balance = 0;
		account_request_cb(channels_info, &account_response_info);
	}
}

static void card_reader_cb_stop_fn(void *fn_ctx, void *chain_ctx)
{
	channel_info_t *channel_info = (channel_info_t *)fn_ctx;
	card_reader_data_t *card_reader_data = (card_reader_data_t *)chain_ctx;
	channels_info_t *channels_info = channel_info->channels_info;

	if(card_reader_data != NULL) {
		char account[32];
		get_ascii_from_u64((char *)&account[0], sizeof(account), card_reader_data->id);

		if(strncmp(account, (char *)&channel_info->channel_record_item.account[0], 32) == 0) {
			channel_info->display_cache_channel.alive_stamps = osKernelSysTick();
			start_popup(channels_info, MODBUS_POPUP_TYPE_PASSWORD_0 + channel_info->channel_id, 2);
		}
	}
}
#endif

void sync_channel_display_cache(channel_info_t *channel_info)
{
	if(channel_info->display_cache_channel.dlt_645_addr_sync != 0) {
		int i;
		channel_info->display_cache_channel.dlt_645_addr_sync = 0;
		debug("channel %d set dlt 645 addr", channel_info->channel_id);

		for(i = 0; i < 6; i++) {
			debug("[%d]:%04x", i, channel_info->display_cache_channel.dlt_645_addr[i]);
		}
	}

	if(channel_info->display_cache_channel.charger_start_sync == 1) {
		channels_info_t *channels_info = (channels_info_t *)channel_info->channels_info;
		channels_settings_t *channels_settings = &channels_info->channels_settings;

		channel_info->display_cache_channel.charger_start_sync = 0;

		if(channel_info->display_cache_channel.onoff == 1) {//开机
			time_t start_ts;

			if(channel_info->state != CHANNEL_STATE_IDLE) {
				debug("");
				return;
			}

			start_ts = get_time();

			channel_info->channel_event_start_display.account_type = channel_info->display_cache_channel.account_type;
			channel_info->channel_event_start_display.charge_mode = channel_info->display_cache_channel.charge_mode;

			switch(channel_info->display_cache_channel.charge_mode) {
				case CHANNEL_RECORD_CHARGE_MODE_DURATION: {
					uint8_t hour = get_u8_from_bcd(get_u8_h_from_u16(channel_info->display_cache_channel.charge_condition_ext));
					uint8_t min = get_u8_from_bcd(get_u8_l_from_u16(channel_info->display_cache_channel.charge_condition_ext));

					channel_info->channel_event_start_display.charge_condition = channel_info->display_cache_channel.charge_condition * 60;

					if(hour == 24) {
						channel_info->channel_event_start_display.start_time = start_ts;
					} else {
						struct tm tm;
						struct tm *now = localtime(&start_ts);
						tm = *now;
						tm.tm_hour = hour;
						tm.tm_min = min;
						channel_info->channel_event_start_display.start_time = mktime(&tm);

						if(channel_info->channel_event_start_display.start_time < start_ts) {
							channel_info->channel_event_start_display.start_time += 86400;
						}
					}
				}
				break;

				case CHANNEL_RECORD_CHARGE_MODE_AMOUNT: {
					channel_info->channel_event_start_display.charge_condition = channel_info->display_cache_channel.charge_condition * get_value_accuracy_base(VALUE_ACCURACY_2, VALUE_ACCURACY_2);
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;

				case CHANNEL_RECORD_CHARGE_MODE_ENERGY: {
					channel_info->channel_event_start_display.charge_condition = channel_info->display_cache_channel.charge_condition * get_value_accuracy_base(VALUE_ACCURACY_0, VALUE_ACCURACY_4);
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;

				case CHANNEL_RECORD_CHARGE_MODE_SOC: {
					channel_info->channel_event_start_display.charge_condition = channel_info->display_cache_channel.charge_condition * get_value_accuracy_base(VALUE_ACCURACY_0, VALUE_ACCURACY_4);
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;

				default: {
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;
			}

			if(channels_settings->authorize != 0) {
				switch(channel_info->display_cache_channel.account_type) {
					case ACCOUNT_TYPE_CARD: {
#if !defined(DISABLE_CARDREADER)
						card_reader_cb_t card_reader_cb;
						card_reader_info_t *card_reader_info = (card_reader_info_t *)channels_info->card_reader_info;
						channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(CARD);
						card_reader_cb.fn = card_reader_cb_start_fn;
						card_reader_cb.fn_ctx = channel_info;
						card_reader_cb.timeout = 60000;

						if(start_card_reader_cb(card_reader_info, &card_reader_cb) == 0) {
							start_popup(channels_info, MODBUS_POPUP_TYPE_SWIPE_CARD, channel_info->channel_id);
						}

#endif
					}
					break;

					case ACCOUNT_TYPE_VIN: {
						channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(VIN);
						channel_info->channel_event_start_display.account_balance = 5;
						display_start_channel(channel_info);
					}
					break;

					default: {
					}
					break;
				}
			} else {
				channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(MANUAL);
				channel_info->channel_event_start_display.account_balance = 0;
				display_start_channel(channel_info);
			}

		} else if(channel_info->display_cache_channel.onoff == 0) { //关机
			if(channels_settings->authorize != 0) {
				switch(channel_info->channel_record_item.start_reason) {
					case channel_record_item_start_reason(CARD): {
						if(channels_settings->authorize_stop == 1) {
#if !defined(DISABLE_CARDREADER)
							card_reader_cb_t card_reader_cb;
							card_reader_info_t *card_reader_info = (card_reader_info_t *)channels_info->card_reader_info;
							channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(CARD);
							card_reader_cb.fn = card_reader_cb_stop_fn;
							card_reader_cb.fn_ctx = channel_info;
							card_reader_cb.timeout = 60000;

							if(start_card_reader_cb(card_reader_info, &card_reader_cb) == 0) {
								start_popup(channels_info, MODBUS_POPUP_TYPE_SWIPE_CARD, channel_info->channel_id);
							}

#endif
						} else {
							channel_request_stop(channel_info, channel_record_item_stop_reason(MANUAL));
						}
					}
					break;

					default: {
						channel_request_stop(channel_info, channel_record_item_stop_reason(MANUAL));
					}
					break;
				}
			} else {
				channel_request_stop(channel_info, channel_record_item_stop_reason(MANUAL));
			}
		}
	}

	if(channel_info->display_cache_channel.account_password_sync == 1) {
		channel_info->display_cache_channel.account_password_sync = 0;
		password_confirm_cb_start_fn(channel_info);
	} else if(channel_info->display_cache_channel.account_password_sync == 2) {
		channel_info->display_cache_channel.account_password_sync = 0;

		if(ticks_duration(channel_info->display_cache_channel.alive_stamps, osKernelSysTick()) <= 60000) {
			if(strncmp((char *)&channel_info->display_cache_channel.password[0],
			           (char *)&channel_info->channel_record_item.password[0],
			           32) == 0) {
				channel_request_stop(channel_info, channel_record_item_stop_reason(MANUAL));
			}
		}
	}
}

int get_channel_record_page_load_item_number(void)
{
	return RECORD_ITEM_CACHE_NUMBER;
}

void channel_record_item_page_item_refresh(channel_record_item_t *channel_record_item, uint16_t offset, uint16_t id)
{
	channels_info_t *channels_info = get_channels();
	record_item_cache_t *record_item_cache = &channels_info->display_cache_channels.record_item_cache[offset];
	uint8_t hour;
	uint8_t min;
	struct tm *tm;
	time_t ts;

	debug("offset:%d, id:%d", offset, id);

	if(offset >= RECORD_ITEM_CACHE_NUMBER) {
		return;
	}

	memset(record_item_cache, 0, sizeof(record_item_cache_t));

	if(channel_record_item == NULL) {
		return;
	}

	record_item_cache->channel_id = channel_record_item->channel_id + 1;
	memcpy(record_item_cache->account, channel_record_item->account, 32);
	ts = channel_record_item->start_time;
	tm = localtime(&ts);
	hour = get_bcd_from_u8(tm->tm_hour);
	min = get_bcd_from_u8(tm->tm_min);
	record_item_cache->start_hour_min = get_u16_from_u8_lh(min, hour);
	ts = channel_record_item->stop_time;
	tm = localtime(&ts);
	hour = get_bcd_from_u8(tm->tm_hour);
	min = get_bcd_from_u8(tm->tm_min);
	record_item_cache->stop_hour_min = get_u16_from_u8_lh(min, hour);
	record_item_cache->start_soc = channel_record_item->start_soc;
	record_item_cache->stop_soc = channel_record_item->stop_soc;
	record_item_cache->energy_h = get_u16_1_from_u32(channel_record_item->energy);
	record_item_cache->energy_l = get_u16_0_from_u32(channel_record_item->energy);
	record_item_cache->amount_h = get_u16_1_from_u32(channel_record_item->amount / 1000000);
	record_item_cache->amount_l = get_u16_0_from_u32(channel_record_item->amount / 1000000);
	//record_item_cache->start_reason = get_u16_0_from_u32(channel_record_item->start_reason);
	record_item_cache->stop_reason = get_u16_0_from_u32(channel_record_item->stop_reason);
}

void display_system_time_update(channels_info_t *channels_info)
{
	time_t ts = get_time();
	struct tm tm = *localtime(&ts);
	app_info_t *app_info = get_app_info();

	app_info->display_cache_app.sys_time[0] = tm.tm_year + 1900;
	app_info->display_cache_app.sys_time[1] = tm.tm_mon + 1;
	app_info->display_cache_app.sys_time[2] = tm.tm_mday;
	app_info->display_cache_app.sys_time[3] = tm.tm_hour;
	app_info->display_cache_app.sys_time[4] = tm.tm_min;
	app_info->display_cache_app.sys_time[5] = tm.tm_sec;

	channels_info->display_cache_channels.time_sync_to_display = 1;
}

void channel_card_authorize_start(channel_info_t *channel_info)
{
	channels_info_t *channels_info = (channels_info_t *)channel_info->channels_info;
	card_reader_info_t *card_reader_info = (card_reader_info_t *)channels_info->card_reader_info;
	card_reader_cb_t card_reader_cb;

	channel_info->display_cache_channel.account_type = ACCOUNT_TYPE_CARD;
	channel_info->display_cache_channel.charge_mode = CHANNEL_RECORD_CHARGE_MODE_UNLIMIT;

	channel_info->channel_event_start_display.account_type = channel_info->display_cache_channel.account_type;
	channel_info->channel_event_start_display.charge_mode = channel_info->display_cache_channel.charge_mode;
	channel_info->channel_event_start_display.start_time = get_time();
	channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(CARD);
	card_reader_cb.fn = card_reader_cb_start_fn;
	card_reader_cb.fn_ctx = channel_info;
	card_reader_cb.timeout = 60000;

	if(start_card_reader_cb(card_reader_info, &card_reader_cb) == 0) {
		start_popup(channels_info, MODBUS_POPUP_TYPE_SWIPE_CARD, channel_info->channel_id);
	}
}
