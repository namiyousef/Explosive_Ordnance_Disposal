/*
 *Authors: Yousef Nami and Matteo (Frank) Bonsignore
 *Iteration: Final
 
 *Important notes/changes made: None
 */
//----------------------------------------------------------------------------//

/*libraries, files and definitions*/

#include <xc.h>
#include <string.h>

//----------------------------------------------------------------------------//

/* Use Internal Oscillator, turn Master Clear Reset off, 
 * turn Low Voltage Protect off */

#pragma config OSC=IRCIO, MCLRE=OFF, LVP=OFF
#define _XTAL_FREQ 8000000

#define EPin LATCbits.LATC0

//----------------------------------------------------------------------------//

/*function to toggle and clear enable bit*/

void E_TOG(void){
    
    EPin = 1;
    
    __delay_us(5);
    
    EPin = 0;
}

//----------------------------------------------------------------------------//

/*function to send four bits to the LCD*/

void LCDout(unsigned char number){

    //set data pins using the four bits from number
    //toggle the enable bit to send data
    
    LATC=((0b00000011&number)<<1)|(LATC&0b11111001);
    LATD=((number&0b00001100)>>2)|(LATD&0b11111100);
    
    E_TOG();
    __delay_us(5);
    
}

//----------------------------------------------------------------------------//

/*function to send data/commands over a 4bit interface*/
void SendLCD(unsigned char Byte, char type){
    
    // set RS pin whether it is a Command (0) or Data/Char (1) sent
    LATAbits.LATA6=type;
    
    char HighBits=0b11110000&Byte; //add a comment for this?
    char LowBits=0b00001111&Byte;
    
    //send high bits of Byte using LCDout function
    LCDout((HighBits>>4));
    __delay_us(10);
    
    //send low bits of Byte using LCDout function
    LCDout(LowBits);
    __delay_us(10);
    
}

//----------------------------------------------------------------------------//

/*function to initialize LCD*/
void initLCD(void){
    
    //clear LCD pins and set them as outputs
    LATAbits.LATA6=0;
    TRISAbits.RA6 = 0;
    
    LATCbits.LATC0 =0;
    TRISCbits.RC0=0;
        
    LATCbits.LATC1=0;
    TRISCbits.RC1=0;  
    
    LATCbits.LATC2=0;
    TRISCbits.RC2=0;
    
    LATDbits.LATD0=0;
    TRISDbits.RD0=0;
    
    LATDbits.LATD1=0;
    TRISDbits.RD1=0;
    
    /* Initialisation sequence code from LCD data sheet*/
    __delay_ms(20);
    
    LCDout(0b0011);
    
    __delay_ms(5);

    LCDout(0b0011);
    
    __delay_us(200);
 
    LCDout(0b0011);
 
    __delay_us(50);
 
    LCDout(0b0010);
    
    __delay_us(50);

    SendLCD(0b00101000,0); //set function
    
    __delay_us(50);
    
    SendLCD(0b00001000,0); //turn display off
    
    __delay_us(50);
    
    SendLCD(0b00000001,0);//clear display
    
    __delay_ms(5);
    
    SendLCD(0b00000110,0); //set entry mode
    
    __delay_us(50);
    
    SendLCD(0b00001100,0); //set display on
    
    __delay_us(50);
}

//----------------------------------------------------------------------------//

/* Set line at which want to display data sent */

void SetLine (char line){
    
    if (line==1){
        
        SendLCD(0x80,0);
        
    } else if (line==2){
        
        SendLCD(0xC0,0);
        
    }

 __delay_us(50);
 
}

//----------------------------------------------------------------------------//

/* Send String to LCD */

void LCD_String(char *string){
    
    while((*string)!=0){
        
        SendLCD(*string++,1); //Send out the current byte pointed to and 
                              //increment pointer
        
        __delay_ms(50);

    }
    
}

//----------------------------------------------------------------------------//




