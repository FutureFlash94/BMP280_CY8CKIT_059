# BMP280_CY8CKIT_059

This CYPRESS PSoC Creator Project is designed with the [CY8CKIT-059 PROTOTYPING KIT](https://www.cypress.com/documentation/development-kitsboards/cy8ckit-059-psoc-5lp-prototyping-kit-onboard-programmer-and) connected with the BMP280 temperature and pressure sensor over SPI. The measured data is transmitted over UART. For a well looking representation of the measured data, [BMP280-Viewer](https://github.com/FutureFlash94/BMP280-Viewer) is recommended.

## SPI

The configuration of the SPI interface:

* MISO: P12_1
* MOSI: P12_2
* CLK : P12_3
* CS  : P12_4

## UART

Measured and calibrated pressure and temperature is transmitted over UART with

* BAUD = 115200
* DATA BITS = 8
* Parity = None
* STOP BIT = 1 

in the following format

* "P:value-T:value"

where P stands for pressure followed by the preasure value and T for temperature followed by the temperture value.
