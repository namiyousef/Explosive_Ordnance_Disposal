/*
 *Authors: Yousef Nami and Matteo "Frank" Bonsignore
 *Iteration: Final

 */
//----------------------------------------------------------------------------//

/*libraries, files and definitions*/

#include <xc.h>
#include "PWM.h"

//----------------------------------------------------------------------------//

/* Use Internal Oscillator, turn Master Clear Reset off, 
 * turn Low Voltage Protect off */

#pragma config OSC=IRCIO, MCLRE=OFF, LVP=OFF
#define _XTAL_FREQ 8000000

//----------------------------------------------------------------------------//

/*initialize PWM*/

void initPWM(void){ 
    
    //clears chip PWM pins and sets them to outputs
    
    LATBbits.LATB0=0;
    TRISBbits.RB0=0;
    
    LATBbits.LATB1=0;
    TRISBbits.RB1=0;
    
    LATBbits.LATB2=0;
    TRISBbits.RB2=0;
    
    LATBbits.LATB3=0;
    TRISBbits.RB3=0;
    
    PTCON0=0b00000000;
    PTCON1=0b10000000; //enable PWM timer
    
    PWMCON0=0b01101111;
    PWMCON1=0x00; //special features, all 0 (default)
    
    PTPERL=0b11000111; //base PWM period low byte
    PTPERH=0; //base PWM period high byte
    
    //clear PDC values
    PDC0L=0b0; //PDC0L bits 7:2 are on period bits 5:0
    PDC0H=0b0; //PDC0H bits 5:0 are on period bits 11:6
    
    PDC1L=0;   //PDC1L bits 7:2 are on period bits 5:0 
    PDC1H=0;   //PDC1H bits 5:0 are on period bits 11:6
    
    PDC2L=0;   //PDC2L bits 7:2 are on period bits 5:0
    PDC2H=0;   //PDC2H bits 5:0 are on period bits 11:6
    
    PDC3L=0;   //PDC3L bits 7:2 are on period bits 5:0
    PDC3H=0;   //PDC3H bits 5:0 are on period bits 11:6
    
    PDC0L=0b10001111;
    PDC0H=0b1;
    PDC1L=0b10001111;
    PDC1H=0b1;

}; 

//----------------------------------------------------------------------------//

/*function to set up PWM cycle*/

void setMotorPWM(struct DC_motor *m){
    
    //tmp variable to store PWM duty cycle
    int PWMduty = ((int)m->power)*(m->PWMperiod);

    if (m->direction) { //if backward
        //low time increases with power
        PWMduty = (PWMduty>>9)+PWMduty>>7;
        PWMduty = m->PWMperiod - PWMduty;
    }
    else { //if forwards
        PWMduty = (PWMduty>>9)+PWMduty>>7;
    }
    
    PWMduty=(PWMduty<<2); //two LSBs are reserved for other things
    *(m->dutyLowByte)=PWMduty&0xFF; //set low duty cycle byte
    *(m->dutyHighByte)=(PWMduty>>8)&0x3F; //set high duty cycle byte
    
    if (m->direction) {
        LATB=LATB | (1<<(m->dir_pin)); //set dir_pin bit in LATB to high 
   } else {
        LATB=LATB&(~(1<<(m->dir_pin))); //set dir_pin bit in LATB to low
        //change or to and    
    }
    
}

//----------------------------------------------------------------------------//

/*sets motors to full speed*/

void setMotorsFullSpeed(struct DC_motor *mL, struct DC_motor *mR) {
    
    //increase both motor powers to 100
    for (mL->power; (mL->power) <= 100;(mL->power)++) {
        
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        mR->power += 1;
       __delay_ms(5);
    
    }
}

//----------------------------------------------------------------------------//

/*function to stop the robot gradually*/

void stop(struct DC_motor *mL, struct DC_motor *mR) {
    
    //decrease both motor powers to 0

    for (mL->power; (mL->power) > 0;(mL->power)--) {
        
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        mR->power -= 1;
       __delay_ms(5);
       
        }
        }

//----------------------------------------------------------------------------//

/*function to move motor*/

    void move(struct DC_motor *mL,struct DC_motor *mR, char dir){
        mL->direction=(dir>>1)&(0b01);
        mR->direction=(dir)&(0b01);
        setMotorsFullSpeed(mL,mR);
        stop(mL,mR);
    }
    
    /*
     * dir takes following values:
     * 
     *      0b00 - forwards
     *      0b11 - backwards
     *      0b10 - left
     *      0b01 - right
     */

//----------------------------------------------------------------------------//









