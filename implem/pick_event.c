#include "pick_event.h"

/*--------------------------------------------------------------------------------------------------------------------*/

widget_array* PICK_DYNARRAY = NULL;

/*--------------------------------------------------------------------------------------------------------------------*/

void initialize_pickid_array(){
    PICK_DYNARRAY = malloc(sizeof(widget_array));
    PICK_DYNARRAY->number_element= 0;
    PICK_DYNARRAY->size_malloced = 80;
    PICK_DYNARRAY->widgets = malloc(sizeof(ei_widget_t*)*PICK_DYNARRAY->size_malloced);
}

/*--------------------------------------------------------------------------------------------------------------------*/

void add_widget_pickid_array(ei_widget_t widget){
    if (PICK_DYNARRAY->size_malloced == PICK_DYNARRAY->number_element) {
        //il faut redimensionner le dynarray
        PICK_DYNARRAY->size_malloced *=2;
        PICK_DYNARRAY->widgets = realloc(PICK_DYNARRAY->widgets, sizeof(ei_widget_t*)*PICK_DYNARRAY->size_malloced);

    }
    PICK_DYNARRAY->number_element++;
    PICK_DYNARRAY->widgets[PICK_DYNARRAY->number_element-1] = widget;
}

/*--------------------------------------------------------------------------------------------------------------------*/

ei_widget_t get_widget_by_pickid(uint32_t index) {
    if (index <= PICK_DYNARRAY->number_element) {
        return PICK_DYNARRAY->widgets[index];
    }
    fprintf(stderr, "erreur index out of bounds");
    return NULL;

}
