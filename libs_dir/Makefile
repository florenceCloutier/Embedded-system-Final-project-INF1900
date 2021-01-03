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

#######################################
#Il s'agit du MakeFile de la librairie#
#######################################

include ../Makefile

#####      Details specifique a la cible       #####
#####  Vous devez les adapter a votre projet   #####

# Nom de votre projet
PROJECTNAME=library

# Fichiers sources
PRJSRC= $(wildcard *.cpp)


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
AR=avr-ar

####### Cible (Target) #######

#Nom des cibles par defaut
TRG=$(PROJECTNAME).a
HEXROMTRG=$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex

####### Creation des commandes du Makefile ####### 

# Make all permet simplement de compiler le projet
all: $(TRG)

# Implementation de la cible
$(TRG): $(OBJDEPS)
	$(AR) -rcs $(TRG) $(OBJDEPS)

# Production des fichiers object
# De C a objet
%.o: %.c
	$(CC) $(CFLAGS) -c $<
# De C++ a objet
%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

# Verification des dependances (header dependencies)
-include *.d

# Production des fichiers hex a partir des fichiers elf
%.hex: %.out
	$(OBJCOPY) -j .text -j .data \
		-O $(HEXFORMAT) $< $@

# Make clean permet d'effacer tout les fichiers generes
# lors de la compilation
clean:
	$(REMOVE) $(TRG) $(TRG).map $(OBJDEPS) $(HEXTRG) *.d


#####                    EOF                   #####
