#include "list_event.h"

list_callback* LIST_CALLBACK = NULL;

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


list_callback* get_list_callback(void) {
    return LIST_CALLBACK;
}

void free_list_callback() {
    while(LIST_CALLBACK!=NULL) {
        list_callback* temp = LIST_CALLBACK->next;
        free(LIST_CALLBACK);
        LIST_CALLBACK = temp;

        // est ce qu'il faut free les pointeurs de la struct ?
    }
}