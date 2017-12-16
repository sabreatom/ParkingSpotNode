/*
 * fifo.h
 *
 *  Created on: 2017. gada 20. nov.
 *      Author: Aleksandrs
 */

#ifndef SRC_FIFO_H_
#define SRC_FIFO_H_

//FIFO structure:
typedef struct {
	unsigned int* buf;
	unsigned int wr_ptr;
	unsigned int rd_ptr;
	unsigned int buf_size;
} FIFO_t;

void FIFO_init(FIFO_t* fifo_ptr, unsigned int* buf_ptr, unsigned int buf_size);
unsigned int FIFO_get_free_space(FIFO_t* fifo_ptr);
unsigned int FIFO_wr_single_element(FIFO_t* fifo_ptr, unsigned int data);
unsigned int FIFO_rd_single_element(FIFO_t* fifo_ptr, unsigned int* data);
void FIFO_flush(FIFO_t* fifo_ptr);
unsigned int FIFO_isEmpty(FIFO_t* fifo_ptr);

#endif /* SRC_FIFO_H_ */
