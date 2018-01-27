#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#include "em_device.h"
//#include "hal-config-types.h"

// This file is auto-generated by Hardware Configurator in Simplicity Studio.
// Any content between $[ and ]$ will be replaced whenever the file is regenerated.
// Content outside these regions will be preserved.

// $[ACMP0]
// [ACMP0]$

// $[ADC0]
// [ADC0]$

// $[BTL_BUTTON]
// [BTL_BUTTON]$

// $[BUTTON]
// [BUTTON]$

// $[CMU]
// [CMU]$

// $[EMU]
// [EMU]$

// $[EXTFLASH]
// [EXTFLASH]$

// $[GPIO]
#define PORTIO_GPIO_SWCLK_PIN         (0)
#define PORTIO_GPIO_SWCLK_PORT        (gpioPortF)
#define PORTIO_GPIO_DBGROUTE_LOC      (0)

#define PORTIO_GPIO_SWDIO_PIN         (1)
#define PORTIO_GPIO_SWDIO_PORT        (gpioPortF)

// [GPIO]$

// $[HDLC]
// [HDLC]$

// $[I2C0]
#define PORTIO_I2C0_SCL_PIN           (1)
#define PORTIO_I2C0_SCL_PORT          (gpioPortC)
#define PORTIO_I2C0_ROUTE_LOC         (4)

#define PORTIO_I2C0_SDA_PIN           (0)
#define PORTIO_I2C0_SDA_PORT          (gpioPortC)

// [I2C0]$

// $[I2CSENSOR]
// [I2CSENSOR]$

// $[IDAC0]
// [IDAC0]$

// $[IOEXP]
// [IOEXP]$

// $[LED]
// [LED]$

// $[LEUART0]
#define PORTIO_LEUART0_RX_PIN         (5)
#define PORTIO_LEUART0_RX_PORT        (gpioPortD)
#define PORTIO_LEUART0_ROUTE_LOC      (0)

#define PORTIO_LEUART0_TX_PIN         (4)
#define PORTIO_LEUART0_TX_PORT        (gpioPortD)

// [LEUART0]$

// $[LFXO]
// [LFXO]$

// $[MSC]
// [MSC]$

// $[PCNT0]
// [PCNT0]$

// $[PORTIO]
// [PORTIO]$

// $[PRS]
// [PRS]$

// $[SERIAL]
// [SERIAL]$

// $[SPIDISPLAY]
// [SPIDISPLAY]$

// $[SPINCP]
// [SPINCP]$

// $[TIMER0]
#define PORTIO_TIMER0_CC0_PIN         (0)
#define PORTIO_TIMER0_CC0_PORT        (gpioPortA)
#define PORTIO_TIMER0_ROUTE_LOC       (0)

// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[UARTNCP]
// [UARTNCP]$

// $[USART1]
#define PORTIO_USART1_RX_PIN          (6)
#define PORTIO_USART1_RX_PORT         (gpioPortD)
#define PORTIO_USART1_ROUTE_LOC       (2)

#define PORTIO_USART1_TX_PIN          (7)
#define PORTIO_USART1_TX_PORT         (gpioPortD)

// [USART1]$

// $[VCOM]
// [VCOM]$

// $[WDOG]
// [WDOG]$

#endif /* HAL_CONFIG_H */

// $[Custom pin names]
#define PWR_GOOD_PIN                  (1)
#define PWR_GOOD_PORT                 (gpioPortA)

#define BOOST_CNTRL_PIN               (2)
#define BOOST_CNTRL_PORT              (gpioPortA)

#define EFM_DISP_SELECT_PIN           (8)
#define EFM_DISP_SELECT_PORT          (gpioPortA)

#define VOUT_CFG_3_PIN                (7)
#define VOUT_CFG_3_PORT               (gpioPortB)

#define VOUT_CFG_0_PIN                (2)
#define VOUT_CFG_0_PORT               (gpioPortC)

#define VOUT_CFG_1_PIN                (3)
#define VOUT_CFG_1_PORT               (gpioPortC)

#define VOUT_CFG_2_PIN                (4)
#define VOUT_CFG_2_PORT               (gpioPortC)

#define UIF_LED0_PIN                  (10)
#define UIF_LED0_PORT                 (gpioPortC)

#define UIF_LED1_PIN                  (11)
#define UIF_LED1_PORT                 (gpioPortC)

// [Custom pin names]$

