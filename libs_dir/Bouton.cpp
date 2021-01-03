
#include "Bouton.h"

/*************************************************
Nom: Bouton()
Fonction: Constructeur de la classe Bouton
**************************************************/
Bouton::Bouton() {
	Appuye_ = 0x08;
}

/*************************************************
Nom: Regarder si appuye
Fonction: V�rifie si le bouton D est activ�.
Debounce int�gr�.
**************************************************/
bool Bouton::RegarderSiAppuye() {

	if (PIND & Appuye_) { 		//
		_delay_ms(10);			// Debounce
		if (PIND & Appuye_) {		//
			return false;
		}
	}
	return true;
}


