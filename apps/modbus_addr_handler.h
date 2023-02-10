

/*================================================================
 *
 *
 *   文件名称：modbus_addr_handler.h
 *   创 建 者：肖飞
 *   创建日期：2022年08月04日 星期四 10时36分06秒
 *   修改日期：2023年02月10日 星期五 16时06分13秒
 *   描    述：
 *
 *================================================================*/
#ifndef _MODBUS_ADDR_HANDLER_H
#define _MODBUS_ADDR_HANDLER_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "app_platform.h"

#define add_enum_modbus_item(name) \
	MODBUS_ADDR_##name

#define add_enum_modbus_item_with_base(name, base) \
	MODBUS_ADDR_##name = (base)

#define add_enum_modbus_word_item(name) \
	MODBUS_ADDR_##name##_L, \
	MODBUS_ADDR_##name##_H

#define add_enum_modbus_word_item_with_base(name, base) \
	MODBUS_ADDR_##name##_L = (base), \
	MODBUS_ADDR_##name##_H

#define add_enum_modbus_buffer(name, length) \
	MODBUS_ADDR_##name##_START, \
	MODBUS_ADDR_##name##_END = (MODBUS_ADDR_##name##_START + (length) - 1)

#define add_enum_modbus_buffer_with_base(name, length, base) \
	MODBUS_ADDR_##name##_START = (base), \
	MODBUS_ADDR_##name##_END = (MODBUS_ADDR_##name##_START + (length) - 1)

#define add_enum_modbus_price_info(seg_id) \
	add_enum_modbus_item(PRICE_INFO_##seg_id##_##STOP_HOUR), \
	add_enum_modbus_item(PRICE_INFO_##seg_id##_##STOP_MIN), \
	add_enum_modbus_word_item(PRICE_INFO_##seg_id##_##PRICE), \
	add_enum_modbus_word_item(PRICE_INFO_##seg_id##_##SERVICE_PRICE)

#define add_enum_modbus_power_module_status(power_module_id) \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##PDU_GROUP_ID), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##CHANNEL_ID), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##RELAY_BOARD_ID), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##WORK_STATE), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##REQUIRE_VOLTAGE), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##REQUIRE_CURRENT), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##OUTPUT_VOLTAGE), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##OUTPUT_CURRENT), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##MODULE_STATE), \
	add_enum_modbus_item(POWER_MODULE_##power_module_id##_STATUS_##CONNECT_STATE)

#define add_enum_modbus_channel_items(channel_id, base) \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##STATE, base), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##VOLTAGE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##CURRENT), \
	add_enum_modbus_word_item(CHANNEL_##channel_id##_ITEM_##CHARGE_ENERGY), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##CHARGE_DURATION), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##REQUIRE_VOLTAGE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##REQUIRE_CURRENT), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##CHANNEL_FAULT), \
	add_enum_modbus_word_item_with_base(CHANNEL_##channel_id##_ITEM_##ACCOUNT_BALANCE, (base + 14)), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##START_STOP), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##REMAIN_MIN), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##SOC), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##AMOUNT), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##PRICE, (base + 38)), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##CHARGER_CONNECT_STATE, (base + 40)), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##INSULATION_RESISTOR, (base + 47)), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##TEMPERATURE_P), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##TEMPERATURE_N), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##CHARGER_LOCK_STATE, (base + 52)), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##INSULATION_STATE, (base + 59)), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##BATTERY_TYPE, (base + 61)), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##TOTAL_BATTERY_RATE_CAPICITY), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##BCP_MAX_CHARGE_VOLTAGE_SINGLE_BATTERY, (base + 64)), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BCS_SINGLE_BATTERY_MAX_VOLTAGE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BCP_MAX_TEMPERATURE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BSM_BATTERY_MAX_TEMPERATURE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BCS_CHARGE_VOLTAGE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BCS_CHARGE_CURRENT), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BHM_MAX_CHARGE_VOLTAGE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##BRM_TOTAL_BATTERY_RATE_VOLTAGE), \
	add_enum_modbus_item_with_base(CHANNEL_##channel_id##_ITEM_##CHARGE_MODE, (base + 90)), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##CHARGE_CONDITION), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##ACCOUNT_TYPE), \
	add_enum_modbus_item(CHANNEL_##channel_id##_ITEM_##PASSWORD_CONFIRM), \
	add_enum_modbus_buffer_with_base(CHANNEL_##channel_id##_ITEM_##ACCOUNT, 10, (base + 900)), \
	add_enum_modbus_buffer(CHANNEL_##channel_id##_ITEM_##PASSWORD, 10)

#define add_enum_modbus_channel_record_status(row, base) \
	add_enum_modbus_item_with_base(CHANNEL_RECORD_##row##_##PAD, base), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##CHANNEL_ID), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##START_TIME), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##STOP_TIME), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##START_SOC), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##STOP_SOC), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##ENERGY), \
	add_enum_modbus_item_with_base(CHANNEL_RECORD_##row##_##AMOUNT, (base) + 9), \
	add_enum_modbus_item(CHANNEL_RECORD_##row##_##STOP_REASON), \
	add_enum_modbus_buffer(CHANNEL_RECORD_##row##_##CARD_ID, 9)

typedef enum {
	//设备信息
	add_enum_modbus_buffer_with_base(DEVICE_ID, 16, 0),
	add_enum_modbus_buffer(URI, 64),
	add_enum_modbus_buffer(IP, 20),
	add_enum_modbus_buffer(SN, 20),
	add_enum_modbus_buffer(GW, 20),
	add_enum_modbus_item(DHCP_ENABLE),
	add_enum_modbus_item(REQUEST_TYPE),
	add_enum_modbus_item(UPGRADE_ENABLE),
	add_enum_modbus_item(RESET_CONFIG),
	add_enum_modbus_item(TZ),

	//桩设置
	add_enum_modbus_item_with_base(AUTHORIZE, 1000),
	add_enum_modbus_item(AUTO_START),
	add_enum_modbus_item(OFFLINE_AUTHORIZE),
	add_enum_modbus_item(PRECHARGE_DISABLE),
	add_enum_modbus_item(POWER_MODULE_TYPE),
	add_enum_modbus_item(HUAWEI_REFERENCE_CURRENT),
	add_enum_modbus_item(REQUEST_POPUP_CLOSE),

	add_enum_modbus_item_with_base(CARD_READER_TYPE, 1350),
	add_enum_modbus_item_with_base(MODULE_MAX_OUTPUT_VOLTAGE, 1400),
	add_enum_modbus_item(MODULE_MIN_OUTPUT_VOLTAGE),
	add_enum_modbus_item(MODULE_MAX_OUTPUT_CURRENT),
	add_enum_modbus_item(MODULE_MIN_OUTPUT_CURRENT),
	add_enum_modbus_item(MODULE_MAX_INPUT_VOLTAGE),
	add_enum_modbus_item(MODULE_MIN_INPUT_VOLTAGE),
	add_enum_modbus_item(MODULE_MAX_OUTPUT_POWER),
	add_enum_modbus_item(CHANNELS_MAX_OUTPUT_POWER),
	add_enum_modbus_item(POWER_MANAGER_GROUP_POLICY),
	add_enum_modbus_item(NOLOAD_PROTECT_DISABLE),
	add_enum_modbus_item(SOC_THRESHOLD),
	add_enum_modbus_item(POWER_THRESHOLD),
	add_enum_modbus_item(MAGNIFICATION),
	add_enum_modbus_word_item(WITHHOLDING),
	add_enum_modbus_item(PE_DETECT_DISABLE),

	//费率
	MODBUS_ADDR_PRICE_INFO_START = 1999,
	add_enum_modbus_price_info(0),
	add_enum_modbus_price_info(1),
	add_enum_modbus_price_info(2),
	add_enum_modbus_price_info(3),
	add_enum_modbus_price_info(4),
	add_enum_modbus_price_info(5),
	add_enum_modbus_price_info(6),
	add_enum_modbus_price_info(7),
	add_enum_modbus_price_info(8),
	add_enum_modbus_price_info(9),
	add_enum_modbus_price_info(10),
	add_enum_modbus_price_info(11),
	add_enum_modbus_price_info(12),
	add_enum_modbus_price_info(13),
	add_enum_modbus_price_info(14),
	add_enum_modbus_price_info(15),
	add_enum_modbus_price_info(16),
	add_enum_modbus_price_info(17),
	add_enum_modbus_price_info(18),
	add_enum_modbus_price_info(19),
	add_enum_modbus_price_info(20),
	add_enum_modbus_price_info(21),
	add_enum_modbus_price_info(22),
	add_enum_modbus_price_info(23),
	add_enum_modbus_price_info(24),
	add_enum_modbus_price_info(25),
	add_enum_modbus_price_info(26),
	add_enum_modbus_price_info(27),
	add_enum_modbus_price_info(28),
	add_enum_modbus_price_info(29),
	add_enum_modbus_price_info(30),
	add_enum_modbus_price_info(31),
	add_enum_modbus_price_info(32),
	add_enum_modbus_price_info(33),
	add_enum_modbus_price_info(34),
	add_enum_modbus_price_info(35),
	add_enum_modbus_price_info(36),
	add_enum_modbus_price_info(37),
	add_enum_modbus_price_info(38),
	add_enum_modbus_price_info(39),
	add_enum_modbus_price_info(40),
	add_enum_modbus_price_info(41),
	add_enum_modbus_price_info(42),
	add_enum_modbus_price_info(43),
	add_enum_modbus_price_info(44),
	add_enum_modbus_price_info(45),
	add_enum_modbus_price_info(46),
	add_enum_modbus_price_info(47),

	//桩状态
	MODBUS_ADDR_POWER_MODULE_STATUS_START = 20000,
	add_enum_modbus_power_module_status(0),
	add_enum_modbus_power_module_status(1),
	add_enum_modbus_power_module_status(2),
	add_enum_modbus_power_module_status(3),
	add_enum_modbus_power_module_status(4),
	add_enum_modbus_power_module_status(5),
	add_enum_modbus_power_module_status(6),
	add_enum_modbus_power_module_status(7),
	add_enum_modbus_power_module_status(8),
	add_enum_modbus_power_module_status(9),
	add_enum_modbus_power_module_status(10),
	add_enum_modbus_power_module_status(11),
	add_enum_modbus_power_module_status(12),
	add_enum_modbus_power_module_status(13),
	add_enum_modbus_power_module_status(14),
	add_enum_modbus_power_module_status(15),
	add_enum_modbus_power_module_status(16),
	add_enum_modbus_power_module_status(17),
	add_enum_modbus_power_module_status(18),
	add_enum_modbus_power_module_status(19),
	add_enum_modbus_power_module_status(20),
	add_enum_modbus_power_module_status(21),
	add_enum_modbus_power_module_status(22),
	add_enum_modbus_power_module_status(23),

	add_enum_modbus_item_with_base(POPUP_TYPE, 20301),
	add_enum_modbus_item(POPUP_VALUE),
	add_enum_modbus_item(CHANNEL_NUMBER),

	add_enum_modbus_item_with_base(CHANNELS_FAULT, 20401),

	//通道地址
	add_enum_modbus_channel_items(0, 30000),
	add_enum_modbus_channel_items(1, 31000),

	//充电记录
	add_enum_modbus_item_with_base(CHANNEL_RECORD_NUMBER, 40000),
	add_enum_modbus_item(CHANNEL_RECORD_NUMBER_NOT_UPLOAD),
	add_enum_modbus_item(CHANNEL_RECORD_YEAR),
	add_enum_modbus_item(CHANNEL_RECORD_MONTH),
	add_enum_modbus_item(CHANNEL_RECORD_DAY),
	add_enum_modbus_item(CHANNEL_RECORD_JUMP),
	add_enum_modbus_channel_record_status(0, 40100),
	add_enum_modbus_channel_record_status(1, 40200),
	add_enum_modbus_channel_record_status(2, 40300),
	add_enum_modbus_channel_record_status(3, 40400),
	add_enum_modbus_channel_record_status(4, 40500),
	add_enum_modbus_channel_record_status(5, 40600),
	add_enum_modbus_channel_record_status(6, 40700),
	add_enum_modbus_channel_record_status(7, 40800),
	add_enum_modbus_channel_record_status(8, 40900),
	add_enum_modbus_channel_record_status(9, 41000),

} modbus_slave_addr_t;

typedef enum {
	MODBUS_POPUP_TYPE_NONE = 0,//无弹窗
	MODBUS_POPUP_TYPE_AUTH_0 = 61,//授权结果
	MODBUS_POPUP_TYPE_AUTH_1,
	MODBUS_POPUP_TYPE_AUTH_2,
	MODBUS_POPUP_TYPE_AUTH_3,
	MODBUS_POPUP_TYPE_PASSWORD_0,//密码确认
	MODBUS_POPUP_TYPE_PASSWORD_1,
	MODBUS_POPUP_TYPE_PASSWORD_2,
	MODBUS_POPUP_TYPE_PASSWORD_3,
	MODBUS_POPUP_TYPE_SWIPE_CARD,//刷卡提示
} modbus_popup_type_t;

char *get_modbus_slave_addr_des(modbus_slave_addr_t addr);
void channels_modbus_data_action(void *fn_ctx, void *chain_ctx);

#ifdef __cplusplus
}
#endif
#endif //_MODBUS_ADDR_HANDLER_H
