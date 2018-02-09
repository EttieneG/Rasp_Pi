/*==========================================================================*
 * Include files listing ===================================================*
 *==========================================================================*/
#include <unistd.h>     /* Required for usleep() */
#include <wiringPi.h>
#include <iomanip>
#include <cstdio>       /* For perror */
#include "dht.hpp"
#include "cmn_defines.h"
#include "rpi_defines.h"

/*==========================================================================*
 * Local constant and macro definitions ====================================*
 *==========================================================================*/
  #define LH_THRESHOLD     26      // Low=~14, High=~38 - pick avg.

/*==========================================================================*
 * Local object declarations and definitions ===============================*
 *==========================================================================*/

/*==========================================================================*
 * Local member function declarations ======================================*
 *==========================================================================*/

/*==========================================================================*
 * Global member function definitions ======================================*
 *==========================================================================*/

using namespace std;

/*******************************************************************************************************************************
  Function Name          : DhtSensor
  Requirements satisfied : Derived
  Description            : Constructor
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
DhtSensor::DhtSensor(DHT_DevType type, uint32_t pin, uint32_t mode) :
           dev_type(type), io_pin(pin), dht_mode(mode) /* Initialiser list */
{
  /* The initialiser list is equivalent to the following
  dev_type = type;
  io_pin   = pin;
  */

  piHiPri(99);
  pinMode(LED_GPIO, OUTPUT);       /* GPIO17 is an output pin */
  pinMode(TEST_PIN, OUTPUT);       /* GPIO27 is an output pin */
//digitalWrite (LED_GPIO, HIGH);   /* LED on                  */
  digitalWrite (TEST_PIN, HIGH);   /* TP High                 */

  threadRunning = false;
  pCallback     = NULL;
  thread        = (pthread_t)0;
}/* end DhtSensor Constructor *************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DhtSensor
  Requirements satisfied : Derived
  Description            : Destructor
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
DhtSensor::~DhtSensor()
{
  digitalWrite(LED_GPIO, LOW);     // LED off
}/* end DhtSensor Destructor **************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DHT_Read
  Requirements satisfied : Derived
  Description            : Blocking call to request and receive data from the sensor
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff (Original Author - Derek Molloy)
  Notes                  : None
*******************************************************************************************************************************/
uint32_t DhtSensor::DHT_Read(float32_t *humidity, float32_t *temperature)
{
  InitTransaction();                   /* Init transaction - Request data from the sensor */
  WaitForData();                       /* Wait for data bits */
  ReadRxData();                        /* Read the data */
  if (VerifyData() == SUCCESS)
  {
    InterpData(humidity, temperature); /* Interpret the data */
    status = DHT_SUCCESS;
  }
  else
  {
    status |= DHT_CHCKSUM;
  }

  return status;
}/* end function DHT_Read *****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DHT_Read
  Requirements satisfied : Derived
  Description            : This function creates the thread to communicate with the sensor.
  Inputs                 : Pointer to the callback function that should execute when a reading was successfully obtained
  Outputs                : None
  Timing                 : None
  Return value           : Result for creating thread
  Author                 : Ettiene Greeff (Original Author - Derek Molloy)
  Notes                  : None
*******************************************************************************************************************************/
int32_t DhtSensor::DHT_Read(CallbackType callback)
{
  if (this->threadRunning == false)
  {
    this->threadRunning = true;
    this->pCallback     = callback;

    /* create the thread, pass the reference, address of the function and data */
    if (pthread_create(&this->thread, NULL, &DHT_CommsThread, static_cast<void *>(this)))
    {
      perror("DHT: Failed to create the thread");
      this->threadRunning = false;
      return -1;
    }
  }

  return 0;
}/* end function DHT_Read *****************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : DHT_GetStatus
  Requirements satisfied : Derived
  Description            : Return the status of the current transaction
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
uint32_t DhtSensor::DHT_GetStatus (void)
{
  void *result;
  if (dht_mode == MODE_BLOCKING)
  { /* Wait for thread to finish and join the calling/main thread */
    pthread_join(this->thread, &result);
  }
  return this->status;
}/* end function DHT_GetStatus ************************************************************************************************/

/*==========================================================================*
 * Friend function definitions =============================================*
 *==========================================================================*/

/*******************************************************************************************************************************
  Function Name          : DHT_CommsThread
  Requirements satisfied : Derived
  Description            : Threaded communication function to request and receive data from the sensor. The thread will
                           continue trying until a successful reading was obtained from the sensor. The data is not interpreted.
  Inputs                 : User defined argument as a void pointer. In this function it is the address of the calling object.
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : Friend functions has access to the private class data members
*******************************************************************************************************************************/
void *DHT_CommsThread(void *arg)
{
  DhtSensor *pSens = static_cast<DhtSensor *>(arg);

  while (pSens->threadRunning)
  {
    digitalWrite (LED_GPIO, HIGH);   // LED on
    pSens->InitTransaction();                   /* Init transaction - Request data from the sensor */
    pSens->WaitForData();                       /* Wait for data bits */
    pSens->ReadRxData();                        /* Read the data */
    if (pSens->VerifyData() == SUCCESS)
    {
      pSens->status = DHT_SUCCESS;
      pSens->pCallback();                       /* Execute the Callback function */
      pSens->threadRunning = false;             /* Terminate the thread */
    }
    else
    {
      pSens->status |= DHT_CHCKSUM;
    }
    digitalWrite(LED_GPIO, LOW);     // LED off
    sleep(2);                                   /* Sleep for 2s min before next reading - Requirement from the data-sheet */
  }
  return 0;
}/* end function DHT_CommsThread **********************************************************************************************/

/*==========================================================================*
 * Local member function definitions =======================================*
 *==========================================================================*/

/*******************************************************************************************************************************
  Function Name          : InitTransaction
  Requirements satisfied : Derived
  Description            :
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void DhtSensor::InitTransaction(void)
{
  /* Clear the RX data buffer */
  for (uint32_t i = 0u; i < 5u; i++)
  {
    rxData[i] = 0;
  }
  status = DHT_BUSY;

  pinMode      (io_pin, OUTPUT); /* gpio starts as output */
  digitalWrite (io_pin, LOW);    /* pull the line low     */
  usleep(18000);                 /* wait for 18ms         */
  digitalWrite (io_pin, HIGH);   /* set the line high     */
  pinMode      (io_pin, INPUT);  /* now gpio is an input  */
}/* end function InitTransaction **********************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : WaitForData
  Requirements satisfied : Derived
  Description            :
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void DhtSensor::WaitForData(void)
{
  /* need to ignore the first and second high after going low */
  delayMicroseconds(10);
  do
  {
    delayMicroseconds(1);
  } while (digitalRead(io_pin) == HIGH);
  do
  {
    delayMicroseconds(1);
  } while (digitalRead(io_pin) == LOW);
  do
  {
    delayMicroseconds(1);
  } while (digitalRead(io_pin) == HIGH);
}/* end function WaitForData **************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : ReadRxData
  Requirements satisfied : Derived
  Description            : Sample the data over time. The bits are sent by varying the duration of the high pulses. A high
                           for 26us - 28us signifies a binary 0, and a high for 70us signifies a binary 1.
  Inputs                 : None
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff (Original Author - Derek Molloy)
  Notes                  : The count variable represents ~2us increments and the LH_THRESHOLD value can bet used to adjust
                           the pulse width timing distinction between a 0 and 1.
*******************************************************************************************************************************/
void DhtSensor::ReadRxData(void)
{
  for (int32_t d = 0; d < 5; d++)  // for each data byte
  { // read 8 bits
    for (int32_t i = 0; i < 8; i++)
    { // for each bit of data
      do
      {
        //delayMicroseconds(1);
        digitalWrite (TEST_PIN, LOW);
      } while (digitalRead(io_pin) == LOW);
      int32_t width = 0;                                           // measure width of each high
      do
      {
        width++;
        delayMicroseconds(1);
        digitalWrite (TEST_PIN, HIGH);
        if (width > 1000)
        {
          status |= DHT_TIMEOUT;
          break;                                                   // missed a pulse -- data invalid!
        }
      } while (digitalRead(io_pin) == HIGH);                       // time it!
      rxData[d] = rxData[d] | ((width > LH_THRESHOLD) << (7 - i)); // shift in the data, msb first if width > the threshold
    }
  }
}/* end function ReadRxData ***************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : InterpData
  Requirements satisfied : Derived
  Description            : Interpret the received data in the RX Buffer in Celsius
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : None
*******************************************************************************************************************************/
void DhtSensor::InterpData(float32_t *humd, float32_t *temp)
{
  *temp = 0.0f;
  *humd = 0.0f;

  if (dev_type == DHT11)
  {
    *humd = (float32_t)(rxData[0] * 10);                      /* one byte - no fractional part       */
    *temp = (float32_t)(rxData[2] * 10);                      /* multiplying to keep code concise    */
  }
  else
  {                                                           /* for DHT22 (AM2302/AM2301)           */
    *humd = (float32_t)( rxData[0]         << 8 | rxData[1]); /* shift MSBs 8 bits left and OR LSBs  */
    *temp = (float32_t)((rxData[2] & 0x7F) << 8 | rxData[3]); /* same again for temperature          */
    if (rxData[2] & 0x80)
    { /* The temperature is negative if the MSB is set */
      *temp *= -1.0f;
    }
  }
}/* end function InterpData ***************************************************************************************************/

/*******************************************************************************************************************************
  Function Name          : VerifyData
  Requirements satisfied : Derived
  Description            : Calculate the checksum of the data
  Inputs                 :
  Outputs                : None
  Timing                 : None
  Return value           : None
  Author                 : Ettiene Greeff
  Notes                  : The checksum will overflow automatically
*******************************************************************************************************************************/
uint32_t DhtSensor::VerifyData(void)
{
  uint32_t datSts = SUCCESS;
  uint8_t  chk = 0;

  for (int32_t i = 0; i < 4; i++)
  {
    chk += rxData[i];
  }

  if (chk != rxData[4])
  {
    datSts = FAILURE;
  }

  return datSts;
}/* end function VerifyData ***************************************************************************************************/

/******************************************************************************************************************************/

/******************************************************************************************************************************/
