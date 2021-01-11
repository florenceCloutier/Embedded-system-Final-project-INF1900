#define F_CPU 8000000UL

#include "sonarScrutation.h"

using namespace std;

 volatile uint8_t up = 0;
 volatile uint8_t compteur = 0; 
 uint8_t timer = 0;


void Sonar::partirMinuterie () {
    cli();
    TCNT0 = 0;
    
    TCCR0B |= (1  << CS02); // prescale a 256
    TIMSK0 |= (1 << TOIE0);

    sei();
}


ISR(TIMER0_OVF_vect){
    PORTD = 0x01;
   
    if(up){
        compteur++; // augmente a chaque 0.0001s
    }
}


void Sonar::sendTrigger()
{
    PORTB = 0x00;
    _delay_us(1);
    PORTB = 0x01;
    _delay_us(10);
    PORTB = 0x00;
}

void Sonar::setupSonarParScrutation()
{
    DDRA = 0x00; //Port A en entrée
    DDRB = 0xff; //Port B en sortie 
}

float Sonar::calculDistanceParScrutation(uint8_t sonarId)
{
    uint8_t distance = 0;

    LCM affichage(&DDRB , &PORTB);

    affichage << "Envoi du trigger";
    _delay_ms(500);
    
    Sonar::sendTrigger();

    while ((PINA & (sonarId)) == 0) //on attend tant que le signal est à 0,
    {
        affichage.write("Attente");
    }
                     // si le signal passe à 1..

    up = 1;

    partirMinuterie();
    
    //affichage.write("Echo");
    
    
    while ((PINA & (sonarId)) == sonarId) //.. on compte tant que la valeur lue sur la pin est 1
    {
        distance = compteur;
        timer = TCNT0;
    }

    up = 0;

    affichage.write("Fin echo");
    
    _delay_ms(500);

    affichage.clear();
    
    float distanceFinale = (float)((timer+255*distance)*32)/5800 + 0.1 ;
    
    compteur = 0;
    distance = 0;
    timer = 0;

    return distanceFinale;



}

