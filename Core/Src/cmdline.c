#include "cmdline.h"
#include "usart.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

#include "parser.h" // Using Command_t
#include "commands.h"

/******************************************************************************
Public definitions
******************************************************************************/

volatile uint8_t cmd_received = 0;

/******************************************************************************
Private definitions
******************************************************************************/

#define RX_BUFF_LEN 16

static char Rx_Buffer[RX_BUFF_LEN];
volatile static uint8_t Rx_index = 0;

#define MAX_SAVED_CMD 4

static char saved_cmd_list[MAX_SAVED_CMD][RX_BUFF_LEN];
static int saved_cmd_iter;
static int num_saved_cmd = 0;

/******************************************************************************
Function helpers prototypes
******************************************************************************/
static void insert_cmd(const char* str);

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

typedef struct Special_Key {
  const uint8_t code;         // key ASCII code
  char (*fn)(void);           // return 0 if callback was successful
} Special_Key_t;

static char enter_key_cb(void);
static char bcksp_key_cb(void);

static char esc_key_cb(void);
static char braket_key_cb(void);

static char left_key_cb(void);
static char up_key_cb(void);
static char right_key_cb(void);
static char down_key_cb(void);

static char end_key_cb(void);
static char home_key_cb(void);

// List of special keys codes and callbacks
const Special_Key_t s_key_list[] =
{
  {ENTER_KEY      , enter_key_cb },
  {BCKSP_KEY      , bcksp_key_cb },

  {ESC_KEY        , esc_key_cb   },
  {BRAKET_KEY     , braket_key_cb},

  {LEFT_ARROW_KEY , left_key_cb  },
  {UP_ARROW_KEY   , up_key_cb},
  {RIGHT_ARROW_KEY, right_key_cb },
  {DOWN_ARROW_KEY , down_key_cb},

  {HOME_ARROW_KEY , home_key_cb  },
  {END_ARROW_KEY  , end_key_cb  },

  {0,0}
};

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

char send_cmd(char *cmd)
{
  char err = 0;
  // char str[RX_BUFF_LEN+1];
  // snprintf(str, sizeof(str), "%s\n", cmd);

  insert_cmd(cmd);
  err = exec_cmd();
  clear_cmd();

  return (char)(err);
}

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

char exec_cmd(void)
{
  char err = parse_cmd(cmd_list, Rx_Buffer);

  switch(err)
  {
    case (char)(-ECMDNF):
      // No command found
      UART_puts("Command [");
      UART_puts(Rx_Buffer);
      UART_puts("] not found.\n\r");
      break;

    case (char)(-EINVARG):
      // Invalid Arguments
      UART_puts("Invalid arguments.\n\r");
      break;

    case (char)(-ENOCMD):
      // Command is empty
    case (char)(-ENOMEM):
      // Command list is empty
    //case (char)(-ENOLIST):
      // No memory available or bad allocation of memory
      break;
    case (char)(-EPERM):
      UART_puts("No permission.\n\r");
      break;
  }

  return err;
}

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

void save_cmd(void)
{
  char *cmd = Rx_Buffer;

  // on each new command, the saved_cmd_list must slide one cmd out
  for(int i = MAX_SAVED_CMD-1; i > 0; i--)
  {
    strcpy(saved_cmd_list[i], saved_cmd_list[i-1]);
  }

  // save new command
  strcpy(saved_cmd_list[0], cmd);
  // when this is 0, no cmd should appear on the screen
  saved_cmd_iter = 0;

  // number of available saved commands
  if(num_saved_cmd < MAX_SAVED_CMD)
  {
    num_saved_cmd++;
  }
}

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

char recv_cmd(void)
{
  const Special_Key_t *s_key_ptr = s_key_list;
  char c = UART_getchar();

  // Is the buffer full?
  if(Rx_index == (RX_BUFF_LEN - 1))
  {
    // Treat as new command
    c = ENTER_KEY;
  }

  Rx_UART_init();

  // Is the received char a control char?
  while(s_key_ptr->code)
  {
    // Is this a control char?
    if(c == (s_key_ptr->code))
    {
      // process as control
      s_key_ptr->fn();
      return (char)(-1);
    }

    s_key_ptr++;
  }

  // Its not a special character. Process as data
  Rx_Buffer[Rx_index] = c;
  Rx_index++;

  return c;
}

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

static void insert_cmd(const char* str)
{
  if((str == NULL) || (str[0] == 0))
    return;

  int len = strlen(str);
  if(len > RX_BUFF_LEN)
    return;

  strcpy(Rx_Buffer, str);
  Rx_index = strlen(str);

  // print cmd
  UART_puts(str);
}

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

void clear_cmd(void)
{
  while(bcksp_key_cb() == 0)
    ;

  // Rx_Buffer[Rx_index] = 0;  // mark end of string
  memset(Rx_Buffer, 0, RX_BUFF_LEN);
}

/******************************************************************************
Key callbacks prototypes
******************************************************************************/

static char esc_key_cb(void)
{
  // this may be an escape code
  // ....

  return 0;
}

static char braket_key_cb(void)
{
  // this may be an escape code
  // ....

  return 0;
}

static char enter_key_cb(void)
{
  UART_puts("\n\r");
  Rx_index = 0;
  // informs main a command was received
  cmd_received = 1;

  return 0;
}

static char bcksp_key_cb(void)
{
  char can_move_cursor = (Rx_index > 0);

  if(can_move_cursor)
  {
    Rx_index--;
    // move cursor left and delete character
    UART_puts("\b \b");
  }

  return !can_move_cursor;
}

static char up_key_cb(void)
{
  char can_move_cursor = ((saved_cmd_iter < MAX_SAVED_CMD) & (saved_cmd_iter < num_saved_cmd));

  // check if there is an available saved cmd
  if(can_move_cursor)
  {
    clear_cmd();
    saved_cmd_iter++;
    insert_cmd(saved_cmd_list[saved_cmd_iter-1]);
  }

  return !can_move_cursor;
}

static char down_key_cb(void)
{
  char can_move_cursor = (saved_cmd_iter > 1);

  clear_cmd();

  // check if there is a newer saved cmd to present
  if(can_move_cursor)
  {
    saved_cmd_iter--;
    insert_cmd(saved_cmd_list[saved_cmd_iter-1]);
  }
  // no cmd to present. Stop cycling through saved cmds
  else
  {
    saved_cmd_iter = 0;
  }

  return !can_move_cursor;
}

static char left_key_cb(void)
{
  char can_move_cursor = (Rx_index > 0);

  if(can_move_cursor)
  {
    Rx_index--;
    // move cursor left
    UART_puts("\033[1D");
  }

  return !can_move_cursor;
}

static char right_key_cb(void)
{
  // make sure there is text to the right of the cursor and inside boundaries
  char can_move_cursor = ((Rx_Buffer[Rx_index] != 0) && (Rx_index < RX_BUFF_LEN));

  if(can_move_cursor)
  {
    Rx_index++;
    // move cursor right
    UART_puts("\033[1C");
  }

  return !can_move_cursor;
}

static char home_key_cb(void)
{
  while(left_key_cb() == 0)
    ;

  return 0;
}

static char end_key_cb(void)
{
  while(right_key_cb() == 0)
    ;

  return 0;
}
