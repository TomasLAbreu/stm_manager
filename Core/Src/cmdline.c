#include "cmdline.h"
#include "usart.h"
#include "utils.h"
#include <string.h>

char Rx_Buffer[RX_BUFF_LEN];
volatile uint8_t Rx_index = 0;

volatile uint8_t cmd_received = 0;

/******************************************************************************
Function prototypes
******************************************************************************/

static void clear_last_cmd(void);
static void insert_cmd(const char* str);

static void process_as_data(char c);
static char process_as_control(char c);

static void enter_key_cb(void);
static void bcksp_key_cb(void);
static void esc_key_cb(void);
static void braket_key_cb(void);
static void dollar_key_cb(void);

static void left_key_cb(void);
// static void up_key_cb(void);
static void right_key_cb(void);
//static void down_key_cb(void);

char UART_Receive(void)
{
  char c = UART_getchar();

  // Is the buffer full?
  if(Rx_index == (RX_BUFF_LEN - 1))
  {
    // Treat as new command
    c = ENTER_KEY;
  }

  // Is this the end of reception?
  // if(c != ENTER_KEY)
  // {
  //  // Not the end of reception. Prepare for next character
  //  Rx_UART_init();
  // }
  Rx_UART_init();

  // Is the received char a control char?
  if(process_as_control(c) == 0)
   return (char)(-1);

    // UART_puts("f");
  // if(!IS_PRINTABLE(c))
  {
  //  c = 0;
  //  enter_key_cb();

  //  // clear receiver buffer
  //  HAL_UART_AbortReceive(&huart3);

    // return (char)(-1);
  }

  // Its not a special character
  process_as_data(c);

  return c;
}
/******************************************************************************
@param     char received 'c' via UART

@brief     process the char received as a special character
******************************************************************************/

typedef struct Special_Key {
  const uint8_t code;
  void (*fn)(void);
} Special_Key_t;

// List of special keys codes and callbacks
const Special_Key_t s_key_list[] =
{
  {ENTER_KEY      , enter_key_cb },
  {BCKSP_KEY      , bcksp_key_cb },

  {ESC_KEY        , esc_key_cb   },
  {BRAKET_KEY     , braket_key_cb},
  {DOLLAR_KEY     , dollar_key_cb},

  {LEFT_ARROW_KEY , left_key_cb  },
  {UP_ARROW_KEY   , dollar_key_cb},
  {RIGHT_ARROW_KEY, right_key_cb },
  {DOWN_ARROW_KEY , clear_last_cmd  },

  {0,0}
};

static char process_as_control(char c)
{
  const Special_Key_t *s_key_ptr = s_key_list;

  while(s_key_ptr->code)
  {
    // Is this a control char?
    if(c == (s_key_ptr->code))
    {
      s_key_ptr->fn();
      return 0;
    }

    s_key_ptr++;
  }

  // Its not a control char
  return (char)(-1);
}

static void enter_key_cb(void)
{
  UART_puts("\n\r");

  // mark end of string
  Rx_Buffer[Rx_index] = 0;
  Rx_index = 0;

  // informs main a command was received
  cmd_received = 1;
}

static void bcksp_key_cb(void)
{
  // Is there characters left to delete?
  if(Rx_index > 0)
  {
    Rx_index--;
    // clear buffer in this position
    // the use of arrows to place the cursor in the command may demand this cleaning
    // Rx_Buffer[Rx_index] = 0;

    // move cursor left and delete character
    UART_puts("\b \b");
  }
}

static void esc_key_cb(void)
{
  // clear_last_cmd();
  // HAL_UART_AbortReceive(&huart3);
  // this may be an escape code
  // ....
  // UART_puts("esc_key_cb");
}

static void braket_key_cb(void)
{
  // clear_last_cmd();
  // HAL_UART_AbortReceive(&huart3);
  // this may be an escape code
  // ....
  // UART_puts("braket_key_cb");
}

static void dollar_key_cb(void)
{
  clear_last_cmd();
  UART_puts("teste"); // print last valid command
  insert_cmd("teste"); // fill Rx_Buffer with last valid command
}

static void left_key_cb(void)
{
  // Is there characters by the left side of the cursor?
  if(Rx_index > 0)
  {
    // move cursor left
    Rx_index--;
    UART_puts("\033[1D");
  }
}

static void right_key_cb(void)
{
  if(Rx_index < strlen(Rx_Buffer))
  {
    Rx_index++;
    UART_puts("\033[1C");
  }
}

// static void down_key_cb(void)
// {

// }

/******************************************************************************
@param     char received 'c' via UART

@brief     process the char received as a data character
******************************************************************************/

static void process_as_data(char c)
{
  // add received char to Rx_Buffer
  Rx_Buffer[Rx_index] = c;
  Rx_index++;
}

/******************************************************************************
@brief     send backspaces via terminal to clear the command line
******************************************************************************/
static void clear_last_cmd(void)
{
  while(Rx_index > 0)
  {
    bcksp_key_cb();
  }

  Rx_Buffer[Rx_index] = 0;  // mark end of string
}

/******************************************************************************
@brief     Assigns 'Rx_Buffer' with string 'str', making 'Rx_index' point to
              the end of 'Rx_Buffer'
@param     String to be inserted in 'Rx_Buffer'
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
}