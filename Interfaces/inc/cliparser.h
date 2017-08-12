/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



#ifndef		__CLIPARSER_H_
#define		__CLIPARSER_H_



/* Include Files **************************************************************************
* @{
*/
#include "types.h"
#include "string.h"
/*
* @}
*/


/* Data Types & Variables *****************************************************************
* @{
*/

#define		PARAM_A		0
#define		PARAM_B		1
#define		PARAM_C		2
#define		PARAM_D		3
#define		PARAM_E		4
#define		PARAM_F		5
#define		PARAM_G		6
#define		PARAM_H		7
#define		PARAM_I		8
#define		PARAM_J		9
#define		PARAM_K		10
#define		PARAM_L		11
#define		PARAM_M		12
#define		PARAM_N		13
#define		PARAM_O		14
#define		PARAM_P		15
#define		PARAM_Q		16
#define		PARAM_R		17
#define		PARAM_S		18
#define		PARAM_T		19
#define		PARAM_U		20
#define		PARAM_V		21
#define		PARAM_W		22
#define		PARAM_X		23
#define		PARAM_Y		24
#define		PARAM_Z		25


/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/

Status_Type CLIPARSER_ParseCommandLine(int iArgc, char* cpaArgv[]);
Status_Type CLIPARSER_GetParamValue(short sParam, char** ppcVal);
Status_Type CLIPARSER_CheckParamVal(char *pctemp);
/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

