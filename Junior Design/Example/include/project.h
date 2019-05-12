/*
 * project.h
 *
 *  Created on: 2018-05-29
 *      Author: John Troxler
 */

#ifndef INCLUDE_PROJECT_H_
#define INCLUDE_PROJECT_H_


/* ============================ CONTEXT SWITCHES ============================ */

/* ============================= GLOBAL DEFINES ============================= */
#define PWM_TBPRD 0x1387		    //4999, sets to 30kHz
#define PWM_DEADBAND 0x0069         //105, 700ns delay
#define PWM_CMPA 0x09C4             //2500, 50% ratio, 9C4
#define TWO_PI 6.282                //Used for duty cycle calculation
#define ONE_PI 3.1415               //Used for slow leg
#define PWM_SHUTDOWN 2500           //5 60Hz cycles
#define DUTY_RATIO 2500             //m(t) or 50% duty ratio
#define PWM_MAX 0x1387              //100% duty cycle
#define OFFSET 50                   //Used to fix output wave

#define VOLT_SLOPE 0.00903          //Input voltage = volAvg * slope + off
#define VOLT_OFF -0.57              //See above

#define CURR_SLOPE 0.00193           //Input Current = curAvg * slope + off
#define CURR_OFF -0.30095


#endif /* INCLUDE_PROJECT_H_ */
