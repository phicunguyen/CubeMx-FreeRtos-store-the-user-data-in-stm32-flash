# CubeMx-FreeRtos-store-the-user-data-in-stm32-flash
This application will save the user data to the stm32-flash and restore it when the power up again.

If your system does not have the EEPROM for storing the user data like volume control, brightness or TV channel number, then this application shows you how to store the user data in stm32-flash. The user setting is stored at the end of stm32 flash.

This code was developed on stm32f407VGTx-discovery which has four LEDs and a user button. When a user button is pressed a LED is on, if press four times then four LEDs will be on. Press the fifth time then all LEDs are off. 

For examples: 

LEDs are all off. If user button is pressed two times then two leds will be on, remove the power cable then plug it again then the last two led was on now will be on.

if user button is pressed one more time then the three leds will be on, recycle the power then three led will be on.

RM0090 Reference manual:

   Section 3.3 Embedded Flash memory in STM32F405xx/07xx and STM32F415xx/17xx list the flash memory map as shown below.

    Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbytes
    Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbytes
    Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbytes
    Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbytes
    Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbytes
    Sector 5 0x0802 0000 - 0x0803 FFFF 128 Kbytes
    Sector 6 0x0804 0000 - 0x0805 FFFF 128 Kbytes
    .
    Sector 11 0x080E 0000 - 0x080F FFFF 128 Kbytes

just for simplicity, This application is used section 11 for storing the data.

First create a user data structure as example below. Note: It can be any structure that you want to store.

    struct led_t {
      uint8_t cnt;  	//count up every time a user button is pressed.
      uint8_t led1;	//not being used
      uint8_t led2;	//not being used
      uint8_t led3;	//not being used
      uint8_t led4;	//not being used
    };

Second declares a flash instance.
    static led_t led;
    static struct flash_t flash_user_data;
  
 Now call flash_create to create a flash instance to hold the user data. 
 
    flash_create(&flash_user_data, (uint8_t *)&led, sizeof(struct led_t));

The flash_create function takes in three parameters:

    1. a flash instance (this case is flash_user_data)
    2. a pointer to your data (led)
    3. sizeof your data.
    
 Create a freertos task to call the flash_update. 
 
      void flash_update(void *arg) {
          while (true) {
		          update_led();
		          flash_user_data.flash_update(&flash_user_data); //write to flash only led data is changed.
		          osDelay(200);
	       }
     }    
