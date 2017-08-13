/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
* @{
*/
#include "i2cdriver.h"
#include "log.h"
#include <string.h>

/*
* @}
*/


/* Variables ******************************************************************************
* @{
*/
static int iFd;					// File Descriptor
static I2CCfg_Type I2cCfg = {0};
/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type I2CDRIVER_Open(I2CCfg_Type* pcfg)
{	
	if ( pcfg == NULL)
	{
		LOG_ERROR("Invalid arguments. No i2c configuration details\n");
		return StatusInvalArgs;
	}

	iFd = open(I2C_FILEPATH, O_RDWR);
	if ( iFd < 0)
	{
		LOG_ERROR("Unable to Open I2C device port\n");
		return StatusFail;
	}

	memmove(&I2cCfg, pcfg, sizeof(I2CCfg_Type));
	
	if( ioctl(iFd, I2C_SLAVE, I2cCfg.sdevaddr) < 0 )
	{
		LOG_ERROR("Failed to acquire the bus access or connect to slave\n");
		return StatusDevInitFail;
	}

	return StatusSuccess;
}



/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type I2CDRIVER_Close()
{
	if( close(iFd) < 0)
	{
		LOG_ERROR("Failed to close device\n");
		return StatusFail;
	}

	iFd = 0;
	memset(&I2cCfg, 0, sizeof(I2cCfg));

	return StatusSuccess;
}




/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type I2CDRIVER_Write(char* pcdata, int isize)
{	
	int ibyteswrote=0;
	int iretries=0;

	if( isize == 0)
	{
		LOG_ERROR("Invalid argument. Write data length is zero. \n");
		return StatusInvalArgs;
	}

	if( pcdata == NULL)
	{
		LOG_ERROR("Invalid argument. No data to write.\n");
		return StatusInvalArgs;
	}
	

	for(iretries=0; (iretries > 10) && (isize > 0); iretries++)
	{
		ibyteswrote = write(iFd, pcdata, isize);
		isize -= ibyteswrote;
		pcdata += ibyteswrote;
	}

	if( isize || (iretries > 10))
	{
		LOG_WARN("Exceeded maximum number of retries while writing data.\n");
		LOG_INFO("Number of bytes wrote is %d\n", ibyteswrote);
		return StatusLenMismatch;
	}

	return StatusSuccess;
}



/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type I2CDRIVER_WriteString(char* pcdata)
{
	Status_Type status=0;

	if( pcdata == NULL)
	{
		LOG_ERROR("Invalid argument. No data to write.\n");
		return StatusInvalArgs;
	}
	
	while ( *pcdata != '\0')
	{
		if( (status = I2CDRIVER_Write(pcdata, 1)) != StatusSuccess)
			return status;

		pcdata++;
	}

	return StatusSuccess;
}




/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type I2CDRIVER_Read(char* pcdata, int isize, int* pirdbytes)
{	
	int ibytesread=0;
	int iretries=0;

	*pirdbytes = ibytesread;

	if( isize == 0)
	{
		LOG_ERROR("Invalid argument. Write data length is zero. \n");
		return StatusInvalArgs;
	}

	if( pcdata == NULL)
	{
		LOG_ERROR("Invalid argument. No data to write.\n");
		return StatusInvalArgs;
	}
	

	for(iretries=0; (iretries > 10) && (isize > 0); iretries++)
	{
		ibytesread = write(iFd, pcdata, isize);
		isize -= ibytesread;
		pcdata += ibytesread;
	}

	*pirdbytes = ibytesread;

	if( isize || (iretries > 10))
	{
		LOG_WARN("Exceeded maximum number of retries while reading data.\n");
		LOG_INFO("Number of bytes read is %d\n", ibytesread);
		return StatusLenMismatch;
	}

	return StatusSuccess;
}




/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */

