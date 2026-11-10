# STM32F446 LQFP64 Pin Summary


| Pin number | Pin name (function after reset) | Pin type | I/O structure | Alternate functions | Additional functions |
| --- | --- | --- | --- | --- | --- |
| 1 | VBAT | S | - | - | - |
| 2 | PC13 | I/O | FT | EVENTOUT | TAMP_1/WKUP1 |
| 3 | PC14-<br>OSC32_IN(PC14) | I/O | FT | EVENTOUT | OSC32_IN |
| 4 | PC15-<br>OSC32_OUT(PC15) | I/O | FT | EVENTOUT | OSC32_OUT |
| 5 | PH0-OSC_IN(PH0) | I/O | FT | EVENTOUT | OSC_IN |
| 6 | PH1-<br>OSC_OUT(PH1) | I/O | FT | EVENTOUT | OSC_OUT |
| 7 | NRST | I/O | RST | - | - |
| 8 | PC0 | I/O | FT | SAI1_MCLK_B,<br>OTG_HS_ULPI_STP,<br>FMC_SDNWE,<br>EVENTOUT | ADC123_IN10 |
| 9 | PC1 | I/O | FT | SPI3_MOSI/I2S3_SD,<br>SAI1_SD_A,<br>SPI2_MOSI/I2S2_SD,<br>EVENTOUT | ADC123_IN11 |
| 10 | PC2 | I/O | FT | SPI2_MISO,<br>OTG_HS_ULPI_DIR,<br>FMC_SDNE0, EVENTOUT | ADC123_IN12 |
| 11 | PC3 | I/O | FT | SPI2_MOSI/I2S2_SD,<br>OTG_HS_ULPI_NXT,<br>FMC_SDCKE0,<br>EVENTOUT | ADC123_IN13 |
| 12 | VSSA | S | - | - | - |
| 13 | VDDA | S | - | - | - |
| 14 | PA0-WKUP(PA0) | I/O | FT | TIM2_CH1/TIM2_ETR,<br>TIM5_CH1, TIM8_ETR,<br>USART2_CTS,<br>UART4_TX, EVENTOUT | ADC123_IN0,<br>WKUP0/TAMP_2 |
| 15 | PA1 | I/O | FT | TIM2_CH2, TIM5_CH2,<br>USART2_RTS,<br>UART4_RX,<br>QUADSPI_BK1_IO3,<br>SAI2_MCLK_B,<br>EVENTOUT | ADC123_IN1 |
| 16 | PA2 | I/O | FT | TIM2_CH3, TIM5_CH3,<br>TIM9_CH1, USART2_TX,<br>SAI2_SCK_B, EVENTOUT | ADC123_IN2 |
| 17 | PA3 | I/O | FT | TIM2_CH4, TIM5_CH4,<br>TIM9_CH2, SAI1_FS_A,<br>USART2_RX,<br>OTG_HS_ULPI_D0,<br>EVENTOUT | ADC123_IN3 |
| 18 | VSS | S | - | - | - |
| 19 | VDD | S | - | - | - |
| 20 | PA4 | I/O | TTa | SPI1_NSS/I2S1_WS,<br>SPI3_NSS/I2S3_WS,<br>USART2_CK,<br>OTG_HS_SOF,<br>DCMI_HSYNC,<br>EVENTOUT | ADC12_IN4,<br>DAC_OUT1 |
| 21 | PA5 | I/O | TTa | TIM2_CH1/TIM2_ETR,<br>TIM8_CH1N,<br>SPI1_SCK/I2S1_CK,<br>OTG_HS_ULPI_CK,<br>EVENTOUT | ADC12_IN5,<br>DAC_OUT2 |
| 22 | PA6 | I/O | FT | TIM1_BKIN, TIM3_CH1,<br>TIM8_BKIN, SPI1_MISO,<br>I2S2_MCK, TIM13_CH1,<br>DCMI_PIXCLK,<br>EVENTOUT | ADC12_IN6 |
| 23 | PA7 | I/O | FT | TIM1_CH1N, TIM3_CH2,<br>TIM8_CH1N,<br>SPI1_MOSI/I2S1_SD,<br>TIM14_CH1,<br>FMC_SDNWE,<br>EVENTOUT | ADC12_IN7 |
| 24 | PC4 | I/O | FT | I2S1_MCK, SPDIFRX_IN2,<br>FMC_SDNE0, EVENTOUT | ADC12_IN14 |
| 25 | PC5 | I/O | FT | USART3_RX,<br>SPDIFRX_IN3,<br>FMC_SDCKE0,<br>EVENTOUT | ADC12_IN15 |
| 26 | PB0 | I/O | FT | TIM1_CH2N, TIM3_CH3,<br>TIM8_CH2N,<br>SPI3_MOSI/I2S3_SD,<br>UART4_CTS,<br>OTG_HS_ULPI_D1,<br>SDIO_D1, EVENTOUT | ADC12_IN8 |
| 27 | PB1 | I/O | FT | TIM1_CH3N, TIM3_CH4,<br>TIM8_CH3N,<br>OTG_HS_ULPI_D2,<br>SDIO_D2, EVENTOUT | ADC12_IN9 |
| 28 | PB2-BOOT1<br>(PB2) | I/O | FT | TIM2_CH4, SAI1_SD_A,<br>SPI3_MOSI/I2S3_SD,<br>QUADSPI_CLK,<br>OTG_HS_ULPI_D4,<br>SDIO_CK, EVENTOUT | - |
| 29 | PB10 | I/O | FT | TIM2_CH3, I2C2_SCL,<br>SPI2_SCK/I2S2_CK,<br>SAI1_SCK_A,<br>USART3_TX,<br>OTG_HS_ULPI_D3,<br>EVENTOUT | - |
| 30 | VCAP_1 | S | - | - | - |
| 31 | VSS | S | - | - | - |
| 32 | VDD | S | - | - | - |
| 33 | PB12 | I/O | FT | TIM1_BKIN, I2C2_SMBA,<br>SPI2_NSS/I2S2_WS,<br>SAI1_SCK_B,<br>USART3_CK, CAN2_RX,<br>OTG_HS_ULPI_D5,<br>OTG_HS_ID, EVENTOUT | - |
| 34 | PB13 | I/O | FT | TIM1_CH1N,<br>SPI2_SCK/I2S2_CK,<br>USART3_CTS, CAN2_TX,<br>OTG_HS_ULPI_D6,<br>EVENTOUT | OTG_HS_VBUS |
| 35 | PB14(1) | I/O | FT | TIM1_CH2N, TIM8_CH2N,<br>SPI2_MISO,<br>USART3_RTS,<br>TIM12_CH1,<br>OTG_HS_DM, EVENTOUT | - |
| 36 | PB15(1) | I/O | FT | RTC_REFIN, TIM1_CH3N,<br>TIM8_CH3N,<br>SPI2_MOSI/I2S2_SD,<br>TIM12_CH2, OTG_HS_DP,<br>EVENTOUT | - |
| 37 | PC6 | I/O | FTf | TIM3_CH1, TIM8_CH1,<br>FMPI2C1_SCL,<br>I2S2_MCK, USART6_TX,<br>SDIO_D6, DCMI_D0,<br>EVENTOUT | - |
| 38 | PC7 | I/O | FTf | TIM3_CH2, TIM8_CH2,<br>FMPI2C1_SDA,<br>SPI2_SCK/I2S2_CK,<br>I2S3_MCK, SPDIFRX_IN1,<br>USART6_RX, SDIO_D7,<br>DCMI_D1, EVENTOUT | - |
| 39 | PC8 | I/O | FT | TRACED0, TIM3_CH3,<br>TIM8_CH3, UART5_RTS,<br>USART6_CK, SDIO_D0,<br>DCMI_D2, EVENTOUT | - |
| 40 | PC9 | I/O | FT | MCO2, TIM3_CH4,<br>TIM8_CH4, I2C3_SDA,<br>I2S_CKIN, UART5_CTS,<br>QUADSPI_BK1_IO0,<br>SDIO_D1, DCMI_D3,<br>EVENTOUT | - |
| 41 | PA8 | I/O | FT | MCO1, TIM1_CH1,<br>I2C3_SCL, USART1_CK,<br>OTG_FS_SOF,<br>EVENTOUT | - |
| 42 | PA9 | I/O | FT | TIM1_CH2, I2C3_SMBA,<br>SPI2_SCK/I2S2_CK,<br>SAI1_SD_B, USART1_TX,<br>DCMI_D0, EVENTOUT | OTG_FS_VBUS |
| 43 | PA10 | I/O | FT | TIM1_CH3, USART1_RX,<br>OTG_FS_ID, DCMI_D1,<br>EVENTOUT | - |
| 44 | PA11(1) | I/O | FT | TIM1_CH4, USART1_CTS,<br>CAN1_RX, OTG_FS_DM,<br>EVENTOUT | - |
| 45 | PA12(1) | I/O | FT | TIM1_ETR, USART1_RTS,<br>SAI2_FS_B, CAN1_TX,<br>OTG_FS_DP, EVENTOUT | - |
| 46 | PA13(JTMS-SWDIO) | I/O | FT | JTMS-SWDIO,<br>EVENTOUT | - |
| 47 | VSS | S | - | - | - |
| 48 | VDD | S | - | - | - |
| 49 | PA14(JTCK-SWCLK) | I/O | FT | JTCK-SWCLK,<br>EVENTOUT | - |
| 50 | PA15(JTDI) | I/O | FT | JTDI,<br>TIM2_CH1/TIM2_ETR,<br>HDMI_CEC,<br>SPI1_NSS/I2S1_WS,<br>SPI3_NSS/I2S3_WS,<br>UART4_RTS, EVENTOUT | - |
| 51 | PC10 | I/O | FT | SPI3_SCK/I2S3_CK,<br>USART3_TX, UART4_TX,<br>QUADSPI_BK1_IO1,<br>SDIO_D2, DCMI_D8,<br>EVENTOUT | - |
| 52 | PC11 | I/O | FT | SPI3_MISO, USART3_RX,<br>UART4_RX,<br>QUADSPI_BK2_NCS,<br>SDIO_D3, DCMI_D4,<br>EVENTOUT | - |
| 53 | PC12 | I/O | FT | I2C2_SDA,<br>SPI3_MOSI/I2S3_SD,<br>USART3_CK, UART5_TX,<br>SDIO_CK, DCMI_D9,<br>EVENTOUT | - |
| 54 | PD2 | I/O | FT | TIM3_ETR, UART5_RX,<br>SDIO_CMD, DCMI_D11,<br>EVENTOUT | - |
| 55 | PB3(JTDO/TRACES<br>WO) | I/O | FT | JTDO/TRACESWO,<br>TIM2_CH2, I2C2_SDA,<br>SPI1_SCK/I2S1_CK,<br>SPI3_SCK/I2S3_CK,<br>EVENTOUT | - |
| 56 | PB4(NJTRST) | I/O | FT | NJTRST, TIM3_CH1,<br>I2C3_SDA, SPI1_MISO,<br>SPI3_MISO,<br>SPI2_NSS/I2S2_WS,<br>EVENTOUT | - |
| 57 | PB5 | I/O | FT | TIM3_CH2, I2C1_SMBA,<br>SPI1_MOSI/I2S1_SD,<br>SPI3_MOSI/I2S3_SD,<br>CAN2_RX,<br>OTG_HS_ULPI_D7,<br>FMC_SDCKE1,<br>DCMI_D10, EVENTOUT | - |
| 58 | PB6 | I/O | FT | TIM4_CH1, HDMI_CEC,<br>I2C1_SCL, USART1_TX,<br>CAN2_TX,<br>QUADSPI_BK1_NCS,<br>FMC_SDNE1, DCMI_D5,<br>EVENTOUT | - |
| 59 | PB7 | I/O | FT | TIM4_CH2, I2C1_SDA,<br>USART1_RX,<br>SPDIFRX_IN0, FMC_NL,<br>DCMI_VSYNC,<br>EVENTOUT | - |
| 60 | BOOT0 | I | B | - | VPP |
| 61 | PB8 | I/O | FT | TIM2_CH1/TIM2_ETR,<br>TIM4_CH3, TIM10_CH1,<br>I2C1_SCL, CAN1_RX,<br>SDIO_D4, DCMI_D6,<br>EVENTOUT | - |
| 62 | PB9 | I/O | FT | TIM2_CH2, TIM4_CH4,<br>TIM11_CH1, I2C1_SDA,<br>SPI2_NSS/I2S2_WS,<br>SAI1_FS_B, CAN1_TX,<br>SDIO_D5, DCMI_D7,<br>EVENTOUT | - |
| 63 | VSS | S | - | - | - |
| 64 | VDD | S | - | - | - |




# STM32F446xx register boundary addresses
| Boundary address | Peripheral | Bus | Register map |
|---|---|---|---|
| 0xA000 0000 - 0xA000 0FFF | FMC control register | AHB3 | Section 11.8.6: FMC register map on page 318 |
| 0xA000 1000 - 0xA000 1FFF | QUADSPI register | AHB3 | Section 12.5.14: QUADSPI register map on page 348 |
| 0x5005 0000 - 0x5005 03FF | DCMI | AHB2 | Section 15.5.12: DCMI register map on page 438 |
| 0x5000 0000 - 0x5003 FFFF | USB OTG FS | AHB2 | Section 31.15.65: OTG_FS/OTG_HS register map on page 1180 |
| 0x4004 0000 - 0x4007 FFFF | USB OTG HS | AHB1 | Section 31.15.65: OTG_FS/OTG_HS register map on page 1180 |
| 0x4002 6400 - 0x4002 67FF | DMA2 | AHB1 | Section 9.5.11: DMA register map on page 231 |
| 0x4002 6000 - 0x4002 63FF | DMA1 | AHB1 | Section 9.5.11: DMA register map on page 231 |
| 0x4002 4000 - 0x4002 4FFF | BKPSRAM | AHB1 | - |
| 0x4002 3C00 - 0x4002 3FFF | Flash interface register | AHB1 | Section 3.8: Flash interface registers on page 80 |
| 0x4002 3800 - 0x4002 3BFF | RCC | AHB1 | Section 6.3.28: RCC register map on page 170 |
| 0x4002 3000 - 0x4002 33FF | CRC | AHB1 | Section 4.4.4: CRC register map on page 91 |
| 0x4002 1C00 - 0x4002 1FFF | GPIOH | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 1800 - 0x4002 1BFF | GPIOG | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 1400 - 0x4002 17FF | GPIOF | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 1000 - 0x4002 13FF | GPIOE | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 0C00 - 0x4002 0FFF | GPIOD | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 0800 - 0x4002 0BFF | GPIOC | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 0400 - 0x4002 07FF | GPIOB | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4002 0000 - 0x4002 03FF | GPIOA | AHB1 | Section 7.4.11: GPIO register map on page 191 |
| 0x4001 5C00 - 0x4001 5FFF | SAI2 | APB2 | Section 28.5.18: SAI register map on page 959 |
| 0x4001 5800 - 0x4001 5BFF | SAI1 | APB2 | Section 28.5.18: SAI register map on page 959 |
| 0x4001 4800 - 0x4001 4BFF | TIM11 | APB2 | Section 18.5.12: TIM10/11/13/14 register map on page 616 |
| 0x4001 4400 - 0x4001 47FF | TIM10 | APB2 | Section 18.5.12: TIM10/11/13/14 register map on page 616 |
| 0x4001 4000 - 0x4001 43FF | TIM9 | APB2 | Section 18.5.12: TIM10/11/13/14 register map on page 616 |
| 0x4001 3C00 - 0x4001 3FFF | EXTI | APB2 | Section 10.3.7: EXTI register map on page 246 |
| 0x4001 3800 - 0x4001 3BFF | SYSCFG | APB2 | Section 8.2.9: SYSCFG register map on page 199 |
| 0x4001 3400 - 0x4001 37FF | SPI4 | APB2 | Section 26.7.10: SPI register map on page 874 |
| 0x4001 3000 - 0x4001 33FF | SPI1 | APB2 | Section 26.7.10: SPI register map on page 874 |
| 0x4001 2C00 - 0x4001 2FFF | SDMMC | APB2 | Section 29.8.16: SDIO register map on page 1016 |
| 0x4001 2000 - 0x4001 23FF | ADC1 - ADC2 - ADC3 | APB2 | Section 13.13.18: ADC register map on page 392 |
| 0x4001 1400 - 0x4001 17FF | USART6 | APB2 | Section 25.6.8: USART register map on page 824 |
| 0x4001 1000 - 0x4001 13FF | USART1 | APB2 | Section 25.6.8: USART register map on page 824 |
| 0x4001 0400 - 0x4001 07FF | TIM8 | APB2 | Section 16.4.21: TIM1&TIM8 register map on page 508 |
| 0x4001 0000 - 0x4001 03FF | TIM1 | APB2 | Section 16.4.21: TIM1&TIM8 register map on page 508 |
| 0x4000 7400 - 0x4000 77FF | DAC | APB1 | Section 14.5.15: DAC register map on page 416 |
| 0x4000 7000 - 0x4000 73FF | PWR | APB1 | Section 5.5: PWR register map on page 115 |
| 0x4000 6C00 - 0x4000 6FFF | HDMI-CEC | APB1 | Section 32.7.7: HDMI-CEC register map on page 1276 |
| 0x4000 6800 - 0x4000 6BFF | CAN2 | APB1 | Section 30.9.5: bxCAN register map on page 1058 |
| 0x4000 6400 - 0x4000 67FF | CAN1 | APB1 | Section 30.9.5: bxCAN register map on page 1058 |
| 0x4000 5C00 - 0x4000 5FFF | I2C3 | APB1 | Section 24.6.11: I2C register map on page 773 |
| 0x4000 5800 - 0x4000 5BFF | I2C2 | APB1 | Section 24.6.11: I2C register map on page 773 |
| 0x4000 5400 - 0x4000 57FF | I2C1 | APB1 | Section 24.6.11: I2C register map on page 773 |
| 0x4000 5000 - 0x4000 53FF | UART5 | APB1 | Section 25.6.8: USART register map on page 824 |
| 0x4000 4C00 - 0x4000 4FFF | UART4 | APB1 | Section 25.6.8: USART register map on page 824 |
| 0x4000 4800 - 0x4000 4BFF | USART3 | APB1 | Section 25.6.8: USART register map on page 824 |
| 0x4000 4400 - 0x4000 47FF | USART2 | APB1 | Section 25.6.8: USART register map on page 824 |
| 0x4000 4000 - 0x4000 43FF | SPDIF-RX | APB1 | Section 27.5.10: SPDIFRX interface register map on page 905 |
| 0x4000 3C00 - 0x4000 3FFF | SPI3 / I2S3 | APB1 | Section 26.7.10: SPI register map on page 874 |
| 0x4000 3800 - 0x4000 3BFF | SPI2 / I2S2 | APB1 | Section 26.7.10: SPI register map on page 874 |
| 0x4000 3000 - 0x4000 33FF | IWDG | APB1 | Section 20.4.5: IWDG register map on page 635 |
| 0x4000 2C00 - 0x4000 2FFF | WWDG | APB1 | Section 21.6.4: WWDG register map on page 642 |
| 0x4000 2800 - 0x4000 2BFF | RTC & BKP Registers | APB1 | Section 22.6.21: RTC register map on page 679 |
| 0x4000 2000 - 0x4000 23FF | TIM14 | APB1 | Section 18.5.12: TIM10/11/13/14 register map on page 616 |
| 0x4000 1C00 - 0x4000 1FFF | TIM13 | APB1 | Section 18.5.12: TIM10/11/13/14 register map on page 616 |
| 0x4000 1800 - 0x4000 1BFF | TIM12 | APB1 | Section 18.5.12: TIM10/11/13/14 register map on page 616 |
| 0x4000 1400 - 0x4000 17FF | TIM7 | APB1 | Section 19.4.9: TIM6&TIM7 register map on page 629 |
| 0x4000 1000 - 0x4000 13FF | TIM6 | APB1 | Section 19.4.9: TIM6&TIM7 register map on page 629 |
| 0x4000 0C00 - 0x4000 0FFF | TIM5 | APB1 | Section 17.4.21: TIMx register map on page 569 |
| 0x4000 0800 - 0x4000 0BFF | TIM4 | APB1 | Section 17.4.21: TIMx register map on page 569 |
| 0x4000 0400 - 0x4000 07FF | TIM3 | APB1 | Section 17.4.21: TIMx register map on page 569 |
| 0x4000 0000 - 0x4000 03FF | TIM2 | APB1 | Section 17.4.21: TIMx register map on page 569 |


