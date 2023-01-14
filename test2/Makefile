################################################################################
# Makefile: Bare-metal cross-compile for STM32F76x
#
# Tomas Abreu
#
# 2023, jan 14
################################################################################
CC  = arm-none-eabi-gcc
AS  = arm-none-eabi-as
# LD  = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
################################################################################
STL  	 = st-flash
CFLAGS = -mthumb -mcpu=cortex-m7 -fmax-errors=5 -D STM32F767xx

################################################################################

CORE_SRC_DIR    = ./Src
CORE_INC_DIR 		= ./Inc

DRIVERS_SRC_DIR = ./Drivers/STM32F7xx_HAL_Driver/Src
DRIVERS_INC_DIR = ./Drivers/STM32F7xx_HAL_Driver/Inc

CMSIS_INC_DIR   = ./Drivers/CMSIS/Device/ST/STM32F7xx/Include/

BLD_DIR = ./build

TARGET=app

################################################################################
SRCS = $(wildcard $(CORE_SRC_DIR)/*.c*)
SRCS+= $(wildcard $(DRIVERS_SRC_DIR)/*.c*)

OBJS = $(filter %.o,$(patsubst $(CORE_SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRCS)))
OBJS+= $(filter %.o,$(patsubst $(DRIVERS_SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRCS)))

DEPS = $(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))

vpath %.c $(CORE_SRC_DIR) $(DRIVERS_SRC_DIR)
vpath %.h $(CORE_INC_DIR) $(DRIVERS_INC_DIR) $(CMSIS_INC_DIR)
################################################################################
# Escape colors for printing messages
BLUE 	=\033[0;34m
GREEN	=\033[0;32m
CYAN	=\033[0;36m
RESET	=\033[0m
# Default print messages
PRINT_GENERATING="${CYAN}Generating $(shell basename $@) ...$(RESET)"
PRINT_BUILDING	="${BLUE}Building $(shell basename $@) ...$(RESET)"
PRINT_LINKING	="${GREEN}Linking $(shell basename $@) ...$(RESET)"

all: $(TARGET).bin

print:
	@echo -e "SRCS: $(SRCS)" | tr ' ' '\n'
	@echo -e "OBJS: $(OBJS)" | tr ' ' '\n'

$(BLD_DIR)/%.o: %.c
	@echo $(PRINT_BUILDING)
	@$(CC) $< -c -o $@ $(CFLAGS) -I $(CORE_INC_DIR) -I $(DRIVERS_INC_DIR) -I $(CMSIS_INC_DIR) -I ./Drivers/CMSIS/Include

startup_stm32f767zitx.o: startup_stm32f767zitx.s
	@echo $(PRINT_BUILDING)
	@$(AS) $< -c -o $@

$(TARGET).elf: STM32F767ZITX_FLASH.ld startup_stm32f767zitx.o $(OBJS)
	@echo $(PRINT_LINKING)
	@$(CC) -T STM32F767ZITX_FLASH.ld -o $(TARGET).elf startup_stm32f767zitx.o $(OBJS)

$(TARGET).bin: $(TARGET).elf
	@echo $(PRINT_GENERATING)
	@$(BIN) -O binary $(TARGET).elf $(TARGET).bin

################################################################################

flash: $(TARGET).bin
	@echo $(PRINT_FLASHING)
	@$(STL) write $(TARGET).bin 0x8000000

erase:
	$(STL) erase

################################################################################

clean:
	rm -f *.o *.elf *.bin
	rm -rf ./build
	mkdir build