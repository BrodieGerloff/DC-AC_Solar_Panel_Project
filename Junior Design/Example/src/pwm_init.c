/*
 * pwm_init.c
 *
 *  Created on: 2018-05-29
 *      Author: John Troxler
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


#include "project.h"

#include "pwm_init.h"


void pwm_init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;	// Disable the clock input to epwm module
    EDIS;

    EPwm1Regs.TBCTL.all = 0 ;	// reset all bits in TBCTL

    //PART 1 CODE

	EPwm1Regs.TBCTL.bit.CLKDIV = 0x00;          //No Scaling
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x00;       //No Scaling
	EPwm1Regs.TBCTL.bit.CTRMODE = 0x00;         //Up Count Mode

	EPwm1Regs.TBCTL.bit.PHSEN = 0x00;           //Not loading Time Base Counter
	EPwm1Regs.TBCTL.bit.SYNCOSEL = 0x01;        //Synchronous output select unused, 01

	EPwm1Regs.TBCTL.bit.PRDLD = 0x00;           //Shadow Register Select

	EPwm1Regs.CMPCTL.bit.SHDWBMODE = 0x00;      //CMPB Shadow Mode
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0x00;      //CMPA Shadow Mode

	EPwm1Regs.TBPHS.all = 0;                    //Not loading Time Base Counter

	EPwm1Regs.DBCTL.bit.IN_MODE = 0x00;         //EPWM1A input/output
	EPwm1Regs.DBCTL.bit.POLSEL = 0x02;          //Active high complementary, 10
	EPwm1Regs.DBCTL.bit.OUT_MODE = 0x03;        //Deadbands, 11

	EPwm1Regs.AQCTLA.bit.ZRO = 0x02;            //EPWM1A zero high, 10
	EPwm1Regs.AQCTLA.bit.CAU = 0x01;            //CMPA inc low, 01

	EPwm1Regs.TBPRD = PWM_TBPRD;                //Set PWM to 30kHz

	EPwm1Regs.DBRED = PWM_DEADBAND;             //Rising Deadband
	EPwm1Regs.DBFED = PWM_DEADBAND;             //Falling Deadband

	EPwm1Regs.CMPA.half.CMPA = 0x0;             //Duty Cycle

	//PART 3 CODE
	EPwm2Regs.TBCTL.bit.CLKDIV = 0x00;          //no clock divides
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x00;       //No Scaling
	EPwm2Regs.TBCTL.bit.CTRMODE = 0x02;         //Stop-Freeze Mode

	EPwm2Regs.TBCTL.bit.PHSEN = 0x00;           //Not loading Time Base Counter
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0x01;        //Synchronous output select unused, 01

    EPwm2Regs.TBPHS.all = 0;                    //No phase?

    EPwm2Regs.DBCTL.bit.IN_MODE = 0x00;         //EPWM2A input/output
    EPwm2Regs.DBCTL.bit.POLSEL = 0x02;          //Active high complementary, 10
    EPwm2Regs.DBCTL.bit.OUT_MODE = 0x03;        //Deadbands, 11

    EPwm2Regs.AQCTLA.bit.ZRO = 0x00;            //No action when 0
    EPwm2Regs.AQCTLA.bit.CAU = 0x00;            //no action when CMPA

    EPwm2Regs.DBRED = PWM_DEADBAND;             //Rising Deadband
    EPwm2Regs.DBFED = PWM_DEADBAND;             //Falling Deadband

    //EPwm2Regs.AQSFRC.bit.RLDCSF = 0x03;         //Write directly to register
    //EPwm2Regs.AQSFRC.bit.ACTSFA = 0x02;         //Set PWM high when OTSFA is toggled
    //EPwm2Regs.AQSFRC.bit.ACTSFB = 0x03;         //Toggle PWM when OTFSB is toggled

    // Enable clocks for PWM modules
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}
