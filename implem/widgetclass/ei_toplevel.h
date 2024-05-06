#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"

typedef struct{
    /* Commun à tous les widgets */
    ei_impl_widget_t widget;

    /* Suite spécifique à un top level*/

    const ei_color_t*	color;
    int*			border_width;
    ei_string_t*		title;
    bool*			closable;
    ei_axis_set_t*		resizable;
    ei_size_ptr_t*		min_size;

    /*variable to move and resize*/
    ei_point_t whereButtonDown;

} ei_impl_toplevel_t;

typedef ei_impl_toplevel_t* ei_toplevel_t;

ei_widget_t toplevel_allocfunc();

void toplevel_releasefunc(ei_widget_t widget);

void toplevel_setdefaultsfunc(ei_widget_t widget);

void toplevel_drawfunc(ei_widget_t widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper);

void toplevel_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief	Prend rien, crée une top_level, renvoie un ei_widgetclass_t* ; permet d'acceder aux fonctions 'drawfunc ...' de top_level
 *
 * @param	void
 */
ei_widgetclass_t* create_toplevel_widgetclass();


#endif //EI_TOPLEVEL_H
