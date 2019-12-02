
#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f4xx_hal.h"
struct flash_t;

typedef void (*flash_func)(struct flash_t *flash);

struct flash_t {
	uint8_t 		*p_mem;				//point to user application memory
	uint8_t 		size;				//size of user application memory
	flash_func		flash_update;		//update the flash if user memory has been changed.
	uint32_t		flash_addr;			//point to the last addr
	uint32_t		flash_offset;		//point to the next available blockr
	uint32_t		flash_data;
};

void flash_create(struct flash_t *flash, uint8_t *p_mem, uint16_t size);

#endif //__FLASH_H
