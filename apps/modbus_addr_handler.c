

/*================================================================
 *
 *
 *   文件名称：modbus_addr_handler.c
 *   创 建 者：肖飞
 *   创建日期：2022年08月04日 星期四 10时34分58秒
 *   修改日期：2022年09月05日 星期一 11时06分47秒
 *   描    述：
 *
 *================================================================*/
#include "modbus_addr_handler.h"

#include "modbus_data_value.h"
#include "channels.h"
#include "charger.h"
#include "display.h"
#include "app.h"
#include "channel.h"
#include "power_manager.h"

#define LOG_DISABLE
#include "log.h"

#define add_modbus_data_get_set_buffer_case(name) \
	MODBUS_ADDR_##name##_START ... MODBUS_ADDR_##name##_END

#define modbus_data_get_set_buffer_start(name) \
	MODBUS_ADDR_##name##_START

#define add_modbus_data_get_set_item_case(name) \
	MODBUS_ADDR_##name

#define add_modbus_data_get_set_word_item_l_case(name) \
	MODBUS_ADDR_##name##_L

#define add_modbus_data_get_set_word_item_h_case(name) \
	MODBUS_ADDR_##name##_H

#define add_modbus_data_get_set_power_module_info_field_case(power_module_id, field_name) \
	add_modbus_data_get_set_item_case(POWER_MODULE_##power_module_id##_STATUS_##field_name)

#define add_modbus_data_get_set_power_module_info_case(power_module_id) \
	add_modbus_data_get_set_power_module_info_field_case(power_module_id, PDU_GROUP_ID): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, CHANNEL_ID): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, RELAY_BOARD_ID): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, WORK_STATE): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, REQUIRE_VOLTAGE): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, REQUIRE_CURRENT): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, OUTPUT_VOLTAGE): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, OUTPUT_CURRENT): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, MODULE_STATE): \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, CONNECT_STATE)

typedef struct {
	uint8_t id;
	uint8_t field;
	uint16_t buffer_start;
	uint16_t offset;
} enum_info_t;

#define add_power_module_field_type_case(field) \
	POWER_MODULE_FIELD_TYPE_##field

typedef enum {
	add_power_module_field_type_case(PDU_GROUP_ID) = 0,
	add_power_module_field_type_case(CHANNEL_ID),
	add_power_module_field_type_case(RELAY_BOARD_ID),
	add_power_module_field_type_case(WORK_STATE),
	add_power_module_field_type_case(REQUIRE_VOLTAGE),
	add_power_module_field_type_case(REQUIRE_CURRENT),
	add_power_module_field_type_case(OUTPUT_VOLTAGE),
	add_power_module_field_type_case(OUTPUT_CURRENT),
	add_power_module_field_type_case(MODULE_STATE),
	add_power_module_field_type_case(CONNECT_STATE),
} power_module_field_type_t;

#define add_get_power_module_enum_info_field_case(power_module_id, enum_info, field_name) \
	case add_modbus_data_get_set_power_module_info_field_case(power_module_id, field_name): { \
		enum_info->id = power_module_id; \
		enum_info->field = add_power_module_field_type_case(field_name); \
	} \
	break

#define add_get_power_module_enum_info_case(power_module_id, enum_info) \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, PDU_GROUP_ID); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, CHANNEL_ID); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, RELAY_BOARD_ID); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, WORK_STATE); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, REQUIRE_VOLTAGE); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, REQUIRE_CURRENT); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, OUTPUT_VOLTAGE); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, OUTPUT_CURRENT); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, MODULE_STATE); \
	add_get_power_module_enum_info_field_case(power_module_id, enum_info, CONNECT_STATE)

static void get_power_module_enum_info(modbus_slave_addr_t addr, enum_info_t *enum_info)
{
	switch(addr) {
			add_get_power_module_enum_info_case(0, enum_info);
			add_get_power_module_enum_info_case(1, enum_info);
			add_get_power_module_enum_info_case(2, enum_info);
			add_get_power_module_enum_info_case(3, enum_info);
			add_get_power_module_enum_info_case(4, enum_info);
			add_get_power_module_enum_info_case(5, enum_info);
			add_get_power_module_enum_info_case(6, enum_info);
			add_get_power_module_enum_info_case(7, enum_info);
			add_get_power_module_enum_info_case(8, enum_info);
			add_get_power_module_enum_info_case(9, enum_info);
			add_get_power_module_enum_info_case(10, enum_info);
			add_get_power_module_enum_info_case(11, enum_info);
			add_get_power_module_enum_info_case(12, enum_info);
			add_get_power_module_enum_info_case(13, enum_info);
			add_get_power_module_enum_info_case(14, enum_info);
			add_get_power_module_enum_info_case(15, enum_info);
			add_get_power_module_enum_info_case(16, enum_info);
			add_get_power_module_enum_info_case(18, enum_info);
			add_get_power_module_enum_info_case(19, enum_info);
			add_get_power_module_enum_info_case(20, enum_info);
			add_get_power_module_enum_info_case(21, enum_info);
			add_get_power_module_enum_info_case(22, enum_info);
			add_get_power_module_enum_info_case(23, enum_info);

		default: {
			debug("not handle addr %d", addr);
			enum_info->id = 0xff;
			enum_info->field = 0xff;
		}
		break;
	}
}

static void modbus_data_action_power_module_info(channels_info_t *channels_info, modbus_data_ctx_t *modbus_data_ctx, enum_info_t *enum_info)
{
	power_manager_info_t *power_manager_info = (power_manager_info_t *)channels_info->power_manager_info;
	power_module_item_info_t *power_module_item_info = power_manager_info->power_module_item_info + enum_info->id;

	if(enum_info->id >= channels_info->power_module_number) {
		modbus_data_value_r(modbus_data_ctx, 0xffff);
		return;
	}

	switch(enum_info->field) {
		case add_power_module_field_type_case(PDU_GROUP_ID): {
			power_module_group_info_t *power_module_group_info = (power_module_group_info_t *)power_module_item_info->power_module_group_info;
			power_manager_group_info_t *power_manager_group_info = (power_manager_group_info_t *)power_module_group_info->power_manager_group_info;
			modbus_data_value_r(modbus_data_ctx, power_manager_group_info->id);
		}
		break;

		case add_power_module_field_type_case(CHANNEL_ID): {
			power_module_group_info_t *power_module_group_info = (power_module_group_info_t *)power_module_item_info->power_module_group_info;
			power_manager_channel_info_t *power_manager_channel_info = (power_manager_channel_info_t *)power_module_group_info->power_manager_channel_info;

			if(power_manager_channel_info != NULL) {
				modbus_data_value_r(modbus_data_ctx, power_manager_channel_info->id);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0xffff);
			}
		}
		break;

		case add_power_module_field_type_case(RELAY_BOARD_ID): {
			modbus_data_value_r(modbus_data_ctx, 0xffff);
		}
		break;

		case add_power_module_field_type_case(WORK_STATE): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.state);
		}
		break;

		case add_power_module_field_type_case(REQUIRE_VOLTAGE): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.setting_output_voltage);
		}
		break;

		case add_power_module_field_type_case(REQUIRE_CURRENT): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.require_output_current);
		}
		break;

		case add_power_module_field_type_case(OUTPUT_VOLTAGE): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.module_output_voltage);
		}
		break;

		case add_power_module_field_type_case(OUTPUT_CURRENT): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.module_output_current);
		}
		break;

		case add_power_module_field_type_case(MODULE_STATE): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.module_status);
		}
		break;

		case add_power_module_field_type_case(CONNECT_STATE): {
			modbus_data_value_r(modbus_data_ctx, power_module_item_info->status.connect_state);
		}
		break;

		default: {
			modbus_data_value_r(modbus_data_ctx, 0xffff);
		}
		break;
	}
}

#define add_modbus_data_get_set_channel_status_field_case(channel_id, field_name) \
	add_modbus_data_get_set_item_case(CHANNEL_##channel_id##_STATUS_##field_name)

#define add_modbus_data_get_set_channel_status_word_field_l_case(channel_id, field_name) \
	add_modbus_data_get_set_word_item_l_case(CHANNEL_##channel_id##_STATUS_##field_name)

#define add_modbus_data_get_set_channel_status_word_field_h_case(channel_id, field_name) \
	add_modbus_data_get_set_word_item_h_case(CHANNEL_##channel_id##_STATUS_##field_name)

#define add_modbus_data_get_set_channel_status_case(channel_id) \
	add_modbus_data_get_set_channel_status_field_case(channel_id, STATE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, VOLTAGE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, CURRENT): \
	case add_modbus_data_get_set_channel_status_word_field_l_case(channel_id, CHARGE_ENERGY): \
	case add_modbus_data_get_set_channel_status_word_field_h_case(channel_id, CHARGE_ENERGY): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, CHARGE_DURATION): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, REQUIRE_VOLTAGE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, REQUIRE_CURRENT): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, CHANNEL_FAULT): \
	case add_modbus_data_get_set_channel_status_word_field_l_case(channel_id, ACCOUNT_BALANCE): \
	case add_modbus_data_get_set_channel_status_word_field_h_case(channel_id, ACCOUNT_BALANCE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, START_STOP): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, REMAIN_MIN): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, SOC): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, AMOUNT): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, PRICE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, CHARGER_CONNECT_STATE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, INSULATION_RESISTOR): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, TEMPERATURE_P): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, TEMPERATURE_N): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, CHARGER_LOCK_STATE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, INSULATION_STATE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BATTERY_TYPE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, TOTAL_BATTERY_RATE_CAPICITY): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BCP_MAX_CHARGE_VOLTAGE_SINGLE_BATTERY): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BCS_SINGLE_BATTERY_MAX_VOLTAGE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BCP_MAX_TEMPERATURE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BSM_BATTERY_MAX_TEMPERATURE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BCS_CHARGE_VOLTAGE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BCS_CHARGE_CURRENT): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BHM_MAX_CHARGE_VOLTAGE): \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, BRM_TOTAL_BATTERY_RATE_VOLTAGE)

#define add_channel_status_field_type_case(field_name) \
	CHANNEL_STATUS_FIELD_TYPE_##field_name

typedef enum {
	add_channel_status_field_type_case(STATE),
	add_channel_status_field_type_case(VOLTAGE),
	add_channel_status_field_type_case(CURRENT),
	add_channel_status_field_type_case(CHARGE_ENERGY),
	add_channel_status_field_type_case(CHARGE_DURATION),
	add_channel_status_field_type_case(REQUIRE_VOLTAGE),
	add_channel_status_field_type_case(REQUIRE_CURRENT),
	add_channel_status_field_type_case(CHANNEL_FAULT),
	add_channel_status_field_type_case(ACCOUNT_BALANCE),
	add_channel_status_field_type_case(START_STOP),
	add_channel_status_field_type_case(REMAIN_MIN),
	add_channel_status_field_type_case(SOC),
	add_channel_status_field_type_case(AMOUNT),
	add_channel_status_field_type_case(PRICE),
	add_channel_status_field_type_case(CHARGER_CONNECT_STATE),
	add_channel_status_field_type_case(INSULATION_RESISTOR),
	add_channel_status_field_type_case(TEMPERATURE_P),
	add_channel_status_field_type_case(TEMPERATURE_N),
	add_channel_status_field_type_case(CHARGER_LOCK_STATE),
	add_channel_status_field_type_case(INSULATION_STATE),
	add_channel_status_field_type_case(BATTERY_TYPE),
	add_channel_status_field_type_case(TOTAL_BATTERY_RATE_CAPICITY),
	add_channel_status_field_type_case(BCP_MAX_CHARGE_VOLTAGE_SINGLE_BATTERY),
	add_channel_status_field_type_case(BCS_SINGLE_BATTERY_MAX_VOLTAGE),
	add_channel_status_field_type_case(BCP_MAX_TEMPERATURE),
	add_channel_status_field_type_case(BSM_BATTERY_MAX_TEMPERATURE),
	add_channel_status_field_type_case(BCS_CHARGE_VOLTAGE),
	add_channel_status_field_type_case(BCS_CHARGE_CURRENT),
	add_channel_status_field_type_case(BHM_MAX_CHARGE_VOLTAGE),
	add_channel_status_field_type_case(BRM_TOTAL_BATTERY_RATE_VOLTAGE),
} channel_status_field_type_t;

#define add_get_channel_status_enum_info_field_case(channel_id, enum_info, field_name) \
	case add_modbus_data_get_set_channel_status_field_case(channel_id, field_name): { \
		enum_info->id = channel_id; \
		enum_info->field = add_channel_status_field_type_case(field_name); \
	} \
	break

#define add_get_channel_status_word_enum_info_field_l_case(channel_id, enum_info, field_name) \
	case add_modbus_data_get_set_channel_status_word_field_l_case(channel_id, field_name): { \
		enum_info->id = channel_id; \
		enum_info->field = add_channel_status_field_type_case(field_name); \
		enum_info->offset = 0; \
	} \
	break

#define add_get_channel_status_word_enum_info_field_h_case(channel_id, enum_info, field_name) \
	case add_modbus_data_get_set_channel_status_word_field_h_case(channel_id, field_name): { \
		enum_info->id = channel_id; \
		enum_info->field = add_channel_status_field_type_case(field_name); \
		enum_info->offset = 1; \
	} \
	break

#define add_get_channel_status_enum_info_case(channel_id, enum_info) \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, STATE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, VOLTAGE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, CURRENT); \
	add_get_channel_status_word_enum_info_field_l_case(channel_id, enum_info, CHARGE_ENERGY); \
	add_get_channel_status_word_enum_info_field_h_case(channel_id, enum_info, CHARGE_ENERGY); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, CHARGE_DURATION); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, REQUIRE_VOLTAGE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, REQUIRE_CURRENT); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, CHANNEL_FAULT); \
	add_get_channel_status_word_enum_info_field_l_case(channel_id, enum_info, ACCOUNT_BALANCE); \
	add_get_channel_status_word_enum_info_field_h_case(channel_id, enum_info, ACCOUNT_BALANCE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, START_STOP); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, REMAIN_MIN); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, SOC); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, AMOUNT); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, PRICE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, CHARGER_CONNECT_STATE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, INSULATION_RESISTOR); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, TEMPERATURE_P); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, TEMPERATURE_N); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, CHARGER_LOCK_STATE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, INSULATION_STATE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BATTERY_TYPE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, TOTAL_BATTERY_RATE_CAPICITY); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BCP_MAX_CHARGE_VOLTAGE_SINGLE_BATTERY); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BCS_SINGLE_BATTERY_MAX_VOLTAGE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BCP_MAX_TEMPERATURE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BSM_BATTERY_MAX_TEMPERATURE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BCS_CHARGE_VOLTAGE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BCS_CHARGE_CURRENT); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BHM_MAX_CHARGE_VOLTAGE); \
	add_get_channel_status_enum_info_field_case(channel_id, enum_info, BRM_TOTAL_BATTERY_RATE_VOLTAGE)

static void get_channel_status_enum_info(modbus_slave_addr_t addr, enum_info_t *enum_info)
{
	switch(addr) {
			add_get_channel_status_enum_info_case(0, enum_info);
			add_get_channel_status_enum_info_case(1, enum_info);

		default: {
			debug("not handle addr %d", addr);
			enum_info->id = 0xff;
			enum_info->field = 0xff;
		}
		break;
	}
}

static void modbus_data_action_channel_status(channels_info_t *channels_info, modbus_data_ctx_t *modbus_data_ctx, enum_info_t *enum_info)
{
	channel_info_t *channel_info = channels_info->channel_info + enum_info->id;

	if(enum_info->id >= channels_info->channel_number) {
		modbus_data_value_r(modbus_data_ctx, 0xffff);
		return;
	}

	switch(enum_info->field) {
		case add_channel_status_field_type_case(STATE): {
			modbus_data_value_r(modbus_data_ctx, channel_info->state);
		}
		break;

		case add_channel_status_field_type_case(VOLTAGE): {
			modbus_data_value_r(modbus_data_ctx, channel_info->voltage);
		}
		break;

		case add_channel_status_field_type_case(CURRENT): {
			modbus_data_value_r(modbus_data_ctx, channel_info->current);
		}
		break;

		case add_channel_status_field_type_case(CHARGE_ENERGY): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				if(enum_info->offset == 0) {
					modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->channel_record_item.energy));
				} else if(enum_info->offset == 1) {
					modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->channel_record_item.energy));
				} else {
					modbus_data_value_r(modbus_data_ctx, 0);
				}
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(CHARGE_DURATION): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				modbus_data_value_r(modbus_data_ctx, (get_time() - channel_info->channel_record_item.start_time) / 60);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(REQUIRE_VOLTAGE): {
			modbus_data_value_r(modbus_data_ctx, channel_info->require_voltage);
		}
		break;

		case add_channel_status_field_type_case(REQUIRE_CURRENT): {
			modbus_data_value_r(modbus_data_ctx, channel_info->require_current);
		}
		break;

		case add_channel_status_field_type_case(CHANNEL_FAULT): {
			int fault;
			fault = get_first_fault(channels_info->faults);

			if(fault == -1) {
				fault = get_first_fault(channel_info->faults);
			}

			modbus_data_value_r(modbus_data_ctx, fault);
		}
		break;

		case add_channel_status_field_type_case(ACCOUNT_BALANCE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				if(enum_info->offset == 0) {
					modbus_data_value_r(modbus_data_ctx, get_u16_0_from_u32(channel_info->channel_record_item.account_balance));
				} else if(enum_info->offset == 1) {
					modbus_data_value_r(modbus_data_ctx, get_u16_1_from_u32(channel_info->channel_record_item.account_balance));
				} else {
					modbus_data_value_r(modbus_data_ctx, 0);
				}
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(START_STOP): {//开关机
			modbus_data_value_r(modbus_data_ctx, (channel_info->state == CHANNEL_STATE_CHARGING) ? 1 : 0);

			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				channel_event_t *channel_event = os_calloc(1, sizeof(channel_event_t));
				channels_event_t *channels_event = os_calloc(1, sizeof(channels_event_t));
				uint8_t channel_id = channel_info->channel_id;
				uint8_t type = (modbus_data_ctx->value != 0) ? CHANNEL_EVENT_TYPE_START_CHANNEL : CHANNEL_EVENT_TYPE_STOP_CHANNEL;

				OS_ASSERT(channel_event != NULL);
				OS_ASSERT(channels_event != NULL);

				switch(type) {
					case CHANNEL_EVENT_TYPE_START_CHANNEL: {
						channel_info->channel_event_start_display.charge_mode = CHANNEL_RECORD_CHARGE_MODE_UNLIMIT;
						channel_info->channel_event_start_display.start_reason = channel_record_item_start_reason(MANUAL);
					}
					break;

					case CHANNEL_EVENT_TYPE_STOP_CHANNEL: {
						channel_info->channel_event_stop.stop_reason = channel_record_item_stop_reason(MANUAL);
					}
					break;

					default: {
					}
					break;
				}

				channel_event->channel_id = channel_id;
				channel_event->type = type;
				channel_event->ctx = &channel_info->channel_event_start_display;

				channels_event->type = CHANNELS_EVENT_CHANNEL;
				channels_event->event = channel_event;

				if(send_channels_event(channels_info, channels_event, 100) != 0) {
					os_free(channels_event->event);
					os_free(channels_event);
					debug("send channel %d type %d failed!", channel_id, type);
				} else {
					debug("send channel %d type %d successful!", channel_id, type);
				}
			}
		}
		break;

		case add_channel_status_field_type_case(REMAIN_MIN): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.remain_min);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(SOC): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.soc);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(AMOUNT): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				modbus_data_value_r(modbus_data_ctx, channel_info->channel_record_item.amount);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(PRICE): {
			time_t ts = get_time();
			uint32_t price = get_current_price(channel_info, ts);
			modbus_data_value_r(modbus_data_ctx, price);
		}
		break;

		case add_channel_status_field_type_case(CHARGER_CONNECT_STATE): {
			modbus_data_value_r(modbus_data_ctx, channel_info->charger_connect_state);
		}
		break;

		case add_channel_status_field_type_case(INSULATION_RESISTOR): {
			modbus_data_value_r(modbus_data_ctx, channel_info->insulation_resistor);
		}
		break;

		case add_channel_status_field_type_case(TEMPERATURE_P): {
			modbus_data_value_r(modbus_data_ctx, channel_info->temperature_p + 20);
		}
		break;

		case add_channel_status_field_type_case(TEMPERATURE_N): {
			modbus_data_value_r(modbus_data_ctx, channel_info->temperature_n + 20);
		}
		break;

		case add_channel_status_field_type_case(CHARGER_LOCK_STATE): {
			modbus_data_value_r(modbus_data_ctx, channel_info->charger_lock_state);
		}
		break;

		case add_channel_status_field_type_case(INSULATION_STATE): {
			modbus_data_value_r(modbus_data_ctx, channel_info->insulation_state);
		}
		break;

		case add_channel_status_field_type_case(BATTERY_TYPE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.brm_data.brm_data.battery_type);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(TOTAL_BATTERY_RATE_CAPICITY): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.brm_data.brm_data.total_battery_rate_capicity);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BCP_MAX_CHARGE_VOLTAGE_SINGLE_BATTERY): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcp_data.max_charge_voltage_single_battery);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BCS_SINGLE_BATTERY_MAX_VOLTAGE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.u1.s.single_battery_max_voltage);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BCP_MAX_TEMPERATURE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcp_data.max_temperature);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BSM_BATTERY_MAX_TEMPERATURE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bsm_data.battery_max_temperature);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BCS_CHARGE_VOLTAGE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bcs_data.charge_voltage);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BCS_CHARGE_CURRENT): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				uint16_t charge_current = 4000 - charger_info->bms_data.bcs_data.charge_current;
				modbus_data_value_r(modbus_data_ctx, charge_current);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BHM_MAX_CHARGE_VOLTAGE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.bhm_data.max_charge_voltage);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		case add_channel_status_field_type_case(BRM_TOTAL_BATTERY_RATE_VOLTAGE): {
			if(channel_info->state == CHANNEL_STATE_CHARGING) {
				charger_info_t *charger_info = (charger_info_t *)channel_info->charger_info;
				modbus_data_value_r(modbus_data_ctx, charger_info->bms_data.brm_data.brm_data.total_battery_rate_voltage);
			} else {
				modbus_data_value_r(modbus_data_ctx, 0);
			}
		}
		break;

		default: {
			modbus_data_value_r(modbus_data_ctx, 0xffff);
		}
		break;
	}
}

#define add_modbus_data_get_set_channel_record_status_field_case(row, field_name) \
	add_modbus_data_get_set_item_case(CHANNEL_RECORD_##row##_##field_name)

#define add_modbus_data_get_set_channel_record_status_buffer_field_case(row, field_name) \
	add_modbus_data_get_set_buffer_case(CHANNEL_RECORD_##row##_##field_name)

#define add_modbus_data_get_set_channel_record_status_case(row) \
	add_modbus_data_get_set_channel_record_status_field_case(row, CHANNEL_ID): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, START_TIME): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, STOP_TIME): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, START_SOC): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, STOP_SOC): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, ENERGY): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, AMOUNT): \
	case add_modbus_data_get_set_channel_record_status_field_case(row, STOP_REASON): \
	case add_modbus_data_get_set_channel_record_status_buffer_field_case(row, CARD_ID)


#define add_channel_record_status_field_type_case(field) \
	CHANNEL_RECORD_STATUS_FIELD_TYPE_##field

typedef enum {
	add_channel_record_status_field_type_case(CHANNEL_ID) = 0,
	add_channel_record_status_field_type_case(START_TIME),
	add_channel_record_status_field_type_case(STOP_TIME),
	add_channel_record_status_field_type_case(START_SOC),
	add_channel_record_status_field_type_case(STOP_SOC),
	add_channel_record_status_field_type_case(ENERGY),
	add_channel_record_status_field_type_case(AMOUNT),
	add_channel_record_status_field_type_case(STOP_REASON),
	add_channel_record_status_field_type_case(CARD_ID),
} channel_record_status_field_type_t;

#define add_get_channel_record_status_enum_info_field_case(row, enum_info, field_name) \
	case add_modbus_data_get_set_channel_record_status_field_case(row, field_name): { \
		enum_info->id = row; \
		enum_info->field = add_channel_record_status_field_type_case(field_name); \
	} \
	break

#define modbus_data_get_set_channel_record_status_buffer_start(row, field_name) \
	modbus_data_get_set_buffer_start(CHANNEL_RECORD_##row##_##field_name)

#define add_get_channel_record_status_enum_info_buffer_field_case(row, enum_info, field_name) \
	case add_modbus_data_get_set_channel_record_status_buffer_field_case(row, field_name): { \
		enum_info->id = row; \
		enum_info->field = add_channel_record_status_field_type_case(field_name); \
		enum_info->buffer_start = modbus_data_get_set_channel_record_status_buffer_start(row, field_name); \
	} \
	break

#define add_get_channel_record_status_enum_info_case(row, enum_info) \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, CHANNEL_ID); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, START_TIME); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, STOP_TIME); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, START_SOC); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, STOP_SOC); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, ENERGY); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, AMOUNT); \
	add_get_channel_record_status_enum_info_field_case(row, enum_info, STOP_REASON); \
	add_get_channel_record_status_enum_info_buffer_field_case(row, enum_info, CARD_ID)

static void get_channel_record_status_enum_info(modbus_slave_addr_t addr, enum_info_t *enum_info)
{
	switch(addr) {
			add_get_channel_record_status_enum_info_case(0, enum_info);
			add_get_channel_record_status_enum_info_case(1, enum_info);
			add_get_channel_record_status_enum_info_case(2, enum_info);
			add_get_channel_record_status_enum_info_case(3, enum_info);
			add_get_channel_record_status_enum_info_case(4, enum_info);
			add_get_channel_record_status_enum_info_case(5, enum_info);
			add_get_channel_record_status_enum_info_case(6, enum_info);
			add_get_channel_record_status_enum_info_case(7, enum_info);
			add_get_channel_record_status_enum_info_case(8, enum_info);
			add_get_channel_record_status_enum_info_case(9, enum_info);

		default: {
			debug("not handle addr %d", addr);
			enum_info->id = 0xff;
			enum_info->field = 0xff;
		}
		break;
	}
}

static void modbus_data_action_channel_record_status(channels_info_t *channels_info, modbus_data_ctx_t *modbus_data_ctx, enum_info_t *enum_info)
{
	uint8_t row = enum_info->id;
	record_item_cache_t *record_item_cache = &channels_info->display_cache_channels.record_item_cache[row];

	if(row >= RECORD_ITEM_CACHE_NUMBER) {
		modbus_data_value_r(modbus_data_ctx, 0xffff);
		return;
	}

	switch(enum_info->field) {
		case add_channel_record_status_field_type_case(CHANNEL_ID): {
			modbus_data_value_r(modbus_data_ctx, record_item_cache->channel_id);
		}
		break;

		case add_channel_record_status_field_type_case(START_TIME): {
			modbus_data_value_r(modbus_data_ctx, record_item_cache->start_hour_min);
		}
		break;

		case add_channel_record_status_field_type_case(STOP_TIME): {
			modbus_data_value_r(modbus_data_ctx, record_item_cache->stop_hour_min);
		}
		break;

		case add_channel_record_status_field_type_case(START_SOC): {
			modbus_data_value_r(modbus_data_ctx, record_item_cache->start_soc);
		}
		break;

		case add_channel_record_status_field_type_case(STOP_SOC): {
			modbus_data_value_r(modbus_data_ctx, record_item_cache->stop_soc);
		}
		break;

		case add_channel_record_status_field_type_case(ENERGY): {//两位小数
			uint16_t energy = get_u32_from_u16_01(record_item_cache->energy_l, record_item_cache->energy_h) / 100;
			modbus_data_value_r(modbus_data_ctx, energy);
		}
		break;

		case add_channel_record_status_field_type_case(AMOUNT): {//两位小数
			uint16_t amount = get_u32_from_u16_01(record_item_cache->amount_l, record_item_cache->amount_h) / 1000000;
			modbus_data_value_r(modbus_data_ctx, amount);
		}
		break;

		case add_channel_record_status_field_type_case(STOP_REASON): {
			modbus_data_value_r(modbus_data_ctx, record_item_cache->stop_reason);
		}
		break;

		case add_channel_record_status_field_type_case(CARD_ID): {
			modbus_data_buffer_rw(modbus_data_ctx, record_item_cache->account, 16 * 2, modbus_data_ctx->addr - enum_info->buffer_start);
		}
		break;

		default: {
			modbus_data_value_r(modbus_data_ctx, 0xffff);
		}
		break;
	}
}


void channels_modbus_data_action(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	channels_settings_t *channels_settings = &channels_info->channels_settings;
	app_info_t *app_info = get_app_info();
	modbus_data_ctx_t *modbus_data_ctx = (modbus_data_ctx_t *)chain_ctx;

	//debug("op:%s, addr:%d",
	//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
	//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
	//      "unknow",
	//      modbus_data_ctx->addr);

	switch(modbus_data_ctx->addr) {
		//设备信息
		case add_modbus_data_get_set_buffer_case(DEVICE_ID): {
			modbus_data_buffer_rw(modbus_data_ctx, app_info->mechine_info.device_id, 16 * 2, modbus_data_ctx->addr - modbus_data_get_set_buffer_start(DEVICE_ID));
		}
		break;

		case add_modbus_data_get_set_buffer_case(URI): {
			modbus_data_buffer_rw(modbus_data_ctx, app_info->mechine_info.uri, 64 * 2, modbus_data_ctx->addr - modbus_data_get_set_buffer_start(URI));
		}
		break;

		case add_modbus_data_get_set_buffer_case(IP): {
			modbus_data_buffer_rw(modbus_data_ctx, app_info->mechine_info.ip, 20 * 2, modbus_data_ctx->addr - modbus_data_get_set_buffer_start(IP));
		}
		break;

		case add_modbus_data_get_set_buffer_case(SN): {
			modbus_data_buffer_rw(modbus_data_ctx, app_info->mechine_info.sn, 20 * 2, modbus_data_ctx->addr - modbus_data_get_set_buffer_start(SN));
		}
		break;

		case add_modbus_data_get_set_buffer_case(GW): {
			modbus_data_buffer_rw(modbus_data_ctx, app_info->mechine_info.gw, 20 * 2, modbus_data_ctx->addr - modbus_data_get_set_buffer_start(GW));
		}
		break;

		case add_modbus_data_get_set_item_case(DHCP_ENABLE): {
			modbus_data_value_rw(modbus_data_ctx, app_info->mechine_info.dhcp_enable);
		}
		break;

		case add_modbus_data_get_set_item_case(REQUEST_TYPE): {
			modbus_data_value_rw(modbus_data_ctx, app_info->mechine_info.request_type);
		}
		break;

		case add_modbus_data_get_set_item_case(UPGRADE_ENABLE): {
			modbus_data_value_rw(modbus_data_ctx, app_info->mechine_info.upgrade_enable);
		}
		break;

		case add_modbus_data_get_set_item_case(RESET_CONFIG): {
			if(modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) {
				app_set_reset_config();
				app_save_config();
				debug("reset config ...");
				HAL_NVIC_SystemReset();
			}
		}
		break;

		case add_modbus_data_get_set_item_case(TZ): {
			modbus_data_value_rw(modbus_data_ctx, app_info->mechine_info.tz);
		}
		break;

		//桩设置
		case add_modbus_data_get_set_item_case(AUTHORIZE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->authorize);
		}
		break;

		case add_modbus_data_get_set_item_case(AUTO_START): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->auto_start);
		}
		break;

		case add_modbus_data_get_set_item_case(VIN_VERIFY_ENABLE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->vin_verify_enable);
		}
		break;

		case add_modbus_data_get_set_item_case(PRECHARGE_ENABLE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->precharge_enable);
		}

		case add_modbus_data_get_set_item_case(POWER_MODULE_TYPE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_module_settings.power_module_type);
		}
		break;

		case add_modbus_data_get_set_item_case(HUAWEI_REFERENCE_CURRENT): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_module_settings.rate_current);
		}
		break;

		case add_modbus_data_get_set_item_case(CARD_READER_TYPE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->card_reader_settings.type);
		}
		break;

		case add_modbus_data_get_set_item_case(MODULE_MAX_OUTPUT_VOLTAGE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_output_voltage);
		}
		break;

		case add_modbus_data_get_set_item_case(MODULE_MIN_OUTPUT_VOLTAGE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_min_output_voltage);
		}
		break;

		case add_modbus_data_get_set_item_case(MODULE_MAX_OUTPUT_CURRENT): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_output_current);
		}
		break;

		case add_modbus_data_get_set_item_case(MODULE_MIN_OUTPUT_CURRENT): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_min_output_current);
		}
		break;

		case add_modbus_data_get_set_item_case(MODULE_MAX_INPUT_VOLTAGE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_input_voltage);
		}
		break;

		case add_modbus_data_get_set_item_case(MODULE_MIN_INPUT_VOLTAGE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_min_input_voltage);
		}
		break;

		case add_modbus_data_get_set_item_case(CHANNELS_MAX_OUTPUT_POWER): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->module_max_output_power);
		}
		break;

		case add_modbus_data_get_set_item_case(POWER_MANAGER_GROUP_POLICY): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_manager_group_policy);
		}
		break;

		case add_modbus_data_get_set_item_case(NOLOAD_PROTECT_DISABLE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->noload_protect_disable);
		}
		break;

		case add_modbus_data_get_set_item_case(SOC_THRESHOLD): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->soc_threshold);
		}
		break;

		case add_modbus_data_get_set_item_case(POWER_THRESHOLD): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->power_threshold);
		}
		break;

		case add_modbus_data_get_set_item_case(MAGNIFICATION): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->magnification);
		}
		break;

		case add_modbus_data_get_set_word_item_l_case(WITHHOLDING): {
			modbus_data_value_rw(modbus_data_ctx, channels_info->display_cache_channels.withholding_l);
			channels_info->display_cache_channels.withholding_sync = 1;
		}
		break;

		case add_modbus_data_get_set_word_item_h_case(WITHHOLDING): {
			modbus_data_value_rw(modbus_data_ctx, channels_info->display_cache_channels.withholding_h);
			channels_info->display_cache_channels.withholding_sync = 1;
		}
		break;

		case add_modbus_data_get_set_item_case(PE_DETECT_DISABLE): {
			modbus_data_value_rw(modbus_data_ctx, channels_settings->pe_detect_disable);
		}
		break;

		//桩状态
		case add_modbus_data_get_set_power_module_info_case(0):
		case add_modbus_data_get_set_power_module_info_case(1):
		case add_modbus_data_get_set_power_module_info_case(2):
		case add_modbus_data_get_set_power_module_info_case(3):
		case add_modbus_data_get_set_power_module_info_case(4):
		case add_modbus_data_get_set_power_module_info_case(5):
		case add_modbus_data_get_set_power_module_info_case(6):
		case add_modbus_data_get_set_power_module_info_case(7):
		case add_modbus_data_get_set_power_module_info_case(8):
		case add_modbus_data_get_set_power_module_info_case(9):
		case add_modbus_data_get_set_power_module_info_case(10):
		case add_modbus_data_get_set_power_module_info_case(11):
		case add_modbus_data_get_set_power_module_info_case(12):
		case add_modbus_data_get_set_power_module_info_case(13):
		case add_modbus_data_get_set_power_module_info_case(14):
		case add_modbus_data_get_set_power_module_info_case(15):
		case add_modbus_data_get_set_power_module_info_case(16):
		case add_modbus_data_get_set_power_module_info_case(17):
		case add_modbus_data_get_set_power_module_info_case(18):
		case add_modbus_data_get_set_power_module_info_case(19):
		case add_modbus_data_get_set_power_module_info_case(20):
		case add_modbus_data_get_set_power_module_info_case(21):
		case add_modbus_data_get_set_power_module_info_case(22):
		case add_modbus_data_get_set_power_module_info_case(23): {
			enum_info_t enum_info;
			get_power_module_enum_info(modbus_data_ctx->addr, &enum_info);
			//debug("%s module %d field %d",
			//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
			//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
			//      "unknow",
			//      enum_info.id,
			//      enum_info.field);

			modbus_data_action_power_module_info(channels_info, modbus_data_ctx, &enum_info);
		}
		break;

		//通道状态
		case add_modbus_data_get_set_channel_status_case(0):
		case add_modbus_data_get_set_channel_status_case(1): {
			enum_info_t enum_info;
			get_channel_status_enum_info(modbus_data_ctx->addr, &enum_info);
			//debug("%s channel %d field %d",
			//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
			//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
			//      "unknow",
			//      enum_info.id,
			//      enum_info.field);

			modbus_data_action_channel_status(channels_info, modbus_data_ctx, &enum_info);
		}
		break;

		//充电记录
		case add_modbus_data_get_set_item_case(CHANNEL_RECORD_NUMBER): {
			channel_record_task_info_t *channel_record_task_info = get_or_alloc_channel_record_task_info(0);
			channel_record_info_t *channel_record_info = &channel_record_task_info->channel_record_info;
			uint16_t start = channel_record_info->start;
			uint16_t end = channel_record_info->end;

			if(end < start) {
				end += CHANNEL_RECORD_NUMBER;
			}

			modbus_data_value_r(modbus_data_ctx, end - start);
		}
		break;

		case add_modbus_data_get_set_item_case(CHANNEL_RECORD_NUMBER_NOT_UPLOAD): {
			channel_record_task_info_t *channel_record_task_info = get_or_alloc_channel_record_task_info(0);

			modbus_data_value_r(modbus_data_ctx, channel_record_task_info->finish_state_count);
		}
		break;

		case add_modbus_data_get_set_item_case(CHANNEL_RECORD_YEAR): {
			modbus_data_value_rw(modbus_data_ctx, channels_info->display_cache_channels.record_dt_cache.year);
			channels_info->display_cache_channels.record_sync = 1;
		}
		break;

		case add_modbus_data_get_set_item_case(CHANNEL_RECORD_MONTH): {
			modbus_data_value_rw(modbus_data_ctx, channels_info->display_cache_channels.record_dt_cache.mon);
			channels_info->display_cache_channels.record_sync = 1;
		}
		break;

		case add_modbus_data_get_set_item_case(CHANNEL_RECORD_DAY): {
			modbus_data_value_rw(modbus_data_ctx, channels_info->display_cache_channels.record_dt_cache.day);
			channels_info->display_cache_channels.record_sync = 1;
		}
		break;

		case add_modbus_data_get_set_item_case(CHANNEL_RECORD_REFRESH): {
			modbus_data_value_r(modbus_data_ctx, 0);
			channels_info->display_cache_channels.record_load_cmd = 1;
			channels_info->display_cache_channels.record_sync = 1;
		}
		break;

		case add_modbus_data_get_set_channel_record_status_case(0):
		case add_modbus_data_get_set_channel_record_status_case(1):
		case add_modbus_data_get_set_channel_record_status_case(2):
		case add_modbus_data_get_set_channel_record_status_case(3):
		case add_modbus_data_get_set_channel_record_status_case(4):
		case add_modbus_data_get_set_channel_record_status_case(5):
		case add_modbus_data_get_set_channel_record_status_case(6):
		case add_modbus_data_get_set_channel_record_status_case(7):
		case add_modbus_data_get_set_channel_record_status_case(8):
		case add_modbus_data_get_set_channel_record_status_case(9): {
			enum_info_t enum_info;
			get_channel_record_status_enum_info(modbus_data_ctx->addr, &enum_info);
			//debug("%s module %d field %d",
			//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
			//      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
			//      "unknow",
			//      enum_info.id,
			//      enum_info.field);

			modbus_data_action_channel_record_status(channels_info, modbus_data_ctx, &enum_info);
		}
		break;

		default:
			debug("error! op:%s, addr:%d",
			      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
			      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
			      "unknow",
			      modbus_data_ctx->addr);
			break;
	}
}
