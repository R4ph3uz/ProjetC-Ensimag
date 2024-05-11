#ifndef PROJETC_IG_ENTRY_H
#define PROJETC_IG_ENTRY_H

#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"

typedef struct{
    /* Commun à tous les widgets */
    ei_impl_widget_t widget;

    /* Suite spécifique à une entry*/

    int*			requested_char_size;
    ei_color_t*	    color;
    int*			border_width;
    ei_font_t*		text_font;
    ei_color_t*		text_color;

    /*variable text */
    char* text;
    bool focus;
    uint8_t position;   //>position du curseur (a partir de la fin du texte)
    bool is_focus_visible; //> boolean for cursor animation

    int decal_x;

} ei_impl_entry_t;

typedef ei_impl_entry_t* ei_entry_t;

ei_widget_t entry_allocfunc();

void entry_releasefunc(ei_widget_t widget);

void entry_setdefaultsfunc(ei_widget_t widget);

void entry_drawfunc(ei_widget_t widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper);

void entry_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief	Prend rien, crée une top_level, renvoie un ei_widgetclass_t* ; permet d'acceder aux fonctions 'drawfunc ...' de top_level
 *
 * @param	void
 */
ei_widgetclass_t* create_entry_widgetclass();


#endif //PROJETC_IG_ENTRY_H
