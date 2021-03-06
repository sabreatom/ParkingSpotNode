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

#include "MAG3110.h"

#include "low_power.h"

int main(void)
{
	uint64_t cur_time = 0;

	parking_data_struct parking_data = {0};
	MAG3110_mag_value_t MAG3110_mag_value = {0};

  /* Chip errata */
  CHIP_Init();

  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(UIF_LED0_PORT, UIF_LED0_PIN, gpioModePushPull, 1);

  Uptime_init();
  Debug_Init();

  //power up delay before configuring Lora module:
  cur_time = Uptime_getValue();
  while ((Uptime_getValue() - cur_time) < 5000);

  Lora_init();
  MAG3110_Init();

  LP_init();

  xprintf("[INFO] Initialization done!\n");

  /* Infinite loop */
  while (1) {
	  GPIO_PinOutSet(UIF_LED0_PORT, UIF_LED0_PIN);
	  //if ((Uptime_getValue() - cur_time) > 2000) {
		  xprintf("[INFO] Checking sensor.\n");
		  //if (MAG3110_checkNewMeasurement()){ //FIXME something wrong here, need to check using logic analyzer
		  	  xprintf("[INFO] Reading sensor value.\n");

			  MAG3110_mag_value = MAG3110_read();
			  parking_data.mag_x_val = MAG3110_mag_value.mag_x_val;
			  parking_data.mag_y_val = MAG3110_mag_value.mag_y_val;
			  parking_data.mag_z_val = MAG3110_mag_value.mag_z_val;

			  //check car presence:
			  if (MAG3110_isCarPresent(MAG3110_mag_value)){
				  parking_data.parked = 1;
			  }
			  else{
				  parking_data.parked = 0;
			  }

			  xprintf("[INFO] Sensor values - x: %d, y: %d, z: %d\n", MAG3110_mag_value.mag_x_val, MAG3110_mag_value.mag_y_val, MAG3110_mag_value.mag_z_val);

			  Lora_send_parking_data(parking_data);

			  //cur_time = Uptime_getValue();
		  //}
	  //}

	//power up delay before configuring Lora module:
	cur_time = Uptime_getValue();
	while ((Uptime_getValue() - cur_time) < 2000);

	GPIO_PinOutClear(UIF_LED0_PORT, UIF_LED0_PIN);
	LP_sleep();
  }
}
