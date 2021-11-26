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
#define  BMP280_CHIP_ID                             0x58

#define  BMP280_REG_PRESS_MSB                       0xF7
#define  BMP280_PRESS_TEMP_DATA_SIZE                6u

#define  BMP280_REG_CALIB_LSB                       0x88
#define  BMP280_CALIB_DATA_SIZE                     24u

#define  BMP280_REG_CTRL_MEAS                       0xF4
#define  BMP280_REG_CONFIG                          0xF5

// REGISTER "CONFIG" bit[5:7]
#define  BMP280_T_STANDBY_0_5MS                     0x00
#define  BMP280_T_STANDBY_62_5MS                    0x01
#define  BMP280_T_STANDBY_125MS                     0x02
#define  BMP280_T_STANDBY_250MS                     0x03
#define  BMP280_T_STANDBY_500MS                     0x04
#define  BMP280_T_STANDBY_1000MS                    0x05
#define  BMP280_T_STANDBY_2000MS                    0x06
#define  BMP280_T_STANDBY_4000MS                    0x07
#define  BMP280_T_STANDBY_POS                       5u

// REGISTER "CONFIG" bit[2:4]
#define  BMP280_FILTER_OFF                          0x00
#define  BMP280_FILTER_COEFF_2                      0x01
#define  BMP280_FILTER_COEFF_4                      0x02
#define  BMP280_FILTER_COEFF_8                      0x03
#define  BMP280_FILTER_COEFF_16                     0x04
#define  BMP280_FILTER_POS                          2u

// REGISTER "CONFIG" bit[0]
#define  BMP280_SPI3_WIRE_DISENABLE                 0x00
#define  BMP280_SPI3_WIRE_ENABLE                    0x01
#define  BMP280_SPI3W_EN_POS                        0u

// REGISTER "CTRL MEAS" osrs_t[5:7]
#define  BMP280_OSRT_T_SKIPPED                      0x00
#define  BMP280_OSRT_T_X1                           0x01
#define  BMP280_OSRT_T_X2                           0x02
#define  BMP280_OSRT_T_X4                           0x03
#define  BMP280_OSRT_T_X8                           0x04
#define  BMP280_OSRT_T_X16                          0x05
#define  BMP280_OSRT_T_POS                          5u

// REGISTER "CTRL MEAS" osrs_p[2:4]
#define  BMP280_OSRT_P_SKIPPED                      0x00
#define  BMP280_OSRT_P_X1                           0x01
#define  BMP280_OSRT_P_X2                           0x02
#define  BMP280_OSRT_P_X4                           0x03
#define  BMP280_OSRT_P_X8                           0x04
#define  BMP280_OSRT_P_X16                          0x05
#define  BMP280_OSRT_P_POS                          2u

// REGISTER "CTRL MEAS" mode[0:1]
#define  BMP280_POWER_MODE_SLEEP                    0x00
#define  BMP280_POWER_MODE_FORCED                   0x01
#define  BMP280_POWER_MODE_NORMAL                   0x03
#define  BMP280_POWER_MODE_POS                      0u

#ifndef BMP280_H
#define BMP280_H

typedef struct s_Bmp280_press_temp {
  CPU_INT32S temp;
  CPU_INT32U press;
} Bmp280_press_temp;

#endif

CPU_INT08U bmp280_spi_read(CPU_INT08U spi_reg_addr);
CPU_VOID bmp280_spi_read_multiple(CPU_INT08U spi_reg_addr, CPU_INT08U reg_values[], CPU_INT08U read_size);
CPU_VOID bmp280_spi_write(CPU_INT08U spi_reg_addr, CPU_INT08U spi_reg_data);
CPU_VOID set_bmp280_config(CPU_INT08U osrt_t, CPU_INT08U osrt_p, CPU_INT08U power_mode, CPU_INT08U t_standby, CPU_INT08U filter, CPU_INT08U spi3w_en);
CPU_VOID wait_bmp280_connecting(CPU_VOID);
CPU_INT08U get_bmp280_chip_id(CPU_VOID);
CPU_VOID read_bmp280_press_temp(CPU_INT08U reg_values[]);
CPU_VOID read_bmp280_calib(CPU_INT08U reg_values[]);
Bmp280_press_temp get_bmp280_press_temp();

/* [] END OF FILE */
