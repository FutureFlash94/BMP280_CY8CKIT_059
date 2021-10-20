/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                           CYPRESS PSoC5
*                                              with the
*                                     CY8CKIT-050 Development Kit
*
* Filename      : bsp.c
* Version       : V1.01
* Programmer(s) : Filip Wagner
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <cyapicallbacks.h>

#define  BSP_MODULE
#include <bsp.h>
#include <bsp_psoc.h>

#include <bsp_uart.h>
#include <bsp_gpio.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/
                                                                  

/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_PreInit()
*
* Description : Board support package pre-initialization funtion.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This funtion initializes peripherals required before the OS is initialized (e.g interrupt controller
*                   external memory.
*********************************************************************************************************
*/


void BSP_PreInit (void)
{
    CPU_INT32U  reg_val;
    CPU_INT32U *p_vect_tbl;
    
    
    reg_val = CPU_REG_NVIC_VTOR;                                   /* Get the NVIC offset table                            */
    DEF_BIT_CLR(reg_val, DEF_BIT_29);                              /* Clear the TBLBASE bit                                */
    
     
    p_vect_tbl                  = (CPU_INT32U *)reg_val;           
                                                                   /* Install the PendSV & Systick Interrupt Handler       */
    p_vect_tbl[CPU_INT_PENDSV]  = (CPU_INT32U  )OS_CPU_PendSVHandler;
    p_vect_tbl[CPU_INT_SYSTICK] = (CPU_INT32U  )OS_CPU_SysTickHandler;
}


/*
*********************************************************************************************************
*                                               BSP_PostInit()
*
* Description : Board Support Package Post initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This funtion initilize peripherlas that requires the OS to be initialized.
*********************************************************************************************************
*/

void  BSP_PostInit (void)
{
    init_gpio();
    init_uart();
}


/*
*********************************************************************************************************
*                                       BSP_CPU_TickInit()
*
* Description : This function reads CPU registers to determine the CPU clock frequency of the chip in KHz.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  BSP_CPU_TickInit  (void)
{
    CPU_INT32U   cpu_clk_freq;
    CPU_INT32U   cnts;
    
    cpu_clk_freq = BSP_CPU_ClkFreq();                             /* Determine SysTick reference freq.                */                                                                        
#if (OS_VERSION > 30000u)                                         /* Determine nbr SysTick increments                 */
    cnts         = (cpu_clk_freq / OSCfg_TickRate_Hz);
#else
    cnts         = (cpu_clk_freq / OS_TICKS_PER_SEC);
#endif
    OS_CPU_SysTickInit(cnts);                                     /* Init uC/OS periodic time src (SysTick).          */
    
}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    return ((CPU_INT32U)BSP_PSOC_CPU_FREQ);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             STRING FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        BSP_STR_ISBLANC()
*
* Description : determinate if given string contains within the first p characters
*               only blanc characters
*
* Argument(s) : s     the string
*               p     number of characters
*
* Return(s)   : DEF_TRUE    If the first p characters contains only blanc character
*               DEF_FALSE   If the first p characters contains a non blanc character
*
* Caller(s)   : application
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN BSP_STR_ISBLANC(char s[], int p) {
  int i = 0;
  
  while (i < p) {
    if (!isblank(s[i])) {
      return DEF_FALSE;
    }
    i++;
  }
  return DEF_TRUE;
}

/*
*********************************************************************************************************
*                                  BSP_STR_ISNUMBER()
*
* Description : determinate if given string contains within the first p characters
*               only numeric characters
*
* Argument(s) : s     the string
*               p     number of characters
*
* Return(s)   : DEF_TRUE    If the first p characters contains only numeric character
*               DEF_FALSE   If the first p characters contains a non numeric character
*
* Caller(s)   : application
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN BSP_STR_ISNUMBER(char s[], int p)
{
  int i = 0;
  
  while (i < p) {
    if (isdigit(s[i]) == 0) {
      return DEF_FALSE;
    }
    i++;
  }
  return DEF_TRUE;
}

/* [] END OF FILE */