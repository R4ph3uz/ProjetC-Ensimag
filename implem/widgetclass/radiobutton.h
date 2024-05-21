#ifndef PROJETC_IG_RADIOBUTTON_H
#define PROJETC_IG_RADIOBUTTON_H

#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"

ei_widget_t radiobutton_allocfunc();

void radiobutton_releasefunc(ei_widget_t widget);

void radiobutton_setdefaultsfunc(ei_widget_t widget);

void radiobutton_drawfunc(ei_widget_t widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper);

void radiobutton_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief	Prend rien, crée une top_level, renvoie un ei_widgetclass_t* ; permet d'acceder aux fonctions 'drawfunc ...' de top_level
 *
 * @param	void
 */
ei_widgetclass_t* create_radiobutton_widgetclass();
#endif //PROJETC_IG_RADIOBUTTON_H
