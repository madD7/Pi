/**************************************** MAD Engineers ***************************************
MAD Engineers
Copyright (c) 2014

Description	: Debug framework

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
* @{
*/
#include "log.h"
#include "types.h"

/*
* @}
*/


/* Variables ******************************************************************************
* @{
*/
static short sVerbosity=LOGS_ERROR;
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
void LOG_SetVerbosity(short sval)
{
	sVerbosity = sval;
}

/***************************************************************************************
Description	: Initialize debug print
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type LOG_Init(short sval)
{
	LOG_SetVerbosity(sval);
	
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
void LOG_LogMsg(short sverb, const char* msg, ...)
{
	va_list argptr;
	short sstderrflg=0;

	if(sverb <= sVerbosity)
	{
		if(sverb < LOGS_INFO )
		{
			sstderrflg = 1;

			if(sverb == LOGS_ERROR)
				fprintf(stderr, "%s", "ERROR: ");
			else if(sverb == LOGS_WARN)
				fprintf(stderr, "%s", "WARN: ");
		}
		else if(sverb == LOGS_INFO)
		{
			fprintf(stdout, "%s", "INFO: ");
		}

		va_start(argptr, msg);

		if(sstderrflg)
			vfprintf(stderr, msg, argptr);
		else
			vfprintf(stdout, msg, argptr);
		
		va_end(argptr);
	}
}


/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */



