#include "em_device.h"
#include "em_chip.h"

#include "hal-config.h"

#include "em_gpio.h"
#include "em_cmu.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(UIF_LED0_PORT, UIF_LED0_PIN, gpioModePushPull, 1);
  GPIO_PinOutSet(UIF_LED0_PORT, UIF_LED0_PIN);

  /* Infinite loop */
  while (1) {
  }
}
