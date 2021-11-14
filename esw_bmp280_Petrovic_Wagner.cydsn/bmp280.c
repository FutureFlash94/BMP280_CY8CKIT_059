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

/*
*********************************************************************************************************
*                                     bmp280_spi_read()
*
* Description : This function waits for the spi interface to be in status done 
*               or idle and then sends the spi register address over the spi 
*               interface and returns first received byte from the slave. 
*               Regarding the datasheet of bmp280 bit7 of the register address 
*               is set to 1.
*
* Argument(s) : none.
*
* Return(s)   : First received byte from bmp280
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_INT08U bmp280_spi_read(CPU_INT08U spi_reg_addr)
{
  /* clear spi buffer and wait for spi to be ready for sending */
  prepare_and_wait_spi_send();
  /* Send register address (set bit7 (R/W) to 1) and a second byte to be able to 
     receive the real data byte of the register address, otherwise SPIM_1 set 
     CS_1 to 1 after sending the first byte and the desired byte never comes.
  */
  spi_send_n_bytes(spi_reg_addr | (1<<7), 2);
  /* wait to send data over spi */
  wait_spi_tx();
  /* return received data byte */
  return spi_get_byte();
}

/*
*********************************************************************************************************
*                                   bmp280_spi_read_multiple()
*
* Description : This function waits for the spi interface to be in status done 
*               or idle and then sends the spi register address over the spi 
*               interface. Finally it writes read_size bytes from the spi rx 
*               buffer to the reg_values array. Regarding the datasheet of 
*               bmp280 bit7 of the register address is set to 1.
*
* Argument(s) : spi_reg_addr ... first byte that is written to spi tx buffer 
*               reg_values ... array where data received over spi is written
*               read_size ... amount of bytes to be read from spi rx buffer
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID bmp280_spi_read_multiple(CPU_INT08U spi_reg_addr, CPU_INT08U reg_values[], CPU_INT08U read_size)
{
  /* clear spi buffer and wait for spi to be ready for sending */
  prepare_and_wait_spi_send();
  /* Send register address (set bit7 (R/W) to 1) and read_size more bytes */
  spi_send_n_bytes(spi_reg_addr | (1<<7), read_size+1);
  /* wait to send data over spi */
  wait_spi_tx();
  /* write read_size received data bytes to reg_values array */
  spi_get_n_bytes(reg_values, read_size);
}

/*
*********************************************************************************************************
*                                       bmp280_spi_write()
*
* Description : This function send one register address byte and the corresponding data byte over spi.
*
* Argument(s) : spi_reg_addr ... address byte
*               spi_reg_data ... data byte
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID bmp280_spi_write(CPU_INT08U spi_reg_addr, CPU_INT08U spi_reg_data)
{
  spi_send_n_bytes(spi_reg_addr & ~(1<<7), 1);
  spi_send_n_bytes(spi_reg_data, 1);
}

/*
*********************************************************************************************************
*                                   set_bmp280_config()
*
* Description : This function configure the bmp280 by setting data in the config and control measurements
*               register.
*
* Argument(s) : osrt_t ... oversampling rate for temperature measurement
*               osrt_p ... oversampling rate for pressure measurement
*               power_mode ... power mode of the bmp280
*               t_standby ... standby time between measurements
*               filter ... filter settings
*               spi3w_en ... 3-wire-spi configuration enable / disable 
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID set_bmp280_config(CPU_INT08U osrt_t, CPU_INT08U osrt_p, 
                           CPU_INT08U power_mode, CPU_INT08U t_standby, 
                           CPU_INT08U filter, CPU_INT08U spi3w_en)
{
  /* clear spi buffer and wait for spi to be ready for sending */
  prepare_and_wait_spi_send();
  /* Go to sleep mode to be able to set data in config register */
  bmp280_spi_write(
    BMP280_REG_CTRL_MEAS,
    (osrt_t<<BMP280_OSRT_T_POS)|(osrt_p<<BMP280_OSRT_P_POS)|(BMP280_POWER_MODE_SLEEP<<BMP280_POWER_MODE_POS)
  );
  /* wait to send data over spi */
  wait_spi_tx();
  /* clear spi buffer and wait for spi to be ready for sending */
  prepare_and_wait_spi_send();
  /* write data to the config register */
  bmp280_spi_write(
    BMP280_REG_CONFIG, 
    (t_standby<<BMP280_T_STANDBY_POS)|(filter<<BMP280_FILTER_POS)|(spi3w_en<<BMP280_SPI3W_EN_POS)
  );
  /* wait to send data over spi */
  wait_spi_tx();
  /* clear spi buffer and wait for spi to be ready for sending */
  prepare_and_wait_spi_send();
  /* write data to the control measurements register register */
  bmp280_spi_write(
    BMP280_REG_CTRL_MEAS, 
    (osrt_t<<BMP280_OSRT_T_POS)|(osrt_p<<BMP280_OSRT_P_POS)|(power_mode<<BMP280_POWER_MODE_POS)
  );
  /* wait to send data over spi */
  wait_spi_tx();
}

/*
*********************************************************************************************************
*                                       wait_bmp280_connecting()
*
* Description : This function waits until the register id of the bmp280 will be returned over spi after 
*               sending the register id address. Consider that this is a blocking function.
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

CPU_VOID wait_bmp280_connecting(CPU_VOID)
{
  CPU_INT08U reg_value = 0u;
  
  do {
    /* clear spi buffer and wait for spi to be ready for sending */
    prepare_and_wait_spi_send();
    spi_send_n_bytes(BMP280_REG_ID | (1<<7), 2);
    /* wait to send data over spi */
    wait_spi_tx();
    if(SPIM_1_GetRxBufferSize() != 0) 
    {
      SPIM_1_ReadRxData(); // No valid data
      reg_value = SPIM_1_ReadRxData();
    }
  } while(reg_value != BMP280_CHIP_ID);
}

/*
*********************************************************************************************************
*                                       init_spi()
*
* Description : This function returns the first received byte from spi after sending the register id 
*               address.
*
* Argument(s) : none.
*
* Return(s)   : First receiver byte from spi.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_INT08U get_bmp280_chip_id(CPU_VOID)
{
  return bmp280_spi_read(BMP280_REG_ID);
}

/*
*********************************************************************************************************
*                                       read_bmp280_press_temp()
*
* Description : This function writes the pressure MSB register address and waits till all measurement 
*               values are received. These values are then written to the given reg_values array. The
*               given array should have at least a size of BMP280_PRESS_TEMP_DATA_SIZE.
*
* Argument(s) : reg_values[] ... array of bytes, where received bytes are written to
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

CPU_VOID read_bmp280_press_temp(CPU_INT08U reg_values[])
{
  bmp280_spi_read_multiple(BMP280_REG_PRESS_MSB, reg_values, BMP280_PRESS_TEMP_DATA_SIZE);
}

/*
*********************************************************************************************************
*                                       read_bmp280_calib()
*
* Description : This function writes the calibrate LSB register address and waits till all calibrate 
*               values are received. These values are then written to the given reg_values array. The
*               given array should have at least a size of BMP280_CALIB_DATA_SIZE.
*
* Argument(s) : reg_values[] ... array of bytes, where received bytes are written to
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Not re-entrant.
*********************************************************************************************************
*/

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
  
  /* Use calibration data to compute temperature */
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
  
  /* Use calibration data to compute press */
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
