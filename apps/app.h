

/*================================================================
 *
 *
 *   文件名称：app.h
 *   创 建 者：肖飞
 *   创建日期：2019年10月11日 星期五 16时56分29秒
 *   修改日期：2022年08月11日 星期四 14时25分13秒
 *   描    述：
 *
 *================================================================*/
#ifndef _APP_H
#define _APP_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_platform.h"
#include "cmsis_os.h"

#include "callback_chain.h"
#if defined(ENABLE_DISPLAY)
#include "display_cache.h"
#endif
#include "storage.h"

#define VER_MAJOR 0
#define VER_MINOR 0
#define VER_REV 0
#define VER_BUILD 0

#pragma pack(push, 1)

typedef struct {
	char device_id[32];
	//ws://www.baidu.com:80/abc?d=e&f=g
	//tcp://www.baidu.com:80
	//udp://www.baidu.com:80
	char uri[128];
	char ip[40];
	char sn[40];
	char gw[40];
	uint8_t dhcp_enable;

	uint8_t request_type;

	uint8_t upgrade_enable;
	uint8_t reset_config;
	char tz;
} mechine_info_t;

#pragma pack(pop)

typedef struct {
	mechine_info_t mechine_info;
	storage_info_t *storage_info;
#if defined(ENABLE_DISPLAY)
	uint8_t mechine_info_invalid;
	display_cache_app_t display_cache_app;
	callback_item_t display_data_invalid_callback_item;
	callback_item_t display_data_changed_callback_item;
#endif
} app_info_t;

typedef enum {
	APP_EVENT_NONE = 0,
	APP_EVENT_HOST_USER_SELECT_CONFIGURATION,
	APP_EVENT_HOST_USER_CLASS_ACTIVE,
	APP_EVENT_HOST_USER_CLASS_SELECTED,
	APP_EVENT_HOST_USER_CONNECTION,
	APP_EVENT_HOST_USER_DISCONNECTION,
	APP_EVENT_HOST_USER_UNRECOVERED_ERROR,
} app_event_t;

app_info_t *get_app_info(void);
int app_load_config(void);
int app_save_config(void);
void app_init(void);
void send_app_event(app_event_t event, uint32_t timeout);
#if defined(ENABLE_DISPLAY)
void load_app_display_cache(app_info_t *app_info);
void sync_app_display_cache(app_info_t *app_info);
#endif
void update_network_ip_config(app_info_t *app_info);
void app_set_reset_config(void);
uint8_t app_get_reset_config(void);
void app(void const *argument);
void idle(void const *argument);

#ifdef __cplusplus
}
#endif

#endif //_APP_H
