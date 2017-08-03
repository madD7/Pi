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

/*
* @}
*/


/* Variables ******************************************************************************
* @{
*/


/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/
int main(int iArgc, char *pArgv[])
{
	Status_Type status=StatusSuccess;

	status = CLIPARSER_ParseCommandLine(iArgc, pArgv);

	return status;
}

/*
* @}
*/





/* ~~~~~ END OF FILE ~~~~~ */

