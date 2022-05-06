

/*================================================================
 *
 *
 *   文件名称：display_cache.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月17日 星期六 09时42分40秒
 *   修改日期：2021年07月19日 星期一 10时07分54秒
 *   描    述：
 *
 *================================================================*/
#include "display_cache.h"
#include "app.h"
#include "channels.h"
#include "log.h"

void load_app_display_cache(app_info_t *app_info)
{
}

void sync_app_display_cache(app_info_t *app_info)
{
}

void load_channels_display_cache(channels_info_t *channels_info)
{
}

void sync_channels_display_cache(channels_info_t *channels_info)
{
}

void load_channel_display_cache(channel_info_t *channel_info)
{
}

void sync_channel_display_cache(channel_info_t *channel_info)
{
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
	record_item_cache->amount_h = get_u16_1_from_u32(channel_record_item->amount);
	record_item_cache->amount_l = get_u16_0_from_u32(channel_record_item->amount);
	record_item_cache->start_reason = get_u16_0_from_u32(channel_record_item->start_reason);
	record_item_cache->stop_reason = get_u16_0_from_u32(channel_record_item->stop_reason);
}

