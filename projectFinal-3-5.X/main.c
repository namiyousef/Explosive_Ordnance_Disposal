/*
 *Authors: Yousef Nami and Matteo "Frank" Bonsignore
 *Iteration: Final
 */
//----------------------------------------------------------------------------//

/*libraries, files and definitions*/

#include <xc.h>
#include <pic18f4331.h>
#include "PWM.h"
#include "IR.h"
#include "LCD.h"
#include "eusart.h"
#include "RFID.h"

/*define pins with names*/

//power button
#define Power LATCbits.LATC3
#define PowerIO TRISCbits.TRISC3

//LED to signal device on/off
#define LEDOn LATCbits.LATC4
#define LEDOnIO TRISCbits.RC4

//LED to signal RFID read/not read
#define CodeRead LATDbits.LATD2
#define CodeReadIO TRISDbits.RD2




//----------------------------------------------------------------------------//

/* Use Internal Oscillator, turn Master Clear Reset off,
 * turn Low Voltage Protect off */

#pragma config OSC=IRCIO, MCLRE=OFF, LVP=OFF, WDTEN = OFF
#define _XTAL_FREQ 8000000

//----------------------------------------------------------------------------//

/* Initialise Global Variables*/

char debouncer = 0;
char reset = 1; //flag to enable reset while robotOff
char robotOn = 0; //turns robot ON/OFF
char RFIDread = 0; //flag to indicate RFID read/not read
char IRsource = 0; //0/1 for IR source found/not found

//----------------------------------------------------------------------------//

/*main function*/

void main(void) {

    /*oscillator setup*/
    OSCCON = 0x72; //set oscillator freq. to 8MHz
    while (!OSCCONbits.IOFS); //wait for oscillator to stabilise


    /* Initialise PIC Chip Pins */

    //clear pins
    Power = 0;
    LEDOn = 0;
    CodeRead = 0;

    //set pin direction
    PowerIO = 1;
    LEDOnIO = 0;
    CodeReadIO = 0;



    /*Define Interrupts*/

    PIR1 = 0; //clear the peripheral interrupt

    //button interrupts
    INTCONbits.GIEH = 1; //enable high interrupt pin
    INTCONbits.GIEL = 1; //enable low interrupt pin
    RCONbits.IPEN = 1; //enable dual interrupt (i.e. low and high)
    INTCONbits.INT0IE = 1; //enable INT0 (RC3 i.e. button) as high interrupt

    //peripheral interrupts
    PIE1bits.RCIE = 1; //enable peripheral interrupt pin
    IPR1bits.RC1IP = 0; //set RX interrupt to low priority



    /* Initialise Peripheral Devices */

    initEusart(); //initialise serial communication with RFID
    initPWM(); //initialise motors
    initLCD(); //initialise LCD screen
    initIR(); //initialise IR sensors


    /* Define initial motor parameters in struct */

    //power - sets percentage of motor power at which motor should run
    //direction - sets direction motor turns, forwards (1) or backwards (0)
    //dutyLowByte - PWM duty low byte address
    //dutyHighByte - PWM duty high byte address
    //dir_pin - pin that controls direction on PORTB
    //PWMperiod - base period of PWM cycle

    struct DC_motor motorL, motorR; //define the struct for the motors

    //right motor
    motorR.power = 0;
    motorR.direction = 0;
    motorR.dutyLowByte = (unsigned char *) (&PDC0L);
    motorR.dutyHighByte = (unsigned char *) (&PDC0H);
    motorR.dir_pin = 0;
    motorR.PWMperiod = 199;

    //left motor
    motorL.power = 0;
    motorL.direction = 0;
    motorL.dutyLowByte = (unsigned char *) (&PDC1L);
    motorL.dutyHighByte = (unsigned char *) (&PDC1H);
    motorL.dir_pin = 2;
    motorL.PWMperiod = 199;


    //set the motor to run based on the values given above
    setMotorPWM(&motorL);
    setMotorPWM(&motorR);

    //define variables for the IR readings
    int leftIR;
    int rightIR;

    char i = 0; //initialise index for 'path' array
    char path[130]; //define array of length 130 to store motor path

    char c = 0; //counter for IR Sensors to verify IR strength
    //readings before moving robot
    int buffer = 800; //bandwidth between left and right IR readings

    while (1) {

        if (robotOn == 1) {

            //this is a 'lost' function. The car moves left until it finds
            //IR signals that are sufficiently strong

            //clear IR values
            leftIR = 0;
            rightIR = 0;

            while ((leftIR < 1500 || rightIR < 1500) && robotOn == 1 && IRsource == 0) {

                findStrengths(&leftIR, &rightIR); //read IR values
                move(&motorL, &motorR, 0b10); //turn left

                path[i] = 0b10; //save turn
                i += 1; //increment index
            }

            IRsource = 1; //IR source has been found, prevents robot from
            //going into 'lost' mode again

            if (RFIDread == 1) {

                while (i > 0) {

                    --i;
                    move(&motorL, &motorR, ~(path[i])); //return along same path

                }

                stop(&motorL, &motorR);

            } else {

                move(&motorL, &motorR, 0b00); //move forwards

                path[i] = 0b00; //save move
                i += 1;

                findStrengths(&leftIR, &rightIR);

                if (leftIR > (rightIR + buffer)) {

                    c += 1;

                    while (leftIR > (rightIR + buffer) && c < 4) {
                        //check measurement 3 times for consistency before
                        //making a move

                        findStrengths(&leftIR, &rightIR);

                        if (c == 3) {

                            move(&motorL, &motorR, 0b10);

                            path[i] = 0b10;
                            i += 1;
                        }


                        c += 1;
                    }

                    c = 0; //reset counter

                } else if ((leftIR + buffer) < rightIR) {

                    c += 1;

                    while ((leftIR + buffer) < rightIR && c < 4) {

                        findStrengths(&leftIR, &rightIR);

                        if (c == 3) {

                            move(&motorL, &motorR, 0b01);

                            path[i] = 0b01;
                            i += 1;
                        }

                        c += 1;

                    }

                    c = 0;

                }


            }




        } else {

            if (reset == 1) {

                stop(&motorL, &motorR);
                reset = 0;
                CodeRead = 0; //reset RFID LED

                //reset path and index
                while (i > 0) {
                    --i;

                    path[i] = 0;

                }

            }

        }

    }
}

//----------------------------------------------------------------------------//

/*high priority interrupt routine*/

void __interrupt(high_priority) my_hp_isr(void) {


    if (INTCONbits.INT0IF) {

        debouncer += 1;

        if (debouncer == 30) {


            debouncer = 0;

            //clear variables
            IRsource = 0;
            RFIDread = 0;

            robotOn = !robotOn;
            reset = 1;

            LEDOn  = !LEDOn ;

            INTCONbits.INT0IF = 0; //sets interrupt flag back to 0
        }


    }
}

//----------------------------------------------------------------------------//

/*low priority interrupt routine*/

void __interrupt(low_priority) my_lp_isr(void) {

    if (PIR1bits.RCIF) {

        //retrieve and display RFID code
        sendRFID();

        IRsource = 1;
        RFIDread = 1;
        CodeRead = 1;

        char LastRFID = RCREG; //clear interrupt flag
    }

}

//----------------------------------------------------------------------------//
