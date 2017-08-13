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
const char* const cpaComType[] = { I2C_NAME_STRING
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

	LOG_PRINT("\t");

	for(sbytecnt=0; sbytecnt < 16; sbytecnt++)
		LOG_PRINT("0x%02x ", sbytecnt);

	sitr = isize/16 + (isize%16?1:0);
	do
	{
		LOG_PRINT("\n0x%x\t", swordcnt);
		
		swordprnt = (swordcnt==sitr?isize%16:16);

		for(sbytecnt=0; sbytecnt < swordprnt; sbytecnt++)
		{
			LOG_PRINT(" %02x  ", *pcdata);
			pcdata++;
		}

		swordcnt++;
	}while(swordcnt < sitr);

	LOG_PRINT("\n");
	return StatusSuccess;
}



int main(int iArgc, char *cpaArgv[])
{
	Status_Type status=StatusSuccess;
	I2CCfg_Type i2ccfg={0};
	char cbuf[256] = {0,1,2,3,4,5,6,7,8,9};
	int icnt=0, ibytes=0;

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
		i2ccfg.schno = (short) ichno;
		i2ccfg.sdevaddr = (short) iaddr;
		i2ccfg.saddr = (short) istartaddr;
		i2ccfg.smemcount = (short) imemcount;
		
		if(ioperatn == 1)
			i2ccfg.rwflg = 1;
		else if (ioperatn == 2)
			i2ccfg.rwflg = 0;
	}

	while(imemcount)
	{
		if(imemcount > sizeof(cbuf))
			icnt = sizeof(cbuf);
		else 
			icnt = imemcount;

		status = I2CDRIVER_Read(cbuf, icnt, &ibytes);

		PrintData(cbuf, ibytes);

		imemcount -= ibytes;

		if(status != StatusSuccess)
			return status;
	}

	return status;
}

/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */

