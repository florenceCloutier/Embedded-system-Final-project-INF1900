#define F_CPU 8000000UL

#define SONAR1 0x01
#define SONAR2 0x02
#define SONAR3 0x04

#include <avr/io.h>
#include <util/delay.h>
#include <util/delay.h>
#include <stdlib.h>

#include "sonarScrutation.h"
#include "LCD.h"
#include "customprocs.h"
#include "Pilote.h"

int main() 
{
	LCM LCD1(&DDRB, &PORTB);
	Sonar sonar;

	float distanceSonar1 = sonar.calculDistanceParScrutation(SONAR1);
	float distanceSonar2 = sonar.calculDistanceParScrutation(SONAR2);
	float distanceSonar3 = sonar.calculDistanceParScrutation(SONAR3);

	char chaine1[4];
	char chaine2[4];
	char chaine3[4];

	dtostrf(distanceSonar1, 4, 1 , chaine1); 
	dtostrf(distanceSonar2, 4, 1 , chaine2);
	dtostrf(distanceSonar3, 4, 1 , chaine3); 


	LCD1[0] = chaine1; 
	LCD1[6] = chaine2;
	LCD1[12] = chaine3;
}