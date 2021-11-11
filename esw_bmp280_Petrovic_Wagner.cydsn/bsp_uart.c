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

#include <bsp_uart.h>

/*
*********************************************************************************************************
*                                       init_uart()
*
* Description : This function initializes the UART_1 communication module. Configuration: 115200,8N1
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

CPU_VOID init_uart(CPU_VOID)
{
    UART_1_Start();
}

/*
*********************************************************************************************************
*                                       uart_get_byte()
*
* Description : Returns the last received byte of data. UART_1_GetChar() is
*               designed for ASCII characters and returns a uint8 where 1 to 255 are values
*               for valid characters and 0 indicates an error occurred or no data is present 
*
* Argument(s) : none.
*
* Return(s)   : Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
*               A returned zero signifies an error condition or no data available.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant, non-blocking.
*********************************************************************************************************
*/

CPU_INT08U uart_get_byte(CPU_VOID)
{
    return UART_1_GetChar();
}

/*
*********************************************************************************************************
*                                       uart_send_byte()
*
* Description : Puts a byte of data into the transmit buffer to be sent when the bus is
*               available. This is a blocking API that waits until the TX buffer has room to
*               hold the data. 
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

CPU_VOID uart_send_byte(CPU_INT08U byte)
{
    UART_1_PutChar(byte);
}

/*
*********************************************************************************************************
*                                      uart_send_string()
*
* Description : Puts a string into the transmit buffer to be sent when the bus is
*               available. This is a blocking API that waits until the TX buffer has room to
*               hold the data. 
*
* Argument(s) : array ... containing the data to transmit
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID uart_send_string(CPU_CHAR array[])
{
    UART_1_PutString(array);
}

/*
*********************************************************************************************************
*                                   uart_send_array()
*
* Description : Puts an array of size N into the transmit buffer to be sent when the bus is
*               available. This is a blocking API that waits until the TX buffer has room to
*               hold the data. 
*
* Argument(s) : array     ... containing the data to transmit
*               byteCount ... number of bytes to be transmitted
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID uart_send_array(CPU_INT08U array[], CPU_INT08U byteCount) 
{
    UART_1_PutArray(array, byteCount);
}

/*
*********************************************************************************************************
*                                  uart_send_press_temp()
*
* Description : Puts a string with a measured bmp280 date values into the transmit 
*               buffer to be sent when the bus is available. This is a blocking 
*               API that waits until the TX buffer has room to hold the data.
*
* Argument(s) : bmp280_data  ... data from bmp280 containing pressure and temperature 
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID uart_send_press_temp(Bmp280_press_temp bmp280_data)
{
    CPU_INT08U tx_msg[UART_1_TX_BUFFER_SIZE] = {0};
    CPU_INT08U tx_msg_len = 0;
    
    CPU_FP64   temp = (CPU_FP64)bmp280_data.temp/100;
    
    tx_msg_len = strlen(strcpy((CPU_CHAR *)tx_msg, "PRESSURE: "));
    tx_msg_len += snprintf((CPU_CHAR *)(tx_msg+tx_msg_len), 15, "%d Pa\n", (CPU_INT32U)bmp280_data.press);
    tx_msg_len += strlen(strcpy((CPU_CHAR *)(tx_msg+tx_msg_len), "TEMPERATURE RAW: "));
    tx_msg_len += snprintf((CPU_CHAR *)(tx_msg+tx_msg_len), 15, "%d DegC\n", (CPU_INT32S)bmp280_data.temp);
    tx_msg_len += strlen(strcpy((CPU_CHAR *)(tx_msg+tx_msg_len), "TEMPERATURE: "));
    tx_msg_len += snprintf((CPU_CHAR *)(tx_msg+tx_msg_len), 15, "%d.%2d DegC\n\n", (CPU_INT32S)temp, (CPU_INT32S)(100*temp)%100);
    
    uart_send_array(tx_msg, tx_msg_len);
    memset(&tx_msg[0],0,sizeof(tx_msg));
}

/* [] END OF FILE */
