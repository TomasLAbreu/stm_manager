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

/******************************************************************************
RX/TX Defines
******************************************************************************/

#define RX_BUFF_LEN 16

extern char Rx_Buffer[RX_BUFF_LEN];
extern volatile uint8_t Rx_index;
extern volatile uint8_t cmd_received;

/******************************************************************************
Function prototypes
******************************************************************************/

extern char UART_Receive(void);

#endif // !__CMDLINE_H__