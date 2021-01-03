/******************************************************************************/
/*                                                                            */
/*      afficheur_7seg.cpp                                                    */
/*                                                                            */
/* Affiche la vitesse des roues sur l'écran                                   */
/* Auteurs: Mohammed Ali Messedi, Felix Duguay,								  */
/*          Yazid Ben Said et Florence Cloutier                               */
/*			                                          						  */
/* Date: 2020-04-17						                                      */
/*			                                          						  */
/******************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "afficheur_7seg.h"
#include "../libs_dir/LCD.h"
#include "../libs_dir/customprocs.h"
#include "../libs_dir/Pilote.h"

// Declaration de variables globales utilisees dans les methodes de l'afficheur 7seg
LCM lcd(&DDRB, &PORTB);
uint8_t valeurs7Seg[tailleTabValeurs7Seg];

/******************************************************************************/
/* Afficheur7Seg::Afficheur7Seg(int vitesseRoueGauche, int vitesseRoueDroite) */
/*                                                                            */
/*      Constructeur par parametres d'afficheur 7 segments                    */
/*                                                                            */
/* Parametre d'entree  : int vitesseRoueGauche                                */
/* Parametre de sortie : int vitesseRoueDroite                                */
/******************************************************************************/
Afficheur7Seg::Afficheur7Seg(int vitesseRoueGauche, int vitesseRoueDroite) 
    : numSeg(pos0seg),
    vitesseRoueGauche_(vitesseRoueGauche), 
    vitesseRoueDroite_(vitesseRoueDroite) 
{
    DDRA |= 0xF8; // A3, A4, A5, A6, A7 en sortie
    DDRC = 0xFF; // Port C en sortie
    PORTA |= 0xF8; // Tous les segments eteints
    PORTC = 0x00; // Tous les segments eteints
}

/******************************************************************************/
/* void Afficheur7Seg::partirMinuterie2() volatile                            */
/*                                                                            */
/*      Part et initialise la minuterie2                                      */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Afficheur7Seg::partirMinuterie2() volatile
{ 
    cli();                  // Arret interruptions                      

    TCNT2 = STARTTIMER ;    // Depart du timer a zero
    TCCR2B |= (1 << CS20);  // Prescale a 256
    TIMSK2 |= (1 << TOIE2); // Permet les interruptions au overflow  

    sei();                  // Permet les interruptions
}

/******************************************************************************/
/* void Afficheur7Seg::arreterMinuterie2() volatile                           */
/*                                                                            */
/*      Arrete la minuterie2 pour pouvoir utiliser le LCD                     */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Afficheur7Seg::arreterMinuterie2() volatile
{
    cli();                  // Arret interruptions

    TCCR2B &= (0<<CS20);
    TIMSK2 &= (0<< TOIE2);  // Arret des interruptions au overflow
    
    
    sei();                  // Permet les interruptions

    PORTC = 0x00;
}       

/******************************************************************************/
/* uint8_t Afficheur7Seg::conversionVitesseEn7Seg() volatile                  */
/*                                                                            */
/*      Converti la valeur decimale de la vitesse de la roue en sorties       */
/*      pour l'afficheur 7segments                                            */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : uint8_t                                              */
/******************************************************************************/
uint8_t Afficheur7Seg::conversionVitesseEn7Seg() volatile
{
    // Variables locales et de retour pour la conversion
    int vitesseRoue;
    uint8_t vitesseConvertie = 0;    // Variable de retour

    // Tableau de conversion d'un chiffre en valeur 7 segments
    valeurs7Seg[0] = valeur07Seg;
    valeurs7Seg[1] = valeur17Seg;
    valeurs7Seg[2] = valeur27Seg;
    valeurs7Seg[3] = valeur37Seg;
    valeurs7Seg[4] = valeur47Seg;
    valeurs7Seg[5] = valeur57Seg;
    valeurs7Seg[6] = valeur67Seg;
    valeurs7Seg[7] = valeur77Seg;
    valeurs7Seg[8] = valeur87Seg;
    valeurs7Seg[9] = valeur97Seg;
    valeurs7Seg[indexTiret] = TIRET;

    // Valeur absolue des vitesses
    if (vitesseRoueDroite_<0) {vitesseRoueDroite_*=-1;}
    if (vitesseRoueGauche_<0) {vitesseRoueGauche_*=-1;}

    // Chiffre aux dizaines ou aux unites selon la position que l'on affiche
    if (numSeg == pos0seg) {vitesseRoue = vitesseRoueGauche_/10;}
    else if(numSeg == pos3seg) {vitesseRoue = vitesseRoueDroite_/10;}
    else if (numSeg == pos1seg) {vitesseRoue = vitesseRoueGauche_%10;}
    else if ( numSeg == pos4seg) {vitesseRoue = vitesseRoueDroite_%10;}
    else if ( numSeg == 5)
    { 
        vitesseConvertie = valeurs7Seg[indexTiret]; 
        return vitesseConvertie;
    }
    else return 0;
    // Vitesse convertie
    vitesseConvertie = valeurs7Seg[vitesseRoue];
    return vitesseConvertie;
}

/******************************************************************************/
/* void Afficheur7Seg::enableSeg() volatile                                   */
/*                                                                            */
/*      Enable le PortA voulu pour afficher un numero                         */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Afficheur7Seg::enableSeg() volatile
{
    PORTA |= 0b11111000;            // Arret de tous les ports de l'afficheur 7seg 
    
    if(numSeg >= pos0seg && numSeg <= pos4seg)
    {
        PORTA &= ~(1 << numSeg);    // Enable la position d'affichage voulue
    }
}

/******************************************************************************/
/* void Afficheur7Seg::displaySeg(uint8_t vitesse) volatile                   */
/*                                                                            */
/*      Affiche le numero voulu a la position voulue sur le 7seg              */
/*                                                                            */
/* Parametre d'entree  : uint8_t vitesse                                      */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Afficheur7Seg::displaySeg(uint8_t vitesse) volatile
{
    PORTC = 0x00;
    
    enableSeg();   
    
    PORTC = vitesse; // Affiche le numero voulu a la position voulue
}

/******************************************************************************/
/* void Afficheur7Seg::setVitesseRoueGauche(int vitesseRoueGauche) volatile   */
/*                                                                            */
/*      Setter de la vitesse de la roue gauche                                */
/*                                                                            */
/* Parametre d'entree  : int vitesseRoueGauche                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Afficheur7Seg::setVitesseRoueGauche(int vitesseRoueGauche) volatile
{
    vitesseRoueGauche_ = vitesseRoueGauche;
}

/******************************************************************************/
/* void Afficheur7Seg::setVitesseRoueDroite(int vitesseRoueDroite) volatile   */
/*                                                                            */
/*      Setter de la vitesse de la roue droite                                */
/*                                                                            */
/* Parametre d'entree  : int vitesseRoueDroite                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Afficheur7Seg::setVitesseRoueDroite(int vitesseRoueDroite) volatile
{
    vitesseRoueDroite_ = vitesseRoueDroite;
}