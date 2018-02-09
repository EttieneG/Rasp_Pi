/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/
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

/*
#define SUCCESS (uint32_t)0
#define FAILURE (uint32_t)1
*/
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
uint32_t DisplayTemp(void)
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
int main()
{
  uint32_t status = FAILURE;
  std::string answer;

  /* Initialise the system into GPIO Pin mode and use the memory mapped hardware directly */
  wiringPiSetupGpio();           /* Must be called once at the start of program execution */
//DhtSensor  tempSens(DHT22, DHT_GPIO);
//LCD_Driver lcd(2, DB4BIT);     /* Initialise the LCD with 2 lines and 4-bit mode */
  pTempSens = new DhtSensor(DHT22, DHT_GPIO, MODE_NON_BLKG);
  pLcdDisp  = new LCD_Driver(2, DB4BIT);     /* Initialise the LCD with 2 lines and 4-bit mode */

  do
  {
    std::cout << "Starting the one-wire sensor program" << std::endl;

  //status = tempSens.DHT_Read(&humid, &temp);
    if (pTempSens->DHT_GetStatus() == DHT_READY)
    { /* DHT not busy */
      status = pTempSens->DHT_Read(DisplayTemp); /* Start new transaction */
      if (status == SUCCESS)
      {//char str[16];

        std::cout << "Threaded transaction created successfully!!!" << std::endl;
      }
      else
      {
        std::cout << "Failed to create thread for temperature sensor" << std::endl;
      }
    }
    else
    {
      std::cout << "DHT Sensor not ready for new transaction" << std::endl;
    }


    status = pTempSens->DHT_GetStatus();
    if (status & DHT_CHCKSUM)
    {
      std::cout << "The checksum failed - Try again" << std::endl;
    }
    if (status & DHT_TIMEOUT)
    {
      std::cout << "The data timed-out - Try again" << std::endl;
    }

    std::cout << " " << std::endl;
    std::cout << "Again? (Y/N) " << std::endl;
    std::cin  >> answer;
  } while (answer != "N");

  delete pTempSens;
  delete pLcdDisp;
}

/*==========================================================================*
 * Local member function definitions =======================================*
 *==========================================================================*/

/******************************************************************************************************************************/




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

  /* end function DHT_Init ****************************************************************************************************/
