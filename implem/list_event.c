#include "list_event.h"

/*----------------------------------------------------------------------------------------------------------------*/

list_callback* LIST_CALLBACK = NULL;

/*----------------------------------------------------------------------------------------------------------------*/


void add_list_callback(ei_callback_t callback, ei_tag_t tag, ei_eventtype_t eventtype, void* user_param) {
    if (LIST_CALLBACK == NULL) {
        LIST_CALLBACK = malloc(sizeof(list_callback));
        LIST_CALLBACK->callback = callback;
        LIST_CALLBACK->eventtype = eventtype;
        LIST_CALLBACK->tag= tag;
        LIST_CALLBACK->user_param = user_param;
        LIST_CALLBACK->next = NULL;
    }
    else {
        list_callback* temp = malloc(sizeof(list_callback));
        temp->callback = callback;
        temp->eventtype = eventtype;
        temp->tag= tag;
        temp->next = LIST_CALLBACK;
        LIST_CALLBACK->user_param = user_param;
        LIST_CALLBACK = temp;
    }
}

/*----------------------------------------------------------------------------------------------------------------*/


void remove_list_callback(ei_callback_t callback, ei_tag_t tag, ei_eventtype_t eventtype, void* user_param) {
    list_callback *parcours = LIST_CALLBACK;

    if(strcmp(parcours->tag,tag)==0 && memcmp(&parcours->eventtype,&eventtype, sizeof(ei_eventtype_t))==0 ) {
        LIST_CALLBACK = LIST_CALLBACK->next;
        LIST_CALLBACK->tag ="zae";
        fprintf(stderr, "delete succesful");
        return;
    }

    while(parcours->next!=NULL) {
        if(strcmp(parcours->next->tag,tag)==0 && memcmp(&parcours->next->eventtype,&eventtype, sizeof(ei_eventtype_t))==0 ) {
            list_callback* temp = parcours->next;
            parcours->next = parcours->next->next;
            free(temp); // Besoin de free les sous structures (je pense pas)?
            fprintf(stderr, "delete succesful");
            return;
        }
        parcours = parcours->next;
    }

    fprintf(stderr, "not found !");
}

/*----------------------------------------------------------------------------------------------------------------*/


list_callback* get_list_callback(void) {
    return LIST_CALLBACK;
}

/*----------------------------------------------------------------------------------------------------------------*/


void free_list_callback() {
    while(LIST_CALLBACK!=NULL) {
        list_callback* temp = LIST_CALLBACK->next;
        free(LIST_CALLBACK);
        LIST_CALLBACK = temp;

        // est ce qu'il faut free les pointeurs de la struct ?
    }
}