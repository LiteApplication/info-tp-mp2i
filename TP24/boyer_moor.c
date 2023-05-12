/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -g boyer_moor.c -o boyer_moor
Date	: 21.04.2023
____________________
*/
#include "boyer_moor.h"
// #package boyer_moor.h

/*
prend une chaîne de caractères en argument et renvoie un tableau de la taille
le nombre de valeurs du type char, qui contient la dernière occurrence de (char)
i dans la case d’indice i, et -1 si (char) i n’apparaît pas dans le motif
*/
int *occ_droite(const char *motif)
{
    int *tab = malloc(255 * sizeof(int));
    for (int i = 0; i < 255; i++)
        tab[i] = -1;

    for (int i = 0; motif[i + 1] != '\0'; i++)
        tab[(int)motif[i]] = i;

    return tab;
}

struct liste_occ *append(struct liste_occ *liste, int occ)
{
    struct liste_occ *new = malloc(sizeof(struct liste_occ));
    new->occ = occ;
    new->suite = liste;

    return new;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

bool shift_buffer(char buffer[BUFFER_SIZE], int keep_left, FILE *fichier)
{
    for (int i = BUFFER_SIZE - keep_left; i < BUFFER_SIZE - 1; i++)
    {
        printf("%c, %c -> ", buffer[i], buffer[i - BUFFER_SIZE + keep_left]);
        buffer[i - BUFFER_SIZE + keep_left] = buffer[i];
        printf("%c, %c\n", buffer[i], buffer[i - BUFFER_SIZE + keep_left]);
    }

    int result = fread(buffer + keep_left, sizeof(char), BUFFER_SIZE - keep_left - 1, fichier);
    if (result == EOF)
        buffer[result + keep_left] = '\0';
    return result != EOF;
}

struct liste_occ *boyer_moore(FILE *fichier, const char *motif)
{
    assert(fichier != NULL);

    struct liste_occ *liste = NULL;
    char buffer[BUFFER_SIZE];
    int *occ_lettres = occ_droite(motif);

    int taille_motif = strlen(motif);
    assert(taille_motif <= BUFFER_SIZE);

    shift_buffer(buffer, 0, fichier);

    // Algorithme de Boyer-Moore
    int i = taille_motif - 1;
    int j = taille_motif - 1;
    while (true)
    {
        if (buffer[i] == motif[j])
        {
            if (j == 0)
            {
                liste = append(liste, i);
                i += taille_motif;
                j = taille_motif - 1;
            }
            else
            {
                i--;
                j--;
            }
        }
        else
        {
            i += taille_motif - min(j, 1 + occ_lettres[(int)buffer[i]]);
            j = taille_motif - 1;
            if (i >= BUFFER_SIZE)
            {
                if (!shift_buffer(buffer, taille_motif - 1, fichier))
                    break;
                i = taille_motif - 1;
            }
        }
    }

    return liste;
}
