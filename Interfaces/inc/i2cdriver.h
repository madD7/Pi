/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



#ifndef		__I2CDRIVER_H_
#define		__I2CDRIVER_H_



/* Include Files **************************************************************************
* @{
*/
#include "types.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
/*
* @}
*/


/* Data Types & Variables *****************************************************************
* @{
*/
#define 	I2C					1	
#define 	I2C_NAME_STRING		"i2c"
#define		I2C_FILEPATH		"/dev/i2c-1"

/***************************************************************************************
Data Structure Name	: I2C data type
Description	: 
Notes		: None
Published	: No*/
typedef struct 
{
	short schno;
	short sdevaddr;
	short rwflg;
	short saddr;
	short smemcount;
}I2CCfg_Type;

/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/
Status_Type I2CDRIVER_Open(I2CCfg_Type* pI2cCfg );
Status_Type I2CDRIVER_Close();
Status_Type I2CDRIVER_Write(char* pcdata, int isize);
Status_Type I2CDRIVER_WriteString(char* pcdata);
Status_Type I2CDRIVER_Read(char* pcdata, int isize, int* pirdbytes);

/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

