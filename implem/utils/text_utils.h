#ifndef PROJETC_IG_TEXT_UTILS_H
#define PROJETC_IG_TEXT_UTILS_H

#include "ei_types.h"
#include "hw_interface.h"
#include "../widgetclass/ei_entry.h"


/**
 * @brief Garde le début du texte jusqu'à taille
 * @param text texte a restreindre
 * @param taille taille a restreindre, doit être supérieur a 0 et inférieur a la taille (0 renvoie text)
 * @return
 */
char* restrict_text(char* text, uint8_t taille);

/**
 * @brief
 * @param text texte auquel on ajoute le charactère, malloc un nouveau text de la bonne taille
 * @param character charactere à insérer dans le texte
 * @param where indice (en partant du debut ou ajouter le caractère
 * @return
 */
char* insert_char(char* text, char character, uint8_t where);
/**
 * @brief
 * @param text texte auquel on ajoute un mot, malloc un nouveau text de la bonne taille
 * @param character charactere à insérer dans le texte
 * @param where indice (en partant du debut ou ajouter le caractère
 * @return
 */
char* insert_word(char* text, const char* word, uint8_t where);

/**
 * @brief
 * @param text texte auquel supprimer le caractère
 * @param where index du caractère à supprimer
 * @return
 */
char* delete_char(char* text, uint8_t where);

/**
 * @brief prend une entrée et une largeur pour dessiner le selection
 * @param entry widget entry ou a eu lieu le click pour changer la position du curseur
 * @param position point ou le click a eu lieu
 * @return
 */
int find_selection_entry(ei_entry_t entry, ei_point_t position);

/**
 * @brief coupe une partie du  texte de debut a fin non inclus (prend des indices en paramatres)
 * @param text texte a couper
 * @param debut point ou le click a eu lieu
 * @param fin fin non inclus
 * @return
 */
char* cut_text(char* text, uint8_t debut, uint8_t fin);


int find_position_cursor_selection_entry(ei_entry_t entry, ei_point_t position);

void find_word(char* text, int where, int* debut, int* fin);

/**
 * \brief prend un texte, deux endroits (debut<=fin) et renvoie le texte à l'interieur.
 * @param text
 * @param debut
 * @param fin
 * @return texte entre debut et fin
 */
char* texte_selectionne(char* text, int32_t debut, int32_t fin);

#endif //PROJETC_IG_TEXT_UTILS_H
