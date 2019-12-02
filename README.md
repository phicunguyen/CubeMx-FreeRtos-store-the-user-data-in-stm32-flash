# CubeMx-FreeRtos-store-the-user-data-in-stm32-flash
This application will save the user data to the stm32-flash and restore it when the power up again.

If your system does not have the EEPROM for storing the user data like volume control or brightness, TV channel number, then this application shows you how to store the user data in stm32-flash. The user setting is stored at the end of stm32 flash.

This code was developed on stm32f407VGTx-discovery which has four LEDs and a user button. When a user button is pressed a LED is on, if press four times then four LEDs will be on. Press the fifth time then all LEDs are off. 

For examples: 

LEDs are all off. If user button is pressed two times then two leds will be on, remove the power cable then plug it again then the last two led was on now will be on.

if user button is pressed one more time then the three leds will be on, recycle the power then three led will be on.
