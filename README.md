# ESW-BMP280

This CYPRESS PSoC Creator Project is designed with the CY8CKIT-059 PROTOTYPING KIT connected with the BMP280 temperature and pressure sensor over SPI.
THe measured data is transmitted over UART.

## SPI

The configuration of the SPI interface:

* MISO: P12_1
* MOSI: P12_2
* CLK : P12_3
* CS  : P12_4

## UART

Measured and calibrated pressure and temperature is transmitted over UART with a BAUD = 115200, DATA BITS = 8, Parity = None, STOP BIT = 1 in the
following format: "P:value-T:value"
