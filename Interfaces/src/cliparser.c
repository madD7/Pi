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
#include "cliparser.h"
#include "log.h"
#include <stdlib.h>
/*
* @}
*/


/* Variables ******************************************************************************
* @{
*/
static char* cpaArgptr[26]={NULL};


/*
* @}
*/



/* ifunctions ******************************************************************************
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
static void __CLIPARSER_PrintHelp()
{
	LOG_PRINT("\n"
			"Command Line Switches:\n"
			"\t[-c<channel type>] [-n<channel no.>] [-d<device address>] [-o<operation type>]"
			"[-s<start addr>] [-r<no. of registers/memory locations>]\n"
			);

	exit (0);
}



/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type CLIPARSER_ParseCommandLine(int iArgc, char* pcaArgv[])
{
	char *pcParam=NULL;
	int i=0, ifunc=0;

	if( iArgc <= 1 )
	{
		LOG_ERROR( "No input parameters specified\n");
		return StatusInsufArgs;
	}

	/* Iterate through cmdline parameters */
	for(i = 1; i < iArgc; i++ )
	{
		pcParam = pcaArgv[i];

		/* Allow parameters to start with '-' */
		if( pcParam[0] != '-' )
		{
			LOG_ERROR("Unrecognised parameter. Must start with '-' \n");
			return StatusInvalArgs;
		}

		if( strlen(pcParam) <= 1 )
		{
			LOG_ERROR("Parameter value not specified\n");
			return StatusInvalArgs;
		}

		if(pcParam[1] == '-')
		{
			LOG_ERROR( " '--' commands are unavailable\n" );
			return StatusInvalArgs;
		}

		if(pcParam[1] == 'h' || pcParam[1] == 'H' || pcParam[1] == '?')
		{
			__CLIPARSER_PrintHelp();
		}
		else if(pcParam[1] >= 'A' && pcParam[1] <= 'Z')
		{
			ifunc = pcParam[1] - 'A';
		}
		else if(pcParam[1] >= 'a' && pcParam[1] <= 'z')
		{
			ifunc = pcParam[1] - 'a';
		}
		else
		{
			LOG_ERROR("Only 'a'/'A' to 'z'/'Z' are allowed as command line input, prefixed with '-'\n" );
			return StatusInvalArgs;
		}

		cpaArgptr[ifunc] = &pcParam[2];
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
Status_Type CLIPARSER_GetParamValue(short sParam, char** ppcVal)
{

	if(CLIPARSER_CheckParamVal(cpaArgptr[sParam]) == StatusSuccess)
	{
		*ppcVal = cpaArgptr[sParam];
	}
	else
	{
		LOG_INFO("No parameter specified for %c\n", (char)sParam+'a');
		return StatusInsufArgs;
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
Status_Type CLIPARSER_CheckParamVal(char *pctemp)
{
	if(pctemp == NULL || *pctemp == '\0')
	{
		LOG_INFO("Parameter value check failed\n");
		return StatusFail;
	}
	else
		return StatusSuccess;
}




/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */





