/*
 * fifo.c
 *
 *  Created on: 2017. gada 20. nov.
 *      Author: Aleksandrs
 */

#include "fifo.h"
#include "stdint.h"

//------------------------------------
//Initialize FIFO:
//------------------------------------

void FIFO_init(FIFO_t* fifo_ptr, unsigned int* buf_ptr, unsigned int buf_size)
{
	fifo_ptr->buf = buf_ptr;
	fifo_ptr->buf_size = buf_size;
	fifo_ptr->rd_ptr = 0;
	fifo_ptr->wr_ptr = 0;
}

//------------------------------------
//Get number of free space in FIFO:
//------------------------------------

unsigned int FIFO_get_free_space(FIFO_t* fifo_ptr)
{
	if (fifo_ptr->wr_ptr == fifo_ptr->rd_ptr){
		return fifo_ptr->buf_size;
	}
	else if (fifo_ptr->wr_ptr > fifo_ptr->rd_ptr){
		return fifo_ptr->buf_size - (fifo_ptr->wr_ptr - fifo_ptr->rd_ptr);
	}
	else {
		return (fifo_ptr->buf_size - fifo_ptr->rd_ptr) + fifo_ptr->wr_ptr;
	}
}

//------------------------------------
//Write single element to FIFO:
//------------------------------------

unsigned int FIFO_wr_single_element(FIFO_t* fifo_ptr, unsigned int data)
{
	if (FIFO_get_free_space(fifo_ptr) > 0){ //enough space for one element
		fifo_ptr->buf[fifo_ptr->wr_ptr] = data;

		if (fifo_ptr->wr_ptr == (fifo_ptr->buf_size - 1)){
			fifo_ptr->wr_ptr = 0;
		}
		else{
			fifo_ptr->wr_ptr++;
		}

		return 1;
	}
	else{ //no free space
		return 0;
	}
}

//------------------------------------
//Read single element from FIFO:
//------------------------------------

unsigned int FIFO_rd_single_element(FIFO_t* fifo_ptr, unsigned int* data)
{
	if (FIFO_get_free_space(fifo_ptr) != fifo_ptr->buf_size){
		*data = fifo_ptr->buf[fifo_ptr->rd_ptr];

		if (fifo_ptr->rd_ptr == (fifo_ptr->buf_size - 1)){
			fifo_ptr->rd_ptr = 0;
		}
		else{
			fifo_ptr->rd_ptr++;
		}

		return 1;
	}
	else{ //nothing to read
		return 0;
	}
}

//------------------------------------
//Flush FIFO buffer:
//------------------------------------

void FIFO_flush(FIFO_t* fifo_ptr)
{
	fifo_ptr->rd_ptr = 0;
	fifo_ptr->wr_ptr = 0;
}

//------------------------------------
//Check if FIFO is empty:
//------------------------------------

unsigned int FIFO_isEmpty(FIFO_t* fifo_ptr){
	if (fifo_ptr->wr_ptr == fifo_ptr->rd_ptr){
		return 1;
	}
	else{
		return 0;
	}
}

//------------------------------------
