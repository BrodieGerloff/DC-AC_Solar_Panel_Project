

#ifndef INCLUDE_USER_INT_H_
#define INCLUDE_USER_INT_H_


// Function Prototypes
interrupt void xint3_isr(void); //Interrupt function for external interrupt
interrupt void pwm_isr(void);   //Interrupt function for PWM interrupt
interrupt void adc_isr(void);   //Interrupt function for ADC interrupt


#endif
