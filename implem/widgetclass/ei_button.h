#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"

typedef struct{
    /* Commun à tous les widgets */
    ei_impl_widget_t widget;

    /* Suite spécifique à un button */
    ei_color_t*	color;
    int*			border_width;
    int*			corner_radius;
    ei_relief_t*		relief;
    ei_string_t*		text;
    ei_font_t*		text_font;
    ei_color_t*		text_color;
    ei_anchor_t*		text_anchor;
    ei_surface_t*		img;
    ei_rect_ptr_t*		img_rect;
    ei_anchor_t*		img_anchor;
    ei_callback_t*		callback;
    ei_user_param_t*	user_param;

    bool is_clicked;
} ei_impl_button_t;

typedef ei_impl_button_t* ei_button_t;

/**
 * alloc func de button
 */
ei_widget_t button_allocfunc();

/**
 * release func de button
 */
void button_releasefunc(ei_widget_t widget);
/**
 * setdefault func de button
 */
void button_setdefaultsfunc(ei_widget_t widget);

/**
 * dessine le button
 */
void button_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper);

void button_geomnotifyfunc(ei_widget_t widget);
/**
 * crée un boutton
 */
ei_widgetclass_t* create_button_widgetclass();


#endif //EI_BUTTON_H
