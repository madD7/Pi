/**************************************** MAD Engineers ***************************************
MAD Engineers
Copyright (c) 2014

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
* @{
*/
#include "types.h"
#include "log.h"
#include "cliparser.h"
#include "i2cdriver.h"
#include "stdlib.h"
/*
* @}
*/


/* Variables ******************************************************************************
* @{
*/
const char* const cpaComType[] = { "NO DEVICE", 
									I2C_NAME_STRING
									};

int ichno=-1;
int ichntype=0;
int ioperatn=0;
int iaddr=0, istartaddr=0;
int imemcount=0;

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
int CheckParameters()
{
	Status_Type status=StatusSuccess;
	char* pcParam=NULL;
	int icnt=0;
	

	if((status = CLIPARSER_GetParamValue(PARAM_V, &pcParam)) == StatusSuccess)
	{
		LOG_SetVerbosity(atoi(pcParam));
	}

	if((status = CLIPARSER_GetParamValue(PARAM_C, &pcParam)) == StatusSuccess)
	{
		for(icnt=0; icnt <= sizeof(cpaComType); icnt++)
		{
			if(strcasecmp(pcParam, cpaComType[icnt]) == 0)
			{
				ichntype = icnt;
				LOG_INFO("Channel type is %s\n", pcParam);
				break;
			}
		}
	}
	
	if((status = CLIPARSER_GetParamValue(PARAM_N, &pcParam)) == StatusSuccess)
	{
		ichno = atoi(pcParam);
		LOG_INFO("Channel No is %d\n", ichno);
	}

	if((status = CLIPARSER_GetParamValue(PARAM_O, &pcParam)) == StatusSuccess)
	{
		switch (pcParam[0])
		{
			case 'r':
			case 'R':
				ioperatn = 1;
				LOG_INFO("Operation = %c\n", pcParam[0]);
			break;

			case 'w':
			case 'W':
				ioperatn = 2;
				LOG_INFO("Operation = %c\n", pcParam[0]);
			break;

			default:
				LOG_ERROR("Invalid operation selected");
				return StatusInvalArgs;
		}

	}

	if((status = CLIPARSER_GetParamValue(PARAM_D, &pcParam)) == StatusSuccess)
	{
		if(sscanf(pcParam, "%x", &iaddr) == 1)
		{
			LOG_INFO("Device address = 0x%x\n",iaddr);
		}
		else
		{
			LOG_ERROR("Invalid format for device id. Enter a hex value prefixed with '0x'. Eg: 0x68\n");
			return StatusInvalArgs;
		}
	}
	
	if((status = CLIPARSER_GetParamValue(PARAM_S, &pcParam)) == StatusSuccess)
	{
		if(sscanf(pcParam, "%x", &istartaddr) == 1)
		{
			LOG_INFO("Start address = 0x%x\n", istartaddr);
		}
		else
		{
			LOG_ERROR("Invalid format for start address. Enter a hex value prefixed with '0x'. Eg: 0x1234\n");
			return StatusInvalArgs;
		}
	}

	if((status = CLIPARSER_GetParamValue(PARAM_R, &pcParam)) == StatusSuccess)
	{
		imemcount = atoi(pcParam);
		LOG_INFO("Memory/Register size = %d\n", imemcount);
	}
	
	return status;
}




/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type PrintData(char *pcdata, int isize)
{
	short sbytecnt=0, swordcnt=0, sitr=0, swordprnt=0;

	if(isize == 0)
	{
		LOG_ERROR("Invalid argument. Print data length is 0\n");
		return StatusInvalArgs;
	}

	if(pcdata == NULL)
	{
		LOG_ERROR("Invalid argument. No data to print\n");
		return StatusInvalArgs;
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
int main(int iArgc, char *cpaArgv[])
{
	Status_Type status=StatusSuccess;
	I2CCfg_Type i2ccfg={0};
	char cbuf[1024] = {0,1,2,3};
	int icnt=0, ibytes=0, istrt=0;
	int ibytcnt=0;

	status = CLIPARSER_ParseCommandLine(iArgc, cpaArgv);
	if(status != StatusSuccess)
	{
		LOG_ERROR("Parsing commandline arguments failure\n");
		return status;
	}

	status = CheckParameters();
	if(status != StatusSuccess)
	{
		LOG_ERROR("Parameters check failure\n");
		return status;
	}

	if(ichntype == I2C)
	{
		i2ccfg.schno = ichno;
		i2ccfg.sdevaddr = (short) iaddr;
		i2ccfg.saddr = (short) istartaddr;
		i2ccfg.smemcount = (short) imemcount;
		
		if(ioperatn == 1)
			i2ccfg.rwflg = 1;
		else if (ioperatn == 2)
			i2ccfg.rwflg = 0;
	}

	status = I2CDRIVER_Open(&i2ccfg);
	if(status != StatusSuccess)
	{
		LOG_ERROR("Unable to Open I2C device\n");
		return status;
	}

	LOG_PRINT("\t");
	for(icnt=0; icnt < 16; icnt++)
		LOG_PRINT("0x%02x ", icnt);

	while(imemcount)
	{
		if(imemcount > sizeof(cbuf))
			icnt = sizeof(cbuf);
		else 
			icnt = imemcount;

		status = I2CDRIVER_Read(&cbuf[istrt], icnt, &ibytes);
		if(status != StatusSuccess)
		{
			LOG_ERROR("Failed to read data on i2cbus\n");
			break;
		}
		
		for(icnt=0; (icnt*16) < ibytes; icnt++)
		{
			LOG_PRINT("\n0x%02x\t", icnt);
			for(ibytcnt=0; ibytcnt<16; ibytcnt++)
			{
				LOG_PRINT(" %02x  ", cbuf[(icnt*16) + ibytcnt]);
			}
			
			LOG_PRINT("\n\t");
			for(ibytcnt=0; ibytcnt<16; ibytcnt++)
			{
				LOG_PRINT("%03d  ", cbuf[(icnt*16) + ibytcnt]);
			}
			LOG_PRINT("\n\n");
		}

		istrt += ibytes;
		imemcount -= ibytes;

		if(status != StatusSuccess)
			break;
	}

	I2CDRIVER_Close();
	return status;
}

/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */

