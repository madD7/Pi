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
	LOG_WARN("\n"
			"Command Line Switches:\n"
			"\n"
			"\t[-c<channel type>] [-n<channel no.>] [-d<device address>] [-o<operation type>]"
			"[-s<start addr>] [-r<no. of registers/memory locations>]\n"
			);
}



/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
Published	: No
*/
Status_Type CLIPARSER_ParseCommandLine(int argc, char* argv[])
{
	char *param=NULL;
	int i=0, ifunc=0;

	if( argc <= 1 )
	{
		LOG_ERROR( "No input parameters specified\n");
		return StatusInsufArgs;
	}

	/* Iterate through cmdline parameters */
	for(i = 1; i < argc; i++ )
	{
		param = argv[i];

		/* Allow parameters to start with '-' */
		if( param[0] != '-' )
		{
			LOG_ERROR("Unrecognised parameter. Must start with '-' \n");
			return StatusInvalArgs;
		}

		if( strlen(param) <= 1 )
		{
			LOG_ERROR("Parameter value not specified\n");
			return StatusInvalArgs;
		}

		if(param[1] == '-')
		{
			LOG_ERROR( " '--' commands are unavailable\n" );
			return StatusInvalArgs;
		}

		if(param[1] == 'h' || param[1] == 'H' || param[1] == '?')
		{
			__CLIPARSER_PrintHelp();
			return StatusFail;
		}
		else if(param[1] >= 'A' && param[1] <= 'Z')
		{
			ifunc = param[1] - 'A';
		}
		else if(param[1] >= 'a' && param[1] <= 'z')
		{
			ifunc = param[1] - 'a';
		}
		else
		{
			LOG_ERROR("Only 'a'/'A' to 'z'/'Z' are allowed as command line input, prefixed with '-'\n" );
			return StatusInvalArgs;
		}

		cpaArgptr[ifunc] = &param[2];
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
Status_Type CLIPARSER_GetParamValue(unsigned char param, char** pval)
{
	if(cpaArgptr[param] != NULL)
	{
		*pval = cpaArgptr[param];
	}
	else
	{
		LOG_ERROR("No parameter specified");
		LOG_ERR_DETAILS("for %c", param+'a');
		LOG_ERROR("\n");
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





