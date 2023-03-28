

/*================================================================
 *
 *
 *   文件名称：display_cache.h
 *   创 建 者：肖飞
 *   创建日期：2021年07月17日 星期六 09时42分47秒
 *   修改日期：2023年03月18日 星期六 13时23分57秒
 *   描    述：
 *
 *================================================================*/
#ifndef _DISPLAY_CACHE_H
#define _DISPLAY_CACHE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_platform.h"
#include "cmsis_os.h"

#pragma pack(push, 1)

typedef struct {
	uint8_t request_type_sync;
	uint8_t request_type;
	uint8_t sys_time_sync;
	uint8_t time_sync;
	uint16_t sys_time[6];
} display_cache_app_t;

typedef struct {
	uint16_t hour;
	uint16_t min;
	uint16_t price_l;
	uint16_t price_h;
	uint16_t service_price_l;
	uint16_t service_price_h;
} price_item_cache_t;

typedef struct {
	uint16_t sec;
	uint16_t min;
	uint16_t hour;
	uint16_t day;
	uint16_t mon;
	uint16_t year;
	uint16_t wday;
} datetime_cache_t;

typedef struct {
	uint16_t year;
	uint16_t mon;
	uint16_t day;
} record_dt_cache_t;

typedef struct {
	uint16_t channel_id;
	uint16_t start_hour_min;//bcd
	uint16_t stop_hour_min;//bcd
	uint16_t start_soc;
	uint16_t stop_soc;
	uint16_t energy_l;
	uint16_t energy_h;
	uint16_t amount_l;
	uint16_t amount_h;
	//uint16_t start_reason;
	uint16_t stop_reason;
	uint16_t account[16];//bcd
	uint16_t reserved[24];
} record_item_cache_t;

typedef struct {
	uint16_t setting_voltage;//0.1v
	uint16_t output_voltage;//0.1v
	uint16_t setting_current;//0.1a
	uint16_t output_current;//0.1a
	uint16_t state;//power_module_status_t
	uint16_t group;
	uint16_t connect_state;
	uint16_t unused[3];
} module_item_cache_t;

#define RECORD_ITEM_CACHE_NUMBER 10
#define MODULE_ITEM_CACHE_NUMBER 10

typedef struct {
	uint8_t price_sync;
	price_item_cache_t price_item_cache[48];

	uint8_t withholding_sync;
	uint16_t withholding_l;
	uint16_t withholding_h;

	uint8_t datetime_sync;
	datetime_cache_t datetime_cache;

	uint8_t record_sync;
	uint8_t record_load_cmd;
	record_dt_cache_t record_dt_cache;
	record_item_cache_t record_item_cache[RECORD_ITEM_CACHE_NUMBER];
	uint8_t module_sync;
	uint8_t module_page;
	module_item_cache_t module_item_cache[MODULE_ITEM_CACHE_NUMBER];

	uint8_t popup_type;//modbus_popup_type_t
	uint8_t popup_value;
} display_cache_channels_t;

typedef struct {
	uint8_t dlt_645_addr_sync;
	uint16_t dlt_645_addr[6];

	uint8_t charger_start_sync;
	uint8_t account_type;//account_type_t
	uint16_t account[16];
	uint16_t password[16];
	uint8_t charge_mode;//channel_record_charge_mode_t
	uint16_t onoff;//
	uint16_t charge_condition;//按金额 32位无符号(0.01元) 按时长 32位无符号(1分) 按电量 32位无符号(1kWh) 按soc 32位无符号(%1)
	uint16_t charge_condition_ext;//按时长充电时：如果立即启动，值为0x2400;如果预约启动，0x1200代表将在12:00启动。
	uint8_t account_password_sync;
	uint32_t alive_stamps;
} display_cache_channel_t;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //_DISPLAY_CACHE_H
