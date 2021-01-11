
#ifndef BOUTON_H
#define BOUTON_H

#define F_CPU 8000000UL

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

class Bouton {
public:
	Bouton();
	bool RegarderSiAppuye();
private:
	int Appuye_;
};
#endif
