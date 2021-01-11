/*************************************************************************************
*Fichier: moteur.h
*Auteurs: Florence Cloutier, Felix Duguay, Mohammed Ali Messedi, Yazid Ben Said                                                                       
*Date: 9 mars 2020
*Description: Fichier d'entetes pour la construction autour de la classe Moteur et implementation de ses methodes                                      
*************************************************************************************/

#ifndef MOTEUR_H
#define MOTEUR_H
#define F_CPU 8000000UL // 8 MHz
#define PUSHED_BUTTON (PIND & 0x04)
#define DELAY_10us 10

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/interrupt.h>



//Differents etats que peut prendre les moteurs

const uint8_t MOTEUR_OUVERT = 0x01, MOTEUR_ETEINT = 0x00;

// Classe Moteur qui est implementee dans le moteur.cpp

class Moteur
{
public:
   Moteur();
   ~Moteur();

   void delaiVariableUs(uint16_t temps);
   void ajustementPWM(float roueGauche, float roueDroite);

};

#endif 
