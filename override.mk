#
#
#================================================================
#   
#   
#   文件名称：override.mk
#   创 建 者：肖飞
#   创建日期：2020年04月02日 星期四 16时40分02秒
#   修改日期：2022年05月10日 星期二 17时00分29秒
#   描    述：
#
#================================================================

QUIET := @

ifeq ("$(origin V)", "command line")
$(info "verbose flag is $(V)")
QUIET :=
endif

ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
      -nrRf $(firstword $(MAKEFILE_LIST)) \
      ECHO="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
ECHO = echo "`expr " [\`expr $C '*' 100 / $T\`" : '\(.*\)$$'`%]"
endif

ifneq ($(call ifdef_any_of,ENABLE_CXX),)
ifdef GCC_PATH
CXX = $(GCC_PATH)/$(PREFIX)g++
else
CXX = $(PREFIX)g++
endif
endif

C_INCLUDES += $(USER_C_INCLUDES)
ifneq ($(call ifdef_any_of,ENABLE_CXX),)
C_INCLUDES += $(USER_CPP_INCLUDES)
endif

OBJECTS := $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(USER_C_SOURCES:.c=.o))

ifneq ($(call ifdef_any_of,ENABLE_CXX),)
OBJECTS += $(addprefix $(BUILD_DIR)/,$(USER_CPP_SOURCES:.cpp=.o))
endif

ifneq ($(call ifdef_any_of,ENABLE_CXX),)
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@$(ECHO) Compiling $@
	$(QUIET)mkdir -p $(dir $@)
	$(QUIET)$(CC) -c $(subst -MMD,-MD,$(CFLAGS)) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(<:.c=.lst) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@$(ECHO) Compiling $@
	$(QUIET)mkdir -p $(dir $@)
	$(QUIET)$(AS) -c $(subst -MMD,-MD,$(CFLAGS)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	@$(ECHO) Compiling $@
	$(QUIET)mkdir -p $(dir $@)
	$(QUIET)$(CXX) -c $(subst -MMD,-MD,$(CFLAGS)) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(<:.cpp=.lst) $< -o $@
else
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@$(ECHO) Compiling $@
	$(QUIET)mkdir -p $(dir $@)
	$(QUIET)$(CC) -c $(subst -MMD,-MD,$(CFLAGS)) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(<:.c=.lst) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@$(ECHO) Compiling $@
	$(QUIET)mkdir -p $(dir $@)
	$(QUIET)$(AS) -c $(subst -MMD,-MD,$(CFLAGS)) $< -o $@
endif

ifneq ($(call ifdef_any_of,ENABLE_CXX),)
USER_C_SOURCES_C = $(filter %.c ,$(USER_C_SOURCES))
USER_C_SOURCES_CPP = $(filter %.cpp ,$(USER_C_SOURCES))
USER_OBJECTS = $(addprefix $(BUILD_DIR)/,$(USER_C_SOURCES_C:.c=.o))
USER_OBJECTS += $(addprefix $(BUILD_DIR)/,$(USER_C_SOURCES_CPP:.cpp=.o))
else
USER_OBJECTS += $(addprefix $(BUILD_DIR)/,$(USER_C_SOURCES:.c=.o))
endif

$(USER_OBJECTS): override.mk target.mk user.mk $(USER_DEPS)

DEPS := $(OBJECTS:.o=.d)
-include $(DEPS)

all: $(USER_OBJECTS)

.PHONY: $(PHONY)
