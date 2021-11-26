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

#include <SPIM_1.h>

CPU_VOID init_spi(CPU_VOID);
CPU_VOID prepare_and_wait_spi_send(CPU_VOID);
CPU_VOID spi_send_n_bytes(CPU_INT08U byte, CPU_INT08U n);
CPU_VOID wait_spi_tx(CPU_VOID);
CPU_INT08U spi_get_byte(CPU_VOID);
CPU_VOID spi_get_n_bytes(CPU_INT08U reg_values[], CPU_INT08U n);

/* [] END OF FILE */