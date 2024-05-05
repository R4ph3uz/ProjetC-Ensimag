#ifndef EI_TOP_LEVEL_H
#define EI_TOP_LEVEL_H

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
    bool isButtonDownOnTop;
    ei_point_t whereButtonDown;

} ei_impl_top_level_t;

typedef ei_impl_top_level_t* ei_top_level_t;

ei_widget_t top_level_allocfunc();

void top_level_releasefunc(ei_widget_t widget);

void top_level_setdefaultsfunc(ei_widget_t widget);

void top_level_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper);

void top_level_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief	Prend rien, crée une top_level, renvoie un ei_widgetclass_t* ; permet d'acceder aux fonctions 'drawfunc ...' de top_level
 *
 * @param	void
 */
ei_widgetclass_t* create_top_level_widgetclass();


#endif //EI_TOP_LEVEL_H
