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

CPU_VOID set_bmp280_config(CPU_INT08U osrt_t, CPU_INT08U osrt_p, CPU_INT08U power_mode, CPU_INT08U t_standby, CPU_INT08U filter, CPU_INT08U spi3w_en)
{
  // Go to sleep mode to override config register
  prepare_and_wait_spi_send();
  bmp280_spi_write(
    BMP280_REG_CTRL_MEAS, 
    (osrt_t<<BMP280_OSRT_T_POS)+(osrt_p<<BMP280_OSRT_P_POS)+(BMP280_POWER_MODE_SLEEP<<BMP280_POWER_MODE_POS)
  );
  wait_spi_tx();
  // set user defined config register values
  prepare_and_wait_spi_send();
  bmp280_spi_write(
    BMP280_REG_CONFIG, 
    (t_standby<<BMP280_T_STANDBY_POS)+(filter<<BMP280_FILTER_POS)+(spi3w_en<<BMP280_SPI3W_EN_POS)
  );
  wait_spi_tx();
  // set user defined control measurements register values
  prepare_and_wait_spi_send();
  bmp280_spi_write(
    BMP280_REG_CTRL_MEAS, 
    (osrt_t<<BMP280_OSRT_T_POS)+(osrt_p<<BMP280_OSRT_P_POS)+(power_mode<<BMP280_POWER_MODE_POS)
  );
  wait_spi_tx();
}

CPU_VOID wait_bmp280_connecting(CPU_VOID)
{
  CPU_INT08U reg_value = 0u;
  
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

CPU_INT08U get_bmp280_chip_id(CPU_VOID)
{
  return bmp280_spi_read(BMP280_REG_ID);
}

CPU_VOID read_bmp280_press_temp(CPU_INT08U reg_values[])
{
  bmp280_spi_read_multiple(BMP280_REG_PRESS_MSB, reg_values, BMP280_PRESS_TEMP_DATA_SIZE);
}

CPU_VOID read_bmp280_calib(CPU_INT08U reg_values[])
{
  bmp280_spi_read_multiple(BMP280_REG_CALIB_LSB, reg_values, BMP280_CALIB_DATA_SIZE);
}

/*
*********************************************************************************************************
*                                     get_bmp280_press_temp()
*
* Description : Reads sensor and calibration data and compute the calibrated sensor data.
*               The temperature is in DegC, resolution is 0.01 DegC, so an output "5123"
*               equal 51.23 DegC. 
*               The pressure is in Pa, so an output "100838" equals 100838 Pa.
*
* Argument(s) : none.
*
* Return(s)   : calibrated temperature and pressure.
*
* Note(s)     : none
*********************************************************************************************************
*/
Bmp280_press_temp get_bmp280_press_temp()
{
  CPU_INT08U  reg_press_temp[BMP280_PRESS_TEMP_DATA_SIZE] = {0};
  CPU_INT08U  reg_calib[BMP280_CALIB_DATA_SIZE] = {0};
  
  CPU_INT32U  adc_T;
  CPU_INT16U  dig_T1;
  CPU_INT16S  dig_T2;
  CPU_INT16S  dig_T3;
  CPU_INT32S  var1,var2, t_fine;
  
  CPU_INT32U  adc_P;
  CPU_INT16U  dig_P1;
  CPU_INT16S  dig_P2;
  CPU_INT16S  dig_P3;
  CPU_INT16S  dig_P4;
  CPU_INT16S  dig_P5;
  CPU_INT16S  dig_P6;
  CPU_INT16S  dig_P7;
  CPU_INT16S  dig_P8;
  CPU_INT16S  dig_P9;
  
  Bmp280_press_temp return_data;
  
  read_bmp280_press_temp(reg_press_temp);
  read_bmp280_calib(reg_calib);
  
  // Use calibration data to compute temperature
  adc_T = ((CPU_INT32S)reg_press_temp[3] << 12) 
        | ((CPU_INT32S)reg_press_temp[4] << 4) 
        | ((CPU_INT32S)reg_press_temp[5] >> 4);
  dig_T1 = reg_calib[0] + (reg_calib[1]<<8);
  dig_T2 = reg_calib[2] + (reg_calib[3]<<8);
  dig_T3 = reg_calib[4] + (reg_calib[5]<<8);
  
  var1 = ((((adc_T / 8) - ((CPU_INT32S) dig_T1 * 2))) * ((CPU_INT32S) dig_T2)) / 2048;
  var2 = (((((adc_T / 16) - ((CPU_INT32S) dig_T1)) * ((adc_T / 16) - ((CPU_INT32S) dig_T1))) / 4096) * ((CPU_INT32S) dig_T3)) / 16384;

  t_fine = var1 + var2;
  return_data.temp = (t_fine * 5 + 128) / 256;
  
  // Use calibration data to compute press
  adc_P = ((CPU_INT32U)reg_press_temp[0] << 12) 
        | ((CPU_INT32U)reg_press_temp[1] << 4) 
        | ((CPU_INT32U)reg_press_temp[2] >> 4);
  dig_P1 =  reg_calib[6] + (reg_calib[7] << 8);
  dig_P2 =  reg_calib[8] + (reg_calib[9] << 8);
  dig_P3 = reg_calib[10] + (reg_calib[11] << 8);
  dig_P4 = reg_calib[12] + (reg_calib[13] << 8);
  dig_P5 = reg_calib[14] + (reg_calib[15] << 8);
  dig_P6 = reg_calib[16] + (reg_calib[17] << 8);
  dig_P7 = reg_calib[18] + (reg_calib[19] << 8);
  dig_P8 = reg_calib[20] + (reg_calib[21] << 8);
  dig_P9 = reg_calib[22] + (reg_calib[23] << 8);
  
  var1 = (((CPU_INT32S) t_fine) / 2) - (CPU_INT32S) 64000;
  var2 = (((var1 / 4) * (var1 / 4)) / 2048) * ((CPU_INT32S) dig_P6);
  var2 = var2 + ((var1 * ((CPU_INT32S) dig_P5)) * 2);
  var2 = (var2 / 4) + (((CPU_INT32S) dig_P4) * 65536);
  var1 = (((dig_P3 * (((var1 / 4) * (var1 / 4)) / 8192)) / 8) + ((((CPU_INT32S) dig_P2) * var1) / 2)) / 262144;
  var1 = ((((32768 + var1)) * ((CPU_INT32S) dig_P1)) / 32768);

  /* Avoid exception caused by division with zero */
  if (var1 != 0)
  {
      return_data.press = (((CPU_INT32U)(((CPU_INT32S)1048576) - adc_P) - (var2 / 4096))) * 3125;
      
      /* Check for overflows against UINT32_MAX/2; if pressure is left-shifted by 1 */
      if (return_data.press < 0x80000000)
      {
          return_data.press = (return_data.press * 2) / ((CPU_INT32U) var1);
      }
      else
      {
          return_data.press = (return_data.press / (CPU_INT32U) var1) * 2;
      }
      var1 = (((CPU_INT32S) dig_P9) * ((CPU_INT32S) (((return_data.press / 8) * (return_data.press / 8)) / 8192))) / 4096;
      var2 = (((CPU_INT32S) (return_data.press / 4)) * ((CPU_INT32S) dig_P8)) / 8192;
      return_data.press = (CPU_INT32U) ((CPU_INT32S) return_data.press + ((var1 + var2 + dig_P7) / 16));
  }
  
  return return_data;
}

/* [] END OF FILE */
