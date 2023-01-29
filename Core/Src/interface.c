#include "utils.h"
#include "errors.h"

#include "usart.h"
#include "gpio.h"
#include "adc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
Private defines
******************************************************************************/
// SRAM1: 0x20020000 - 0x2007BFFF (368kB)
// read and write memory

// base pointer
#define SRAM_BASE (uint8_t *)(0x20060000)
// top pointer
#define SRAM_TOP  (uint8_t *)(0x2006FFFF)

/******************************************************************************
Function Helpers Prototypes
******************************************************************************/

/******************************************************************************
@function  Version
@usage		 VER

@brief	 	 Prints the version and colaborators info
******************************************************************************/
char ver_cb(uint8_t argc, char** argv)
{
	if(argc != 1)
		return (char)(-EINVARG);

	UART_puts("Current version: 2.0\n\rUsing STM32F767ZI-NUCLEO.\n\n\r");
	UART_puts("Developers:\n\r- Tomas Abreu\n\r- Diogo Fernandes\n\rJanuary 2023\n\r");
	return 0;
}

char clear_cb(uint8_t argc, char** argv)
{
	UART_puts("\e[1;1H\e[2J");
	return 0;
}
/******************************************************************************
@function  Memory Read
@usage		 MR <addr16> <length8>

@brief	 	 Read <length> memory positions, starting on <addr>
******************************************************************************/
char mr_cb(uint8_t argc, char** argv)
{
	uint32_t addr;
	uint16_t len;
	char str[64];

	// add flags
	// 	-b : select number of bytes to read (default)
	//	-a : select initial addr and final addr
	if(argc != 3)
		return (char)(-EINVARG);

	addr = my_atoi(argv[1]);
	if(!IS_ADDR16(addr))
		return (char)(-EINVARG);

	len = my_atoi(argv[2]);
	if((len == 0) || (!IS_ADDR8(len)))
		return (char)(-EINVARG);

	uint8_t* mem_ptr = (SRAM_BASE + addr);

	// Check if mem_ptr won't go over memory top
	if((mem_ptr + (len - 1)) > SRAM_TOP)
		return (char)(-EINVARG);

	// print header
	snprintf(str, sizeof(str),"Reading %0d bytes starting from %0ld\n\n\r", len, addr);
	UART_puts(str);
	UART_puts("  ADDR |   0  1  2  3   4  5  6  7  |   TEXT   |\n\r");
	UART_puts("=======|============================|==========|\n\r");

	uint8_t i, j;
	char new_byte_str[8];

	for (i = 0; i < len; i++, mem_ptr++)
	{
		// begin of the line
		if((i % 8) == 0)
		{
			snprintf(str, sizeof(str), "0x%04lX | ", addr + i);
			j = 0;
		}

		// in each 4 bytes print double spaces
		snprintf(new_byte_str, sizeof(new_byte_str), " %02X", (*mem_ptr));

		if(j == 4)
		{
			strcat(str, " ");
		}

		strcat(str, new_byte_str);

		if(j == 7)
		{
			strcat(str, "  |");
			for (j = 8; j > 0; j--)
			{
				char c = !IS_PRINTABLE(*(mem_ptr+1-j)) ? ' ' : (char)*(mem_ptr+1-j);
				snprintf(new_byte_str, sizeof(new_byte_str), "%c", c);
				strcat(str, new_byte_str);

				if(j == 5)
					strcat(str, "  ");
			}
			strcat(str, "|");
			// j leaves this for loop with the same value as it entered
			j = 7;
		}

		// is this the last byte? (of the line / of the entire read)
		if((i == len-1) || (j == 7))
		{
			UART_puts(str);
			UART_puts("\n\r");
		}

		j++;
	}

	return 0;
}

/******************************************************************************
@function  Memory Write
@usage		 MW <addr16> <length8> <byte8>

@brief	 	 Write <byte>, starting on memory address <addr> for <lenght> positions
******************************************************************************/
/*
	> mw 0 "hello"

	stores strlen("hello") bytes starting on address 0
	OUTPUT:
	String stored with %d bytes length

	> mr 0 8 | exec

	read command from addr 0 with 8 bytes length and execute it
*/
char mw_cb(uint8_t argc, char** argv)
{
	uint32_t addr;
	uint16_t len;
	uint16_t byte;

	if(argc != 4)
		return (char)(-EINVARG);

	addr = my_atoi(argv[1]);
	if(!IS_ADDR16(addr))
		return (char)(-EINVARG);

	len = (char) my_atoi(argv[2]);
	if((len == 0) || (!IS_ADDR8(len)))
		return (char)(-EINVARG);

	byte = (char) my_atoi(argv[3]);
	if(!IS_ADDR8(byte))
		return (char)(-EINVARG);

	uint8_t* memory_ptr = (SRAM_BASE + addr);

	// Check if memory_ptr won't go over memory top
	if((memory_ptr + (len - 1)) > SRAM_TOP)
		return (char)(-EINVARG);

	uint8_t i;
	char str[42];	// Output message

	for (i = 0; i < len; i++, addr++, memory_ptr++)
	{
		// write memory pointed by memory_ptr
		(*memory_ptr) = byte;

		sprintf(str, "0x%04lX  successfully written.\n\r", addr);
		UART_puts(str);
	}
	// return memory_write(addr, len, byte);
	return 0;
}

/******************************************************************************
@function  Make Pin Input
@usage		 MI <port_addr8> <pin_setting8>

@brief
******************************************************************************/
char mi_cb(uint8_t argc, char** argv)
{
	uint16_t port_addr;
	uint32_t pin_setting;
	char str[32];	// Output message

	if(argc != 3)
		return (char)(-EINVARG);

	port_addr = my_atoi(argv[1]);
	if(!IS_PORT_ADDR(port_addr))
		return (char)(-EINVARG);

	pin_setting = my_atoi(argv[2]);
	if(!IS_PIN_SETTING(pin_setting))
		return (char)(-EINVARG);

	if(GPIO_check_res_pins(port_addr, &pin_setting))
		UART_puts("Reserved pins cannot be configured.\n\r");

	GPIO_config_pins(port_addr, pin_setting, GPIO_MODE_INPUT);

	// check entire byte for bits at level high
	for(uint8_t i = 0; i < 16; i++)
	{
		// checks if LSB is 1
		if(((pin_setting & 0x01) == 1) && (GPIO_PIN_MODE(port_addr, i) == GPIO_MODE_INPUT))
		{
			sprintf(str, "Pin %2d configured as input.\n\r", i);
			UART_puts(str);
		}
		pin_setting >>= 1;
	}
	return 0;
}

/******************************************************************************
@function  Make Pin Output
@usage		 MO <port_addr8> <pin_setting8>

@brief
******************************************************************************/
char mo_cb(uint8_t argc, char** argv)
{
	uint16_t port_addr;
	uint32_t pin_setting;
	char str[32];	// Output message

	if(argc != 3)
		return (char)(-EINVARG);

	port_addr = my_atoi(argv[1]);
	if(!IS_PORT_ADDR(port_addr))
		return (char)(-EINVARG);

	pin_setting = my_atoi(argv[2]);
	if(!IS_PIN_SETTING(pin_setting))
		return (char)(-EINVARG);

	if(GPIO_check_res_pins(port_addr, &pin_setting))
		UART_puts("Reserved pins cannot be configured.\n\r");

	GPIO_config_pins(port_addr, pin_setting, GPIO_MODE_OUTPUT_PP);

	// check entire byte for bits at level high
	for(uint8_t i = 0; i < 16; i++)
	{
		// checks if LSB is 1
		if(((pin_setting & 0x01) == 1) && (GPIO_PIN_MODE(port_addr, i) == GPIO_MODE_OUTPUT_PP))
		{
			sprintf(str, "Pin %2d configured as output.\n\r", i);
			UART_puts(str);
		}
		pin_setting >>= 1;
	}
	return 0;
}

/******************************************************************************
@function  Read Digital Input
@usage		 RD <port_addr8> <pin_setting8>

@brief
******************************************************************************/
char rd_cb(uint8_t argc, char** argv)
{
	uint16_t port_addr;
	uint32_t pin_setting;
	char str[42];	// Output message

	if(argc != 3)
		return (char)(-EINVARG);

	port_addr = my_atoi(argv[1]);
	if(!IS_PORT_ADDR(port_addr))
		return (char)(-EINVARG);

	pin_setting = my_atoi(argv[2]);
	if(!IS_PIN_SETTING(pin_setting))
		return (char)(-EINVARG);

	sprintf(str, "GPIO Port %c\n\r", (char)(port_addr + 'A'));
	UART_puts(str);

	// check entire byte for bits at level high
	for(uint8_t i = 0; i < 16; i++)
	{
		// checks if LSB is 1
		if((pin_setting & 0x01) == 1)
		{
			// PinMode as input?
			if(GPIO_PIN_MODE(port_addr, i) == GPIO_MODE_INPUT)
			{
				uint8_t val;
				val = HAL_GPIO_ReadPin(GPIO_Ports[port_addr], GPIO_Pins[i]);
				sprintf(str, "Pin %2d - '%d'\n\r", i, val);
			}
			// not able to read pin value
			else
			{
				sprintf(str, "Pin %2d not configured as input mode.\n\r", i);
			}

			UART_puts(str);
		}
		else
			UART_puts("0\n\r");

		pin_setting >>= 1;
	}
	return 0;
}

/******************************************************************************
@function  Write Digital Output
@usage		 WD <port_addr8> <pin_setting8> <pin_values8>

@brief
******************************************************************************/
char wd_cb(uint8_t argc, char** argv)
{
	uint16_t port_addr;
	uint32_t pin_setting;
	uint16_t pin_values;
	char str[42];	// Output message

	if(argc != 4)
		return (char)(-EINVARG);

	port_addr = my_atoi(argv[1]);
	if(!IS_PORT_ADDR(port_addr))
		return (char)(-EINVARG);

	pin_setting = my_atoi(argv[2]);
	if(!IS_PIN_SETTING(pin_setting))
		return (char)(-EINVARG);

	pin_values = my_atoi(argv[3]);
	if(!IS_ADDR8(pin_values))
		return (char)(-EINVARG);

	sprintf(str, "GPIO Port %c\n\r", (char)(port_addr + 'A'));
	UART_puts(str);

	// check entire byte for bits at level high
	for(uint8_t i = 0; i < 16; i++)
	{
		// checks if LSB is 1
		if((pin_setting & 0x01) == 1)
		{
			// PinMode as output push-pull?
			if(GPIO_PIN_MODE(port_addr, i) == GPIO_MODE_OUTPUT_PP)
			{
				uint8_t pin_val = (pin_values & 0x01);
				HAL_GPIO_WritePin(GPIO_Ports[port_addr], GPIO_Pins[i], (GPIO_PinState)pin_val);
				sprintf(str, "Pin %2d - '%d'\n\r", i, pin_val);
			}
			// not able to read pin value
			else
			{
				sprintf(str, "Pin %2d not configured as output mode.\n\r", i);
			}

			UART_puts(str);
		}

		pin_setting >>= 1;
		pin_values >>= 1;
	}

	return 0;
}

/******************************************************************************
@function  Analog Read
@usage		 RA <addr2>

@brief
******************************************************************************/
char ra_cb(uint8_t argc, char** argv)
{
	uint8_t addr;
	uint32_t adcValue;
	uint32_t volt;
	char str[40];	// Output message

	if(argc != 2)
		return (char)(-EINVARG);

	addr = my_atoi(argv[1]);
	if(!IS_ADDR4(addr))
		return (char)(-EINVARG);

	// PinMode as output push-pull?
	if(ADC_Config_Channel(&hadc1, addr))
	{
		// not able to read pin value
		UART_puts("Pin not configured as input mode.\n\r");
		return (char)(-EPERM);
	}

	adcValue = ADC_Polling_Conv(&hadc1);
	volt = ((double)adcValue * 3300 / 4095);
	sprintf(str, "ADC Channel %2d value: %ld mV.\n\r", addr, volt);
	UART_puts(str);

	// Restore initial pin mode
	ADC_DeConfig_Channel(&hadc1, addr);

	return 0;
}


/******************************************************************************



Function Helpers Implementation



******************************************************************************/

