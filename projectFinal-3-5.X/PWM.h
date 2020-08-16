/*
 *Authors: Yousef Nami and Matteo (Frank) Bonsignore
 *Iteration: Final
 
 *Important notes/changes made: None
 */
//----------------------------------------------------------------------------//

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif
   /*define a struct for the motors*/
    
    struct DC_motor {
        char power; //ranges from 0-100. Equivalent of percentage of motor power
        char direction; //determines motor direction (0 = forwards, 1 backwards)
        unsigned char *dutyLowByte; //give defs for these
        unsigned char *dutyHighByte; //give defs for these
        char dir_pin; //translates "direction" to pins on the h-bridge
        int PWMperiod; //not sure what this again?
    };
    
    void initPWM(void);
    void setMotorPWM(struct DC_motor *m);
    void setMotorsFullSpeed(struct DC_motor *m_L, struct DC_motor *m_R);
    void stop(struct DC_motor *m_L, struct DC_motor *m_R);
    void delay(int ms); //not sure we need this anymore?
    void move(struct DC_motor *mL,struct DC_motor *mR, char dir);
    




#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

