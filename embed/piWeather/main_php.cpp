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
  Function Name          : DisplayTemp
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
  /* Display readings on LCD */
  sprintf(str, "Temp = %.2f", temp/10.0f);
  pLcdDisp->DispStrXY(0, 1, str);
  sprintf(str, "Humd = %.2f", humid/10.0f);
  pLcdDisp->DispStrXY(0, 2, str);
  /* Display readings on Web-page */

  std::cout << "<div><p>Temperature = "  << temp  / 10.0f << "°C</p></div>\n";
  std::cout << "<div><p>Humidity    = "  << humid / 10.0f << "% </p></div>\n";

  return 0u;
}/* end function DisplayTemp **************************************************************************************************/

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
  /* Initialise the system into GPIO Pin mode and use the memory mapped hardware directly */
  wiringPiSetupGpio();                      /* Must be called once at the start of program execution */
  pTempSens = new DhtSensor  (DHT22, DHT_GPIO, MODE_BLOCKING);
  pLcdDisp  = new LCD_Driver (2, DB4BIT);   /* Initialise the LCD with 2 lines and 4-bit mode */

  if (pTempSens->DHT_GetStatus() == DHT_READY)
  { /* DHT not busy */
    (void)pTempSens->DHT_Read(DisplayTemp); /* Start new transaction */
  }

  (void)pTempSens->DHT_GetStatus();         /* This should block the call until successful reading was received */

  delete pTempSens;
  delete pLcdDisp;
}/* end function main *********************************************************************************************************/

