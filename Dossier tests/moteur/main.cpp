#define F_CPU 8000000UL
#include "moteur.h"


int main(){
    Moteur moteur;
    moteur.ajustementPWM(10,90);
}

