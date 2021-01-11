#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "Pilote.h"
#include "LCD.h"
#include "customprocs.h"

#define delai_initialisation_trigger 2
#define delai_mise_en_marche_trigger 10 

class Sonar {

public:

    void sendTrigger();

    void partirMinuterie();

    void setupSonarParScrutation();
    float calculDistanceParScrutation(uint8_t sonarId);
  
};


