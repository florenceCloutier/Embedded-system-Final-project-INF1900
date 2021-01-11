#ifndef AFFICHEUR7SEG_H
#define AFFICHEUR7SEG_H

#include <avr/interrupt.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <stdlib.h>

class Afficheur7Seg
{
public:
   Afficheur7Seg();
   Afficheur7Seg(int vitesseRoueGauche, int vitesseRoueDroite) : vitesseRoueGauche_(vitesseRoueGauche), vitesseRoueDroite_(vitesseRoueDroite), numSeg(3)
   {
       DDRA |= 0xF8;
       DDRC = 0xFF;
       PORTA |= 0xF8;
       PORTC = 0x00;
   }
   //void afficherVitesseRoues(uint8_t vitesseRoueGauche, uint8_t vitesseRoueDroite);
   uint8_t conversionVitesseEn7Seg() volatile;
   //void TCNT2_init();
   void partirMinuterie2() volatile; 
   // retourne la valeur numerique correspondant a la valeur
   // analogique sur le port A.  pos doit etre entre 0 et 7
   // inclusivement.  Seulement les 10 bits de poids faible
   // sont significatifs.

   void enable_seg() volatile;
   void display_seg(uint8_t number) volatile;
   uint8_t numSeg;
private:
   int vitesseRoueGauche_;
   int vitesseRoueDroite_;

   

};

#endif /* AFFICHEUR7SEG */
