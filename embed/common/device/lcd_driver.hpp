
#ifndef LCD_DRIVER_H 
#define LCD_DRIVER_H

/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/
#include "cmn_types.h"

/*==========================================================================*
 * Global constant and macro definitions ===================================*
 *==========================================================================*/
#define DB4BIT 4u
#define DB8BIT 8u

/* Command definitions, see page 24 of the datasheet for more info */
#define HD44780_CMD_CLEAR_DISPLAY               0x01
#define HD44780_CMD_RETURN_HOME                 0x02
#define HD44780_CMD_DISPLAY_SHIFT_ON            0x07
#define HD44780_CMD_DISPLAY_SHIFT_OFF           0x06
#define HD44780_CMD_DISPLAY_ON_CURSOR_BLINK     0x0F
#define HD44780_CMD_DISPLAY_ON_BLINK            0x0D
#define HD44780_CMD_DISPLAY_ON_CURSOR           0x0E
#define HD44780_CMD_DISPLAY_ON                  0x0C
#define HD44780_CMD_DISPLAY_OFF                 0x08
#define HD44780_CMD_DISPLAY_SHIFT_RIGHT         0x1C
#define HD44780_CMD_DISPLAY_SHIFT_LEFT          0x18
#define HD44780_CMD_MOVE_CURSOR_RIGHT           0x14
#define HD44780_CMD_MOVE_CURSOR_LEFT            0x10
#define HD44780_CMD_1_LINE_MODE                 0x20 /* N  = 0 */
#define HD44780_CMD_2_LINE_MODE                 0x28 /* N  = 1 */
#define HD44780_CMD_8BIT_DATA_LENGTH            0x30 /* DL = 1 */
#define HD44780_CMD_4BIT_DATA_LENGTH            0x20 /* DL = 0 */
#define HD44780_CMD_5X8_CHAR_DOTS               0x20 /* F  = 0 */
#define HD44780_CMD_5X10_CHAR_DOTS              0x24 /* F  = 1 */

typedef enum
{
  DATA_IN  = 0,
  DATA_OUT = 1
} DB_Dir;

/*******************************************************************************************************************************
  Class Name             : LCD_Driver
  Requirements satisfied : Derived
  Description            :
  Member Functions       : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
class LCD_Driver
{
private:
/*==========================================================================*
 * Private Data Members ====================================================*
 *==========================================================================*/
  uint32_t lines;
  uint32_t busWidth; /* Data bus width */
  uint32_t dataBus[8];

/*==========================================================================*
 * Private Member Functions ================================================*
 *==========================================================================*/
  void     WriteData   (uint8_t byte);
  uint8_t  ReadData    (void);
  void     SetDbDir    (DB_Dir dir);
  void     SetData     (const uint8_t data);
  void     GetData     (uint8_t *pData);
  uint32_t LCD_Busy    (void);
  void     Write_IR    (uint8_t byte);
  void     Write_DR    (uint8_t data);
  void     GotoXY      (uint8_t x, uint8_t y);

public:
/*==========================================================================*
 * Public Data Members =====================================================*
 *==========================================================================*/

/*==========================================================================*
 * Public Member Functions =================================================*
 *==========================================================================*/
  /* Public constructor */
  explicit LCD_Driver(uint32_t lines, uint32_t bitMode);
  /* Destructor */
  ~LCD_Driver();

  void DispStrXY      (uint8_t x, uint8_t y, char *pStr);
  void WriteString    (char *pStr);
  void DispCharXY     (uint8_t x, uint8_t y, char ch);
  void DispCentre     (uint8_t line,  char *pStr);
  void Return         (void);
  void Clear_Display  (void);
  void DispClearLine  (uint8_t line);
};



#endif
