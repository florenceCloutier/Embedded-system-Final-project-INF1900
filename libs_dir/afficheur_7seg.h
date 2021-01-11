/******************************************************************************/
/*                                                                            */
/*      afficheur_7seg.h                                                      */
/*                                                                            */
/* Affiche la vitesse des roues sur l'écran                                   */
/* Auteurs: Mohammed Ali Messedi, Felix Duguay,								         */
/*          Yazid Ben Said et Florence Cloutier                               */
/*			                                          						         */
/* Date: 2020-04-17						                                          */
/*			                                          						         */
/******************************************************************************/


#ifndef AFFICHEUR7SEG_H
#define AFFICHEUR7SEG_H

#include <avr/interrupt.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <stdlib.h>

// Define pour les methodes de l'afficheur 7seg
#define STARTTIMER 0
#define tailleTabValeurs7Seg 11
#define valeur07Seg 0b11111100
#define valeur17Seg 0b01100000
#define valeur27Seg 0b11011010
#define valeur37Seg 0b11110010
#define valeur47Seg 0b01100110
#define valeur57Seg 0b10110110
#define valeur67Seg 0b10111110
#define valeur77Seg 0b11100000
#define valeur87Seg 0b11111110
#define valeur97Seg 0b11100110
#define TIRET 0b00000010
#define indexTiret 10
#define pos0seg 3
#define pos3seg 6
#define pos1seg 4
#define pos4seg 7
/******************************************************************************/
/* class Afficheur7Seg                                                        */
/*                                                                            */
/*      Declaration de la classe Afficheur7Seg                                */
/*                                                                            */
/******************************************************************************/
class Afficheur7Seg
{
public:
   // Constructeur par parametres 
   Afficheur7Seg(int vitesseRoueGauche, int vitesseRoueDroite);
   
   // Methodes pour l'affiche sur 7 segments
   uint8_t conversionVitesseEn7Seg() volatile;
   void partirMinuterie2() volatile; 
   void arreterMinuterie2() volatile;
   void enableSeg() volatile;
   void displaySeg(uint8_t vitesse) volatile;

   // Setters des vitesses
   void setVitesseRoueGauche(int vitesseRoueGauche) volatile;
   void setVitesseRoueDroite(int vitesseRoueDroite) volatile;
   
   uint8_t numSeg; // Numero du segment a afficher

private:
   int vitesseRoueGauche_; 
   int vitesseRoueDroite_;
};

#endif /* AFFICHEUR7SEG */
