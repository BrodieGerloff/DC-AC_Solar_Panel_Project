//Functions to set up the interrupts for this project.

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#include "project.h"

#include "interrupt_init.h"
#include "user_interrupts.h"

void ext_int_setup(void) {

    //Set XINT3 to xint3_isr
    EALLOW;
    PieVectTable.XINT3 = &xint3_isr;
    EDIS;

    //Enable the external interrupt
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;       //Enable PIE
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1;     //Set PIEIER12.1
    IER |= M_INT12;                           //Set IER INT12

    //Setup GPIO 32
    EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;    //Set as GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;     //Set as Input
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 6;  // Qual period = SYSCLKOUT/16
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 0;   // XINT3 Sync to SYSCLKOUT
    EDIS;

    //Set GPIO32 to xINT3
    EALLOW;
    GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 0;
    EDIS;

    //Enabling XINT3
    XIntruptRegs.XINT3CR.bit.POLARITY = 1;   //Rising edge trigger
    XIntruptRegs.XINT3CR.bit.ENABLE = 1;     // Enable XINT3

}


void adc_int_setup(void){

    //Vector Table
    EALLOW;
    PieVectTable.ADCINT = &adc_isr;
    EDIS;

    //Enable ADCINT in PIE
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;          //Enable interrupt INT1.6
    IER |= M_INT1;

    //ADC Setup
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;     //Step 1: ePWM1 enables SOC
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 1;       //Step 2: set Max_CONV1 = 1 so conv00 and conv01 are active
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;        //Step 3: Set CONV00 to bit ADCINA0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1;        //Step 3: Set CONV01 to bit ADCINA1
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;       //Enable SEQ1 interrupt

    EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = 1;       // SOCa pulse on counter reset
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event

}

//Setup for the PWM interrupt
void pwm_int_setup(void) {

    //Vector table- set PWM interrupt
    EALLOW;
    PieVectTable.EPWM1_INT = &pwm_isr;
    EDIS;

    //Setup PWM interrupts
    EPwm1Regs.ETSEL.bit.INTSEL = 1;     //Interrupt when counter resets (Equals zero)
    EPwm1Regs.ETSEL.bit.INTEN = 1;      //Enable interrupt
    EPwm1Regs.ETPS.bit.INTPRD = 1;      //Interrupt on first event (must be set or won't start)

    //Enable interrupts for INT3 (INT3.1 is PWM 1)
    IER |= M_INT3;

    //Enable 3.1
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

}
