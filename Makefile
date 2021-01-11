########       AVR Project Makefile       ########
#####                                        #####
#####      Makefile produit et ecrit par     #####
#####   Florence Cloutier, Felix Duguay, 
#####  Mohammed Ali Messedi, Yazid Ben Said  #####
#####           INF1900 - 2020               #####
#####                                        #####
#####      Inspire du MakeFile original      #####
#####  										 #####
##################################################

# Ce Makefile permet de compiler des projets
# pour les microcontroleurs Atmel AVR sur 
# Linux ou Unix, en utilisant l'outil AVR-GCC. 
# Ce Makefile supporte C & C++

###############################
#Il s'agit du MakeFile general#
###############################

# Nom du microcontroleur cible
MCU=atmega324pa

# Nom de votre projet
PROJECTNAME=general

# Niveau d'optimization
OPTLEVEL=s

# Programmer ID
AVRDUDE_PROGRAMMERID=usbasp


####################################################
#####         Configuration terminee           #####
#####                                          #####
#####  Le reste de cette section contient les  #####
##### details d'implementation permettant      #####
##### de mieux comprendre le fonctionnement de ##### 
#####   ce Makefile en vue de sa modification  #####
####################################################


####### variables #######

#compilateur utilise
CC=avr-gcc
#pour copier le contenu d'un fichier objet vers un autre
OBJCOPY=avr-objcopy
#pour permettre le transfert vers le microcontroleur
AVRDUDE=avrdude
#pour supprimer les fichiers lorsque l'on appel make clean
REMOVE=rm -f
# HEXFORMAT -- format pour les fichiers produient .hex
HEXFORMAT=ihex


####### Options de compilation #######

# # Flags pour le compilateur en C
CFLAGS=-I. -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall           

LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU)                             

# # Flags pour le compilateur en C++
CXXFLAGS=-fno-exceptions   

####### Cible (Target) #######

#Nom des cibles par defaut
TRG=$(PROJECTNAME).elf
HEXROMTRG=$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex


####### Definition de tout les fichiers objets #######

# Cette fonction permet de differencier les fichiers .c
# des fichiers .cpp
# Fichier C
CFILES=$(filter %.c, $(PRJSRC))
# Fichier C++
CPPFILES=$(filter %.cpp, $(PRJSRC))

# Liste de tout les fichiers objet que nous devons creer
OBJDEPS=$(CFILES:.c=.o) \
		$(CPPFILES:.cpp=.o)

# Creation des cibles Phony (Phony Target)
# En plus de la commande make qui permet de compiler
# le projet, on peut utiliser les commandes
# make all, make install et make clean
.PHONY: all install clean 

#####                    EOF                   #####
