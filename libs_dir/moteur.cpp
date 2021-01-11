/*************************************************************************************
*Fichier: moteur.cpp
*Auteurs: Florence Cloutier, Felix Duguay, Mohammed Ali Messedi, Yazid Ben Said                                                                       
*Date: 9 mars 2020
*Description: Construction autour de la classe Moteur et implementation de ses methodes                                      
*************************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "moteur.h"

// Enumeration pour le sens dans lequel se deplace le robot
enum class Direction
{
  AVANCE,
  TOURNESURPLACE,
  ARRET
};

/******************************************************************************/
/*                                                                            */
/*      Constructeur: Construit un objet de la classe Moteur avec des attributs  */
/*                    par defaut.                                             */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
Moteur::Moteur()
{
  DDRD |= (1 << 4) | (1<<5); // DDRD en mode sortie

  TCNT1 = STARTTIMER;

  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); //
	TCCR1B |= (1 << CS11);
	TCCR1C = 0;
}


/******************************************************************************/
/*      Destructeur: ne fait rien                                             */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
Moteur::~Moteur()
{}


/******************************************************************************
 *                                                                            
 *      Méthode qui ajuste l'onde PWM de façon matérielle
 *                                                                            
 * Parametre d'entree  : float rouGauche, float roueDroite                      
 * Parametre de sortie : aucun                                                
 *******************************************************************************/
void Moteur::ajustementPWM(float roueGauche, float roueDroite) 
{
  float pourcentageGauche = roueGauche / 100.0;
	float pourcentageDroite = roueDroite / 100.0;
  PORTD |= (1 << portD5); // D5 en sortie
  PORTD |= (1 << portD4); // D4 en sortie
  OCR1B = fabs(pourcentageGauche * maxTimer); // Valeur roue gauche a comparer
  OCR1A = fabs(pourcentageDroite * maxTimer); // Valeur roue droite a comparer
}





