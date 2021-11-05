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

#include <bmp280.h>

CPU_VOID bmp280_config(CPU_INT08U ctrl_meas_value, CPU_INT08U config_value) {
  spi_send_four_bytes(BMP280_REG_CTRL_MEAS&~(1<<7), ctrl_meas_value, BMP280_REG_CONFIG&~(1<<7), config_value);
  wait_spi_tx();
}

CPU_INT08U get_chip_id(CPU_VOID) {
  spi_send_n_bytes(BMP280_REG_ID, 2);
  wait_spi_tx();
  return spi_get_byte();
}

CPU_VOID burst_read(CPU_INT08U reg_values[], CPU_INT08U byteCount) {
  spi_send_n_bytes(BMP280_REG_PRESS_MSB, byteCount+1);
  wait_spi_tx();
  spi_get_n_bytes(reg_values, byteCount);
}

/* [] END OF FILE */
