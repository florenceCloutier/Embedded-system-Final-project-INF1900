#define F_CPU 8000000UL

#define SONAR1 0x01
#define SONAR2 0x02
#define SONAR3 0x04



#include "afficheur_7seg.h"

#define arret 0

volatile Afficheur7Seg affichage7Seg(-5,5);
int main() 
{

	affichage7Seg.partirMinuterie2();
	
	for (;;){

		

 	//affichage7Seg.display_seg(6,affichage7Seg.conversionVitesseEn7Seg(3));
	//uint8_t minuterie2Expiree = 0;

	//_delay_ms(5);
	}
	// while(Afficheur7Seg::nbOvf != 1000)
	// {
	// 	//
	
	// };
}

/******************************************************************************/
/*      ISR(TIMER2_COMPB_vect)                                                */
/*                                                                            */
/*      Interruption du timer 2 sur comparaison avec OCR2B                    */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/

 

ISR(TIMER2_OVF_vect) 
 {
    
    //Afficheur7Seg::nbOvf ++;
	

    //display_seg(counter, digits_seg[counter-3]);
    affichage7Seg.display_seg(affichage7Seg.conversionVitesseEn7Seg());
	affichage7Seg.numSeg++;
    if (affichage7Seg.numSeg >= 9) affichage7Seg.numSeg = 3;

 }