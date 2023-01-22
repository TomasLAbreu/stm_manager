#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#include <stdint.h>

/******************************************************************************
Special Key Ascii Codes
******************************************************************************/

#define ENTER_KEY   0x0D
#define BCKSP_KEY   0x7F
#define ESC_KEY     0x1B
#define BRAKET_KEY  0x5B
#define DOLLAR_KEY  0x24

#define LEFT_ARROW_KEY  'D'
#define RIGHT_ARROW_KEY 'C'
#define UP_ARROW_KEY    'A'
#define DOWN_ARROW_KEY  'B'

#define HOME_ARROW_KEY  'H'
#define END_ARROW_KEY   'F'

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