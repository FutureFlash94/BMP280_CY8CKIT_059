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
* Description : This function initializes the SPIM_1 communication module. 
*               Configuration:  CPHA = 0; CPOL = 0; Data lines: MISO + MOSI;
*                               Data bits = 8; Shift direction = MSB first; 
*                               Data rate = 1000 kbps;
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
*                                prepare_and_wait_spi_send()
*
* Description : This fuction clears the transmit and receive buffer of the spi. 
*               Furthermore it waits for the spi status to be set to done or idle.
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

CPU_VOID prepare_and_wait_spi_send(CPU_VOID) 
{
  /* Clear the transmit buffer before next reading (good practice) */
  SPIM_1_ClearTxBuffer();
  /* Clear the receive buffer before next reading */
  SPIM_1_ClearRxBuffer();
  /* Ensure that previous SPI read is done, or SPI is idle before sending data */
  while(!(SPIM_1_ReadTxStatus() & (SPIM_1_STS_SPI_DONE | SPIM_1_STS_SPI_IDLE)));
}

/*
*********************************************************************************************************
*                                   spi_send_n_bytes()
*
* Description : This function writes n bytes to the spi tx buffer, beginning with
*               byte and continue with byte+1 till byte+n-1.
*
* Argument(s) : byte ... first byte that is written to spi tx buffer
*               n ... amount of bytes to be write spi tx buffer
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID spi_send_n_bytes(CPU_INT08U byte, CPU_INT08U n)
{
  for(CPU_INT08U byteCountTemp = 0u; byteCountTemp < n; byteCountTemp++) {
    SPIM_1_WriteTxData(byte+byteCountTemp);
  }
}

/*
*********************************************************************************************************
*                                   wait_spi_tx()
*
* Description : Waits for transmission of data stored in the spi tx buffer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Blocking.
*********************************************************************************************************
*/

CPU_VOID wait_spi_tx(CPU_VOID)
{
  while(!(SPIM_1_ReadTxStatus() & (SPIM_1_STS_SPI_DONE)));
}

/*
*********************************************************************************************************
*                                       spi_get_byte()
*
* Description : This function waits and returns first received data over spi interface.
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
  /* wait for receiving data over spi interface */
  while(SPIM_1_GetRxBufferSize() == 0);
  /* No valid data (received during sending register address) */
  SPIM_1_ReadRxData();
  /* Return first valid data */
  return SPIM_1_ReadRxData();
}

/*
*********************************************************************************************************
*                                     spi_get_n_bytes()
*
* Description : This function waits to receive data over spi interface and write
*               n bytes to the given reg_values array.
*
* Argument(s) : reg_values ... array where data received over spi is written
*               n ... amount of bytes to be write to the reg_values array
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID spi_get_n_bytes(CPU_INT08U reg_values[], CPU_INT08U n)
{
  /* wait for receiving data over spi interface */
    while(SPIM_1_GetRxBufferSize() == 0);
    /* No valid data (received during sending register address) */
    SPIM_1_ReadRxData();
    /* Write n valid data to array */
    for(CPU_INT08U byteCountTemp = 0u; byteCountTemp < n; byteCountTemp++) {
        reg_values[byteCountTemp] = SPIM_1_ReadRxData();
    }
}


/* [] END OF FILE */
