#include "radiobutton.h"

typedef struct{
    /* Commun à tous les widgets */
    ei_impl_widget_t widget;

    /* Suite spécifique à un radiobutton*/
    int*			border_width;
    ei_string_t*		title;

    bool*			closable;
    ei_axis_set_t*		resizable;
    ei_size_ptr_t*		min_size;

    /*variable to move and resize*/
    ei_point_t whereButtonDown;

} ei_impl_radiobutton_t;

typedef ei_impl_radiobutton_t* ei_radiobutton_t;

ei_widget_t radiobutton_allocfunc(){

}

void radiobutton_releasefunc(ei_widget_t widget){}

void radiobutton_setdefaultsfunc(ei_widget_t widget){

}

void radiobutton_drawfunc(ei_widget_t widget,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t* clipper){

}

void radiobutton_geomnotifyfunc(ei_widget_t widget){

}

/**
 * @brief	Prend rien, crée une top_level, renvoie un ei_widgetclass_t* ; permet d'acceder aux fonctions 'drawfunc ...' de top_level
 *
 * @param	void
 */
ei_widgetclass_t* create_radiobutton_widgetclass(){

}