
/*  A hd44780 library for use with the TI MSP430
    Copyright (C) 2011 nikosapi

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __HD44780_H__
#define __HD44780_H__

#include <stdint.h>
#include "hd44780_config.h"

/* A library for controlling a Hitachi HD44780 display driver with a TI MSP430
   
    The HD44780U datasheet is mentioned throughout the code, you can download
    a copy here: http://lcd-linux.sourceforge.net/pdfdocs/hd44780.pdf

    Requirements:
    1) You must use 4-bit mode, which means the least significant data lines
        don't get connected. This is a bit slower, but makes better use of
        the limited pins on the smaller MSP430 devices.
    2) All 7 lines coming from the display controller (DB7-DB4, RS, RW,
        and EN) must be connected to the same port on the microcontroller.
    3) The four data lines must be connected in order, in a contiguous set
        of pins. For example: DB7=P1.6, DB6=P1.5, DB5=P1.4, DB4=P1.3
    4) The following must be #define'd in order to use this library:
        HD44780_PORT: The OUT register of the port that the display
                      is connected to (eg. P1OUT)
        HD44780_PORTDIR: The port's DIR register (eg. P1DIR)
        HD44780_PORTIN: The port's IN register (eg. P1IN)
        HD44780_RS: The pin that's connected to RS (eg. BIT0)
        HD44780_RW: The pin that's connected to RW (eg. BIT1)
        HD44780_EN: The pin that's connected to EN (eg. BIT2)
        HD44780_BUSY: The pin that's connected to DB7 (eg. for P1.7, use BIT7)
        HD44780_DATA: A mask containing all the data lines, for example:
                      If DB7 is connected to P1.7, use (BIT7|BIT6|BIT5|BIT4)
        HD44780_DATA_OFFSET: The data lines' offset relative to DB0 (eg. for
                             the example shown in HD44780_DATA, this must be
                             set to 4)
*/

// Command definitions, see page 24 of the datasheet for more info
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
#define HD44780_CMD_1_LINE_MODE                 0x20
#define HD44780_CMD_2_LINE_MODE                 0x28

/* HD44780 initialization routine, must be run before any other hd44780_*
   functions. This function clears the display so there's no need to do
   so after calling this function.

   The argument sets the number of lines on your display. It can be one of
   two values, either HD44780_CMD_1_LINE_MODE or HD44780_CMD_2_LINE_MODE.
   Once initialized, the HD44780_CMD_*_LINE_MODE commands should _not_ be
   issued again (as specified in the datasheet). */
void hd44780_init(uint8_t);

/* Send the HD44780 a command. Commands are #define'd above (HD44780_CMD_*).
   Note: There is no need to check the busy flag before sending commands,
         this is done for you in the function. */
void hd44780_send_command(uint8_t);

/* Write a single character to the display. 
   Note: This function waits for the busy flag for you. */
void hd44780_write_char(char);

/* Write a null-terminated string of characters to the display. This
   function simply loops over your string writing one character at a
   time using hd44780_write_char(). */
void hd44780_write_string(char*);

/* Returns the busy flag as reported by the HD44780.
   Returns either 0x80 (busy) or 0x00 (not busy). */
uint8_t hd44780_get_busy_flag();

#ifndef HD44780_PORT
    #error "HD44780_PORT not defined!"
#endif

#ifndef HD44780_PORTDIR
    #error "HD44780_PORTDIR not defined!"
#endif

#ifndef HD44780_PORTIN
    #error "HD44780_PORTIN not defined!"
#endif

#ifndef HD44780_RS
    #error "HD44780_RS not defined!"
#endif

#ifndef HD44780_RW
    #error "HD44780_RW not defined!"
#endif

#ifndef HD44780_EN
    #error "HD44780_EN not defined!"
#endif

#ifndef HD44780_BUSY
    #error "HD44780_BUSY not defined!"
#endif

#ifndef HD44780_DATA
    #error "HD44780_DATA not defined!"
#endif

#ifndef HD44780_DATA_OFFSET
    #error "HD44780_DATA_OFFSET not defined!"
#endif

#endif // __HD44780_H__
