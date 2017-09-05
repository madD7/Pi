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
#include <errno.h>

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
	char cai2cpath[100] = {0}; 

	if ( pcfg == NULL)
	{
		LOG_ERROR("Invalid arguments. No i2c configuration details\n");
		return StatusInvalArgs;
	}

	sprintf(cai2cpath, "%s%hd", I2C_FILEPATH, pcfg->schno);
	iFd = open(cai2cpath, O_RDWR);
	if ( iFd < 0)
	{
		LOG_ERROR("Unable to Open I2C device port\n");
		LOG_ERR_DETAILS("Open returned with error %d\n", errno);
		return StatusFail;
	}

	memmove(&I2cCfg, pcfg, sizeof(I2CCfg_Type));
	
	if( ioctl(iFd, I2C_SLAVE, I2cCfg.sdevaddr) < 0 )
	{
		LOG_ERROR("Failed to acquire the bus access or connect to slave\n");
		return StatusDevInitFail;
	}

	LOG_INFO("I2C device port opened successfully\n");

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

	LOG_INFO("I2C device port closed successfully\n");

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
	
	for(iretries=0; (iretries < 10) && (isize > 0); iretries++)
	{
		ibyteswrote = write(iFd, pcdata, isize);

		if(ibyteswrote >= 0)
		{
			isize -= ibyteswrote;
			pcdata += ibyteswrote;
		}
		LOG_PRINT(".");
	}
	LOG_PRINT("\n");

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
	
	for(iretries=0; (iretries < 10) && (isize > 0); iretries++)
	{
		ibytesread = read(iFd, pcdata, isize);
		
		if(ibytesread >= 0)
		{
			isize -= ibytesread;
			pcdata += ibytesread;
		}
		LOG_PRINT(".");
	}
	LOG_PRINT("\n");

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

