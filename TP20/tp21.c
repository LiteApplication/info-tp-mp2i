#include "tp21.h"
#include "grandes_lettres.h"

int longueur(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        i = i + 1;
    }
    return i;
}

int nombre_occurences(char a, char *s)
{
    int i = 0;
    int n = 0;
    while (s[i] != '\0')
    {
        if (s[i] == a)
        {
            n = n + 1;
        }
        i = i + 1;
    }
    return n;
}

int derniere_occurence(char a, char *s)
{
    int l = longueur(s);
    for (int i = l - 1; i >= 0; i--)
    {
        if (s[i] == a)
        {
            return i;
        }
    }
    return -1;
}

char **split(char a, char *s)
{
    int nb_tab = nombre_occurences(a, s) + 2; // +1 for the last word
    // +1 for the last NULL
    int l = longueur(s);
    char **tab = malloc(nb_tab * sizeof(char *));
    for (int i = 0; i < nb_tab; i++)
    {
        tab[i] = malloc(l * sizeof(char));
    }

    int begin = 0;
    int end = 0;
    int cell = 0;
    while (s[end] != '\0')
    {
        if (s[end] == a)
        {
            for (int i = begin; i < end; i++)
            {
                tab[cell][i - begin] = s[i];
            }
            tab[cell][end - begin] = '\0';
            begin = end + 1;
            cell++;
        }
        end++;
    }
    for (int i = begin; i < end; i++) // Last iteration
    {
        tab[cell][i - begin] = s[i];
    }
    tab[cell][end - begin] = '\0';
    tab[cell + 1] = NULL; // Last cell is NULL
    return tab;
}

void affichage(int largeur, char *s)
{
    // Algo glouton
    int pos = 0;
    char **words = split(' ', s);
    int i = 0;
    while (words[i] != NULL)
    {
        int l = longueur(words[i]);
        if (pos + l <= largeur)
        {
            printf("%s ", words[i]);
            pos = pos + l + 1;
            i++;
        }
        else
        {
            printf("\n");
            pos = 0;
            if (l > largeur) // On sait jamais
            {
                printf("%s\n", words[i]);
                i++;
            }
        }
    }
    printf("\n");
}

void ascii_art(char *s)
{
    char **words = split(' ', s);
    int i = 0;
    while (words[i] != NULL)
    {
        int l = longueur(words[i]);
        int *pos = malloc(l * sizeof(int));
        for (int j = 0; j < l; j++)
        {
            pos[j] = 0;
        }
        for (int h = 0; h < LETTER_HEIGHT; h++)
        {
            for (int j = 0; j < l; j++)
            {
                char *letter = grande_lettre(words[i][j]);
                while (letter[pos[j]] != '\n' && letter[pos[j]] != '\0')
                {
                    printf("%c", letter[pos[j]]);
                    fflush(stdout);
                    pos[j] = pos[j] + 1;
                }
                pos[j] = pos[j] + 1;
                printf("   ");
            }
            printf("\n");
        }
        i++;
    }
}
