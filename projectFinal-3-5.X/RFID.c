/*
 *Authors: Yousef Nami and Matteo (Frank) Bonsignore
 *Iteration: Final
 
 */
//----------------------------------------------------------------------------//
#include "RFID.h"
#include "LCD.h"
#include "eusart.h"

char currentVal = 0; //char to store hexadecimal bit pairs
char checkSum = 0; //char to store checksum values
char index = 1; //defines index of bits to display
char a; //char to store value received from serial

void sendRFID(void) {
    SetLine(1); //set line back to 1
    SendLCD(0b00000001,0); //clear display
    a = getCharSerial();
    if (a == 0x02) {
        while (a != 0x0A) {
            a = getCharSerial();
            if (index <= 12) {
                SendLCD(a, 1);

                //converts ASCII characters to their decimal values

                if (a > 64) { //if ASCII character is a letter

                    a = a - 55;

                } else { //if it's a number

                    a = a - 48;
                }

                currentVal = (a | currentVal);

                if (index % 2 == 1) { //if its odd

                    currentVal = currentVal << 4;

                } else { //if its even

                    if (index < 11) {

                        checkSum = checkSum^currentVal;

                    } else {

                        if (checkSum == currentVal) {

                            SetLine(2);

                            LCD_String("Match");

                        } else {

                            SetLine(2);

                            LCD_String("No Match");

                        }
                    }

                    currentVal = 0;

                }

                index = index + 1;
            }

        }

    }
}
