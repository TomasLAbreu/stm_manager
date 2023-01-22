################################################################################
# Makefile: Bare-metal cross-compile for STM32F76x
#
# Tomas Abreu
#
# 2023, jan 14
################################################################################
# Escape colors for printing messages
BLUE  =\033[0;34m
GREEN =\033[0;32m
CYAN  =\033[0;36m
RESET =\033[0m
################################################################################

BLD_DIR         = build
BIN_DIR         = bin

CORE_SRC_DIR    = Core/Src
CORE_INC_DIR    = Core/Inc

DRIVERS_SRC_DIR = Drivers/STM32F7xx_HAL_Driver/Src
DRIVERS_INC_DIR = Drivers/STM32F7xx_HAL_Driver/Inc

CMSIS_INC_DIR   = Drivers/CMSIS/Device/ST/STM32F7xx/Include/

# Startup files
LDSCRIPT        = STM32F767ZITX_FLASH.ld
STARTUP         = startup_stm32f767zitx

################################################################################

TOOLCHAIN = arm-none-eabi
STL       = st-flash
CPU       = cortex-m7
N_JOBS   ?= 8

# Build artifact name
TARGET    = $(BIN_DIR)/app
# TARGET    = app

MAKEFLAGS     += --no-print-directory -j${N_JOBS}
.DEFAULT_GOAL := all

################################################################################
# Toolchain Tools and Flags

DEFINES = \
	-DDEBUG \
	-DUSE_HAL_DRIVER \
	-DSTM32F767xx

INCLUDES = \
	-I$(CORE_INC_DIR) \
	-I$(DRIVERS_INC_DIR) \
	-I$(CMSIS_INC_DIR) \
	-I./Drivers/CMSIS/Include
	# -I./Drivers/STM32F7xx_HAL_Driver/Inc/Legacy \

CC = $(TOOLCHAIN)-gcc
CCFLAGS = \
	-mcpu=$(CPU) \
	-mfloat-abi=hard \
	-std=gnu11 -fmax-errors=5\
	-g3 -c -MMD -MP \
	-O0 -ffunction-sections -fdata-sections -Wall -fstack-usage \
	$(DEFINES) \
	$(INCLUDES)

# LD = $(TOOLCHAIN)-ld
LDFLAGS = \
	-mcpu=$(CPU) \
	-mfpu=fpv5-d16 \
	-mfloat-abi=hard -mthumb \
	--specs=nosys.specs \
	-Wl,--gc-sections -static --specs=nano.specs \
	-Wl,--start-group -lc -lm \
	-Wl,--end-group

# AS = $(TOOLCHAIN)-as
ASFLAGS = \
	-mcpu=$(CPU) -g3 -c -MMD -MP \
	-mfpu=fpv5-d16 \
	-mfloat-abi=hard -mthumb \
	--specs=nano.specs \

BIN = $(TOOLCHAIN)-objcopy

################################################################################

SRCS = $(wildcard $(DRIVERS_SRC_DIR)/*.c*)
SRCS+= $(wildcard $(CORE_SRC_DIR)/*.c*)

OBJS = $(filter %.o,$(patsubst $(DRIVERS_SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRCS)))
OBJS+= $(filter %.o,$(patsubst $(CORE_SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRCS)))

DEPS = $(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))

vpath %.c $(CORE_SRC_DIR) $(DRIVERS_SRC_DIR)
vpath %.h $(CORE_INC_DIR) $(DRIVERS_INC_DIR) $(CMSIS_INC_DIR)
-include ./build/*.d

################################################################################
.PHONY:all
all: $(TARGET).bin ## Generate target elf file

################################################################################

$(BLD_DIR)/%.o: %.c
	@echo "$(BLUE)Building $@$(RESET)"
	@$(CC) $< -c -o $@ $(CCFLAGS)

$(BLD_DIR)/$(STARTUP).o: $(STARTUP).s
	@echo "$(BLUE)Building $@$(RESET)"
	@$(CC) $< -c -o $@ $(ASFLAGS)

################################################################################

$(TARGET).elf: $(LDSCRIPT) $(OBJS) $(BLD_DIR)/$(STARTUP).o
	@echo "$(GREEN)Linking $@$(RESET)"
	@$(CC) -o $@ -T $^ $(LDFLAGS)
# 	@arm-none-eabi-size $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	@echo "$(CYAN)Generating $@$(RESET)"
	@$(BIN) -O binary $(TARGET).elf $(TARGET).bin

################################################################################
.PHONY: flash erase

flash: $(TARGET).bin ## Flash STM32
	@echo "$(GREEN)Flashing $<$(RESET)"
	@$(STL) --reset write $< 0x8000000

erase: ## Erase STM32 memory
	@echo "$(GREEN)Erasing$(RESET)"
	@$(STL) erase

################################################################################
.PHONY: clean help

clean: ## Clean build artifacts
	@echo "$(GREEN)Cleaning$(RESET)"
	@rm -rf *.o *.elf *.bin $(BLD_DIR) $(BIN_DIR)
	@mkdir $(BLD_DIR) $(BIN_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(CYAN)%-15s $(RESET)%s\n", $$1, $$2}'
