/*************************************************************************************
*Fichier: moteur.h
*Auteurs: Florence Cloutier, Felix Duguay, Mohammed Ali Messedi, Yazid Ben Said                                                                       
*Date: 9 mars 2020
*Description: Fichier d'entetes pour la construction autour de la classe Moteur et implementation de ses methodes                                      
*************************************************************************************/

#ifndef MOTEUR_H
#define MOTEUR_H

#define F_CPU 8000000UL // 8 MHz

#define STARTTIMER 0 
#define portD4 4
#define portD5 5
#define maxTimer 255

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/interrupt.h>


// Classe Moteur qui est implementee dans le moteur.cpp
class Moteur
{
public:
   Moteur();
   ~Moteur();

   void ajustementPWM(float roueGauche, float roueDroite);
};

#endif 
