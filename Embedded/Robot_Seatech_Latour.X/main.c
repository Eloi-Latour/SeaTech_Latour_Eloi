/* 
 * File:   main.c
 * Author: TABLE 6
 *
 * Created on 2 octobre 2024, 09:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"

int main(void) {
    /***********************************************************************************************/
    //Initialisation oscillateur
    /***********************************************************************************************/
    InitOscillator();
    InitTimer23();
    InitTimer1();
    InitPWM();
    InitADC1();
    //PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);

    /***********************************************************************************************/
    // Configuration des input et output (IO)
    /***********************************************************************************************/

    InitIO();
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;

    LED_BLANCHE_2 = 1;
    LED_BLEUE_2 = 1;
    LED_ORANGE_2 = 1;
    LED_ROUGE_2 = 1;
    LED_VERTE_2 = 1;

    /***********************************************************************************************/
    // Boucle Principale
    /***********************************************************************************************/

    while (1) {
    }
} // fin main

