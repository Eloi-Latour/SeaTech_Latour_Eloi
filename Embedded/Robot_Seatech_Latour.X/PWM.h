#ifndef PWM_H
#define PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, int NumMot);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, char Moteur);


#endif /* PWM_H */