//All of the user created interrupt functions

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <math.h>

#include "project.h"

#include "user_interrupts.h"

extern volatile unsigned short edgeCount;
extern volatile double pwmAngle;
extern volatile int volCount;
extern volatile float32 volSum;
extern volatile float32 volAvg;
extern volatile float32 inputVoltage;
extern volatile int curCount;
extern volatile float32 curSum;
extern volatile float32 curAvg;
extern volatile float32 inputCurrent;
extern volatile unsigned int pwmCount;

enum states {INIT, VALID, RUN} currentState = INIT;

interrupt void xint3_isr(void) {
    //FSM
    switch(currentState) {
        case INIT:                      //Initial state: wait to run
            edgeCount++;
            if (edgeCount >= 1) {
                currentState = VALID;
            }
            break;
        case VALID:                     //Validate state: wait for 20 cycles
            edgeCount++;
            if (edgeCount > 20) {
                currentState = RUN;
                EALLOW;
                SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
                EDIS;
            }
            break;
        case RUN:                       //Run state: reset values
            edgeCount = 0;
            //Take Average Voltage Value
            volAvg = volSum / volCount;
            inputVoltage = volAvg * VOLT_SLOPE + VOLT_OFF;
            volSum = 0.0;
            volCount = 0;

            //Take average current value
            curAvg = curSum / curCount;
            inputCurrent = curAvg * CURR_SLOPE + CURR_OFF;
            curSum = 0.0;
            curCount = 0;

            //Reset duty cycle angle and slow leg
            pwmAngle = 0;
            //EPwm2Regs.AQSFRC.bit.OTSFA = 1;
            //EPwm2Regs.AQSFRC.bit.OTSFB = 1;
            //toggle = 0;

            //Reset PWM counter
            pwmCount = 0;


            break;
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;        //Acknowledge Interrupt

}

interrupt void pwm_isr(void) {

    //Allow XINT to interrupt PWM
    Uint16 xintSave = PieCtrlRegs.PIEIER12.all;
    IER = 0x0000;
    IER &= M_INT12;
    EINT;

    //Update duty ratio if in the run state
    if (currentState == RUN) {
        pwmAngle = pwmAngle + (TWO_PI / 500);
        if (pwmAngle <= ONE_PI) {
            EALLOW;
            EPwm1Regs.CMPA.half.CMPA = DUTY_RATIO * sin(pwmAngle) + OFFSET;
            EPwm2Regs.AQCTLA.bit.ZRO = 0b10;
            EDIS;
        }
        else {
            EALLOW;
            EPwm1Regs.CMPA.half.CMPA = PWM_MAX + DUTY_RATIO * sin(pwmAngle) - OFFSET;
            EPwm2Regs.AQCTLA.bit.ZRO = 0b01;
            EDIS;
        }

        //Increment shutdown count
        pwmCount++;
    }

    //Reset if five 60Hz clock cycles are missed
    if (pwmCount > PWM_SHUTDOWN) {
        edgeCount = 0;
        pwmCount = 0;
        currentState = INIT;
        EALLOW;
        EPwm1Regs.CMPA.half.CMPA = 0;
        EDIS;
    }

    //If angle > pi, toggle slow leg
    /*if (pwmAngle > ONE_PI) {
        EPwm2Regs.AQCTLA.bit.ZRO = 0b01;
        //EPwm2Regs.AQSFRC.bit.OTSFA = 1;
        //EPwm2Regs.AQSFRC.bit.OTSFB = 1;
    }
    else {
        EPwm2Regs.AQCTLA.bit.ZRO = 0b10;
    }*/

    //Reset to disable XINT
    DINT;
    PieCtrlRegs.PIEIER12.all = xintSave;

    EPwm1Regs.ETCLR.bit.INT = 1;                    //Clears interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;         //Acknowledge Interrupt
}

interrupt void adc_isr(void) {
    //volSum = volSum + (AdcRegs.ADCRESULT0>>4);
    volSum = volSum + (float32)(AdcMirror.ADCRESULT0);
    curSum = curSum + (float32)(AdcMirror.ADCRESULT1);
    volCount++;
    curCount++;

    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}


