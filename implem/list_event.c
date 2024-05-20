#include "list_event.h"
#include "ei_implementation.h"
/*----------------------------------------------------------------------------------------------------------------*/

list_callback* LIST_CALLBACK = NULL;

/*----------------------------------------------------------------------------------------------------------------*/


void add_list_callback(ei_callback_t callback, ei_tag_t tag, ei_eventtype_t eventtype, void* user_param) {
    if (LIST_CALLBACK == NULL) {
        LIST_CALLBACK = SAFE_MALLOC(sizeof(list_callback));
        LIST_CALLBACK->callback = callback;
        LIST_CALLBACK->eventtype = eventtype;
        LIST_CALLBACK->tag = SAFE_MALLOC(sizeof(char)*(strlen(tag)+1));
        strcpy(LIST_CALLBACK->tag, tag);
        LIST_CALLBACK->user_param = user_param;
        LIST_CALLBACK->next = NULL;
    }
    else {
        list_callback* temp = SAFE_MALLOC(sizeof(list_callback));
        temp->callback = callback;
        temp->eventtype = eventtype;
        temp->tag = SAFE_MALLOC(sizeof(char)*(strlen(tag)+1));
        strcpy(temp->tag, tag);
        temp->next = LIST_CALLBACK;
        temp->user_param = user_param;
        LIST_CALLBACK = temp;
    }
}

/*----------------------------------------------------------------------------------------------------------------*/


void remove_list_callback(ei_callback_t callback, ei_tag_t tag, ei_eventtype_t eventtype, void* user_param) {
    list_callback *parcours = LIST_CALLBACK;

    if(strcmp(parcours->tag,tag)==0
        && parcours->eventtype==eventtype
        && callback==parcours->callback
        && user_param == parcours->user_param) {
        list_callback* temp = LIST_CALLBACK;
        LIST_CALLBACK = LIST_CALLBACK->next;
            if (user_param!= parcours->user_param)
                fprintf(stderr, "C'EST PAS LE MEME USER PARAM");
        free(temp->tag);
        free(temp);
        return;
    }

    while(parcours->next!=NULL) {
        if(strcmp(parcours->next->tag,tag)==0
            && memcmp(&parcours->next->eventtype,&eventtype, sizeof(ei_eventtype_t))==0
            && callback==parcours->next->callback
            && eventtype==parcours->next->eventtype
            && user_param == parcours->next->user_param) {
             list_callback* temp = parcours->next;
            if (user_param!= parcours->next->user_param)
                fprintf(stderr, "C'EST PAS LE MEME USER PARAM 2");
            parcours->next = parcours->next->next;

            free(temp); // Besoin de free les sous structures (je pense pas)?
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