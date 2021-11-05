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
#include <bsp_spi.h>

#define  BMP280_REG_ID                              0xD0
#define  BMP280_REG_PRESS_MSB                       0xF7

#define  BMP280_CHIP_ID                             0x58

CPU_INT08U get_chip_id(CPU_VOID);

/* [] END OF FILE */
