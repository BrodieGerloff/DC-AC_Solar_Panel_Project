/*
 * gpio_init.c
 *
 *  Created on: 2018-05-29
 *      Author: John Troxler
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#include "project.h"

#include "gpio_init.h"
#include "main.h"

void gpio_init(void)
{
	EALLOW;

	//PART 1 CODE

	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up GPIO1

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   //GPIO0 => EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   //GPIO1 => EPWM1B

    //PART 2 CODE

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;     //GPIO4 = GPIO24
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;      //GPIO4 is output
    GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;    //Set GPIO4 to low

    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;     //GPIO5 = GPIO5
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;      //GPIO5 is output
    GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;    //Set GPIO5 to low

    //PART 3 CODE

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up GPIO2
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up GPIO3

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   //GPIO0 => EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   //GPIO1 => EPWM2B

	EDIS;
}
