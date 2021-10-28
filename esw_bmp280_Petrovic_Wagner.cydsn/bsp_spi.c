/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <bsp_spi.h>

/*
*********************************************************************************************************
*                                       init_spi()
*
* Description : This function initializes the SPIM_1 communication module. Configuration: -
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID init_spi(CPU_VOID)
{
    SPIM_1_Start();
}

/*
*********************************************************************************************************
*                                       spi_get_byte()
*
* Description : 
*
* Argument(s) : none.
*
* Return(s)   : Next byte read from SPI RX buffer.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_INT08U spi_get_byte(CPU_VOID)
{
    // while(SPIM_1_GetRxBufferSize() == 0);
    return SPIM_1_ReadRxData();
}

/*
*********************************************************************************************************
*                                       spi_send_byte()
*
* Description : 
*
* Argument(s) : byte ... containing the data to transmit
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID spi_send_byte(CPU_INT08U byte)
{
    SPIM_1_WriteTxData(byte);
}

/* [] END OF FILE */
