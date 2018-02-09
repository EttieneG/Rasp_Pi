/**********************************************************************************************

    Security Classification : RESTRICTED

    Title                   : Data definitions for common C data types
    File path and name      : $HeadURL: https://ee-svr/svn/adarter/common/trunk/include/cmn_types.h $
    File Number/Ref         : 06019-62141-411
    Latest Author           : $Author: bror $
    Latest Update           : $Date: 2015-08-14 10:07:40 +0200 (Fri, 14 Aug 2015) $
    Repository Revision     : $Revision: 27593 $
    Latest Issue            :
    Description             : This header file contains data definitions for common C data
                              types. This definition file should be generated for each
                              applicable platform to ensure data size compatibility
                              between different platforms.
                              This file was ported from Mk I CVS repository, version 1.3
    Associated Docs         : 06019-62XXX-422 (SRS)
                              06019-62XXX-439 (STD)
                              06019-62XXX-438 (STR)
    Associated Files        : Refer CSCI make file
    Compiler and Version    : GCC 4.5.2 (PowerPC cross-compiler from CodeSourcery)
    Compiler Options        : Refer CSCI make file
    Restrictions            : Platform definition specifies the operational implementation.
    Hardware Info           : PowerPC 7410
    Copyright               : Denel Dynamics, a division of Denel (Pty) Ltd
                              The copyright to the computer program(s) herein is the property
                              of Denel Aerospace. The program(s) may be used/or copied only with the
                              written permission of Denel Aerospace or in accordance with the terms
                              and conditions stipulated in the agreement/contract under which
                              the program has been supplied.

**********************************************************************************************/
#ifndef CMN_TYPEDEFINES_H
#define CMN_TYPEDEFINES_H

/* Include files listing */
/* None */

/****************************************************************************
 * Global constant and macro definitions                                    *
 ****************************************************************************/
/* None */

/****************************************************************************
 * Global object declarations and definitions                               *
 ****************************************************************************/

/*u4c: cmn: A project shall not contain unused type declarations
  ->Various type declarations are included in a common types definition file that can be accessed by a subsystem as required.
*/

/************************* SIMIS *************************/
#if defined(LINUX) || defined(SIMIS)

#include <stdint.h>

typedef char char_t;
typedef float float32_t;
typedef double float64_t;

#else /* ifdef SIMIS */

#define CMN_STDINT

#endif /* ifdef LINUX */

/************************ POWERPC ************************/
#ifdef POWERPC
typedef char char_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef float float32_t;
typedef double float64_t;
#endif /* ifdef POWERPC */

/******************** PC_APPLICATION *********************/
#ifdef PC_APPLICATION

typedef char char_t;
typedef float float32_t;
typedef double float64_t;

#  if __linux__
#    include <stdint.h>
#  else /* __linux__ */

/* WIN32/64 Assumed for the following section*/
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef __int64 int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

#  endif /* __linux__ */

#endif /* ifdef PC_APPLICATION */

/************************** MFP **************************/
#ifdef MFP
typedef char char_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef float float32_t;
typedef double float64_t;
#endif                             /* ifdef MFP */

/*********************** PIC_MICRO ***********************/
#ifdef PIC_MICRO
typedef char char_t;

typedef char int8_t;
typedef int int16_t;
#ifndef G3
typedef short long int24_t;
#endif /* ifndef G3 */
typedef long int32_t;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
#ifndef G3
typedef unsigned short long uint24_t;
#endif /* ifndef G3 */
typedef unsigned long uint32_t;

typedef float float32_t;
#endif /* ifdef PIC_MICRO */

/************************ TMS320 *************************/
#ifdef TMS320

typedef int int16_t;
typedef long int32_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
typedef float float32_t;
typedef long double float64_t;

typedef char char_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

#endif /* ifdef TMS320 */

/************************** PCU **************************/
#ifdef PCU
typedef char char_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef float float32_t;
typedef double float64_t;

#endif /* ifdef PCU */


/****************************************************************************
 * Global member function declarations                                      *
 ****************************************************************************/
/* None */

#endif /* ifndef CMN_TYPEDEFINES_H */
