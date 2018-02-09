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

//#include <msp430.h>
#include <stdint.h>

#include "hd44780.hpp"

// Internal functions
static uint8_t _read_4bit();
static uint8_t _read_8bit();
static void _write_4bit(uint8_t);
static void _write_8bit(uint8_t);

void hd44780_init(uint8_t line_mode){
    HD44780_PORTDIR = HD44780_RS | HD44780_RW | HD44780_EN | HD44780_DATA;
    HD44780_PORT &= ~(HD44780_RS | HD44780_RW | HD44780_EN | HD44780_DATA);

    _write_4bit(0x02); // Enable 4bit mode
    
    // We can only set the number of lines upon initialization (see
    // datasheet, page 27). The status bit cannot be checked otherwise the
    // controller will ignore this command.
    _write_8bit(line_mode);
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);
}

void hd44780_send_command(uint8_t c){
    while (hd44780_get_busy_flag());
    HD44780_PORT &= ~HD44780_RS;
    _write_8bit(c);
}

void hd44780_write_char(char c){
    while (hd44780_get_busy_flag());
    HD44780_PORT |= HD44780_RS;
    _write_8bit(c);
}

void hd44780_write_string(char *str){
    while (*str != '\0')
        hd44780_write_char(*str++);
}

uint8_t hd44780_get_busy_flag(){
    HD44780_PORT &= ~(HD44780_RS | HD44780_DATA);
    return _read_8bit() & 0x80;
}

// Read an 8bit value using two calls to _read_4bit()
static uint8_t _read_8bit(){
    uint8_t data;
    HD44780_PORTDIR &= ~HD44780_DATA;
    HD44780_PORT |= HD44780_RW;
    data = _read_4bit() << 4;
    data |= _read_4bit();
    return data;
}

// Read a 4bit data value according to the timing characteristics and
// timing diagrams listed on page 49 and page 58 of the datasheet.
// We assume the MCLK frequency is 16MHz that way this code will also
// work on slower chips without any problems.
static uint8_t _read_4bit(){
    // no need to wait 60ns for t_AS, the function call provides the delay
    HD44780_PORT |= HD44780_EN;

    // Need to wait 360ns for data lines to stabilize the data lines (t_DDR)
    nop(); nop(); nop(); nop(); nop(); nop();

    uint8_t data = HD44780_PORTIN >> HD44780_DATA_OFFSET; // Data is valid, read 4bits
    HD44780_PORT &= ~HD44780_EN;

    // Satisfy T_cycE requirement (this is probably overkill)
    nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); 

    return data;
}

// Writes an 8bit value on the bus using _write_4bit() 
static void _write_8bit(uint8_t data){
    HD44780_PORTDIR |= HD44780_DATA;
    HD44780_PORT &= ~HD44780_RW;
    _write_4bit(data>>4);
    _write_4bit(data & 0x0f);
}

// Does a 4bit write (the 4 MSB are ignored)
// Assuming we're running at 16MHz (~62ns per clock)
// Timing characteristics and timing diagrams are available on page 49 and
// page 58 of the datasheet
static void _write_4bit(uint8_t data){
    // no need to wait 60ns for t_AS, the function call provides the delay
    HD44780_PORT |= HD44780_EN;
    HD44780_PORT |= data << HD44780_DATA_OFFSET;
    
    // Satisfy the PW_EH (hold EN for 450ns)
    // and t_DSW requirement (data set-up 195ns)
    nop(); nop(); nop(); nop(); nop(); nop();

    HD44780_PORT &= ~HD44780_EN;
    
    // The data hold t_H is 10ns, no extra delay is needed
    HD44780_PORT &= ~HD44780_DATA;

    // Delay required to satisfy t_cycE parameter (min 1000ns for 1 EN cycle)
    nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop();
}

