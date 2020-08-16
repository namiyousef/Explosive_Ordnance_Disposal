/*
 *Authors: Yousef Nami and Matteo (Frank) Bonsignore
 *Iteration: Final
//----------------------------------------------------------------------------//

/*libraries, files and definitions*/

#include <xc.h>

//----------------------------------------------------------------------------//

/* Initialise USART for Serial Communication */
void initEusart(void){   
    
    //clear RX port and set it as input (for RFID)
    LATCbits.LATC7 = 0;
    TRISCbits.RC7 = 1;
    
    SPBRG = 207;// sets baud rate to 9600
    SPBRGH = 0;
    BAUDCONbits.BRG16 = 1; // set baud rate scaling to 16 bit mode.
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continuous receive mode
    RCSTAbits.SPEN = 1; //enable serial port, other settings default
    TXSTAbits.TXEN = 0; //enable transmitter, other settings default
}

//----------------------------------------------------------------------------//

//ideally would like to change this

/*reads RFID data*/

char getCharSerial (void){
    
    while(!PIR1bits.RCIF); //wait for data byte to arrive
    
    return RCREG; //return data byte as a character
    
}

//----------------------------------------------------------------------------//
