/*
*********************************************************************************************************
*
*                                       MES1 Embedded Software (RTOS)
*
* Filename      : app.c
* Version       : V1.3
* Programmer(s) : Filip Wagner
                  
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

#define  APP_USER_IF_SIGN_ON                        0u
#define  APP_USER_IF_VER_TICK_RATE                  1u
#define  APP_USER_IF_CPU                            2u
#define  APP_USER_IF_CTXSW                          3u
#define  APP_USER_IF_STATE_MAX                      4u

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB   App_TaskStartTCB;
static  CPU_STK  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   App_TaskCMD_TCB;
static  CPU_STK  App_TaskCMDStk[APP_CFG_TASK_CMD_STK_SIZE];

static  OS_TCB   App_TaskSine_TCB;
static  CPU_STK  App_TaskSineStk[APP_CFG_TASK_SINE_STK_SIZE];

static  OS_TCB   App_TaskCosine_TCB;
static  CPU_STK  App_TaskCosineStk[APP_CFG_TASK_COSINE_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskStart  (void *p_arg);

static  void  App_TaskCMD (void *p_arg);
static  void  App_TaskSINE (void *p_arg);
static  void  App_TaskCOSINE (void *p_arg);

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
  
  /* create CMD task */
  OSTaskCreate((OS_TCB      *)&App_TaskCMD_TCB,
               (CPU_CHAR    *)"TaskCMD",
               (OS_TASK_PTR  )App_TaskCMD, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_CMD_PRIO,
               (CPU_STK     *)&App_TaskCMDStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_CMD_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_CMD_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
  /* create SINE task */
  OSTaskCreate((OS_TCB      *)&App_TaskSine_TCB,
               (CPU_CHAR    *)"TaskSINE",
               (OS_TASK_PTR  )App_TaskSINE, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_SINE_PRIO,
               (CPU_STK     *)&App_TaskSineStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_SINE_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_SINE_STK_SIZE,
               (OS_MSG_QTY   )10u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
  /* create COSINE task */
  OSTaskCreate((OS_TCB      *)&App_TaskCosine_TCB,
               (CPU_CHAR    *)"TaskCOSINE",
               (OS_TASK_PTR  )App_TaskCOSINE, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_COSINE_PRIO,
               (CPU_STK     *)&App_TaskCosineStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_COSINE_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_COSINE_STK_SIZE,
               (OS_MSG_QTY   )10u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
}

/*
*********************************************************************************************************
*                                          App_TaskSINE()
*
* Description : SINE Task checks received message from CMD Task and give in case
*               of missing or invalid parameter an error message back 
*               via the UART interface. For the parameter "all" it output all 
*               sine-values from 0-360 with step-size 10. For all numeric parameter 
*               between 0-360 the sin-value will be send vie the UART interface.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskSINE()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static void App_TaskSINE (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  OS_MSG_SIZE  msg_size;
  CPU_INT08U   angle_array[4] = {0};
  CPU_INT16U   angle_value;
  CPU_INT08U*  p_rx_msg;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    
    /* wait for message */
    p_rx_msg = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &os_err);
    if (os_err == OS_ERR_NONE) {
      /* message received */
      if (msg_size <= 3) {
        /* message contains function but no parameter */
        uart_send_string("Argument of function empty!\n");
      }
      else {
        /* get part of message between index = 4 and msg_size but not more then msg_size = 6 */
        memcpy(angle_array, (p_rx_msg+4), (msg_size > 6 ? 6 : msg_size)-3);
        if (BSP_STR_ISBLANC((CPU_CHAR *)angle_array, strlen((CPU_CHAR *)angle_array))) {
          /* message contains function but no parameter */
          uart_send_string("Argument of function empty!\n");
        }
        else if (strncmp((CPU_CHAR *)angle_array, "all", 3) == 0) {
          /* message = 'sin all' -> send sine-values from 0-360 with step-size 10 */
          for (CPU_INT16U a = 0; a <= 360; a+=10) {
            uart_send_double("SINE-RES: ", sin(a * M_PI / 180));
          }
        } else {
          /* check if argument is a number and between 0 and 360 */
          CPU_BOOLEAN isNumber = BSP_STR_ISNUMBER((CPU_CHAR *)angle_array, strlen((CPU_CHAR *)angle_array));
          angle_value = atoi((CPU_CHAR *)angle_array);
          
          if (isNumber == DEF_TRUE && angle_value >= 0 && angle_value <= 360) {
            /* argument is a number and between 0 and 360, send calculated sine-value */
            uart_send_double("SINE-RES: ", sin(angle_value * M_PI / 180));
          }
          else {
            /* send message to user that argument exceeds expected value*/
            uart_send_string("Please use a number between 0 and 360\n");
          }
        }
      }
    }
  }
}

/*
*********************************************************************************************************
*                                          App_TaskCOSINE()
*
* Description : COSINE Task checks received message from CMD Task and give in case
*               of missing or invalid parameter an error message back 
*               via the UART interface. For the parameter "all" it output all 
*               cosine-values from 0-360 with step-size 10. For all numeric parameter 
*               between 0-360 the cosin-value will be send vie the UART interface.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskCOSINE()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static void App_TaskCOSINE (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  OS_MSG_SIZE  msg_size;
  CPU_INT08U   angle_array[4] = {0};
  CPU_INT16U   angle_value;
  CPU_INT08U*  p_rx_msg;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {

    /* wait for message */
    p_rx_msg = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &os_err);
    if (os_err == OS_ERR_NONE) {
      /* message received */
      if (msg_size <= 3) {
        /* message contains function but no parameter */
        uart_send_string("Argument of function empty!\n");
      }
      else {
        /* get part of message between index = 4 and msg_size but not more then msg_size = 6 */
        memcpy(angle_array, (p_rx_msg+4), (msg_size > 6 ? 6 : msg_size)-3);
        if (BSP_STR_ISBLANC((CPU_CHAR *)angle_array, strlen((CPU_CHAR *)angle_array))) {
          /* message contains function but no parameter */
          uart_send_string("Argument of function empty!\n");
        }
        else if (strncmp((CPU_CHAR *)angle_array, "all", 3) == 0) {
          /* message = 'cos all' -> send cosine-values from 0-360 with step-size 10 */
          for (CPU_INT16U a = 0; a <= 360; a+=10) {
            uart_send_double("COSINE-RES: ", cos(a * M_PI / 180));
          }
        } else {
          /* check if argument is a number and between 0 and 360 */
          CPU_BOOLEAN isNumber = BSP_STR_ISNUMBER((CPU_CHAR *)angle_array, strlen((CPU_CHAR *)angle_array));
          angle_value = atoi((CPU_CHAR *)angle_array);
          
          if (isNumber == DEF_TRUE && angle_value >= 0 && angle_value <= 360) {
            /* argument is a number and between 0 and 360, send calculated cosine-value */
            uart_send_double("COSINE-RES: ", cos(angle_value * M_PI / 180));
          }
          else {
            /* send message to user that argument exceeds expected value*/
            uart_send_string("Please use a number between 0 and 360\n");
          }
        }
      }
    }
  }
}

/*
*********************************************************************************************************
*                                          App_TaskCMD()
*
* Description : CMD Task checks for available bytes within the UART receive buffer. If correct string is
*               available (e.g. PC -> uC: #abc$ or #Hellor World$), process the message and send it to SINE or
*               COSINE Task for further processing, otherwise it output a help message via the UART interface.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskCMD()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static  void  App_TaskCMD (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_INT08U   rec_byte = 0x00;
  CPU_INT08U   rx_msg[UART_1_RX_BUFFER_SIZE] = {0};
  CPU_INT08U   idx = 0x00;
  CPU_INT08U   rec_byte_cnt = 0x00;
  CPU_BOOLEAN  str_available = DEF_FALSE;
  
  /* prevent compiler warnings */
  (void)p_arg;
  (void)Start_of_Packet;
  (void)End_of_Packet;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    /* check if a byte is available */
    rec_byte = uart_get_byte();
    /* check if the received byte is '#'*/
    if(rec_byte == Start_of_Packet){
      /* if received byte was correct */
      while(DEF_TRUE){
        /* receive byte by byte */
        rec_byte = uart_get_byte();
        /* check if byte is something meaningful */
        if(rec_byte){
          /* save byte into software receive buffer and increment idx */
          rx_msg[idx++] = rec_byte;
        }
        /* initiate scheduler */
        OSTimeDlyHMSM(0, 0, 0, 20, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &os_err);
        /* check if received byte is '$' */
        if(rx_msg[idx-1]==End_of_Packet){
          /* if end of packet is reached -> break */
          break;
        }
      }
      /* Remove end of packet character */
      rx_msg[idx-1] = '\0';
      /* message received, calculate received bytes, -2 because of '#' & '$' */
      rec_byte_cnt = idx-2;
      /* signal that a string is available */
      str_available = DEF_TRUE;
    }
    /* if received byte wasn't start of packet */
    else{
      /* reset software receive buffer */
      memset(&rx_msg[0],0,sizeof(rx_msg));
      /* reset string available signal */
      str_available = DEF_FALSE;
      /* reset received byte variable */
      rec_byte = 0x00;
      /* reset idx */
      idx = 0x00;
    }
    /* check if message is available */
    if(str_available){
      /* process received message */
      /* check if message start with 'sin ' */
      if (strncmp((CPU_CHAR *)rx_msg,"sin ",4)==0) {
        /* message start with 'sin ' */
        if (rec_byte_cnt<=6) {
          /* message is not longer as 7 characters (e.g. 'sin 360', 'sin all') */
          /* send received message to SINE Task*/
          OSTaskQPost((OS_TCB      *)&App_TaskSine_TCB,
                      (CPU_INT08U  *)&rx_msg[0],
                      (OS_MSG_SIZE  )sizeof(rx_msg),
                      (OS_OPT       )OS_OPT_POST_FIFO,
                      (OS_ERR      *)&os_err);
        } else {
          /* send message to user that entered message exceeds expected length*/
          uart_send_string("Please use a number between 0 and 360\n");
        }
      }
      /* check if message start with 'cos ' */
      else if (strncmp((CPU_CHAR *)rx_msg,"cos ",4)==0){
        /* message start with 'cos ' */
        if (rec_byte_cnt<=6) {
          /* message is not longer as 7 characters (e.g. 'cos 360', 'cos all') */
          /* send received message to COSINE Task*/
          OSTaskQPost((OS_TCB      *)&App_TaskCosine_TCB,
                      (CPU_INT08U  *)&rx_msg[0],
                      (OS_MSG_SIZE  )sizeof(rx_msg),
                      (OS_OPT       )OS_OPT_POST_FIFO,
                      (OS_ERR      *)&os_err);
        } else {
          /* send message to user that entered message exceeds expected length*/
          uart_send_string("Please use a number between 0 and 360\n");
        }
      }
      else {
        /* unknown message received, send help message to user */
        uart_send_string("Command not found!\n\n");
        uart_send_string("Please use on of the following commands:\n");
        uart_send_string("\t*) \"sin x\" where x is a number between 0 and 360 or \"sin all\"\n");
        uart_send_string("\t*) \"cos x\" where x is a number between 0 and 360 or \"cos all\"\n");
      }
      /* reset software receive buffer */
      memset(&rx_msg[0],0,sizeof(rx_msg));
      /* reset string available signal */
      str_available = DEF_FALSE;
      /* reset received byte variable */
      rec_byte = 0x00;
      /* reset idx */
      idx = 0x00;
    }
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 0, 100, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
  }
}

/* END OF FILE */