#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdint.h>

/******************************************************************************
Global Variables
******************************************************************************/

// callback - function pointer
typedef char (*Command_cb)(uint8_t, char *[]);

// brief simple struct to hold data for a single command
typedef struct Command
{
  const char *cmd;            // the command string to match against
  const char *help;           // the help text associated with cmd
  const char *usage;          // command usage
  Command_cb fn;              // the function to call when cmd is matched
} Command_t;

extern const Command_t cmd_list[];

#endif // !__COMMANDS_H__
