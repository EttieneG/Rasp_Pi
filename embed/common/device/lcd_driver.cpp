/*********************************************************************************
 Title          : LCD Driver

 File Name      : LCD_Driver.c

 AUTHOR         : E.J. Greeff

 Creation Date  : 18/07/2003 

 Latest Update  : 12/08/2003

 Latest Version : 1.0

 Hardware Info  : Driver for Hitachi HD44780 LCD Controller on 8051 Devolopment
                  platform (Siemens C509L)

*********************************************************************************/

/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include "lcd_driver.hpp"
#include "cmn_defines.h"
#include "rpi_defines.h"

/*==========================================================================*
 * Local constant and macro definitions ====================================*
 *==========================================================================*/
#define LCD_LEN 16      // 16 characters in one line

/*==========================================================================*
 * Local object declarations and definitions ===============================*
 *==========================================================================*/

/*==========================================================================*
 * Global member function definitions ======================================*
 *==========================================================================*/

/*******************************************************************************************************************************
  Function Name          : LCD_Driver
  Requirements satisfied : Derived
  Description            : Constructor
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : We can only set the number of lines upon initialization (see datasheet, page 27). The status bit
                           cannot be checked otherwise the controller will ignore this command.
*******************************************************************************************************************************/
LCD_Driver::LCD_Driver(uint32_t lines, uint32_t bitMode) :
                       lines(lines), busWidth(bitMode) /* Initialiser list */
{
  dataBus[0] = LCD_D4_GPIO;
  dataBus[1] = LCD_D5_GPIO;
  dataBus[2] = LCD_D6_GPIO;
  dataBus[3] = LCD_D7_GPIO;

  SetDbDir(DATA_IN);                        /* Set the data bus direction */

  /* Set control signal direction */
  pinMode  (LCD_EN_GPIO, OUTPUT);
  pinMode  (LCD_RS_GPIO, OUTPUT);
  pinMode  (LCD_RW_GPIO, OUTPUT);

  usleep(50000);                            /* wait for more than 40ms after power-up       */
  digitalWrite (LCD_RS_GPIO, LOW);          /* Clear the RS bit */
  if (busWidth == DB4BIT) WriteData(HD44780_CMD_4BIT_DATA_LENGTH);/* Send the command without checking for BUSY flag */
  else                    WriteData(HD44780_CMD_8BIT_DATA_LENGTH);/* Send the command without checking for BUSY flag */
  delayMicroseconds(100u);
  if (lines == 1u)        WriteData(HD44780_CMD_1_LINE_MODE | HD44780_CMD_5X8_CHAR_DOTS);
  else                    WriteData(HD44780_CMD_2_LINE_MODE | HD44780_CMD_5X8_CHAR_DOTS);
  WriteData(HD44780_CMD_DISPLAY_OFF);
  delayMicroseconds(100u);
  WriteData(HD44780_CMD_CLEAR_DISPLAY);
  usleep(5000);                             /* wait for more than 4ms                       */
  Write_IR(HD44780_CMD_DISPLAY_ON);         /* Turn display on with no cursor               */
//Write_IR(HD44780_CMD_DISPLAY_ON_CURSOR);  /* Turn display on with cursor                  */
  Write_IR(HD44780_CMD_DISPLAY_SHIFT_OFF);  /* Entry mode - Increment address by one, shifts cursor to right   */
  Clear_Display();                          /* Clear the display                            */
}/* end LCD_Driver Constructor ************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DhtSensor
  Requirements satisfied : Derived
  Description            : Destructor
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
LCD_Driver::~LCD_Driver()
{
  Clear_Display();                          /* Clear the display                            */
  Write_IR(HD44780_CMD_DISPLAY_OFF);        /* Display off */
}/* end LCD_Driver Destructor *************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : Clear_Display
  Requirements satisfied : Derived
  Description            : Function to clear the LCD display
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::Clear_Display(void)
{
  Write_IR(HD44780_CMD_CLEAR_DISPLAY);
}/* end function Clear_Display ************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : Return
  Requirements satisfied : Derived
  Description            : Function to return cursor
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::Return(void)
{
  Write_IR(HD44780_CMD_RETURN_HOME);
}/* end function Return *******************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : GotoXY
  Requirements satisfied : Derived
  Description            : Sets the address of the next character to be output by the LCD display.
  Inputs                 : Both x and y coordinates passed
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::GotoXY (uint8_t x, uint8_t y)
{
  uint8_t startPos[5] = {0, 0, 64, 20, 84};

  x = x + startPos[y];   /* Decode start position */
  Write_IR(0x80 | x);    /* Write start position to Instruction register */
}/* end function GotoXY *******************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DispCentre
  Requirements satisfied : Derived
  Description            : Displays a string on the display with automatic centering
  Inputs                 : line number, Pointer to string
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::DispCentre (uint8_t line,  char *pStr)
{
  int32_t StartPos;
  StartPos = (LCD_LEN / 2u) - ((char)strlen(pStr) / 2u) ;
  if (StartPos < 0) StartPos = 0;
  DispStrXY((uint8_t)StartPos, line, pStr);
}/* end function DispCentre ***************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DispClearLine
  Requirements satisfied : Derived
  Description            : Clears the line specified
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::DispClearLine (uint8_t line)
{
  GotoXY(1, line);
  for (uint8_t i = 0u; i < LCD_LEN; i++)
  {
    Write_DR(' '); /* Write white space */
  }
}/* end function DispClearLine ************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : WriteString
  Requirements satisfied : Derived
  Description            : Write string to display
  Inputs                 : Pointer to the string
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::WriteString(char *pStr)
{
  while (*pStr != '\0')
  {
    Write_DR(*pStr++);
  }
}/* end function WriteString **************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DispStrXY
  Requirements satisfied : Derived
  Description            : Displays a string on the LCD display.
                           First parameter are the location where the string is to be outputted.
                           If x parameter is = 100 then do not position the display.
                           Max string = LCD_LEN chars
  Inputs                 : x and y coordinates, Pointer to string
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::DispStrXY (uint8_t x, uint8_t y, char *pStr)
{
  uint8_t i = 0u;

  if (x != 100)
  {
    GotoXY(x, y);
  }
  while ((pStr[i]) && (i < LCD_LEN))
//while ((*(pOutStr + i)) && (i < LCD_LEN)) /* Same functionality */
  {
    Write_DR(pStr[i]);                      /* Send the character to the LCD */
//  Write_DR(*(pStr + i));                  /* Same functionality */
    i++;                                    /* Increment the pointer index */
  }
}/* end function DispStrXY ****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DispCharXY
  Requirements satisfied : Derived
  Description            : Displays a character on the LCD display.
                           First parameter are the location where the string is to be outputted.
                           If x parameter is = 100 then do not position the display.
  Inputs                 : x and y coordinates, Character to display
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::DispCharXY (uint8_t x, uint8_t y, char ch)
{
  if (x != 100)
  {
    GotoXY(x, y);
  }
  Write_DR(ch);
}/* end function DispCharXY ***************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : Write_DR
  Requirements satisfied : Derived
  Description            : Function to write to the Data Register
  Inputs                 : Data for DR - character to display on LCD
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::Write_DR(uint8_t data)
{
  while(LCD_Busy());                /* wait for busy flag to clear before writing to DR */
  digitalWrite (LCD_RS_GPIO, HIGH); /* Set the RS bit */
  WriteData(data);                  /* Send the data */
}/* end function Write_DR *****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : Write_IR
  Requirements satisfied : Derived
  Description            : Function to write to the Instruction Register
  Inputs                 : Command/data for IR
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::Write_IR(uint8_t cmd)  // Write to Instruction Register
{
  while (LCD_Busy());              /* Wait for busy flag to clear before writing to IR */
  digitalWrite (LCD_RS_GPIO, LOW); /* Clear the RS bit */
  WriteData(cmd);                  /* Send the command */
}/* end function Write_IR *****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : LCD_Busy
  Requirements satisfied : Derived
  Description            : Function to determine the status of the busy flag
  Inputs                 : None
  Outputs                : Busy Flag status
  Timing                 : None
  Return value           : Busy Flag status
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
uint32_t LCD_Driver::LCD_Busy(void)
{
  uint8_t data;

  digitalWrite (LCD_RS_GPIO, LOW); /* Clear the RS bit */
  data = ReadData();
  return data & 0x80;
}/* end function LCD_Busy *****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : WriteData
  Requirements satisfied : Derived
  Description            : Performs the complete transaction to transfer data onto the LCD DataBus
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::WriteData(uint8_t byte)
{
  SetDbDir(DATA_OUT);                 /* Set the data bus direction */
  digitalWrite (LCD_RW_GPIO, LOW);    /* Clear the RW line */

  if (busWidth == DB4BIT)
  { /* Write the MSB 4 bits first */
    digitalWrite (LCD_EN_GPIO, HIGH); /* Set EN */
    SetData(byte >> 4u);              /* Put data on bus */
    delayMicroseconds(1u);            /* PW_EH > 450ns   */
    digitalWrite (LCD_EN_GPIO, LOW);  /* Clear EN - Clock data */
    delayMicroseconds(1u);            /* t_cycE (min 1000ns for 1 EN cycle) */
  }
  digitalWrite (LCD_EN_GPIO, HIGH);   /* Set EN */
  SetData(byte);                      /* Put data on bus */
  delayMicroseconds(1u);              /* PW_EH > 450ns   */
  digitalWrite (LCD_EN_GPIO, LOW);    /* Clear EN - Clock data */
  delayMicroseconds(1u);              /* t_cycE (min 1000ns for 1 EN cycle) */
}/* end function WriteData ****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : ReadData
  Requirements satisfied : Derived
  Description            : Performs the complete transaction to transfer data from the LCD registers
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
uint8_t LCD_Driver::ReadData(void)
{
  uint8_t data;
  uint8_t msbData;

  SetDbDir(DATA_IN);                  /* Set the data bus direction */
  digitalWrite (LCD_RW_GPIO, HIGH);   /* Set the RW line */

  if (busWidth == DB4BIT)
  { /* Read the MSB 4 bits first */
    digitalWrite (LCD_EN_GPIO, HIGH); /* Set EN */
    delayMicroseconds(1u);            /* Wait 360ns for data lines to stabilize the data lines (t_DDR) */
    GetData(&msbData);                /* Read data from bus */
    digitalWrite (LCD_EN_GPIO, LOW);  /* Clear EN */
    delayMicroseconds(1u);            /* t_cycE (min 1000ns for 1 EN cycle) */
  }
  digitalWrite (LCD_EN_GPIO, HIGH);   /* Set EN */
  delayMicroseconds(1u);              /* Wait 360ns for data lines to stabilize the data lines (t_DDR) */
  GetData(&data);                     /* Read data from bus */
  digitalWrite (LCD_EN_GPIO, LOW);    /* Clear EN */
  delayMicroseconds(1u);              /* t_cycE (min 1000ns for 1 EN cycle) */

  if (busWidth == DB4BIT)
  { /* Merge the data into one byte */
    data |= (msbData << 4u);
  }

  return data;
}/* end function ReadData *****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : SetData
  Requirements satisfied : Derived
  Description            : Set the GPIO pins alocated to put the data onto the LCD DataBus
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::SetData (const uint8_t data)
{
  uint8_t mask = 1;

  for (uint32_t pin = 0u; pin < busWidth; pin++)
  {
    digitalWrite (dataBus[pin], (data & mask));
    mask <<= 1;
  }
}/* end function SetData ******************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : GetData
  Requirements satisfied : Derived
  Description            : Read the GPIO pins alocated for the LCD DataBus
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::GetData (uint8_t *pData)
{
  uint8_t data = 0u;

  for (uint32_t pin = 0u; pin < busWidth; pin++)
  {
    data |= ((digitalRead (dataBus[pin])) << pin);
  }
  *pData = data;
}/* end function GetData ******************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : SetDbDir
  Requirements satisfied : Derived
  Description            : Set the direction of the GPIO pins used for data transfer
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void LCD_Driver::SetDbDir (DB_Dir dir)
{
  if (dir == DATA_IN)
  {
    for (uint32_t pin = 0u; pin < busWidth; pin++)
    {
      pinMode(dataBus[pin], INPUT);
    }
  }
  else
  {
    for (uint32_t pin = 0u; pin < busWidth; pin++)
    {
      pinMode(dataBus[pin], OUTPUT);
    }
  }
}/* end function SetDbDir *****************************************************************************************************/

/************************************************ END LCD_Driver **************************************************************/

