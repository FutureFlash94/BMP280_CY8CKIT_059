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

CPU_INT08U bmp280_spi_read(CPU_INT08U spi_reg_addr)
{
  prepare_and_wait_spi_send();
  spi_send_n_bytes(spi_reg_addr | (1<<7), 2);
  wait_spi_tx();
  return spi_get_byte();
}

CPU_VOID bmp280_spi_read_multiple(CPU_INT08U spi_reg_addr, CPU_INT08U reg_values[], CPU_INT08U read_size)
{
  prepare_and_wait_spi_send();
  spi_send_n_bytes(spi_reg_addr | (1<<7), read_size+1);
  wait_spi_tx();
  spi_get_n_bytes(reg_values, read_size);
}

CPU_VOID bmp280_spi_write(CPU_INT08U spi_reg_addr, CPU_INT08U spi_reg_data)
{
  spi_send_n_bytes(spi_reg_addr & ~(1<<7), 1);
  spi_send_n_bytes(spi_reg_data, 1);
}

CPU_VOID set_bmp280_config(CPU_INT08U ctrl_meas_value, CPU_INT08U config_value) {
  prepare_and_wait_spi_send();
  bmp280_spi_write(BMP280_REG_CTRL_MEAS, ctrl_meas_value);
  bmp280_spi_write(BMP280_REG_CONFIG, config_value);
  wait_spi_tx();
}

CPU_VOID wait_bmp280_connecting(CPU_VOID) {
  CPU_INT08U reg_value = 0u;
  CPU_INT32U temp_ctr = 0u;
  
  do {
    prepare_and_wait_spi_send();
    spi_send_n_bytes(BMP280_REG_ID | (1<<7), 2);
    wait_spi_tx();
    if(SPIM_1_GetRxBufferSize() != 0) 
    {
      SPIM_1_ReadRxData(); // No valid data
      reg_value = SPIM_1_ReadRxData();
    }
  } while(reg_value != BMP280_CHIP_ID);
}

CPU_INT08U get_bmp280_chip_id(CPU_VOID) {
  return bmp280_spi_read(BMP280_REG_ID);
}

CPU_VOID get_bmp280_press_temp(CPU_INT08U reg_values[]) {
  bmp280_spi_read_multiple(BMP280_REG_PRESS_MSB, reg_values, BMP280_PRESS_TEMP_DATA_SIZE);
}

/* [] END OF FILE */
