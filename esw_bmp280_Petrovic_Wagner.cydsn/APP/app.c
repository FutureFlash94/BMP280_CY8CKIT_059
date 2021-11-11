/*
*********************************************************************************************************
*
*                                       MES1 Embedded Software (RTOS)
*
* Filename      : app.c
* Version       : V1.0
* Programmer(s) : Mihael Petrovic, Filip Wagner
                  
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <cyapicallbacks.h>

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

#define  BMP280_CONFIG_OSRT_T               BMP280_OSRT_T_X4
#define  BMP280_CONFIG_OSRT_P               BMP280_OSRT_P_X2
#define  BMP280_CONFIG_POWER_MODE           BMP280_POWER_MODE_NORMAL
#define  BMP280_CONFIG_T_STANDBY            BMP280_T_STANDBY_0_5MS
#define  BMP280_CONFIG_FILTER               BMP280_FILTER_COEFF_2
#define  BMP280_CONFIG_SPI3W_EN             BMP280_SPI3_WIRE_DISENABLE

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB   App_TaskStartTCB;
static  CPU_STK  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   App_TaskCOM_TCB;
static  CPU_STK  App_TaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

static  OS_TCB   App_TaskPRESS_TCB;
static  CPU_STK  App_TaskPRESSStk[APP_CFG_TASK_PRESS_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskStart  (void *p_arg);

static  void  App_TaskCOM (void *p_arg);
static  void  App_TaskPRESS (void *p_arg);

static  void  App_TaskCreate (void);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Startup Code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  main (void)
{
  OS_ERR  os_err;
  
  BSP_PreInit();                                              /* Perform BSP pre-initialization.                      */

  CPU_Init();                                                 /* Initialize the uC/CPU services                       */

  OSInit(&os_err);                                            /* Init uC/OS-III.                                      */
  
  OSTaskCreate((OS_TCB      *)&App_TaskStartTCB,              /* Create the start task                                */
               (CPU_CHAR    *)"Start",
               (OS_TASK_PTR  )App_TaskStart, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_START_PRIO,
               (CPU_STK     *)&App_TaskStartStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
  
  OSStart(&os_err);                                            /* Start multitasking (i.e. give control to uC/OS-III).  */
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
  OS_ERR       err;
     
 (void)p_arg;

  BSP_PostInit();                                               /* Perform BSP post-initialization functions.       */
  
  BSP_CPU_TickInit();                                           /* Perfrom Tick Initialization                      */
  
#if (OS_CFG_STAT_TASK_EN > 0u)
  OSStatTaskCPUUsageInit(&err);
#endif    

#ifdef CPU_CFG_INT_DIS_MEAS_EN
  CPU_IntDisMeasMaxCurReset();
#endif      

  // wait for bmp280 sensor to be connected
  wait_bmp280_connecting();

  App_TaskCreate();                                             /* Create application tasks.                         */
  
  while (DEF_TRUE) {                                            /* Task body, always written as an infinite loop.   */
    OSTimeDlyHMSM(0, 0, 0, 100, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &err);
  }
}


/*
*********************************************************************************************************
*                                          App_TaskCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
  /* declare and define function local variables */
  OS_ERR  os_err;
  
  /* create COM task */
  OSTaskCreate((OS_TCB      *)&App_TaskCOM_TCB,
               (CPU_CHAR    *)"TaskCOM",
               (OS_TASK_PTR  )App_TaskCOM, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_COM_PRIO,
               (CPU_STK     *)&App_TaskCOMStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_COM_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_COM_STK_SIZE,
               (OS_MSG_QTY   )10u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
  /* create PRESS task */
  OSTaskCreate((OS_TCB      *)&App_TaskPRESS_TCB,
               (CPU_CHAR    *)"TaskPRESS",
               (OS_TASK_PTR  )App_TaskPRESS, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_PRESS_PRIO,
               (CPU_STK     *)&App_TaskPRESSStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_PRESS_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_PRESS_STK_SIZE,
               (OS_MSG_QTY   )10u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
}

/*
*********************************************************************************************************
*                                          App_TaskPRESS()
*
* Description : PRESS Task configure the connected bmp280 and read measured 
*               pressure and temperature. After the read it send the data to the
*               COM Task over TaskQ.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskPRESS()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static void App_TaskPRESS (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR             os_err;
  Bmp280_press_temp  bmp280_data;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  set_bmp280_config(
    BMP280_CONFIG_OSRT_T, BMP280_CONFIG_OSRT_P, BMP280_CONFIG_POWER_MODE,
    BMP280_CONFIG_T_STANDBY, BMP280_CONFIG_FILTER, BMP280_CONFIG_SPI3W_EN
  );
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    
    /* read pressure and temperature from bmp280 */
    bmp280_data = get_bmp280_press_temp();
    
    /* send readed bmp280 data to COM Task*/
    OSTaskQPost((OS_TCB      *)&App_TaskCOM_TCB,
                (CPU_INT08U  *)&bmp280_data,
                (OS_MSG_SIZE  )sizeof(bmp280_data),
                (OS_OPT       )OS_OPT_POST_FIFO,
                (OS_ERR      *)&os_err);
    
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 1, 0,
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
  }
}

/*
*********************************************************************************************************
*                                          App_TaskCOM()
*
* Description : COM Task wait for a message and send it's data via the UART interface.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskCOM()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static  void  App_TaskCOM (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR             os_err;
  OS_MSG_SIZE        msg_size;
  Bmp280_press_temp* p_rx_msg;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    
    /* wait for message */
    p_rx_msg = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &os_err);
    
    if (os_err == OS_ERR_NONE) {
      /* if message received */
      uart_send_press_temp(*p_rx_msg);
    }
  }
}

/* END OF FILE */