

/*================================================================
 *
 *
 *   文件名称：display_cache.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月17日 星期六 09时42分40秒
 *   修改日期：2023年02月08日 星期三 15时24分04秒
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

static void load_device_id(uint16_t *id, uint16_t *cache, uint16_t size)
{
	u_uint16_bytes_t *u_uint16_bytes_id = (u_uint16_bytes_t *)id;
	u_uint16_bytes_t *u_uint16_bytes_cache = (u_uint16_bytes_t *)cache;
	int i;

	for(i = 0; i < size; i++) {
		uint8_t byte0 = u_uint16_bytes_id->s.byte0;
		uint8_t byte1 = u_uint16_bytes_id->s.byte1;

		if(byte0 == 0) {
			byte0 = 0xff;
		}

		if(byte1 == 0) {
			byte1 = 0xff;
		}

		u_uint16_bytes_cache->s.byte0 = byte1;
		u_uint16_bytes_cache->s.byte1 = byte0;
		u_uint16_bytes_id++;
		u_uint16_bytes_cache++;
	}
}

static void set_device_id(uint16_t *id, uint16_t *cache, uint16_t size)
{
	u_uint16_bytes_t *u_uint16_bytes_id = (u_uint16_bytes_t *)id;
	u_uint16_bytes_t *u_uint16_bytes_cache = (u_uint16_bytes_t *)cache;
	int i;

	for(i = 0; i < size; i++) {
		uint8_t byte0 = u_uint16_bytes_cache->s.byte0;
		uint8_t byte1 = u_uint16_bytes_cache->s.byte1;

		if(byte0 == 0xff) {
			byte0 = 0;
		}

		if(byte1 == 0xff) {
			byte1 = 0;
		}

		u_uint16_bytes_id->s.byte0 = byte1;
		u_uint16_bytes_id->s.byte1 = byte0;
		u_uint16_bytes_id++;
		u_uint16_bytes_cache++;
	}
}

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
		int d0;
		int d1;
		int d2;
		int d3;
		int port;
		int ret = sscanf(app_info->mechine_info.uri, "tcp://%d.%d.%d.%d:%d",
		                 &d0,
		                 &d1,
		                 &d2,
		                 &d3,
		                 &port);

		if(ret == 5) {
			app_info->display_cache_app.ip[0] = d0;
			app_info->display_cache_app.ip[1] = d1;
			app_info->display_cache_app.ip[2] = d2;
			app_info->display_cache_app.ip[3] = d3;
			app_info->display_cache_app.ip[4] = port;
		}
	}

	load_device_id((uint16_t *)app_info->mechine_info.device_id, (uint16_t *)app_info->display_cache_app.device_id, 16);

	{
		time_t ts = get_time();
		struct tm tm = *localtime(&ts);
		u_uint16_bytes_t u_uint16_bytes;

		app_info->display_cache_app.set_time[0] = get_bcd_from_u8(tm.tm_sec);
		app_info->display_cache_app.set_time[1] = get_bcd_from_u8(tm.tm_min);
		app_info->display_cache_app.set_time[2] = get_bcd_from_u8(tm.tm_hour);
		app_info->display_cache_app.set_time[3] = get_bcd_from_u8(tm.tm_mday);
		app_info->display_cache_app.set_time[4] = get_bcd_from_u8(tm.tm_mon + 1);
		u_uint16_bytes.v = tm.tm_year + 1900;
		app_info->display_cache_app.set_time[5] = get_u16_from_u8_lh(get_bcd_from_u8(u_uint16_bytes.s.byte0), get_bcd_from_u8(u_uint16_bytes.s.byte1));
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

	if(app_info->display_cache_app.ip_sync != 0) {
		app_info->display_cache_app.ip_sync = 0;

		snprintf(app_info->mechine_info.uri, 256, "tcp://%d.%d.%d.%d:%d",
		         app_info->display_cache_app.ip[0],
		         app_info->display_cache_app.ip[1],
		         app_info->display_cache_app.ip[2],
		         app_info->display_cache_app.ip[3],
		         app_info->display_cache_app.ip[4]);
		app_info->mechine_info_invalid = 1;
	}

	if(app_info->display_cache_app.device_id_sync != 0) {
		app_info->display_cache_app.device_id_sync = 0;
		set_device_id((uint16_t *)app_info->mechine_info.device_id, (uint16_t *)app_info->display_cache_app.device_id, 16);

		app_info->mechine_info_invalid = 1;
	}

	if(app_info->display_cache_app.set_time_sync != 0) {
		time_t ts;
		struct tm tm = {0};
		u_uint16_bytes_t u_uint16_bytes;

		app_info->display_cache_app.set_time_sync = 0;

		tm.tm_sec = get_u8_from_bcd(app_info->display_cache_app.set_time[0]);
		tm.tm_min = get_u8_from_bcd(app_info->display_cache_app.set_time[1]);
		tm.tm_hour = get_u8_from_bcd(app_info->display_cache_app.set_time[2]);
		tm.tm_mday = get_u8_from_bcd(app_info->display_cache_app.set_time[3]);
		tm.tm_mon = get_u8_from_bcd(app_info->display_cache_app.set_time[4]) - 1;
		u_uint16_bytes.v = app_info->display_cache_app.set_time[5];
		tm.tm_year = get_u16_from_u8_lh(get_u8_from_bcd(u_uint16_bytes.s.byte0), get_u8_from_bcd(u_uint16_bytes.s.byte1)) - 1900;
		ts = mktime(&tm);

		if(set_time(ts) == 0) {
			debug("set time successful!");
		} else {
			debug("set time failed!");
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
	account_response_info_t *account_response_info = (account_response_info_t *)channels_notify_ctx->ctx;

	switch(channels_notify_ctx->notify) {
		case CHANNELS_NOTIFY_CARD_VERIFY_RESULT:
		case CHANNELS_NOTIFY_VIN_VERIFY_RESULT: {
			if(account_response_info->code != ACCOUNT_STATE_CODE_OK) {
				start_popup(channels_info, MODBUS_POPUP_TYPE_AUTH, account_response_info->code);
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

		if(channels_info->display_cache_channels.record_load_cmd == 1) {
			struct tm tm = {0};
			uint8_t year_l = get_u8_l_from_u16(channels_info->display_cache_channels.record_dt_cache.year);
			uint8_t year_h = get_u8_h_from_u16(channels_info->display_cache_channels.record_dt_cache.year);

			channels_info->display_cache_channels.record_load_cmd = 0;

			tm.tm_year = get_u16_from_bcd_b01(year_l, year_h) - 1900;
			tm.tm_mon = get_u8_from_bcd(channels_info->display_cache_channels.record_dt_cache.mon) - 1;
			tm.tm_mday = get_u8_from_bcd(channels_info->display_cache_channels.record_dt_cache.day);
			channel_record_task_info->page_load_time = mktime(&tm);

			channel_record_item_page_load_location(channel_record_task_info);
		}
	}

	if(channels_info->display_cache_channels.module_sync == 1) {
		uint8_t module_page = channels_info->display_cache_channels.module_page;
		power_manager_info_t *power_manager_info = (power_manager_info_t *)channels_info->power_manager_info;
		int base_offset = module_page * 10;
		int i;

		channels_info->display_cache_channels.module_sync = 0;

		for(i = 0; i < MODULE_ITEM_CACHE_NUMBER; i++) {
			int offset = base_offset + i;
			module_item_cache_t *module_item_cache = &channels_info->display_cache_channels.module_item_cache[i];

			if(offset < power_manager_info->power_modules_info->power_module_number) {
				power_module_item_info_t *power_module_item_info = power_manager_info->power_module_item_info + offset;

				module_item_cache->setting_voltage = power_module_item_info->status.setting_output_voltage;
				module_item_cache->output_voltage = power_module_item_info->status.module_output_voltage;
				module_item_cache->setting_current = power_module_item_info->status.setting_output_current;
				module_item_cache->output_current = power_module_item_info->status.module_output_current;
				module_item_cache->state = power_module_item_info->status.module_status;
				module_item_cache->group = power_module_item_info->id;
				module_item_cache->connect_state = power_module_item_info->status.connect_state;
			} else {
				memset(module_item_cache, 0, sizeof(module_item_cache_t));
			}
		}
	}
}

void load_channel_display_cache(channel_info_t *channel_info)
{
}

static void display_start_channel(channel_info_t *channel_info)
{
	channels_info_t *channels_info = channel_info->channels_info;
	channel_event_t *channel_event;
	channels_event_t *channels_event;
	channel_event = os_calloc(1, sizeof(channel_event_t));
	channels_event = os_calloc(1, sizeof(channels_event_t));

	OS_ASSERT(channel_event != NULL);
	OS_ASSERT(channels_event != NULL);

	channel_event->channel_id = channel_info->channel_id;
	channel_event->type = CHANNEL_EVENT_TYPE_START_CHANNEL;
	channel_event->ctx = &channel_info->channel_event_start_display;

	channels_event->type = CHANNELS_EVENT_CHANNEL;
	channels_event->event = channel_event;

	if(send_channels_event(channels_info, channels_event, 100) != 0) {
	}
}

static void account_request_cb(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	account_response_info_t *account_response_info = (account_response_info_t *)chain_ctx;
	channels_notify_ctx_t channels_notify_ctx;

	channels_notify_ctx.notify = CHANNELS_NOTIFY_CARD_VERIFY_RESULT;
	channels_notify_ctx.ctx = account_response_info;
	do_callback_chain(channels_info->channels_notify_chain, &channels_notify_ctx);

	switch(account_response_info->code) {
		case ACCOUNT_STATE_CODE_OK: {
			channel_info_t *channel_info = (channel_info_t *)account_response_info->channel_info;
			debug("balance:%d", account_response_info->balance);
			channel_info->channel_event_start_display.account_balance = account_response_info->balance;
			memcpy(channel_info->channel_event_start_display.serial_no, account_response_info->serial_no, sizeof(channel_info->channel_event_start_display.serial_no));
			display_start_channel(channel_info);
		}
		break;

		default: {
			debug("code:%d", account_response_info->code);
		}
		break;
	}
}

static void password_confirm_cb_fn(channel_info_t *channel_info)
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
static void card_reader_cb_fn(void *fn_ctx, void *chain_ctx)
{
	channel_info_t *channel_info = (channel_info_t *)fn_ctx;
	card_reader_data_t *card_reader_data = (card_reader_data_t *)chain_ctx;
	channels_info_t *channels_info = channel_info->channels_info;

	if(card_reader_data != NULL) {
		get_ascii_from_u64((char *)&channel_info->display_cache_channel.account[0], sizeof(channel_info->display_cache_channel.account), card_reader_data->id);
		channel_info->display_cache_channel.alive_stamps = osKernelSysTick();
		start_popup(channels_info, MODBUS_POPUP_TYPE_PASSWORD, 0);
	} else {
		account_response_info_t account_response_info = {0};
		account_response_info.channel_info = channel_info;
		account_response_info.code = ACCOUNT_STATE_CODE_GET_CARD_TIMEOUT;
		account_response_info.balance = 0;
		account_request_cb(channels_info, &account_response_info);
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

			channel_info->channel_event_start_display.charge_mode = channel_info->display_cache_channel.charge_mode;

			switch(channel_info->display_cache_channel.charge_mode) {
				case CHANNEL_RECORD_CHARGE_MODE_DURATION: {
					channel_info->channel_event_start_display.charge_condition = get_u32_from_u16_01(channel_info->display_cache_channel.charge_condition_l, channel_info->display_cache_channel.charge_condition_h) * 60;
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;

				case CHANNEL_RECORD_CHARGE_MODE_AMOUNT: {
					channel_info->channel_event_start_display.charge_condition = get_u32_from_u16_01(channel_info->display_cache_channel.charge_condition_l, channel_info->display_cache_channel.charge_condition_h) * get_value_accuracy_base(VALUE_ACCURACY_2, VALUE_ACCURACY_2);
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;

				case CHANNEL_RECORD_CHARGE_MODE_ENERGY: {
					channel_info->channel_event_start_display.charge_condition = get_u32_from_u16_01(channel_info->display_cache_channel.charge_condition_l, channel_info->display_cache_channel.charge_condition_h) * get_value_accuracy_base(VALUE_ACCURACY_0, VALUE_ACCURACY_4);
					channel_info->channel_event_start_display.start_time = start_ts;
				}
				break;

				default: {

				}
				break;
			}

			if(channels_settings->authorize != 0) {
				if(channel_info->display_cache_channel.account_type == ACCOUNT_TYPE_CARD) {
#if !defined(DISABLE_CARDREADER)
					card_reader_cb_t card_reader_cb;
					card_reader_info_t *card_reader_info = (card_reader_info_t *)channels_info->card_reader_info;
					channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(CARD);
					card_reader_cb.fn = card_reader_cb_fn;
					card_reader_cb.fn_ctx = channel_info;
					card_reader_cb.timeout = 5000;
					start_card_reader_cb(card_reader_info, &card_reader_cb);
#endif
				} else if(channel_info->display_cache_channel.account_type == ACCOUNT_TYPE_VIN) {
					channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(VIN);
					display_start_channel(channel_info);
				} else {
				}
			} else {
				channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(MANUAL);
				channel_info->channel_event_start_display.account_balance = 5;
				display_start_channel(channel_info);
			}

		} else {//关机
			channel_request_stop(channel_info, channel_record_item_stop_reason(MANUAL));
		}
	}

	if(channel_info->display_cache_channel.account_password_sync == 1) {
		channel_info->display_cache_channel.account_password_sync = 0;
		password_confirm_cb_fn(channel_info);
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
	ts = channel_record_item->start_time;
	tm = localtime(&ts);
	hour = get_bcd_from_u8(tm->tm_hour);
	min = get_bcd_from_u8(tm->tm_min);
	record_item_cache->stop_hour_min = get_u16_from_u8_lh(min, hour);
	record_item_cache->energy_h = get_u16_1_from_u32(channel_record_item->energy);
	record_item_cache->energy_l = get_u16_0_from_u32(channel_record_item->energy);
	record_item_cache->amount_h = get_u16_1_from_u32(channel_record_item->amount / 1000000);
	record_item_cache->amount_l = get_u16_0_from_u32(channel_record_item->amount / 1000000);
	//record_item_cache->start_reason = get_u16_0_from_u32(channel_record_item->start_reason);
	record_item_cache->stop_reason = get_u16_0_from_u32(channel_record_item->stop_reason);
}

