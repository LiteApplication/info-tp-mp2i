/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -g -Wno-overflow /home/alexis/Documents/MP2I/INFO/TP21/steganographie.c -o steganographie
Date	: 12.05.2023
Run 	: ./steganographie secret.pgm lol
____________________
*/

// #run: secret.pgm "lol"
// #args: -Wno-overflow
#include "steganographie.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

image *charger_image(char *fichier)
{

    FILE *f = fopen(fichier, "r");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    image *img = malloc(sizeof(image));

    int version, max_val;
    fscanf(f, "P%d\n%d %d\n%d", &version, &img->larg, &img->haut, &max_val);
    if (version != 2)
    {
        printf("Le fichier n'est pas au format PPM\n");
        return NULL;
    }

    assert(max_val <= (uint16_t)(-1)); // max_val doit rentrer dans un uint16_t

    img->pix = malloc(img->haut * sizeof(uint16_t *));
    for (int i = 0; i < img->haut; i++)
    {
        img->pix[i] = malloc(img->larg * sizeof(uint16_t));
        for (int j = 0; j < img->larg; j++)
            fscanf(f, " %hu", &img->pix[i][j]);
    }

    fclose(f);
    return img;
}

char caractere(uint16_t *tab)
{
    char result = 0;
    for (int i = 0; i < 8; i++)
    {
        result = result << 1;
        result = result | (tab[i] & 1U);
        // printf("%d, %hu ;", tab[i] & 1U, tab[i]);
    }
    // printf("\n");
    return result;
}

void message(image *img, FILE *f)
{
    for (int i = 0; i < img->haut; i++)
    {
        char c = caractere(img->pix[i]);
        if (c == '\0')
            break;
        fprintf(f, "%c", c);
    }
}

void inserer_caractere(uint16_t *tab, char c)
{
    for (int i = 7; i >= 0; i--)
    {
        tab[i] = tab[i] >> 1 << 1 | (c & 1U);
        c = c >> 1;
    }
}

image *cacher(image *img, char *msg)
{
    for (int i = 0; i < img->haut; i++)
    {
        inserer_caractere(img->pix[i], msg[i]);
    }
    return img;
}

int sauvegarder_image(char *fichier, image *img)
{
    FILE *f = fopen(fichier, "w");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return 1;
    }

    const uint16_t max_couleur = -1;

    fprintf(f, "P2\n%d %d\n%hu\n", img->larg, img->haut, max_couleur);
    for (int i = 0; i < img->haut; i++)
    {
        for (int j = 0; j < img->larg; j++)
            fprintf(f, "%hu ", img->pix[i][j]);
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}

int decrypt(char *file)
{
    image *img = charger_image(file);
    if (img == NULL)
    {
        printf("Erreur lors du chargement de l'image\n");
        return 1;
    }

    message(img, stdout);

    return 0;
}

int encrypt(char *file, char *text)
{
    image *img = charger_image(file);
    if (img == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir l'image source\n");
    }
    bool valid = true;
    if (strlen(text) > (size_t)img->haut)
    {
        fprintf(stderr, "L'image n'est pas assez haute pour cacher le message.\n");
        valid = false;
    }
    if (img->larg < 8)
    {
        fprintf(stderr, "L'image n'est pas assez large.\n");
        valid = false;
    }
    if (valid)
    {
        cacher(img, text);
        sauvegarder_image("secret.pgm", img);
    }

    for (int i = 0; i < img->haut; i++)
    {
        free(img->pix[i]);
    }
    free(img->pix);
    free(img);

    if (!valid)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <image.pgm> [message]\n", argv[0]);
        return 1;
    }

    if (argc == 2)
    {
        decrypt(argv[1]);
        printf("\n");
    }
    else
    {
        if (strcmp(argv[2], "-i") == 0)
        {
            if (argc != 4)
            {
                fprintf(stderr, "Usage: %s <image.pgm> -i <message file>\n", argv[0]);
                return 1;
            }

            FILE *fp = fopen(argv[3], "r");
            fseek(fp, 0L, SEEK_END);
            int size = ftell(fp);

            char *text = malloc((size + 1) * sizeof(char));
            fseek(fp, 0L, SEEK_SET);
            fread(text, sizeof(char), size, fp);
            text[size] = '\0';

            encrypt(argv[1], text);
        }
        else
        {
            encrypt(argv[1], argv[2]);
        }
    }

    return 0;
}