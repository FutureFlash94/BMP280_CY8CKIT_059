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

CPU_INT08U get_chip_id(CPU_VOID) {
  spi_send_two_bytes(BMP280_REG_ID, BMP280_REG_ID+1);
  wait_spi_tx();
  return spi_get_byte();
}

/* [] END OF FILE */
