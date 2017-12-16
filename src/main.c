#include "em_device.h"
#include "em_chip.h"

#include "hal-config.h"

#include "em_gpio.h"
#include "em_cmu.h"

#include "xprintf.h"

#include "Lora.h"
#include "debug_serial.h"
#include "uptime.h"

#include "tps62740_psu.h"

int main(void)
{
	uint64_t cur_time = 0;

	parking_data_struct parking_data = {0};
	parking_data.mag_x_val = 100;
	parking_data.mag_y_val = 115;
	parking_data.mag_z_val = 1000;
	parking_data.parked = 1;

  /* Chip errata */
  CHIP_Init();

  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(UIF_LED0_PORT, UIF_LED0_PIN, gpioModePushPull, 1);
  GPIO_PinOutSet(UIF_LED0_PORT, UIF_LED0_PIN);

  Uptime_init();
  Debug_Init();

  //power up delay before configuring Lora module:
  cur_time = Uptime_getValue();
  while ((Uptime_getValue() - cur_time) < 5000);

  Lora_init();

  xprintf("[INFO] Initialization done!");

  /* Infinite loop */
  while (1) {
	  if ((Uptime_getValue() - cur_time) > 1000) {
		  Lora_send_parking_data(parking_data);
		  cur_time = Uptime_getValue();
	  }
  }
}
