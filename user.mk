#
#
#================================================================
#   
#   
#   文件名称：user.mk
#   创 建 者：肖飞
#   创建日期：2019年10月25日 星期五 13时04分38秒
#   修改日期：2022年08月31日 星期三 09时01分36秒
#   描    述：
#
#================================================================
include config.mk

ifndef_any_of = $(filter undefined,$(foreach v,$(1),$(origin $(addprefix CONFIG_,$(v)))))
ifdef_any_of = $(filter-out undefined,$(foreach v,$(1),$(origin $(addprefix CONFIG_,$(v)))))

USER_C_INCLUDES += -Iapps
USER_C_INCLUDES += -Iapps/modules
USER_C_INCLUDES += -Iapps/modules/os
USER_C_INCLUDES += -Iapps/modules/drivers
USER_C_INCLUDES += -Iapps/modules/hardware
USER_C_INCLUDES += -Iapps/modules/app
USER_C_INCLUDES += -Iapps/modules/app/charger
USER_C_INCLUDES += -Iapps/modules/app/power_modules
USER_C_INCLUDES += -Iapps/modules/app/power_manager
ifneq ($(call ifdef_any_of,ENABLE_USB_OTG),)
USER_C_INCLUDES += -Iapps/modules/app/ftpd
endif
USER_C_INCLUDES += -Iapps/modules/app/vfs_disk
USER_C_INCLUDES += -Iapps/modules/app/net_client
USER_C_INCLUDES += -Iapps/modules/tests
USER_C_INCLUDES += -IcJSON
ifneq ($(call ifdef_any_of,ENABLE_CXX),)
USER_CPP_INCLUDES += -Iapps/modules/os/cxx
endif

USER_C_SOURCES += apps/os_memory.c
USER_C_SOURCES += apps/os_random.c
USER_C_SOURCES += apps/app.c
USER_C_SOURCES += apps/uart_debug_handler.c
USER_C_SOURCES += apps/probe_tool_handler.c
USER_C_SOURCES += apps/channels_config.c
USER_C_SOURCES += apps/can_config.c
USER_C_SOURCES += apps/storage_config.c
USER_C_SOURCES += apps/gpio_map.c
USER_C_SOURCES += apps/channels_custom.c
ifeq ($(call ifdef_any_of,DISABLE_DISPLAY),)
USER_C_SOURCES += apps/modbus_addr_handler.c
USER_C_SOURCES += apps/display_cache.c
endif
ifeq ($(call ifdef_any_of,DISABLE_VOICE),)
USER_C_SOURCES += apps/channels_notify_voice.c
endif
USER_C_SOURCES += apps/power_manager_group_policy_config.c
ifneq ($(call ifdef_any_of,ENABLE_CXX),)
USER_CPP_SOURCES += apps/modules/tests/test_cpp.cpp
endif

USER_C_SOURCES += apps/modules/app/config_utils.c
USER_C_SOURCES += apps/modules/app/poll_loop.c
USER_C_SOURCES += apps/modules/app/request.c
USER_C_SOURCES += apps/modules/app/probe_tool.c
USER_C_SOURCES += apps/modules/app/uart_debug.c
ifneq ($(call ifdef_any_of,ENABLE_USB_OTG),)
USER_C_SOURCES += apps/modules/app/file_log.c
endif
USER_C_SOURCES += apps/modules/app/net_client/net_client.c
ifneq ($(call ifdef_any_of,NET_CLIENT_PROTOCOL_UDP),)
USER_C_SOURCES += apps/modules/app/net_client/net_protocol_udp.c
endif
ifneq ($(call ifdef_any_of,NET_CLIENT_PROTOCOL_TCP),)
USER_C_SOURCES += apps/modules/app/net_client/net_protocol_tcp.c
endif
ifneq ($(call ifdef_any_of,NET_CLIENT_PROTOCOL_TLS),)
USER_C_SOURCES += apps/modules/app/net_client/net_protocol_tls.c
endif
ifneq ($(call ifdef_any_of,NET_CLIENT_REQUEST_DEFAULT),)
USER_C_SOURCES += apps/modules/app/net_client/request_default.c
endif
ifneq ($(call ifdef_any_of,NET_CLIENT_REQUEST_SSE),)
USER_C_SOURCES += apps/modules/app/net_client/request_sse.c
endif
ifneq ($(call ifdef_any_of,NET_CLIENT_REQUEST_OCPP_1_6),)
USER_C_SOURCES += apps/modules/app/net_client/test_https.c
USER_C_SOURCES += apps/modules/app/net_client/https.c
USER_C_SOURCES += apps/modules/app/net_client/websocket.c
USER_C_SOURCES += apps/modules/app/net_client/request_ocpp_1_6.c
endif
USER_C_SOURCES += apps/modules/app/ftp_client.c
USER_C_SOURCES += apps/modules/app/ntp_client.c
USER_C_SOURCES += apps/modules/app/net_callback.c
ifneq ($(call ifdef_any_of,ENABLE_USB_OTG),)
USER_C_SOURCES += apps/modules/app/ftpd/ftpd.c
#USER_C_SOURCES += apps/modules/app/vfs_disk/pseudo_disk_io.c
#USER_C_INCLUDES += -Iapps/modules/app/ftpd/vfs_ramdisk
#C_SOURCES := $(filter-out Middlewares/Third_Party/FatFs/src/diskio.c ,$(C_SOURCES))
USER_C_SOURCES += apps/modules/app/vfs_disk/vfs.c
USER_C_SOURCES += apps/modules/app/mt_file.c
endif
USER_C_SOURCES += apps/modules/app/can_data_task.c
USER_C_SOURCES += apps/modules/app/uart_data_task.c
USER_C_SOURCES += apps/modules/app/duty_cycle_pattern.c
ifneq ($(call ifdef_any_of,ENABLE_USB_OTG),)
USER_C_SOURCES += apps/modules/app/usbh_user_callback.c
endif
USER_C_SOURCES += apps/modules/app/early_sys_callback.c
USER_C_SOURCES += apps/modules/app/connect_state.c
USER_C_SOURCES += apps/modules/app/ntc_temperature.c
USER_C_SOURCES += apps/modules/app/pt_temperature.c
USER_C_SOURCES += apps/modules/app/can_command.c
ifneq ($(call ifdef_any_of,ENABLE_USB_OTG),)
USER_C_SOURCES += apps/modules/app/usb_upgrade.c
endif
USER_C_SOURCES += apps/modules/app/firmware_upgrade_internal_flash.c
ifeq ($(call ifdef_any_of,DISABLE_DISPLAY),)
USER_C_SOURCES += apps/modules/app/display.c
endif
ifeq ($(call ifdef_any_of,DISABLE_VOICE),)
USER_C_SOURCES += apps/modules/app/voice.c
endif
ifeq ($(call ifdef_any_of,DISABLE_POWER_MANAGER),)
USER_C_SOURCES += apps/modules/app/power_modules/power_modules.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_none.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_pseudo.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_huawei.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_increase.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_infy.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_stategrid.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_yyln.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_winline.c
USER_C_SOURCES += apps/modules/app/power_modules/power_modules_handler_zte.c
USER_C_SOURCES += apps/modules/app/power_manager/power_manager.c
USER_C_SOURCES += apps/modules/app/power_manager/power_manager_handler_native.c
USER_C_SOURCES += apps/modules/app/power_manager/power_manager_group_policy_chain.c
endif
USER_C_SOURCES += apps/modules/app/charger/channels_config_helper.c
USER_C_SOURCES += apps/modules/app/charger/channels.c
USER_C_SOURCES += apps/modules/app/charger/channel.c
ifneq ($(call ifdef_any_of,CHARGER_CHANNEL_NATIVE),)
USER_C_SOURCES += apps/modules/app/charger/channel_handler_native.c
endif
ifneq ($(call ifdef_any_of,CHARGER_CHANNEL_PROXY_REMOTE),)
USER_C_SOURCES += apps/modules/app/charger/channel_handler_proxy_remote.c
endif
ifneq ($(call ifdef_any_of,CHARGER_CHANNEL_PROXY_LOCAL),)
USER_C_SOURCES += apps/modules/app/charger/channel_handler_proxy_local.c
endif
ifneq ($(call ifdef_any_of,CHARGER_CHANNEL_PROXY_REMOTE CHARGER_CHANNEL_PROXY_LOCAL),)
USER_C_SOURCES += apps/modules/app/charger/channels_comm_proxy.c
endif
ifneq ($(call ifdef_any_of,MULTI_CHARGE_PROXY),)
USER_C_SOURCES += apps/modules/app/charger/multi_charge_comm_proxy.c
endif
USER_C_SOURCES += apps/modules/app/charger/charger.c
USER_C_SOURCES += apps/modules/app/charger/charger_bms.c
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_GB CHARGER_BMS_HANDLER_PLC_CCS CHARGER_BMS_HANDLER_GB_MULTI_CHARGE),)
USER_C_SOURCES += apps/modules/app/bms_multi_data.c
endif
USER_C_SOURCES += apps/modules/app/charger/function_board.c
ifneq ($(call ifdef_any_of,FUNCTION_BOARD_HANDLER_485),)
USER_C_SOURCES += apps/modules/app/charger/function_board_handler_485.c
endif
ifneq ($(call ifdef_any_of,FUNCTION_BOARD_HANDLER_V5),)
USER_C_SOURCES += apps/modules/app/charger/function_board_handler_v5.c
endif
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_GB),)
USER_C_SOURCES += apps/modules/app/charger/charger_bms_gb.c
endif
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_PLC_CCS),)
USER_C_SOURCES += apps/modules/app/charger/charger_bms_plc_ccs.c
endif
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_JP),)
USER_C_SOURCES += apps/modules/app/charger/charger_bms_jp.c
endif
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_GB_MULTI_CHARGE),)
USER_C_SOURCES += apps/modules/app/charger/charger_bms_gb_multi_charge.c
endif
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_AC),)
USER_C_SOURCES += apps/modules/app/charger/charger_bms_ac.c
endif
ifneq ($(call ifdef_any_of,CHARGER_BMS_HANDLER_NOBMS),)
USER_C_SOURCES += apps/modules/app/charger/charger_bms_nobms.c
endif
USER_C_SOURCES += apps/modules/app/charger/energy_meter.c
USER_C_SOURCES += apps/modules/app/charger/energy_meter_handler_dc.c
USER_C_SOURCES += apps/modules/app/charger/energy_meter_handler_ac.c
USER_C_SOURCES += apps/modules/app/charger/energy_meter_handler_ac_hlw8032.c
USER_C_SOURCES += apps/modules/app/charger/energy_meter_handler_ac_sdm_220.c
USER_C_SOURCES += apps/modules/app/charger/energy_meter_handler_ac_sdm_630.c
USER_C_SOURCES += apps/modules/app/charger/channel_record.c
ifeq ($(call ifdef_any_of,DISABLE_CARDREADER),)
USER_C_SOURCES += apps/modules/app/charger/card_reader.c
USER_C_SOURCES += apps/modules/app/charger/card_reader_handler_pseudo.c
USER_C_SOURCES += apps/modules/app/charger/card_reader_handler_zlg.c
USER_C_SOURCES += apps/modules/app/charger/card_reader_handler_mt_318_626.c
USER_C_SOURCES += apps/modules/app/charger/card_reader_handler_mt_318_628.c
endif
ifneq ($(call ifdef_any_of,CHARGER_CHANNEL_PROXY_REMOTE),)
USER_C_SOURCES += apps/modules/app/charger/channels_comm_proxy_remote.c
endif
ifneq ($(call ifdef_any_of,CHARGER_CHANNEL_PROXY_LOCAL),)
USER_C_SOURCES += apps/modules/app/charger/channels_comm_proxy_local.c
endif
USER_C_SOURCES += apps/modules/hardware/flash.c
USER_C_SOURCES += apps/modules/hardware/dlt_645_master_txrx.c
USER_C_SOURCES += apps/modules/hardware/hw_adc.c
USER_C_SOURCES += apps/modules/hardware/modbus_slave_txrx.c
USER_C_SOURCES += apps/modules/hardware/modbus_master_txrx.c
USER_C_SOURCES += apps/modules/hardware/modbus_spec.c
USER_C_SOURCES += apps/modules/hardware/storage.c
ifneq ($(call ifdef_any_of,STORAGE_OPS_25LC1024),)
USER_C_SOURCES += apps/modules/hardware/storage_25lc1024.c
endif
ifneq ($(call ifdef_any_of,STORAGE_OPS_24LC128),)
USER_C_SOURCES += apps/modules/hardware/storage_24lc128.c
endif
ifneq ($(call ifdef_any_of,STORAGE_OPS_W25Q256),)
USER_C_SOURCES += apps/modules/hardware/storage_w25q256.c
endif
USER_C_SOURCES += apps/modules/drivers/spi_txrx.c
USER_C_SOURCES += apps/modules/drivers/can_txrx.c
USER_C_SOURCES += apps/modules/drivers/can_ops_hal.c
USER_C_SOURCES += apps/modules/drivers/usart_txrx.c
USER_C_SOURCES += apps/modules/os/event_helper.c
USER_C_SOURCES += apps/modules/os/callback_chain.c
USER_C_SOURCES += apps/modules/os/bitmap_ops.c
USER_C_SOURCES += apps/modules/os/iap.c
USER_C_SOURCES += apps/modules/os/os_utils.c
USER_C_SOURCES += apps/modules/os/net_utils.c
USER_C_SOURCES += apps/modules/os/cpu_utils.c
USER_C_SOURCES += apps/modules/os/log.c
USER_C_SOURCES += apps/modules/os/object_class.c
USER_C_SOURCES += apps/modules/os/retarget.c
USER_C_SOURCES += apps/modules/os/syscalls.c
ifneq ($(call ifdef_any_of,ENABLE_CXX),)
USER_CPP_SOURCES += apps/modules/os/cxx/override.cpp
endif
USER_C_SOURCES += apps/modules/tests/test_serial.c
USER_C_SOURCES += apps/modules/tests/test_event.c
USER_C_SOURCES += apps/modules/tests/test_storage.c
USER_C_SOURCES += cJSON/cJSON.c

USER_CFLAGS += -DtraceTASK_SWITCHED_IN=StartIdleMonitor -DtraceTASK_SWITCHED_OUT=EndIdleMonitor
USER_CFLAGS += -DLOG_CONFIG_FILE=\"log_config.h\"
USER_CFLAGS += -DCJSON_API_VISIBILITY -DCJSON_EXPORT_SYMBOLS -DENABLE_LOCALES -Dcjson_EXPORTS

#USER_CFLAGS += -DLOG_DISABLE
#USER_CFLAGS += -DALLOC_TRACE_DISABLE

CFLAGS += $(USER_CFLAGS) $(CONFIG_CFLAGS)

#LDFLAGS += -u _printf_float -Wl,--wrap=srand -Wl,--wrap=rand
LDFLAGS += -u _printf_float

default: all

IAP_FILE := apps/modules/os/iap.h

#define update-iap-include
#	if [ -f $(IAP_FILE) ]; then
#		touch $(IAP_FILE);
#	fi
#endef

ifneq ($(call ifdef_any_of,USER_APP),)
build-type := .app.stamps
build-type-invalid := .bootloader.stamps
CFLAGS += -DUSER_APP
LDSCRIPT = STM32F207VETx_FLASH_APP.ld
#$(info $(shell $(update-iap-include)))
$(info "build app!")
else
build-type := .bootloader.stamps
build-type-invalid := .app.stamps
LDSCRIPT = STM32F207VETx_FLASH.ld
#$(info $(shell $(update-iap-include)))
$(info "build bootloader!")
endif

$(build-type) :
#	$(shell $(update-iap-include))
	-rm $(build-type-invalid)
	touch $@


PHONY += all
PHONY += default

USER_DEPS := config.mk $(build-type) $(LDSCRIPT)

cscope: all
	rm cscope e_cs -rf
	mkdir -p cscope
	#$(silent)tags.sh prepare;
	$(silent)touch dep_files;
	$(silent)touch raw_dep_files;
	$(silent)for f in $$(find . -type f -name "*.d" 2>/dev/null); do \
		cat "$$f" >> raw_dep_files; \
	done;
	for i in $$(cat "raw_dep_files" | sed 's/^.*://g' | sed 's/[\\ ]/\n/g' | sort -h | uniq); do \
		if test "$${i:0:1}" = "/";then \
			echo "$$i" >> dep_files; \
		else \
			readlink -f "$$i" >> dep_files; \
		fi; \
	done; \
	$(silent)rm raw_dep_files
	$(silent)cat dep_files | sort | uniq | sed 's/^\(.*\)$$/\"\1\"/g' >> cscope/cscope.files;
	$(silent)cat dep_files | sort | uniq >> cscope/ctags.files;
	$(silent)rm dep_files
	$(silent)tags.sh cscope;
	$(silent)tags.sh tags;
	$(silent)tags.sh env;

clean: clean-cscope
clean-cscope:
	rm cscope e_cs -rf

firmware:
	python apps/modules/fw.py -f build/eva.bin

