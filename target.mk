#
#
#================================================================
#   
#   
#   文件名称：target.mk
#   创 建 者：肖飞
#   创建日期：2022年03月18日 星期五 10时17分06秒
#   修改日期：2022年03月18日 星期五 12时35分12秒
#   描    述：
#
#================================================================

ifneq ($(call ifdef_any_of,ENABLE_CXX),)
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(subst -specs=nano.specs,-specs=nosys.specs,$(LDFLAGS)) -o $@
	$(SZ) $@
endif
