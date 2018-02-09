/*******************************************************************************************************************************

    Security Classification : Confidential
    Title                   :
    File path and name      : $HeadURL: https://ee-svr/svn/adarter/embed/pcp/operational/trunk/sw/device/pcp_hsddevice.c $
    File Number/Ref         : xxxxx-xxxxx-411
    Latest Author           : $Author: greb $
    Latest Update           : $Date: 2015-09-04 15:44:09 +0200 (Fri, 04 Sep 2015) $
    Repository Revision     : $Revision: 27717 $
    Latest Issue            : VDD
    Description             :
    Associated Docs         : xxxxx-xxxxx-422 (SRS)
                              xxxxx-xxxxx-424 (SDD)
                              xxxxx-xxxxx-373 (STR)
    Associated Files        : See the include file listing
    Compiler and Version    :
    Compiler Options        :
    Restrictions            : None
    Hardware Info           : TMS320F28335
    Copyright               : Denel Dynamics, a division of Denel (Pty) Ltd
                              The copyright to the computer program(s) herein is the property of Denel Aerospace. The program(s)
                              may be used/or copied only with the written permission of Denel Aerospace or in accordance with 
                              the terms and conditions stipulated in the agreement/contract under which the program has been
                              supplied.

*******************************************************************************************************************************/
/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/
//#include <string>
#include <boost/python.hpp>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <iomanip>
#include <string>
#include <cstdio> /* for sprintf() */
#include "cmn_types.h"
#include "cmn_defines.h"
#include "dht.hpp"
#include "rpi_defines.h"
#include "lcd_driver.hpp"

/*==========================================================================*
 * Local constant and macro definitions ====================================*
 *==========================================================================*/

/*==========================================================================*
 * Local object declarations and definitions ===============================*
 *==========================================================================*/
DhtSensor  *pTempSens;
LCD_Driver *pLcdDisp;

/*==========================================================================*
 * Local member function declarations ======================================*
 *==========================================================================*/

/*==========================================================================*
 * Global member function definitions ======================================*
 *==========================================================================*/
/*******************************************************************************************************************************
  Function Name          : ServiceTemp
  Requirements satisfied : Derived
  Description            : Temperature callback function
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
uint32_t SensorService(void)
{
  float32_t  humid  = 0.0f;
  float32_t  temp   = 0.0f;
  char str[16];

  pTempSens->InterpData(&humid, &temp);

  sprintf(str, "Temp = %.2f", temp/10.0f);
  pLcdDisp->DispStrXY(0, 1, str);
  sprintf(str, "Humd = %.2f", humid/10.0f);
  pLcdDisp->DispStrXY(0, 2, str);

  std::cout << "Success!!!" << std::endl;
  std::cout << "Temperature = "  << temp  / 10.0f << "°C" << std::endl;
  std::cout << "Humidity    = "  << humid / 10.0f << "%"  << std::endl;

  return 0u;
}


/*******************************************************************************************************************************
  Function Name          : main
  Requirements satisfied : Derived
  Description            :
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
uint32_t ReadDhtSensor(void)
{
  uint32_t status = FAILURE;
  std::string answer;

  /* Initialise the system into GPIO Pin mode and use the memory mapped hardware directly */
  wiringPiSetupGpio();           /* Must be called once at the start of program execution */
  pTempSens = new DhtSensor(DHT22, DHT_GPIO);
  pLcdDisp  = new LCD_Driver(2, DB4BIT);     /* Initialise the LCD with 2 lines and 4-bit mode */


  //status = tempSens.DHT_Read(&humid, &temp);
  if (pTempSens->DHT_GetStatus() == DHT_READY)
  { /* DHT not busy */
    status = pTempSens->DHT_Read(SensorService); /* Start new transaction */
/*  if (status == SUCCESS)
    {//char str[16];
      std::cout << "Threaded transaction created successfully!!!" << std::endl;
    }
    else
    {
      std::cout << "Failed to create thread for temperature sensor" << std::endl;
    }*/
  }
  else
  {
    //std::cout << "DHT Sensor not ready for new transaction" << std::endl;
  }
  status = pTempSens->DHT_GetStatus();


  delete pTempSens;
  delete pLcdDisp;

  return status;
}

/*******************************************************************************************************************************
  Function Name          :
  Requirements satisfied : Derived
  Description            :
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
BOOST_PYTHON_MODULE(pi_weather)
{
  using namespace boost::python;
  def("ReadDhtSensor", ReadDhtSensor);
}
/* end function HSDD_Init ****************************************************************************************************/


/*==========================================================================*
 * Local member function definitions =======================================*
 *==========================================================================*/

/******************************************************************************************************************************/
