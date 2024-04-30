#ifndef PROJETC_IG_FRAME_H
#define PROJETC_IG_FRAME_H

#include "ei_widgetclass.h"
#include "ei_types.h"

typedef struct{
    /* Commun à tous les widgets */
    ei_widget_t widget;

    /* Suite spécifique à une  frame*/
    ei_size_t*		requested_size;
    const ei_color_t*	color;
    int*			border_width;
    ei_relief_t*		relief;
    ei_string_t*		text;
    ei_font_t*		text_font;
    ei_color_t*		text_color;
    ei_anchor_t*		text_anchor;
    ei_surface_t*		img;
    ei_rect_ptr_t*		img_rect;
    ei_anchor_t*		img_anchor;

} ei_impl_frame_t;
typedef ei_impl_frame_t* ei_frame_t;

ei_widget_t frame_allocfunc();

void frame_releasefunc(ei_widget_t widget);

void frame_setdefaultsfunc(ei_widget_t widget);

void frame_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper);

void frame_geomnotifyfunc(ei_widget_t widget);

ei_widgetclass_t* create_frame_widgetclass();

#endif //PROJETC_IG_FRAME_H

