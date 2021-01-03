/******************************************************************************/
/*                                                                            */
/*      sonarMinuterie.cpp                                                    */
/*                                                                            */
/* Envoi le trigger et recoit l'echo pour calculer                            */
/* la distance entre le robot et des objets environnants                      */
/*                                                                            */
/******************************************************************************/

#define F_CPU 8000000UL

#include "sonarMinuterie.h"

using namespace std;

// Variables globales volatiles et non-volatiles utilisees dans l'interruption et pour l'utilisation  // du timer
volatile uint8_t up = 0; // Pour incrementer le compteur seulement lorsque voulu
volatile uint8_t compteur = 0; // Le compteur que l'on incremente pour calculer la distance
uint8_t timer = STARTTIMER; // Variable pour gerer le TCNT0

/******************************************************************************/
/* void Sonar::partirMinuterie()                                              */
/*                                                                            */
/*      Initialise et part le timer/counter 0 du ATMega au mode voulue        */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Sonar::partirMinuterie() 
{
    cli(); // Arret des interruptions
    TCNT0 = STARTTIMER; // Initialise le timer/counter 0 a 0
    TCCR0B |= (1  << CS02); // prescale a 256
    TIMSK0 |= (1 << TOIE0); // Masque
    sei(); // Permet les interruptions
}

void Sonar::arreterMinuterie0() 
{
    cli(); // Arret des interruptions

    TCCR0B |= (0  << CS02); 
    TIMSK0 |= (0 << TOIE0); // Arret des interruptions par overflow
    
    sei(); // Permet les interruptions
}

/******************************************************************************/
/* ISR(TIMER0_OVF_vect         )                                              */
/*                                                                            */
/*      Interruption generee par le overflow du timer/counter 0               */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
ISR(TIMER0_OVF_vect)
{
    if(up){
        compteur++; // augmente a chaque 0.0001s
    }
}

/******************************************************************************/
/* void Sonar::sendTrigger()                                                  */
/*                                                                            */
/*      Envoie le trigger a partir d'un sonar avec les delais voulus          */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Sonar::sendTrigger()
{
    PORTB = 0x00; // S'assure qu'aucun trigger est envoye
    _delay_us(delai_initialisation_trigger);
    PORTB = 0x01; // Envoi du trigger
    _delay_us(delai_mise_en_marche_trigger); // Duree de 10 micro-secondes de l'envoi
    PORTB = 0x00; // Arret de l'envoi du trigger
}

/******************************************************************************/
/* void Sonar::setupSonarAvecMinuterie()                                      */
/*                                                                            */
/*      Setup les ports utilises par la minuterie                             */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void Sonar::setupSonarAvecMinuterie()
{
    DDRA = 0x00; //Port A en entrée
    DDRB = 0xFF; //Port B en sortie 
}

/******************************************************************************/
/* void Sonar::calculDistanceAvecMinuterie(uint8_t sonarId)                   */
/*                                                                            */
/*      Calcul de la distance entre le robot et les objets environnants       */
/*                                                                            */
/* Parametre d'entree  : sonarId                                              */
/* Parametre de sortie : distanceFinale                                       */
/******************************************************************************/
float Sonar::calculDistanceAvecMinuterie(uint8_t sonarId)
{
    uint8_t distance = 0; // Verifie que la distance initiale est de zero

    Sonar::sendTrigger(); // Envoi du trigger

    while ((PINA & (sonarId)) == 0) {} // Attente tant que le signal echo est à 0
 
    up = 1; // Si le signal echo passe à 1..

    partirMinuterie(); // Depart de la minuterie
    
    while ((PINA & (sonarId)) == sonarId) // Incrementation du cptr tant que la pin est a 1
    {
        distance = compteur;
        timer = TCNT0;
    }

    up = 0; //  Fin du signal echo
    
    float distanceFinale = (float)((timer+additionDist*distance)*multipleDist)/diviseurDist; // Calcul de la distance
    
    // Reinitialise les variables pour le calcul de distance
    compteur = 0;
    distance = 0;
    timer = STARTTIMER;

    return distanceFinale;
}

/******************************************************************************/
/* Manoeuvre Sonar::determinerQuelleManoeuvreEffectuer(float distanceGauche,  */
/*                  float distanceAvant, float distanceDroite)                */
/*                                                                            */
/*      Determine la manoeuvre a effectuer                                    */
/*                                                                            */
/* Parametre d'entree  : float distanceGauche, float distanceAvant            */
/*                       float distanceDroite)                                */
/* Parametre de sortie : Manoeuvre                                            */
/******************************************************************************/
Manoeuvre Sonar::determinerQuelleManoeuvreEffectuer(float distanceGauche, float distanceAvant, float distanceDroite)
{
    if(distanceGauche >= 3 && distanceAvant >= 3 && distanceDroite >= 1 && distanceDroite < 3)
    {
        manoeuvre = man1OkOkAttn ;	
    }
    else if(distanceGauche >= 1 && distanceGauche < 3 && distanceAvant >= 3 && distanceDroite >= 3)
    {
        manoeuvre = man2AttnOkOk ;
    }
    else if(distanceGauche >= 3 && distanceAvant < 1 && distanceDroite < 1)
    {
        manoeuvre = man3OkDngrDngr ;
    }
    else if(distanceGauche < 1 && distanceAvant < 1 && distanceDroite >= 3)
    {
        manoeuvre = man4DngrDngrOk ;
    }
    else if(distanceGauche < 1 && distanceAvant < 1 && distanceDroite < 1)
    {
        manoeuvre = man5DngrDngrDngr ;
    }
    else if(distanceGauche >= 1 && distanceGauche < 3 && distanceAvant >= 3 && distanceDroite >= 1 && distanceDroite < 3)
    {
        manoeuvre = man6AttnOkAttn ;
    }
    else
    {
        manoeuvre = aucune;
    }
    return manoeuvre;
}

