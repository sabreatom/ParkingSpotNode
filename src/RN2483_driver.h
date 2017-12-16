/*
 * RN2483_driver.h
 *
 *  Created on: 2017. gada 18. nov.
 *      Author: Aleksandrs
 */

#ifndef SRC_RN2483_DRIVER_H_
#define SRC_RN2483_DRIVER_H_

//RN2483 command responses:
typedef enum {
	RN2483_CMD_OK = 0,
	RN2483_CMD_FAIL
} RN2483_cmd_response_t;

void RN2483_Init();
unsigned char RN2483_chr_in(); //temporary
RN2483_cmd_response_t RN2483_send_cmd(const char* cmd);
void RN2483_send_data(uint8_t* data, uint8_t num_bytes);
void RN2483_flush_rx_fifo();
uint8_t RN2483_check_rx_fifo();

#endif /* SRC_RN2483_DRIVER_H_ */
