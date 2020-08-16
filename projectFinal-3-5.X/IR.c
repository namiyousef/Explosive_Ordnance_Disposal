/*
 *Authors: Yousef Nami and Matteo (Frank) Bonsignore
 *Iteration: Final
  */
//----------------------------------------------------------------------------//

/*libraries, files and definitions*/

#include <xc.h>
#include "IR.h"

//left IR
#define LeftIR LATAbits.LATA3
#define LeftIRIO TRISAbits.RA3

//right IR
#define RightIR LATAbits.LATA4
#define RightIRIO TRISAbits.RA4

/* Use Internal Oscillator, turn Master Clear Reset off, 
 * turn Low Voltage Protect off */

#pragma config OSC=IRCIO, MCLRE=OFF, LVP=OFF, WDTEN = OFF
#define _XTAL_FREQ 8000000

//----------------------------------------------------------------------------//

/* Initialise IR */

void initIR(void) {
    
    ANSEL0 = 0; //set all AN pins to digital I/O
    ANSEL1 = 0; //set AN8 pin to digital I/O
    
    //clear values
    LeftIR = 0;
    RightIR = 0;

    //set directions
    LeftIRIO = 1; 
    RightIRIO = 1;
    
    //configure CAP2/3 pins to measure pulse width of IR sensor signal from
    //falling to rising edge
    
    CAP2CON = 0b01000110;
    CAP3CON = 0b01000110;

    T5CON = 0b00010001; //activate Timer5, 1:4  prescaler

}

//----------------------------------------------------------------------------//

/*function to calculate the value of the IR signals in binary*/

    void findStrengths(int *leftIR, int *rightIR){
        
    //record pulse widths of the left/right IR signals
    *leftIR = (CAP2BUFL|(CAP2BUFH<<8));
    *rightIR = (CAP3BUFL|(CAP3BUFH<<8));    
}  

//----------------------------------------------------------------------------//



