#ifndef TP21_H
#define TP21_H

#include <stdlib.h>
#include <stdio.h>

int longueur(const char *s);
int nombre_occurences(char a, char *s);
int derniere_occurence(char a, char *s);
char **split(char a, char *s);

void affichage(int largeur, char *s);
void ascii_art(char *s);

#endif // TP21_H