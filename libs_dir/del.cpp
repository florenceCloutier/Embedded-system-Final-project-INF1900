/*************************************************************************************
*Fichier: del.cpp
*Auteurs: Florence Cloutier, Felix Duguay, Mohammed Ali Messedi, Yazid Ben Said                                                                       
*Date: 9 mars 2020
*Description: Construction autour de la classe Del et implementation de ses methodes                                      
*************************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h> 
#include "del.h"

/******************************************************************************/
/*                                                                            */
/*      Constructeur: Construit un objet de la classe Del avec des attributs  */
/*                    par defaut.                                             */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
Del::Del()
{
   DDRD |= (1<<0) | (1<<1) | (1<<2) | (1<<7); // Port D0 et D1 en sortie
}


/******************************************************************************/
/*      Destructeur: ne fait rien                                             */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
Del::~Del()
{}


/******************************************************************************
 *                                                                            
 *      Methode qui change la couleur de la Del pour la couleur passee en parametre       
 *                                                                            
 * Parametre d'entree  : Couleur couleur                                                
 * Parametre de sortie : aucun                                                
 *******************************************************************************/
void Del::changerCouleur(Couleur couleur, IdDel idDel)
{
   if (idDel == IdDel::GAUCHE && couleur == Couleur::ROUGE){
		PORTD |= (1<<1);
		PORTD &= 0b11111110;
	} else if (idDel == IdDel::GAUCHE && couleur == Couleur::VERT){
		PORTD |= (1<<0);
		PORTD &= 0b11111101;
	} else if (idDel == IdDel::GAUCHE && couleur == Couleur::ETEINT){
		PORTD &= 0b11111100;
	} else if (idDel == IdDel::DROITE && couleur == Couleur::VERT){
		PORTD |= (1<<2);
		PORTD &= 0b01111111;
	} else if (idDel == IdDel::DROITE && couleur == Couleur::ROUGE){
		PORTD |= (1<<7);
		PORTD &= 0b11111011;
	} else if (idDel == IdDel::DROITE && couleur == Couleur::ETEINT){
		PORTD &= 0b01111011;
	}
}

