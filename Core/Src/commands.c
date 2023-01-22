#include "commands.h"
#include "usart.h"
#include "errors.h"

#include <stdio.h>
#include <string.h>

// Callbacks Includes
#include "interface.h"
#include "cmdline.h"
#include "sampling.h"

/******************************************************************************
@name			cmd_list
@brief	 	List of commands, used in parsing. Each command is made of:
					- Command name
					- Command help text
					- Command callback function
******************************************************************************/
const Command_t cmd_list[] =
{
	{
		"?",
		"Help information",
		"Display extended information on a given command\n\r? <cmd>",
		help_cb
	},
	{
		"version",
		"Display firmware version",
		"",
		ver_cb
	},
	{
		"clear",
		"Clear screen",
		"",
		clear_cb
	},
	{
		"mr",
		"Memory Read",
		"mr <addr16> <length8>",
		mr_cb
	},
	{
		"mw",
		"Memory Write",
		"mw <addr16> <length8> <byte8>",
		mw_cb
	},
	{
		"mi",
		"Make Input",
		"mi <port_addr8> <pin_setting16>",
		mi_cb
	},
	{
		"mo",
		"Make Output",
		"mo <port_addr8> <pin_setting16>",
		mo_cb
	},
	{
		"rd",
		"Read Digital Pin",
		"rd <port_addr8> <pin_setting16>",
		rd_cb
	},
	{
		"wd",
		"Write Digital Pin",
		"wd <port_addr8> <pin_setting16> <pin_values8>",
		wd_cb
	},
	{
		"ra",
		"Read Analog",
		"ra <addr4>",
		ra_cb
	},
	{
		"sp",
		"Sampling Period",
		"sp <timeScale> <timeVal>\n\r  <timeScale>: s - seconds; ms - milissecs.; micro - microsecs.\n\r  <timeVal>: hex value",
		sp_cb
	},
	// {
	// 	"AC",
	// 	"Analog Channel",
	// 	"AC <adcCHnum>",
	// 	ac_cb
	// },
	// {
	// 	"FN",
	// 	"Filter ON",
	// 	"FN <filter>\n\r  <filter>: LP - low pass; HP - high pass; BP - band pass",
	// 	fn_cb
	// },
	// {
	// 	"FF",
	// 	"Filter OFF",
	// 	"",
	// 	ff_cb
	// },
	{
		"S",
		"Sample",
		"S [<k>]\n\r\t<k> Sample k values",
		s_cb
	},
	// {
	// 	"ST",
	// 	"Stop Sampling",
	// 	"",
	// 	st_cb
	// },
	// {
	// 	"WG",
	// 	"Wave Generator",
	// 	"WG <wave> <freq>\n\r  <wave>: sqr - square; sin - sine; tri - triangular\n\r  <freq>: hex value, in Hz. Max 0x64 = 100hz",
	// 	wg_cb
	// },
	{ //end of Command list
		0,
		0,
		0
	},
};

/******************************************************************************
@function  Help
@usage		 ?

@brief	 	 Print a list of the valid commands
******************************************************************************/
#define OPDONE 		2

static char print_arg_help(const Command_t *p, char arg[])
{
	char str[256];

	if(strcmp(p->cmd,arg) == 0)
	{
		snprintf(str, sizeof(str), "%-6s %-28s\n\r%s\n\r", p->cmd, p->help, p->usage);
		UART_puts(str);
		return OPDONE;
	}

	return ECMDNF;
}

static char print_full_help(const Command_t *p, char arg[])
{
	char str[128];

//	snprintf(str, sizeof(str), "%-6s %-28s %s\n\r", p->cmd, p->help, p->usage);
	snprintf(str, sizeof(str), "%-6s %s\n\r", p->cmd, p->help);
	UART_puts(str);

	return 0;
}

char help_cb(uint8_t argc, char** argv)
{
	const Command_t *p = cmd_list;
	char str[128]; // Output message. Max message len is the same as buffer used in UART_puts
	char (*fun_ptr)(const Command_t *, char*) = &print_full_help;
	char retval = 0;

	if(argc > 2) // number of arguments invalid?
		return (char)(-EINVARG);

	if(argc == 2)
	{
		fun_ptr = &print_arg_help;
	}
	else
	{
		UART_puts("List of utils:\n\r");
		UART_puts("$      Insert last valid command\n\r");
		UART_puts("ESC    Clears input line\n\n\r");

		UART_puts("List of valid commands:\n\r");
//		snprintf(str, sizeof(str), "%-6s|%-28s|%-s\n\n\r", "CMD", "HELP TEXT", "USAGE");
//		UART_puts(str);
	}

	while (p->cmd && (retval != OPDONE))
	{
		retval = fun_ptr(p, argv[1]);
		p++;
	}

	if(retval == ECMDNF)
	{
		snprintf(str, sizeof(str), "Command [%s] not found.\n\r", argv[1]);
		UART_puts(str);
	}

	return 0;
}
