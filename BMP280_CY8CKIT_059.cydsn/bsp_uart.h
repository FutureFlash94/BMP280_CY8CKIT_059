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

#include <cpu.h>
#include <stdio.h>

#include <UART_1.h>
#include <bmp280.h>

CPU_VOID init_uart(CPU_VOID);
CPU_INT08U uart_get_byte(CPU_VOID);
CPU_VOID uart_send_byte(CPU_INT08U byte);
CPU_VOID uart_send_string(CPU_CHAR array[]);
CPU_VOID uart_send_array(CPU_INT08U array[], CPU_INT08U byteCount);
CPU_VOID uart_send_press_temp(Bmp280_press_temp bmp280_data);

/* [] END OF FILE */
