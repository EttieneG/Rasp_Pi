/*******************************************************************************************************************************

    Security Classification : Confidential
    Title                   : Header file containing the Raspberry PI definitions for this project
    File path and name      : $HeadURL: https://ee-svr/svn/adarter/embed/pcp/operational/trunk/sw/include/csci_defines.h $
    File Number/Ref         : 06019-62650-411
    Latest Author           : $Author: greb $
    Latest Update           : $Date: 2016-06-27 15:23:52 +0200 (Mon, 27 Jun 2016) $
    Repository Revision     : $Revision: 28627 $
    Latest Issue            : VDD
    Description             : CSCI specific definitions.
    Associated Docs         : 06019-62650-422 (SRS)
                              06019-62650-424 (SDD)
                              06019-62650-373 (STR)
    Associated Files        : See the include file listing
    Compiler and Version    : GCC 4.2.1 (PowerPC cross-compiler frm CodeSourcery)
    Compiler Options        : refer to CSCI make file
    Restrictions            : None
    Hardware Info           : PowerPC 7410
    Copyright               : Denel Dynamics, a division of Denel (Pty) Ltd
                              The copyright to the computer program(s) herein is the property of Denel Aerospace. The program(s)
                              may be used/or copied only with the written permission of Denel Aerospace or in accordance with 
                              the terms and conditions stipulated in the agreement/contract under which the program has been
                              supplied.

*******************************************************************************************************************************/
#ifndef RPI_DEFINES_H
#define RPI_DEFINES_H
/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/

/*==========================================================================*
 * Global constant and macro definitions ===================================*
 *==========================================================================*/

/* GPIO Definitions and mapping */
//#define GPIO_01 (uint32_t)1
//#define GPIO_02 (uint32_t)2
//#define GPIO_03 (uint32_t)3
//#define GPIO_04 (uint32_t)4
//#define GPIO_05 (uint32_t)5
//#define GPIO_06 (uint32_t)6
//#define GPIO_07 (uint32_t)7
//#define GPIO_08 (uint32_t)8
//#define GPIO_09 (uint32_t)9
//#define GPIO_10 (uint32_t)10
//#define GPIO_11 (uint32_t)11
//#define GPIO_12 (uint32_t)12
  #define GPIO_13 (uint32_t)13
//#define GPIO_14 (uint32_t)14
//#define GPIO_15 (uint32_t)15
  #define GPIO_16 (uint32_t)16
  #define GPIO_17 (uint32_t)17
//#define GPIO_18 (uint32_t)18
  #define GPIO_19 (uint32_t)19
//#define GPIO_20 (uint32_t)20
//#define GPIO_21 (uint32_t)21
  #define GPIO_22 (uint32_t)22
  #define GPIO_23 (uint32_t)23
  #define GPIO_24 (uint32_t)24
  #define GPIO_25 (uint32_t)25
  #define GPIO_26 (uint32_t)26
  #define GPIO_27 (uint32_t)27

#define LED_GPIO GPIO_19  /* GPIO19 for LED  pin 35 */
#define DHT_GPIO GPIO_26  /* GPIO26 for temp sensor */
#define TEST_PIN GPIO_13

#define LCD_RS_GPIO GPIO_17 /* LCD RS  Control Signal */
#define LCD_RW_GPIO GPIO_27 /* LCD R/W Control Signal */
#define LCD_EN_GPIO GPIO_22 /* LCD ENA Control Signal */
#define LCD_D4_GPIO GPIO_23 /* LCD D4 Data line */
#define LCD_D5_GPIO GPIO_24 /* LCD D5 Data line */
#define LCD_D6_GPIO GPIO_25 /* LCD D6 Data line */
#define LCD_D7_GPIO GPIO_16 /* LCD D7 Data line */


/*==========================================================================*
 * Global object declarations and definitions ==============================*
 *==========================================================================*/

 /*==========================================================================*
 * Global member function declarations =====================================*
 *==========================================================================*/

#endif  /* RPI_DEFINES_H */

/******************************************************************************************************************************/
