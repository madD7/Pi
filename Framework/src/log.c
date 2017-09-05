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
void LOG_LogMsg(const char* pcfname, int ilineno, short sverb, const char* msg, ...)
{
	va_list argptr;
	FILE *pF = stdout;

	if(sverb <= sVerbosity)
	{
		if(sverb < LOGS_INFO )
		{
			pF = stderr;
		}
		else
		{ 
			pF = stdout;
		}

		if(pcfname != NULL)
			fprintf(pF, "%s:%d | ", pcfname, ilineno);

		if(sverb == LOGS_ERROR)
			fprintf(pF, "%s", "ERROR: ");
		else if(sverb == LOGS_WARN)
			fprintf(pF, "%s", "WARN: ");
		else if(sverb == LOGS_INFO)
			fprintf(pF, "%s", "INFO: ");

		va_start(argptr, msg);

		vfprintf(pF, msg, argptr);
		
		va_end(argptr);
	}
}


/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */



