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
    while(SPIM_1_GetRxBufferSize() == 0);
    SPIM_1_ReadRxData(); // No valid data
    return SPIM_1_ReadRxData();
}

CPU_VOID spi_get_n_bytes(CPU_INT08U reg_values[], CPU_INT08U byteCount)
{
    while(SPIM_1_GetRxBufferSize() == 0);
    SPIM_1_ReadRxData(); // No valid data
    
    for(CPU_INT08U byteCountTemp = 0u; byteCountTemp < byteCount; byteCountTemp++) {
        reg_values[byteCountTemp] = SPIM_1_ReadRxData();
    }
}

/*
*********************************************************************************************************
*                                   spi_send_one_byte()
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

CPU_VOID spi_send_one_byte(CPU_INT08U byte)
{
    /* Clear the transmit buffer before next reading (good practice) */
    SPIM_1_ClearTxBuffer();
    
    /* Ensure that previous SPI read is done, or SPI is idle before sending data */
    while(!(SPIM_1_ReadTxStatus() & (SPIM_1_STS_SPI_DONE | SPIM_1_STS_SPI_IDLE)));
    SPIM_1_WriteTxData(byte);
}

/*
*********************************************************************************************************
*                                   spi_send_two_bytes()
*
* Description : 
*
* Argument(s) : byte1 ... containing the data to transmit
*               byte2 ... containing the data to transmit
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID spi_send_four_bytes(CPU_INT08U byte1, CPU_INT08U byte2, CPU_INT08U byte3, CPU_INT08U byte4)
{
    /* Clear the transmit buffer before next reading (good practice) */
    SPIM_1_ClearTxBuffer();
    
    /* Ensure that previous SPI read is done, or SPI is idle before sending data */
    while(!(SPIM_1_ReadTxStatus() & (SPIM_1_STS_SPI_DONE | SPIM_1_STS_SPI_IDLE)));
    SPIM_1_WriteTxData(byte1);
    SPIM_1_WriteTxData(byte2);
    SPIM_1_WriteTxData(byte3);
    SPIM_1_WriteTxData(byte4);
}


CPU_VOID spi_send_n_bytes(CPU_INT08U byte, CPU_INT08U byteCount)
{
    /* Clear the transmit buffer before next reading (good practice) */
    SPIM_1_ClearTxBuffer();
    
    /* Ensure that previous SPI read is done, or SPI is idle before sending data */
    while(!(SPIM_1_ReadTxStatus() & (SPIM_1_STS_SPI_DONE | SPIM_1_STS_SPI_IDLE)));
    for(CPU_INT08U byteCountTemp = 0u; byteCountTemp < byteCount; byteCountTemp++) {
      SPIM_1_WriteTxData(byte+byteCountTemp);
    }
}

/*
*********************************************************************************************************
*                                   wait_spi_tx()
*
* Description : Waits for transmission of data stored in the spi tx buffer.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : Blocking.
*********************************************************************************************************
*/

CPU_VOID wait_spi_tx(CPU_VOID)
{
  while(!(SPIM_1_ReadTxStatus() & (SPIM_1_STS_SPI_DONE | SPIM_1_STS_SPI_IDLE)));
}

/* [] END OF FILE */
