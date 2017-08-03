/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



#ifndef		__TYPES_H_     
#define		__TYPES_H_    


/* Include Files **************************************************************************
* @{
*/

/*
* @}
*/


/* Data Types & Variables *****************************************************************
* @{
*/

/***************************************************************************************
Data Structure Name	: Status_Type 
Description	: Return status type
Notes		: None
Published	: No
*/
typedef enum
{
	StatusSuccess		=0,
	StatusInsufArgs,
	StatusInvalArgs,
	StatusDevInitFail,
	StatusChkSumErr,
	StatusLenMismatch,
	StatusInsufMem,		
	StatusFail			=255
}Status_Type;

/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/


/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

