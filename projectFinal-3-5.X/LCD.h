/*
 *Authors: Yousef Nami and Matteo (Frank) Bonsignore
 *Iteration: Final
 
 *Important notes/changes made: None
 */
//----------------------------------------------------------------------------//


#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void E_TOG(void);
    void LCDout(unsigned char number);
    void SendLCD(unsigned char Byte, char type);
    void initLCD(void);
    void SetLine (char line);
    void LCD_String(char *string);




#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

