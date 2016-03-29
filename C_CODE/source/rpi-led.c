#include "rpi-gpio.h"
#include "rpi-led.h"

//#define LED_GPIO_NUM 47
int lednum = 4;
int led[4] = {17, 21, 5, 13};

void LED_INIT()
{
	//RPI_SetGpioOutput(LED_GPIO_NUM);
	int i;
	for (i = 0; i < lednum; i++){
		RPI_SetGpioOutput(led[i]);
	}
}

void LED_ON(int i)
{
	//RPI_SetGpioValue(LED_GPIO_NUM,RPI_IO_LO);
	RPI_SetGpioValue(led[i],RPI_IO_LO);
}

void LED_OFF(int i)
{
	//RPI_SetGpioValue(LED_GPIO_NUM,RPI_IO_HI);
	RPI_SetGpioValue(led[i],RPI_IO_HI);
}
