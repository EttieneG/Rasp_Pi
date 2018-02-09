
#ifndef __HD44780_CONFIG_H__
#define __HD44780_CONFIG_H__

#define HD44780_PORT        P1OUT
#define HD44780_PORTDIR     P1DIR
#define HD44780_PORTIN      P1IN

#define HD44780_RS          BIT0
#define HD44780_RW          BIT1
#define HD44780_EN          BIT2
#define HD44780_BUSY        BIT7
#define HD44780_DATA        (BIT7 | BIT6 | BIT5 | BIT4)
#define HD44780_DATA_OFFSET 4

#endif // __HD44780_CONFIG_H__
