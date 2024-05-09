#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"
#include "ei_entry.h"

#include <ei_entry.h>

ei_widget_t entry_allocfunc(){
    ei_impl_entry_t* entry = malloc(sizeof(ei_impl_entry_t));
    entry->color = malloc(sizeof(ei_color_t));
    entry->requested_char_size = malloc(sizeof(int));
    entry->text_font = malloc(sizeof(ei_font_t));
    entry->text_color = malloc(sizeof(ei_color_t));
    entry->border_width = malloc(sizeof(int));
    entry->focus=malloc(sizeof(bool));
    return (ei_widget_t) entry;
}

void entry_releasefunc(ei_widget_t widget){
    ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
    free(entry->color);
    free(entry->border_width);
    free(entry->text_font);
    free(entry->text_color);
    free(entry->requested_char_size);
//    free(entry->focus);
    // free trucs spécifique aux widgets ?

    free(entry);
}

void entry_setdefaultsfunc(ei_widget_t widget){
    ei_entry_t entry = (ei_entry_t) widget;

    /* Suite spécifique à une  entry*/

    entry->color->alpha = 100;
    entry->color->blue = 255;
    entry->color->green = 100;
    entry->color->red= 0;
    *entry->border_width = 2;
    ei_const_string_t name = "misc/font.ttf";
    ei_fontstyle_t style = ei_style_normal;
    *entry->text_font = hw_text_font_create(name, style, 20);
    *entry->text_color= (ei_color_t) {0,0,0};
    *entry->requested_char_size= 100;
    entry->focus=false;
}

void entry_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_entry_t entry = (ei_entry_t) widget;

    int top_left_x = widget->screen_location.top_left.x;
    int top_left_y = widget->screen_location.top_left.y;

    ei_point_t* points = malloc(4*sizeof(ei_point_t));
    points[0] = (ei_point_t) {top_left_x, top_left_y };
    points[1] = (ei_point_t) {top_left_x+widget->screen_location.size.width, top_left_y };
    points[2] = (ei_point_t) {top_left_x+widget->screen_location.size.width, top_left_y+widget->screen_location.size.height };
    points[3] = (ei_point_t) {top_left_x, top_left_y+widget->screen_location.size.height };
    size_t nb_points = 4;

    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    ei_draw_polygon(surface, points, nb_points, *entry->color, clipper);
    ei_draw_polygon(pick_surface, points, nb_points, *entry->widget.pick_color, clipper);

    ei_const_string_t texte ;

    if(entry->text){
        uint32_t decal_x =0;// widget->screen_location.size.width/10;
        uint32_t decal_y = 0;//widget->screen_location.size.height/2;
        ei_point_t place = {widget->screen_location.top_left.x+decal_x,widget->screen_location.top_left.y+decal_y};
        ei_const_string_t texte = (ei_const_string_t) entry->text;
        ei_draw_text(surface, &place, entry->text, *entry->text_font, *entry->text_color, clipper);
    }
    // place une border autour de l'entry
    if(entry->focus) {
        //mets un curseur si focus
        uint32_t decal_x =5;// widget->screen_location.size.width/10;
        uint32_t decal_y = 0;//widget->screen_location.size.height/2;
        ei_point_t place = {widget->screen_location.top_left.x+decal_x,widget->screen_location.top_left.y+decal_y};
        ei_draw_polyline(surface, points, nb_points,(ei_color_t){40,40,40,255}, NULL);

    }

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

void entry_geomnotifyfunc(ei_widget_t widget){

}

/**
 * @brief	Prend rien, crée une entry, renvoie un ei_widgetclass_t* ; permet d'acceder aux fonctions 'drawfunc ...' de top_level
 *
 * @param	void
 */
ei_widgetclass_t* create_entry_widgetclass(){
    ei_widgetclass_t* res = malloc(sizeof(ei_widgetclass_t));
    res->allocfunc = entry_allocfunc;
    res->releasefunc = entry_releasefunc;
    res->drawfunc = entry_drawfunc;
    res->geomnotifyfunc= entry_geomnotifyfunc;
    res->setdefaultsfunc = entry_setdefaultsfunc;

    const char name[] = "entry";
    strcpy(res->name, name);

    res->next = NULL;
    return res;
}


