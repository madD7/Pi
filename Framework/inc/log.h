/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



#ifndef		__LOG_H_
#define		__LOG_H_



/* Include Files **************************************************************************
* @{
*/
#include <stdio.h>
#include <stdarg.h>
#include "types.h"

/*
* @}
*/


/* Data Types & Variables *****************************************************************
* @{
*/


/* Verbosity Levels */
#define LOGS_ERROR			0
#define LOGS_WARN			1
#define	LOGS_ERR_DETAIL		2
#define LOGS_INFO			3
#define LOGS_ALL			255



/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/

Status_Type LOG_Init(short sval);
void LOG_SetVerbosity(short sval);
void LOG_LogMsg(short sverb, const char* msg, ...);

#define LOG_PRINT(msg, ...)				LOG_LogMsg((-1), msg, ## __VA_ARGS__)
#define LOG_ERROR(msg, ...)				LOG_LogMsg(LOGS_ERROR, msg, ## __VA_ARGS__)
#define LOG_ERR_DETAILS(msg, ...)		LOG_LogMsg(LOGS_ERR_DETAIL, msg, ## __VA_ARGS__)
#define LOG_WARN(msg, ...)				LOG_LogMsg(LOGS_WARN, msg, ## __VA_ARGS__)
#define LOG_INFO(msg, ...)				LOG_LogMsg(LOGS_INFO, msg, ## __VA_ARGS__)


/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

