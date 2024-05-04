#ifndef PICK_EVENT_H
#define PICK_EVENT_H

#include "ei_types.h"


/**
 * @brief structure de tableau dynamique pour que on puisse accéder a un element avec son pick_id
 *
 */
typedef struct widget_array {
    ei_widget_t* widgets;
    size_t number_element;
    size_t size_malloced;
}widget_array;

/**
 * @brief initialise le tableau dynamique global pour associer l'id a un widget
 */
void initialize_pickid_array();

/**
 * @brief ajoute le pointeur du widget au tableau dynamique globale , redimensionne avec realloc si nécéssaire
 *         les pickid sont augmentés de 1 a chaque nouveau widget pour correspondre a l'index dans le tableau
 * @param widget le pointeur du widget à ajouter (son pickid doit correspondre a son indice dans le tableau)
 */
void add_widget_pickid_array(ei_widget_t widget);

/**
 * @brief retourne le widget correspondant a l'indice
 * @param index l'indice du widget demandé, doit être inférieur au nombre d'élément dans le tableau dynamique
 */
ei_widget_t get_widget_by_pickid(uint32_t index);




#endif //PICK_EVENT_H
