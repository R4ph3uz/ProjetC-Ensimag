#include "invalidated_rect_list.h"

/*--------------------------------------------------------------------------------------------------------------------------------*/

ei_linked_rect_t* INVALIDATED_RECT_LIST = NULL;

/*--------------------------------------------------------------------------------------------------------------------------------*/

ei_linked_rect_t* get_invalidated_rect_list(){
    return INVALIDATED_RECT_LIST;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/


void set_invalidated_rect_list(ei_linked_rect_t* list){
    INVALIDATED_RECT_LIST =list;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/

void reinitialize_invalidated_rect_list(){
    while(INVALIDATED_RECT_LIST!=NULL) {
        ei_linked_rect_t* temp = INVALIDATED_RECT_LIST;
        INVALIDATED_RECT_LIST = INVALIDATED_RECT_LIST->next;
        free(temp);
    }

}