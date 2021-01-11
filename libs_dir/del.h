/*************************************************************************************
*Fichier: del.h
*Auteurs: Florence Cloutier, Felix Duguay, Mohammed Ali Messedi, Yazid Ben Said                                                                       
*Date: 9 mars 2020
*Description: Fichier d'entetes pour la construction autour de la classe Del et implementation de ses methodes                                      
*************************************************************************************/

#ifndef DEL_H
#define DEL_H

#include <avr/io.h>
#include <util/twi.h>


//Index qui indique quelles couleurs peut prendre la Del

enum class Couleur
{
   ETEINT = 0X00,
   ROUGE = 0X01,
   VERT = 0X02
};

enum class IdDel
{
   GAUCHE, DROITE
};

// Classe Del qui est implemente dans le del.cpp

class Del
{
public:
   Del();
   ~Del();

   void changerCouleur(Couleur couleur, IdDel idDel);
};

#endif 
