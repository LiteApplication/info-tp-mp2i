#ifndef ALG_H
#define ALG_H

typedef struct alg
{
    char lettre;
    struct alg *fils, *frere, *parent;
    bool end;
} alg;

struct stack
{
    alg *a;
    struct stack *next;
};

typedef struct stack **stack;

/**
 * @brief Crée une nouvelle pile
 * @return La pile créée
 */
stack init_stack();

/**
 * @brief Ajoute un élément à la pile
 * @param s Pile à laquelle ajouter l'élément
 * @param a L'élément à ajouter
 */
void push(stack s, alg *a);
/**
 * @brief Retire un élément de la pile
 * @param s Pile de laquelle retirer l'élément
 * @return L'élément retiré
 */
alg *pop(stack s);

/**
 * @brief Retourne le premier élément de la pile
 * @param s Pile dont on veut le premier élément
 * @return Le premier élément de la pile
 */
alg *top(stack s);

/**
 * @brief Affiche un mot en remontant la chaîne de caractères
 * @note Complexité : O(longueur du mot)
 */
void print_up(alg *a);

/**
 * FONCTIONS DE L'EXERCICE
 */

/* Complexités :
 * - affiche : O(n + m * l) avec n la taille de l'arbre, l la longueur moyenne des mots, m le nombre de mots
 * - insert_word : O(longueur du mot) (le nombre d'itérations par lettre est majoré par 26)
 * - delete_word : O(longueur du mot) (pareil que pour insert_word)
 */
void affiche(alg *a);
void insert_word(alg *a, char *word);
void delete_word(alg *a, char *word);

#endif // ALG_H