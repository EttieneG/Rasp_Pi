/**********************************************************************************************

    Security Classification : Confidential
    Title                   : Header file with Common and Standard Definitions for
                              global use by every *.c file of the application
    File path and name      : $HeadURL: https://ee-svr/svn/adarter/common/trunk/include/cmn_defines.h $
    File Number/Ref         : 06019-62141-411
    Latest Author           : $Author: bror $
    Latest Update           : $Date: 2015-08-14 10:07:40 +0200 (Fri, 14 Aug 2015) $
    Repository Revision     : $Revision: 27593 $
    Latest Issue            :
    Description             : Common definitions for use in *.c files
                              This file was ported from Mk I CVS repository, version 1.1
    Associated Docs         : 06019-62XXX/25000-422 (SRS)
                              06019-62XXX/25000-439 (STD)
                              06019-62XXX/25000-438 (STR)
    Associated Files        : Refer CSCI's make file
    Compiler and Version    : GCC 4.5.2 (PowerPC cross-compiler from CodeSourcery)
    Compiler Options        : Refer CSCI's make file
    Restrictions            : None
    Hardware Info           : PowerPC 7410
    Copyright               : Denel Dynamics, a division of Denel (Pty) Ltd
                              The copyright to the computer program(s) herein is the property
                              of Denel Aerospace. The program(s) may be used/or copied only with the
                              written permission of Denel Aerospace or in accordance with the terms
                              and conditions stipulated in the agreement/contract under which
                              the program has been supplied.

**********************************************************************************************/
#ifndef CMN_DEFINES_H
#define CMN_DEFINES_H

/* Include files listing */
#include "cmn_types.h"

#ifdef WINDOWSCODE
#include "stdafx.h" /*Win specific, remove this line for Unix*/
#endif              /* ifdef WINDOWSCODE */

#ifdef SIMISCODE
#include "system_params.c"
/*u4c: cmn: A project shall not contain unused type declarations
  ->This type definition is included in a common definition file that can be accessed by various subsystems as required.
*/

typedef enum _bool
{
  false = 0, true = 1
} bool;
#endif /* ifdef SIMISCODE */


/****************************************************************************
 * Global constant and macro definitions                                    *
 ****************************************************************************/

#ifndef NULL
#define NULL ((void *)0)
#endif /* ifndef NULL */

#ifdef PIC_MICRO

#ifndef TRUE
#define TRUE (uint8_t)1
#endif /* ifndef TRUE */

#ifndef FALSE
#define FALSE (uint8_t)0
#endif /* ifndef FALSE */

#ifndef PASS
#define PASS (uint8_t)0
#endif /* ifndef PASS */

#ifndef FAIL
#define FAIL (uint8_t)1
#endif /* ifndef FAIL */

#ifndef HIGH
#define HIGH (uint8_t)1
#endif /* ifndef HIGH */

#ifndef LOW
#define LOW (uint8_t)0
#endif /* ifndef LOW */

#ifndef ON
#define ON (uint8_t)1
#endif /* ifndef ON */

#ifndef OFF
#define OFF (uint8_t)0
#endif /* ifndef OFF */

#ifndef SET
#define SET (uint8_t)1
#endif /* ifndef SET */

#ifndef RESET
#define RESET (uint8_t)0
#endif /* ifndef RESET */

#ifndef ENABLE
#define ENABLE (uint8_t)1
#endif /* ifndef ENABLE */

#ifndef DISABLE
#define DISABLE (uint8_t)0
#endif /* ifndef DISABLE */

#ifndef SUCCESS
#define SUCCESS (uint8_t)0
#endif /* ifndef SUCCESS */

#ifndef FAILURE
#define FAILURE (uint8_t)1
#endif /* ifndef FAILURE */

#ifndef START
#define START (uint8_t)1
#endif /* ifndef START */

#ifndef STOP
#define STOP (uint8_t)0
#endif /* ifndef STOP */

#ifndef ERROR
#define ERROR (uint8_t)1
#endif /* ifndef ERROR */

#ifndef ERROR_8B
#define ERROR_8B (uint8_t)1
#endif /* ifndef ERROR_8B */

#else /* ifdef PIC_MICRO*/

#ifndef TRUE
#define TRUE (uint32_t)1
#endif /* ifndef TRUE */

#ifndef FALSE
#define FALSE (uint32_t)0
#endif /* ifndef FALSE */

#ifndef PASS
#define PASS (uint32_t)0
#endif /* ifndef PASS */

#ifndef FAIL
#define FAIL (uint32_t)1
#endif /* ifndef FAIL */

#ifndef HIGH
#define HIGH (uint32_t)1
#endif /* ifndef HIGH */

#ifndef LOW
#define LOW (uint32_t)0
#endif /* ifndef LOW */

#ifndef ON
#define ON (uint32_t)1
#endif /* ifndef ON */

#ifndef OFF
#define OFF (uint32_t)0
#endif /* ifndef OFF */

#ifndef SET
#define SET (uint32_t)1
#endif /* ifndef SET */

#ifndef RESET
#define RESET (uint32_t)0
#endif /* ifndef RESET */

#ifndef ENABLE
#define ENABLE (uint32_t)1
#endif /* ifndef ENABLE */

#ifndef DISABLE
#define DISABLE (uint32_t)0
#endif /* ifndef DISABLE */

#ifndef SUCCESS
#define SUCCESS (uint32_t)0
#endif /* ifndef SUCCESS */

#ifndef FAILURE
#define FAILURE (uint32_t)1
#endif /* ifndef FAILURE */

#ifndef START
#define START (uint32_t)1
#endif /* ifndef START */

#ifndef STOP
#define STOP (uint32_t)0
#endif /* ifndef STOP */

#ifndef ERROR
#define ERROR (uint32_t)1
#endif /* ifndef ERROR */

#ifndef BUSY
#define BUSY (uint32_t)1
#endif /* ifndef BUSY */

#ifndef READY
#define READY (uint32_t)0
#endif /* ifndef READY */

#ifndef ERROR_8B
#define ERROR_8B (uint8_t)1
#endif /* ifndef ERROR_8B */

#endif /* PIC_MICRO Not defined*/


/*============================================================================*
 * Variable bit(s) set definitions                                            *
 *============================================================================*/

/* For next three definitions, LSB number is  0 */
/* SETBITNO(VarName, BitNo) */
#define SETBITNO(x, y)    ((x) | ((uint32_t)0x1 << (y)))  /* Sets a bit in a var */
/* RESETBITNO(VarName, BitNo)*/
#define RESETBITNO(x, y)  ((x) & ~((uint32_t)0x1 << (y))) /* Resets a bit in a var */
/* CHKBITNOSET(VarName, BitNo)*/
#define CHKBITNOSET(x, y) (((x) >> (y)) & (uint32_t)0x1)  /* Checks if a bit is set in a var */

/* For next three definitions, a template value must be provided */
/* SETBITS(VarName, TemplateVal) */
#define SETBITS(x, y)   ((x) | (y))        /* Sets bits in a var */
/* RESETBITS(VarName, TemplateVal) */
#define RESETBITS(x, y) ((x) & ~(y))       /* Resets bits in a var */
/* CHKBITSSET(VarName, TemplateVal) */
#define CHKBITSSET(x, y)    ((x) & (y))    /* Checks if bits are set in a var */


/****************************************************************************
 * Global object declarations and definitions                               *
 ****************************************************************************/
#ifndef PIC_MICRO

/*******************************************************************************************************************************
  Structure Name         : CMN_BigEndianWord
  Requirements satisfied : Derived
  Description            : Sharc Processor Big-Endian 32-bit Word definition
  Member Functions       : None
  Author                 : Roelof Broodryk
  Notes                  : None
*******************************************************************************************************************************/
/*u4c: cmn: A project shall not contain unused type declarations
  ->This type definition is included in a common definition file that can be accessed by various subsystems as required.
*/
typedef struct _CMN_BigEndianWord
{
  uint32_t uiMSB : 16;
  uint32_t uiLSB : 16;
} CMN_BigEndianWord; /* sharc Big-Endian 32-bit Word definition */
#endif               /* ifndef PIC_MICRO */

/****************************************************************************
 * Global member function declarations                                      *
 ****************************************************************************/
/* None */


#endif /* ifndef CMN_DEFINES_H */
