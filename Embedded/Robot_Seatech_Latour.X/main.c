
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
#include "ADC.h"
#include "Robot.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"

int main(void) {
    /***********************************************************************************************/
    //Initialisation oscillateur
    /***********************************************************************************************/
    InitOscillator();
    InitIO();
    InitTimer23();
    InitTimer1();
    InitTimer4();
    InitPWM();
    InitADC1();
    InitUART();
    //PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);

    /***********************************************************************************************/
    // Configuration des input et output (IO)
    /***********************************************************************************************/

    /*
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
     */
    /***********************************************************************************************/
    // Boucle Principale
    /***********************************************************************************************/
    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreGauche2 = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreDroit2 = 34 / volts - 5;

            if (robotState.distanceTelemetreGauche < 30) {
                LED_BLEUE_1 = 1;
            } else {
                LED_BLEUE_1 = 0;
            }

            if (robotState.distanceTelemetreCentre < 30) {
                LED_ORANGE_1 = 1;
            } else {
                LED_ORANGE_1 = 0;
            }

            if (robotState.distanceTelemetreGauche2 < 30) {
                LED_BLANCHE_1 = 1;
            } else {
                LED_BLANCHE_1 = 0;
            }

            if (robotState.distanceTelemetreDroit2 < 30) {
                LED_VERTE_1 = 1;
            } else {
                LED_VERTE_1 = 0;
            }

            if (robotState.distanceTelemetreDroit < 30) {
                LED_ROUGE_1 = 1;
            } else {
                LED_ROUGE_1 = 0;
            }
        }
        SendMessage("Bonjour", 7);
        //SendMessageDirect((unsigned char*) "Bonjour", 7);
        __delay32(4000000);

    }
} // fin main

unsigned char stateRobot;

void OperatingSystemLoop(void) {

    if (timestamp > 60000) {
        PWMSetSpeedConsigne(0, MOTEUR_DROIT);
        PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
        stateRobot = STATE_ATTENTE;
    } else {
        switch (stateRobot) {
            case STATE_ATTENTE:
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_ATTENTE_EN_COURS;
            case STATE_ATTENTE_EN_COURS:
                //if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
                break;
            case STATE_AVANCE:
                PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
                PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
                stateRobot = STATE_AVANCE_EN_COURS;
                break;
            case STATE_AVANCE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_TOURNE_GAUCHE:
                PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-5, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_TOURNE_GAUCHE_PLUS:
                PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_PLUS_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_TOURNE_DROITE:
                PWMSetSpeedConsigne(5, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_TOURNE_DROITE_PLUS:
                PWMSetSpeedConsigne(10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_PLUS_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_TOURNE_SUR_PLACE_GAUCHE:
                PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_TOURNE_SUR_PLACE_DROITE:
                PWMSetSpeedConsigne(10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            case STATE_RECULE:
                PWMSetSpeedConsigne(30, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
                stateRobot = STATE_RECULE_EN_COURS;
                break;
            case STATE_RECULE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            default:
                stateRobot = STATE_ATTENTE;
                break;
        }
    }
}
unsigned char nextStateRobot = 0;


void SetNextRobotStateInAutomaticMode() {
    //Determination de la position des obstacles en fonction des ééètlmtres
    unsigned char sensorState = 0b00000;
    if (robotState.distanceTelemetreDroit2 < 30)
        sensorState |= 0b10000;
    if (robotState.distanceTelemetreDroit < 35)
        sensorState |= 0b01000;
    if (robotState.distanceTelemetreCentre < 45)
        sensorState |= 0b00100;
    if (robotState.distanceTelemetreGauche < 35)
        sensorState |= 0b00010;
    if (robotState.distanceTelemetreGauche < 30)
        sensorState |= 0b00001;
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    
    switch (sensorState){
        case 0b00000:
            positionObstacle = PAS_D_OBSTACLE;
            break;
        case 0b10000:
            positionObstacle = OBSTACLE_A_DROITE_EXTREME;
            break;
        case 0b01000:
            positionObstacle = OBSTACLE_A_DROITE;
            break;
        case 0b00100:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b00010:
            positionObstacle = OBSTACLE_A_GAUCHE;
            break;
        case 0b00001:
            positionObstacle = OBSTACLE_A_GAUCHE_EXTREME;
            break;
        case 0b11000:
            positionObstacle = OBSTACLE_A_DROITE;
            break;
        case 0b10100:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b10010:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b10001:
            positionObstacle = PAS_D_OBSTACLE;
            break;
        case 0b01100:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b01010:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b01001:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b00110:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b00101:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b00011:
            positionObstacle = OBSTACLE_A_GAUCHE;
            break;
        case 0b11100:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b11010:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b11001:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b10110:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b10101:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b10011:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b01110:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b01101:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b01011:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b00111:
            positionObstacle = OBSTACLE_EN_FACE;
            break;
        case 0b11110:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b11101:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b11011:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b10111:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b01111:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
        case 0b11111:
            positionObstacle = OBSTACLE_PARTOUT;
            break;
            
    }

    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE_PLUS;
    else if (positionObstacle == OBSTACLE_A_DROITE_EXTREME)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE_PLUS;
    else if (positionObstacle == OBSTACLE_A_GAUCHE_EXTREME)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_PARTOUT)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;



    //Si l?on n?est pas dans la transition de lé?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}
