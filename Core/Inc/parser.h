#ifndef __PARSER_H__
#define __PARSER_H__

#include "commands.h"

#define DELIMETER " " // command arguments delimiter

extern char parser(const Command_t cmd_list[], const char *str);

#endif // !__PARSER_H__
