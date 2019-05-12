/*
 * main.c
 *
 *  Created on: 2018-05-29
 *      Author: John Troxler
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#include "project.h"

#include "main.h"
#include "gpio_init.h"
#include "pwm_init.h"

#include "interrupt_init.h"
#include "user_interrupts.h"

#include <math.h>

/******************************* Local #defines *******************************/


/******************************* Local typedefs *******************************/


/************************* Local function prototypes **************************/
//void mainLoop(void);            //Main loop (unused)

/********************* Code sections to be copied to RAM **********************/


/****************************** Local structures ******************************/


/************************ Global variable definitions *************************/
volatile unsigned short edgeCount = 0;
volatile double pwmAngle = 0.0;
//Voltage in variables
volatile int volCount = 0;
volatile float32 volSum = 0;
volatile float32 volAvg = 0;
volatile float32 inputVoltage = 0;
//Current in variables
volatile int curCount = 0;
volatile float32 curSum = 0;
volatile float32 curAvg = 0;
volatile float32 inputCurrent = 0;
volatile unsigned int pwmCount = 0;

void main(void)
{
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    InitSysCtrl();


#ifdef FLASH_LINK
    // Copy time critical code and Flash setup code to RAM
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();
#endif

    // Set the High-Speed Peripheral Clock Prescaler (HISPCP) such that HSPCLK = 25 MHz
    // HSPCLK = SYSCLKOUT/(HISPCP x 2)
    EALLOW;
#if CPU_FRQ_150MHZ
    SysCtrlRegs.HISPCP.all = 3;
#else
    SysCtrlRegs.HISPCP.all = 2;
#endif
    EDIS;

    // Step 2. Initialize GPIO:
    gpio_init();

    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    InitPieVectTable();

    // Step 4. Initialize all the Device Peripherals:
    pwm_init();
    InitAdc();


    // Step 5. User specific code, enable interrupts:

    //Set the global variables:
    edgeCount = 0;
    pwmAngle = 0.0;

    //InitAdc();

    //Set Up Interrupts
    ext_int_setup();
    pwm_int_setup();
    adc_int_setup();
    EINT;


    // Enable global Interrupts and higher priority real-time debug events:
    //EINT;   // Enable Global interrupt INTM
    //ERTM;   // Enable Global real time interrupt DBGM

    while (1)
    {
        asm(" NOP");
    }
}
