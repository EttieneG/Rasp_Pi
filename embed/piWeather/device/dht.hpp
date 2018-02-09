/*******************************************************************************************************************************

    Security Classification : Confidential
    Title                   : Header file for
    File path and name      : $HeadURL: https://ee-svr/svn/adarter/embed/pcp/operational/trunk/sw/app/pcp_global.h $
    File Number/Ref         : xxxxx-xxxxx-411
    Latest Author           : $Author: greb $
    Latest Update           : $Date: 2015-09-04 14:03:55 +0200 (Fri, 04 Sep 2015) $
    Repository Revision     : $Revision: 27714 $
    Latest Issue            : VDD
    Description             :
    Associated Docs         :
    Associated Files        : See the include file listing
    Compiler and Version    : arm-linux-gnueabihf-g++ (crosstool-NG linaro-1.13.1+bzr2650 - Linaro GCC 2014.03) 4.8.3 20140303
    Compiler Options        : refer to CSCI make file
    Restrictions            : None
    Hardware Info           : Raspberry PI V3
    Copyright               : None

*******************************************************************************************************************************/
#ifndef PI_DHT_H
#define PI_DHT_H

/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/
#include "cmn_types.h"

/*==========================================================================*
 * Global constant and macro definitions ===================================*
 *==========================================================================*/
#define DHT_READY   0x00u
#define DHT_SUCCESS 0x00u
#define DHT_FAILURE 0x01u
#define DHT_CHCKSUM 0x02u
#define DHT_TIMEOUT 0x04u
#define DHT_BUSY    0x08u

#define MODE_NON_BLKG 0u
#define MODE_BLOCKING 1u

typedef enum
{
  DHT11 = 0, /* The DHT11 uses only 8 bits */
  DHT22 = 1
} DHT_DevType;

typedef uint32_t (*CallbackType)(void);

/*******************************************************************************************************************************
  Class Name             :
  Requirements satisfied : Derived
  Description            :
  Member Functions       : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
class DhtSensor
{
private:
/*==========================================================================*
 * Private Data Members ====================================================*
 *==========================================================================*/
  DHT_DevType dev_type;
  uint32_t    status = DHT_READY;
  uint32_t    io_pin;
  uint32_t    dht_mode;
  uint8_t     rxData[5] = {};

  bool threadRunning;

  CallbackType pCallback; /* Pointer to callback function */
  pthread_t thread;

/*==========================================================================*
 * Private Member Functions ================================================*
 *==========================================================================*/
  void     InitTransaction(void);
  void     WaitForData    (void);
  void     ReadRxData     (void);
  uint32_t VerifyData     (void);

public:
/*==========================================================================*
 * Public Data Members =====================================================*
 *==========================================================================*/

/*==========================================================================*
 * Public Member Functions =================================================*
 *==========================================================================*/
  /* Public constructor */
  explicit DhtSensor(DHT_DevType type, uint32_t pin, uint32_t mode);
  /* Destructor */
  ~DhtSensor();

  void     InterpData    (float32_t *humd, float32_t *temp);
  uint32_t DHT_Read      (float32_t *humidity, float32_t *temperature);
  int32_t  DHT_Read      (CallbackType callback);  /* threaded callback */
  uint32_t DHT_GetStatus (void);

/*==========================================================================*
 * Friend Functions ========================================================*
 *==========================================================================*/
  friend void *DHT_CommsThread(void *arg);
};

/*==========================================================================*
 * Global Functions ========================================================*
 *==========================================================================*/
void *DHT_CommsThread(void *arg); /* callback functions for threads */

#endif  /* PI_DHT_H */

/******************************************************************************************************************************/
