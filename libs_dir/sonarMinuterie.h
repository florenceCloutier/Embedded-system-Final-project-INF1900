/******************************************************************************/
/*                                                                            */
/*      sonarMinuterie.h                                                      */
/*                                                                            */
/* Calcule la distance aux objets environnants                                */
/* Auteurs: Mohammed Ali Messedi, Felix Duguay,					              */
/*          Yazid Ben Said et Florence Cloutier                               */
/*			                                          			              */
/* Date: 2020-04-17						                                      */
/*			                                          					      */
/******************************************************************************/

#ifndef SONAR_H
#define SONAR_H

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Pilote.h"
#include "LCD.h"
#include "customprocs.h"

#define STARTTIMER 0
#define delai_initialisation_trigger 1
#define delai_mise_en_marche_trigger 10 
#define additionDist 255 
#define multipleDist 32
#define diviseurDist 5800

// Enumeration pour les manoeuvres possible
enum Manoeuvre {man1OkOkAttn, man2AttnOkOk, man3OkDngrDngr, man4DngrDngrOk, 
			    man5DngrDngrDngr, man6AttnOkAttn, aucune};					

/******************************************************************************/
/* class Sonar                                                                */
/*                                                                            */
/*      Declaration de la classe Sonar                                        */
/*                                                                            */
/******************************************************************************/
class Sonar {
public:
    void partirMinuterie(); // Methode qui part la minuterie 0
    void arreterMinuterie0();
    void setupSonarAvecMinuterie(); // Methode qui setup le sonar
    void sendTrigger(); // Methode qui envoie le trigfer
    float calculDistanceAvecMinuterie(uint8_t sonarId); // Calcul de la distance des objets
    
    Manoeuvre determinerQuelleManoeuvreEffectuer(float distanceGauche, float distanceAvant, float distanceDroite);
private:
    Manoeuvre manoeuvre;
};

#endif /* SONAR */
