#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#include <stdint.h>

/******************************************************************************
RX/TX Defines
******************************************************************************/

extern volatile uint8_t cmd_received;

/******************************************************************************
Function prototypes
******************************************************************************/

extern void clear_cmd(void);

extern char send_cmd(char *cmd);
extern char recv_cmd(void);

extern char exec_cmd(void);
extern void save_cmd(void);

#endif // !__CMDLINE_H__