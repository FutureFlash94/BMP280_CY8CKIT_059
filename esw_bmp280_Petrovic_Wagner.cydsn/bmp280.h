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
#define  BMP280_REG_CTRL_MEAS                       0xF4
#define  BMP280_REG_CONFIG                          0xF5
#define  BMP280_REG_PRESS_MSB                       0xF7

#define  BMP280_CHIP_ID                             0x58
#define  BMP280_BURST_READ_SIZE                     6u

CPU_VOID bmp280_config(CPU_INT08U ctrl_meas_value, CPU_INT08U config_value);
CPU_INT08U get_chip_id(CPU_VOID);
CPU_VOID burst_read(CPU_INT08U reg_values[], CPU_INT08U byteCount);

/* [] END OF FILE */
