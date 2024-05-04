#ifndef LIST_EVENT_H
#define LIST_EVENT_H

#include "ei_event.h"

/**
 * @brief structure pour avoir une liste globale de tous les callback (seulement ceux avec un tag)
 */
typedef struct list_callback {
    ei_callback_t callback;
    ei_tag_t tag;
    ei_eventtype_t eventtype;
    void* user_param;
    struct list_callback* next;
}list_callback;

/**
 * @brief ajoute un callback a la liste des callbacks
 * @param callback
 * @param tag
 * @param eventtype
 * @param user_param
 */
void add_list_callback(ei_callback_t callback, ei_tag_t tag, ei_eventtype_t eventtype, void* user_param);

/**
 * return la liste globale avec tous les callbacks non spécifiques qui sont bind
 */
list_callback* get_list_callback(void);

/**
 * Free la liste globale des callbacks
 */
void free_list_callback();

#endif //LIST_EVENT_H
