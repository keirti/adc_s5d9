/*******************************************************************************
(C) COPYRIGHT Smart Power Solutions Limited - 2020

FILE
    threada_interface.c

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    Enter file description

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

/*=============================================================================*
 ANSI C & System-wide Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
#include "led_thread.h"
#include "adc_thread.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "threadx_interface.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
typedef struct
{
	void(*init_func)(void);
}init_func_ptr;

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
init_func_ptr thread_create_func_arr[] = 
{
	{&led_thread_create},
	{&adc_thread_create},
};

const uint8_t num_thread_creates = ((sizeof(thread_create_func_arr))/(sizeof(init_func_ptr)));

/*=============================================================================*
 Private Function Definitions (static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Private Function Implementations (Static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Function Implementations
*=============================================================================*/
/*-------------------------------------------------------------------* 

  NAME
	thread_create_main
  
  DESCRIPTION
	loop called from main.c to create all the threads within the table
	above
  PARAM
	None
  
  RETURNS
	None

*--------------------------------------------------------------------*/
void thread_create_main(void)
{
	uint8_t i = 0u;
	/*
	 * Loop for all the size of the array and call all the functions
	 * Check they arent null just for safety
	 */
    for(i = 0u; i < num_thread_creates; i++)
    {
        if(thread_create_func_arr[i].init_func != NULL)
        {
            thread_create_func_arr[i].init_func();
        }
    }
}

/*=============================================================================*
End Of File
*=============================================================================*/
