# 1 "RFID.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/Applications/microchip/xc8/v2.10/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "RFID.c" 2






# 1 "./RFID.h" 1
# 15 "./RFID.h"
    void sendRFID(void);
# 7 "RFID.c" 2

# 1 "./LCD.h" 1
# 17 "./LCD.h"
    void E_TOG(void);
    void LCDout(unsigned char number);
    void SendLCD(unsigned char Byte, char type);
    void initLCD(void);
    void SetLine (char line);
    void LCD_String(char *string);
# 8 "RFID.c" 2

# 1 "./eusart.h" 1
# 15 "./eusart.h"
    void initEusart(void);
    char getCharSerial(void);
# 9 "RFID.c" 2


char currentVal = 0;
char checkSum = 0;
char index = 1;
char a;

void sendRFID(void) {
    SetLine(1);
    SendLCD(0b00000001,0);
    a = getCharSerial();
    if (a == 0x02) {
        while (a != 0x0A) {
            a = getCharSerial();
            if (index <= 12) {
                SendLCD(a, 1);



                if (a > 64) {

                    a = a - 55;

                } else {

                    a = a - 48;
                }

                currentVal = (a | currentVal);

                if (index % 2 == 1) {

                    currentVal = currentVal << 4;

                } else {

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
