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
//#include <cmath>

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
	OCR1A = 0;
	OCR1B = 0;

  TCNT1 = 0;

  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); //Table 16-4.
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
 *      Methode qui provoque un delai
 *                                                                            
 * Parametre d'entree  : uint16_t temps                                                
 * Parametre de sortie : aucun                                                
 *******************************************************************************/
void Moteur::delaiVariableUs(uint16_t temps)
{
  for(uint16_t i = 0 ; i < temps/10; i++)
    _delay_us(DELAY_10us);
}

/******************************************************************************
 *                                                                            
 *      Méthode qui ajuste l'onde PWM de façon matérielle
 *                                                                            
 * Parametre d'entree  : uint8_t freq                      
 * Parametre de sortie : aucun                                                
 *******************************************************************************/
void Moteur::ajustementPWM(float roueGauche, float roueDroite) 
{
  float pourcentageGauche = roueGauche/100;
	float pourcentageDroite = roueDroite/100;
  PORTD |= (1<<5);
  PORTD |= (1<<4);
  OCR1A = fabs(pourcentageDroite*255);
  OCR1B = fabs(pourcentageGauche*255);
  
}





