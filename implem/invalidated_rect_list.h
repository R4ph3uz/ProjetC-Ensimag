#ifndef INVALIDATED_RECT_LIST_H
#define INVALIDATED_RECT_LIST_H

#include "ei_types.h"

/**
 * @brief
 * @return the list with all the invalidated rect
 */
ei_linked_rect_t* get_invalidated_rect_list();

void set_invalidated_rect_list(ei_linked_rect_t* list);

/**
 * @brief met a NULL la liste des invalidated rect (cette fonction free tous les rectangles a l'intérieur)
 */
void reinitialize_invalidated_rect_list();

#endif //INVALIDATED_RECT_LIST_H
