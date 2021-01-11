/******************************************************************************/
/*                                                                            */
/* 		main.cpp			                              					  */
/*                                                                            */
/* Main du programme pour le robot en projet 1 INF1900              		  */
/* Auteurs: Mohammed Ali Messedi, Felix Duguay,								  */
/*          Yazid Ben Said et Florence Cloutier                           	  */
/*			                                          						  */
/* Date: 2020-04-17						                                      */
/*                                                                            */
/******************************************************************************/

/*********** Include de tous les .h ***********/
#include "../libs_dir/moteur.h"
#include "../libs_dir/afficheur_7seg.h"
#include "../libs_dir/Bouton.h"
#include "../libs_dir/del.h"
#include "../libs_dir/LCD.h"
#include "../libs_dir/sonarMinuterie.h"
#include "../libs_dir/customprocs.h"
#include "../libs_dir/Pilote.h"
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include <stdlib.h>
/**********************************************/

// Constantes pour le sonar
#define SONARGAUHE 0x01
#define SONARAVANT 0x04
#define SONARDROIT 0x02

// Constantes pour le TCNT2 prescale a 1024 (utilise pour l'affichage 7 seg)
#define temps4ParSeconde 100

// Constantes pour vitesses des roues
#define arret 0
#define vitesseRoueMax 90
#define vitesseRoue78 78
#define vitesseRoue66 66
#define vitesseRoue63 63
#define vitesseRoueMin 50
#define vitesseRoue52 52
#define vitesseRoue41 41

// Constante de temps pour manoeuvre 1 et 2
#define tempsRoueMan1Man2 39

// Constante de temps pour manoeuvre 5
#define tempsRoueMan5 66 

// Constante temps pour manoeuvre 6
#define tempsRoueMan6 56

// Constantes pour delay
#define temps100ms 1
#define temps125ms 1
#define temps200ms 1
#define temps500ms 200

// Constantes pour l'affichage LCD
#define LCD_DDR DDRB 
#define LCD_PORT PORTB // Le port occupé par l'afficheur LCD 
#define pos0LCD 0
#define pos1LCD 1
#define pos3LCD 3
#define pos6LCD 6
#define pos9LCD 9
#define pos12LCD 12
#define pos15LCD 15
#define longueurAffichage 3
#define tailleChaine 4
#define precision 1
#define nbDistances 3
#define temps1000ms 2000
#define temps2000ms 4000


// Declaration avec vitesses par defaut de l'afficheur 7 seg
volatile Afficheur7Seg affichage7Seg(00, 00);

/******************************************************************************/
/* ISR(TIMER2_OVF_vect)								                          */
/*                                                                            */
/*      L'interruption par overflow de la minuterie 2			              */
/*      Affiche les vitesses des roues sur l'afficheur 7 seg                  */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
ISR(TIMER2_OVF_vect) 
{								
	affichage7Seg.displaySeg(affichage7Seg.conversionVitesseEn7Seg());
	affichage7Seg.numSeg++;										// Incrementation du num de segment a afficher  												
	if (affichage7Seg.numSeg >= 8) {
		affichage7Seg.numSeg = 3;	// Pour rester dans l'intervalle des segments		
	}
}

int main()
{
	for (;;) {
		//Declaration des objets du robot
		Bouton bouton; 
		Sonar sonar;
		Moteur moteur;
		LCM lcd(&DDRB, &PORTB);
		Del delGauche;
		Del delDroite;
		Couleur couleurDelDroite;
		Couleur couleurDelGauche;
					
		Manoeuvre manoeuvre; 

		float distanceGauche = 0;
		float distanceDroite = 0;
		float distanceAvant = 0;

		/************************** Mode detection **************************/

		while (!bouton.RegarderSiAppuye()) {
			// Declaration de la size des tableux de char affiches
			char chaineGauche[tailleChaine];
			char chaineAvant[tailleChaine];
			char chaineDroite[tailleChaine];

			// Calcul des distances des objets environnants
			distanceGauche = sonar.calculDistanceAvecMinuterie(SONARGAUHE);
			distanceDroite = sonar.calculDistanceAvecMinuterie(SONARAVANT);
			distanceAvant = sonar.calculDistanceAvecMinuterie(SONARDROIT);
			
			float distances[nbDistances]={distanceGauche, distanceAvant, distanceDroite}; // Pour parcourir les distances

			uint8_t position = pos0LCD ; // La position sur l'ecran LCD
			
			// Affichage des distances aux objets environnants
			dtostrf(distanceGauche, longueurAffichage, precision, chaineGauche); 
			dtostrf(distanceAvant,  longueurAffichage, precision, chaineAvant);
			dtostrf(distanceDroite, longueurAffichage, precision, chaineDroite); 
			
			lcd[pos0LCD] = chaineGauche;
			lcd[pos3LCD] = "m";
			
			lcd[pos6LCD] = chaineAvant;
			lcd[pos9LCD] = "m";
			
			lcd[pos12LCD] = chaineDroite;
			lcd[pos15LCD] = "m";
			
			for (float& uneDistance : distances)  { 
				if (uneDistance < 1) {
					lcd[LCM_FW_HALF_CH + position] = "DNGR";
				}
				else if (uneDistance >= 1 && uneDistance < 3 ) {
					lcd[LCM_FW_HALF_CH + position] = "ATTN"; 
				}
				else {
					lcd[LCM_FW_HALF_CH + position] = " OK ";
				}
				position += pos6LCD ; // Pour centrer les differents affichage sur l'ecran
			} // Fin for
			_delay_ms(temps4ParSeconde); // Verification tous les 250ms
		} //Fin while

	/*************************** Mode manoeuvre ***************************/
	
		affichage7Seg.partirMinuterie2(); // Depart des interruptions pour l'affichage 7 seg

		// Determine quelle manoeuvre a effectuer selon les distances
		manoeuvre = sonar.determinerQuelleManoeuvreEffectuer(distanceGauche, distanceAvant, distanceDroite);
		
		// Switch case pour les differentes manoeuvres
		switch (manoeuvre) {
			
			case man1OkOkAttn: // Debut manoeuvre 1

				// Affichage LCD
				cli();
				lcd.clear();
				lcd[pos3LCD] = "Manoeuvre 1"; 
				sei();

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				for (int i=0; i<tempsRoueMan1Man2; i++) {
					// Changement vitesse moteurs
					moteur.ajustementPWM(vitesseRoueMax-i, vitesseRoueMax);
					// Affichage vitesses moteurs pendant 100 ms
					affichage7Seg.setVitesseRoueGauche(vitesseRoueMax - i);
					_delay_us(temps100ms); 	
					affichage7Seg.setVitesseRoueDroite(vitesseRoueMax);
					_delay_us(temps100ms);
				}
				
				// Affichage vitesses 52,90 pendant 1000ms
				_delay_us(temps1000ms);
			
				for (int j=0; j<tempsRoueMan1Man2; j++) {
					// Changement vitesse moteurs
					moteur.ajustementPWM(vitesseRoue52 + j, vitesseRoueMax);
					// Affichage vitesses moteurs pendant 100 ms
					affichage7Seg.setVitesseRoueGauche(vitesseRoue52 + j);
					_delay_us(temps100ms); 
					affichage7Seg.setVitesseRoueDroite(vitesseRoueMax);
					_delay_us(temps100ms);
				}

				// Affichage vitesses 90,90 pendant 2000ms
				_delay_us(temps2000ms);

				// Fin de la manoeuvre 1, arreter le robot 
				moteur.ajustementPWM(arret, arret);
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);

				// Changement couleurs dels ETEINT ETEINT
				couleurDelGauche = Couleur::ETEINT;
				couleurDelDroite = Couleur::ETEINT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);

				cli();
				lcd.clear();
				lcd[pos1LCD] = "Fin manoeuvre 1";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps1000ms);
				// Remise en mode detection
				
				break;
			/************************** Fin case man1 **************************/

			case man2AttnOkOk : // Debut manoeuvre 2

				// Affichage LCD
				cli();
				lcd.clear();
				lcd[pos3LCD] = "Manoeuvre 2"; 
				sei();

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				for (int i=0; i<tempsRoueMan1Man2; i++){
					// Changement vitesse moteurs
					moteur.ajustementPWM(vitesseRoueMax, vitesseRoueMax - i);
					// Affichage vitesses moteurs pendant 100 ms
					affichage7Seg.setVitesseRoueGauche(vitesseRoueMax);
					_delay_us(temps100ms); 
					affichage7Seg.setVitesseRoueDroite(vitesseRoueMax - i);
					_delay_us(temps100ms);												
				}	

				// Affichage vitesses 90,52 pendant 1000ms
				_delay_us(temps1000ms);
				
				for (int i=0; i<tempsRoueMan1Man2; i++){
					// Changement vitesse moteurs
					moteur.ajustementPWM(vitesseRoueMax, vitesseRoue52+ i );
					// Affichage vitesses moteurs pendant 100 ms
					affichage7Seg.setVitesseRoueGauche(vitesseRoueMax);
					_delay_us(temps100ms); 
					affichage7Seg.setVitesseRoueDroite(vitesseRoue52 + i);
					_delay_us(temps100ms);
				}	

				// Affichage vitesses 90,90 pendant 2000ms
				_delay_us(temps2000ms);

				// Fin de la manoeuvre 2, arreter le robot
				moteur.ajustementPWM(arret, arret);
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);

				// Changement couleurs dels ETEINT ETEINT
				couleurDelGauche = Couleur::ETEINT;
				couleurDelDroite = Couleur::ETEINT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);

				cli();
				lcd.clear();
				lcd[pos1LCD] = "Fin manoeuvre 2";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps1000ms);
				// Remise en mode detection

				break;
			/************************** Fin case man2 **************************/

			case man3OkDngrDngr : // Debut manoeuvre 3

				// Affichage LCD
				cli();
				lcd.clear();
				lcd[pos3LCD] = "Manoeuvre 3"; 
				sei();

				/****** Etape 1 ******/

				// Changement couleurs dels ROUGE VERT
				couleurDelGauche = Couleur::ROUGE;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesse moteurs -50,50
				moteur.ajustementPWM(-vitesseRoueMin, vitesseRoueMin);

				// Affichage vitesses -50,50 pendant 1000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoueMin);
				affichage7Seg.setVitesseRoueDroite(vitesseRoueMin);
				_delay_us(temps1000ms);
					
				/****** Etape 2 ******/

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);

				// Changement vitesse moteurs 66,66
				moteur.ajustementPWM(vitesseRoue66, vitesseRoue66);

				// Affichage vitesses 66,66 pendant 2000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoue66);
				affichage7Seg.setVitesseRoueDroite(vitesseRoue66);
				_delay_us(temps2000ms);

				/****** Etape 3 ******/

				// Changement couleurs dels VERT ROUGE
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::ROUGE;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesse moteurs 50,-50
				moteur.ajustementPWM(vitesseRoueMin, -vitesseRoueMin);
				
				// Affichage vitesses 50,-50 pendant 1000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoueMin);
				affichage7Seg.setVitesseRoueDroite(vitesseRoueMin);
				_delay_us(temps1000ms);

				/****** Etape 4 ******/

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesse moteurs 78,78
				moteur.ajustementPWM(vitesseRoue78, vitesseRoue78);

				// Affichage vitesses 78,78 pendant 2000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoue78);
				affichage7Seg.setVitesseRoueDroite(vitesseRoue78);
				_delay_us(temps2000ms);

				// Fin de la manoeuvre 3, arreter le robot
				moteur.ajustementPWM(arret, arret);	
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);

				// Changement couleurs dels ETEINT ETEINT
				couleurDelGauche = Couleur::ETEINT;
				couleurDelDroite = Couleur::ETEINT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);				
				
				cli();
				lcd.clear();
				lcd[pos1LCD] = "Fin manoeuvre 3";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps1000ms);
				// Remise en mode detection

				break;
			/************************** Fin case man3 **************************/

			case man4DngrDngrOk : // Debut manoeuvre 4

				// Affichage LCD 
				cli();		
				lcd.clear();		
				lcd[pos3LCD] = "Manoeuvre 4"; 
				sei();

				/****** Etape 1 ******/

				// Changement couleurs dels VERT ROUGE 
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::ROUGE;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);

				// Changement vitesse moteurs 50,-50
				moteur.ajustementPWM(vitesseRoueMin, -vitesseRoueMin);

				// Affichage vitesses 50,-50 pendant 1000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoueMin);
				affichage7Seg.setVitesseRoueDroite(vitesseRoueMin);
				_delay_us(temps1000ms);

				/****** Etape 2 ******/

				// Changement couleurs dels VERT VERT
				couleurDelDroite = Couleur::VERT;
				couleurDelGauche = Couleur::VERT;
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);

				// Changement vitesse moteurs 66,66
				moteur.ajustementPWM(vitesseRoue66, vitesseRoue66);

				// Affichage vitesses 66,66 pendant 2000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoue66);
				affichage7Seg.setVitesseRoueDroite(vitesseRoue66);
				_delay_us(temps2000ms);

				/****** Etape 3 ******/

				// Changement couleurs dels VERT ROUGE
				couleurDelGauche = Couleur::ROUGE;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesse moteurs -50,50
				moteur.ajustementPWM(-vitesseRoueMin, vitesseRoueMin);

				// Affichage vitesses -50,50 pendant 1000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoueMin);
				affichage7Seg.setVitesseRoueDroite(vitesseRoueMin);
				_delay_us(temps1000ms);

				/****** Etape 4 ******/

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesse moteurs 78,78
				moteur.ajustementPWM(vitesseRoue78, vitesseRoue78);

				// Affichage vitesses 78,78 pendant 2000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoue78);
				affichage7Seg.setVitesseRoueDroite(vitesseRoue78);
				_delay_us(temps2000ms);

				// Fin de la manoeuvre, arreter le robot
				moteur.ajustementPWM(arret, arret);	
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);

				// Changement couleurs dels ETEINT ETEINT
				couleurDelGauche = Couleur::ETEINT;
				couleurDelDroite = Couleur::ETEINT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);				
				
				cli();
				lcd.clear();
				lcd[pos1LCD] = "Fin manoeuvre 4";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps1000ms);
				// Remise en mode detection
				
				break;
			/************************** Fin case man4 **************************/

			case man5DngrDngrDngr : // Debut manoeuvre 5

				// Affichage LCD
				cli();
				lcd.clear();
				lcd[pos3LCD] = "Manoeuvre 5";
				sei();

				/****** Etape 1 ******/

				// Changement couleurs dels VERT ROUGE 
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::ROUGE;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesse moteurs 50,-50
				moteur.ajustementPWM(vitesseRoueMin, -vitesseRoueMin);

				// Affichage vitesses 50,-50 pendant 2000 ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoueMin);
				affichage7Seg.setVitesseRoueDroite(vitesseRoueMin);
				_delay_us(temps2000ms);

				// Remise a zero de la vitesse des roues
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);

				/****** Etape 2 ******/

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				for (int i=0; i<tempsRoueMan5; i = i + 3){
					// Changement vitesse moteurs
					moteur.ajustementPWM(arret+i, arret+i);
					// Affichage vitesses moteurs pendant 125 ms
					affichage7Seg.setVitesseRoueGauche(arret + i);
					_delay_us(temps125ms);
					affichage7Seg.setVitesseRoueDroite(arret + i);
					_delay_us(temps125ms);
				}	

				// Affichage vitesses 63,63 pendant 2000ms
				affichage7Seg.setVitesseRoueGauche(vitesseRoue63);
				affichage7Seg.setVitesseRoueDroite(vitesseRoue63);
				_delay_us(temps2000ms);

				// Fin de la manoeuvre, arreter le robot
				moteur.ajustementPWM(arret, arret);
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);	

				// Changement couleurs dels ETEINT ETEINT
				couleurDelGauche = Couleur::ETEINT;
				couleurDelDroite = Couleur::ETEINT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);

				cli();
				lcd.clear();
				lcd[1] = "Fin manoeuvre 5";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps1000ms);
				// Remise en mode detection

				break;
			/************************** Fin case man5 **************************/
			
			case man6AttnOkAttn : // Debut manoeuvre 6

				// Affichage LCD
				cli();
				lcd.clear();
				lcd[pos3LCD] = "Manoeuvre 6"; 
				sei();

				// Changement couleurs dels VERT VERT
				couleurDelGauche = Couleur::VERT;
				couleurDelDroite = Couleur::VERT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);		
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				// Changement vitesses roues a 90,90
				affichage7Seg.setVitesseRoueGauche(vitesseRoueMax);
				affichage7Seg.setVitesseRoueDroite(vitesseRoueMax);

				for (int i=0; i<tempsRoueMan6; i+=7){
					// Changement vitesse moteurs
					moteur.ajustementPWM(vitesseRoueMax-i, vitesseRoueMax-i);
					// Affichage vitesses moteurs pendant 500 ms
					affichage7Seg.setVitesseRoueGauche(vitesseRoueMax - i);
					_delay_us(temps500ms); 
					affichage7Seg.setVitesseRoueDroite(vitesseRoueMax - i);
					_delay_us(temps500ms);
				}	

				// Affichage vitesses 41,41 pendant 2000ms
				_delay_us(temps2000ms);

				// Fin de la manoeuvre, arreter le robot
				moteur.ajustementPWM(arret, arret);
				affichage7Seg.setVitesseRoueGauche(arret);
				affichage7Seg.setVitesseRoueDroite(arret);

				// Changement couleurs dels ETEINT ETEINT
				couleurDelGauche = Couleur::ETEINT;
				couleurDelDroite = Couleur::ETEINT;
				delGauche.changerCouleur(couleurDelGauche, IdDel::GAUCHE);
				delDroite.changerCouleur(couleurDelDroite, IdDel::DROITE);
				
				cli();
				lcd.clear();
				lcd[pos1LCD] = "Fin manoeuvre 6";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps1000ms);
				// Remise en mode detection

				break;
			/************************** Fin case man6 **************************/
			
			case aucune : // Lorsqu'aucun manoeuvre n'est a faire
				
				// Affichage LCD
				cli();
				lcd.clear();
				lcd[pos1LCD] = "Combinaison non evaluee";
				affichage7Seg.arreterMinuterie2();
				_delay_ms(temps2000ms);
				// Remise en mode detection

				break;
		} // Fin switch
	} // Fin for(;;)
} // Fin main
