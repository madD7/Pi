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
Description	: Initialize debug print
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type LOG_Init(short sval)
{

	sVerbosity = sval;

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

	if(sverb < LOGS_INFO )
		sstderrflg = 1;

	if(sverb <= sVerbosity)
	{
		va_start(argptr, msg);
		vfprintf(stdout, msg, argptr);

		if(sstderrflg)
			vfprintf(stderr, msg, argptr);

		va_end(argptr);
	}
}


/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */



