#include "flash.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/******************* local defines *******************************/
#define FLASH_SECTOR_USER__START_ADDR 	0x080E0000UL
#define FLASH_SECTOR_USER__END_ADDR 	0x080EFFFFUL
#define FLASH_SECT_PAGE		11

/******************* local struct *******************************/
/******************* local function prototypes ******************/
static void s_flash_update(struct flash_t *flash);
static void s_flash_write(uint32_t addr, uint8_t *pdata, uint32_t size);
static void s_flash_read(uint32_t addr, uint8_t *pdata, uint32_t size);
static bool s_get_flash(struct flash_t *flash,uint8_t *p_mem, uint32_t size);

/****************** public functions *****************************/
/*
 * Create an instance of the flash
 * p_mem - point to the user application memory
 * size  - size of the user application memory
 */
void flash_create(struct flash_t *flash, uint8_t *p_mem, uint16_t size) {
	flash->size = size;
	flash->p_mem = p_mem;
	flash->flash_update = s_flash_update;
	s_get_flash(flash, p_mem, size);
}

/****************** local functions *****************************/
/*
 * return the stored data that in the flash to the user application.
 */
static bool s_get_flash(struct flash_t *flash, uint8_t *p_mem, uint32_t size) {
	bool res=false;

	uint32_t *p_addr = (uint32_t *)FLASH_SECTOR_USER__START_ADDR;
	flash->flash_offset = (size >>2) + 1 + 1;

	/* search for the address where was the last time the data store */
	while (*p_addr != 0xFFFFFFFF) {
		flash->flash_addr 	= *p_addr;
		p_addr += flash->flash_offset;
		res = true;
	}

	/* No user data found in flash, store the init data */
	if (!res) {
		flash->flash_addr = FLASH_SECTOR_USER__START_ADDR;
		flash->flash_data = flash->flash_addr + sizeof(flash->flash_addr);
		//save the flash address pointer and size of data and data
		s_flash_write(flash->flash_addr,  (uint8_t *)&flash->flash_addr, sizeof(flash->flash_addr));
		s_flash_write(flash->flash_data, p_mem, size);
		HAL_FLASH_Lock();
	} else {
		//found data store in flash
		//copy data from flash to the application.
		flash->flash_data = flash->flash_addr + sizeof(flash->flash_addr);
		s_flash_read(flash->flash_data, p_mem, size);
	}
	return res;
}

/*
 * Check if the user application memory has changed the update the flash.
 */
void s_flash_update(struct flash_t *flash) {
	//check if the user data has been changed.
	//if yes, then store it in the flash
	if (memcmp((uint8_t *)flash->p_mem, (uint8_t *)flash->flash_data, flash->size)) {
		//advance to next empty slot.
		//+1 is for where store the flash address and flash_offset is number of data bytes
		//*4 is 4 bytes (uint32_t)
		flash->flash_addr += ((flash->flash_offset) * 4);
		flash->flash_data = flash->flash_addr + sizeof(flash->flash_addr);
		s_flash_write(flash->flash_addr,   (uint8_t *)&flash->flash_addr, sizeof(flash->flash_addr));
		s_flash_write(flash->flash_data , flash->p_mem, flash->size);
		HAL_FLASH_Lock();
	}
}

static void s_flash_write(uint32_t addr, uint8_t *pdata, uint32_t size) {
	//uint32_t flashAddress = FLASH_SECTOR_USER__ADDR + addr;
	HAL_FLASH_Unlock();
	if (addr+size < FLASH_SECTOR_USER__END_ADDR) {
		for(uint32_t i=0; i<size; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr++ , ((uint8_t *)pdata)[i]);
		}
	}
}

static void s_flash_read(uint32_t addr, uint8_t *pdata, uint32_t size) {
	//uint32_t flashAddress = FLASH_SECTOR_USER__ADDR + addr;
	for(uint32_t i=0; i<size; i++) {
		*((uint8_t *)pdata + i) = *(uint8_t *)addr;
		addr++;
	}
}

/*
static void flash_erase(int sector) {
	HAL_FLASH_Unlock();
	//Erase the required Flash sector
	FLASH_Erase_Sector(sector, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();
}
*/

