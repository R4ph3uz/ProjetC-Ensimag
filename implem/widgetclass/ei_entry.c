#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"
#include "ei_entry.h"

#include <ei_entry.h>
#include <ei_utils.h>
#include <stdlib.h>
#include "../draw_utils/draw_utils.h"
/*---------------------------------------------------------------------------------------------------------------------*/

ei_widget_t entry_allocfunc(){
    ei_impl_entry_t* entry = malloc(sizeof(ei_impl_entry_t));
    entry->color = malloc(sizeof(ei_color_t));
    entry->requested_char_size = malloc(sizeof(int));
    entry->text_font = malloc(sizeof(ei_font_t));
    entry->text_color = malloc(sizeof(ei_color_t));
    entry->border_width = malloc(sizeof(int));
    entry->focus=malloc(sizeof(bool));
    entry->text= malloc(sizeof(char));
    return (ei_widget_t) entry;
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_releasefunc(ei_widget_t widget){
    ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
    free(entry->color);
    free(entry->border_width);
    free(entry->text_font);
    free(entry->text_color);
    free(entry->requested_char_size);
    free(entry->text);
//    free(entry->focus);
    // free trucs spécifique aux widgets ?

    free(entry);
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_setdefaultsfunc(ei_widget_t widget){
    ei_entry_t entry = (ei_entry_t) widget;

    /* Suite spécifique à une  entry*/

    entry->color->alpha = 255;
    entry->color->blue = 255;
    entry->color->green = 255;
    entry->color->red= 255;
    *entry->border_width = 1;
    ei_const_string_t name = "misc/font.ttf";
    ei_fontstyle_t style = ei_style_normal;
    *entry->text_font = hw_text_font_create(name, style, 20);
    *entry->text_color= (ei_color_t) {0,0,0,255};
    *entry->requested_char_size= 100;
    entry->focus=false;
    entry->position= 0 ;
    *entry->text = '\0';
    entry->decal_x =0;
}

/*---------------------------------------------------------------------------------------------------------------------*/


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
    if(entry->border_width!=NULL) { // doit etre fait avant de dessiner la entry (vu que en dessous
        ei_point_t* border = malloc(4*sizeof(ei_point_t));
        if (entry->focus)
            *entry->border_width = 2;
        else
            *entry->border_width= 1;
        border[0] = (ei_point_t) {top_left_x - (*entry->border_width), top_left_y - *entry->border_width};
        border[1] = (ei_point_t) {top_left_x + widget->screen_location.size.width + *entry->border_width, top_left_y - *entry->border_width };
        border[2] = (ei_point_t) {top_left_x + widget->screen_location.size.width + *entry->border_width, top_left_y + widget->screen_location.size.height + *entry->border_width };
        border[3] = (ei_point_t) {top_left_x - (*entry->border_width), top_left_y + widget->screen_location.size.height + *entry->border_width };
        ei_color_t color = (ei_color_t) {0,0,0, 255};
        ei_draw_polygon(surface, border, 4, color, clipper);
        free(border);
    }
    ei_draw_polygon(surface, points, nb_points, *entry->color, clipper);
    ei_draw_polygon(pick_surface, points, nb_points, *entry->widget.pick_color, clipper);

    if(entry->text){
        /* test place cursor pour voir s'il est en dehors de l'entry */
        const char* entry_text_restreint = restrict_text(entry->text,entry->position);
        int width, height;
        hw_text_compute_size(entry_text_restreint, *entry->text_font,&width, &height);
        if(width > entry->widget.screen_location.size.width - entry->decal_x) { // si dépasse a droite
            fprintf(stderr, "AIAIIAI\n");
            entry->decal_x = -width+entry->widget.screen_location.size.width;
        }
        if (width < - entry->decal_x) { //si dépasse a gauche
            fprintf(stderr, "WTFF %d", width);
            entry->decal_x = width;
            fprintf(stderr, "WTFF %d", entry->decal_x);
        }
        ei_point_t place = {widget->screen_location.top_left.x+entry->decal_x,widget->screen_location.top_left.y};
        ei_rect_t clipper_text = entry->widget.screen_location;
        ei_draw_text(surface, &place, entry->text, *entry->text_font, *entry->text_color, &clipper_text);
    }
    // place une border autour de l'entry
    if(entry->focus) {
        //mets un curseur si focus
        uint32_t decal_y = 0;//widget->screen_location.size.height/2;

        //calcul de la place du curseur |
        ei_point_t* place_cursor ;
        if (entry->text) {
            const char* entry_text_restreint = restrict_text(entry->text, entry->position);
            char* text = (char*) ei_entry_get_text(widget);
            int pos1 = (uint8_t) fminf((float) find_position_cursor_selection_entry(entry, entry->debut_selection),
                                       (float)find_position_cursor_selection_entry(entry, entry->fin_selection));
            int pos2 = (uint8_t) fmaxf((float) find_position_cursor_selection_entry(entry, entry->debut_selection),
                                       (float)find_position_cursor_selection_entry(entry, entry->fin_selection));
            char* new= cut_text(text, pos1,pos2);
            fprintf(stderr, "\ndebut selection %d, fin %d texte avant: %s, apres %s\n",pos1,pos2, text, new);
            int width, height;
            hw_text_compute_size(entry_text_restreint, *entry->text_font,&width, &height);
            place_cursor = &(ei_point_t){
                widget->screen_location.top_left.x + entry->decal_x + width -5,
                widget->screen_location.top_left.y - 5,
            };
            free((char*)entry_text_restreint);

            // const char* entry_text_restreint = restrict_text(entry->text, entry->position);
            // // fprintf(stderr, "texte res: %s  vs text normal %s \n", entry_text_restreint, entry->text);
            // int width, height;
            // hw_text_compute_size(entry_text_restreint, *entry->text_font,&width, &height);
            // place_cursor = &(ei_point_t){
            //     widget->screen_location.top_left.x + entry->decal_x + width - 10,
            //     widget->screen_location.top_left.y + decal_y - 5
            // };
            // free((char*)entry_text_restreint);
        }
        else
        //No text
            place_cursor = &(ei_point_t){
                widget->screen_location.top_left.x + entry->decal_x - 10,
                widget->screen_location.top_left.y + decal_y - 5
            };


        //draw cursor au bonne endroit
        ei_const_string_t cursor_text = "|";
        if (entry->is_focus_visible && !entry->is_in_selection)
            ei_draw_text(surface,place_cursor, cursor_text, *entry->text_font,(ei_color_t){0,0,0,255} , clipper );

        if(entry->is_in_selection){
            ei_point_t* rect = malloc(sizeof(ei_point_t)* 4);
            rect[0] = ei_point(find_selection_entry(entry,entry->debut_selection) , entry->widget.screen_location.top_left.y);
            rect[1] = ei_point(find_selection_entry(entry,entry->debut_selection), entry->widget.screen_location.top_left.y + entry->widget.screen_location.size.height);
            rect[2] = ei_point(find_selection_entry(entry,entry->fin_selection), entry->widget.screen_location.top_left.y + entry->widget.screen_location.size.height);
            rect[3] = ei_point(find_selection_entry(entry,entry->fin_selection), entry->widget.screen_location.top_left.y);

            ei_color_t select_color = (ei_color_t) {25, 25, 200, 100};
            ei_draw_polygon(surface, rect ,4, select_color, &entry->widget.screen_location);
        }
    }

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_geomnotifyfunc(ei_widget_t widget){

}

/*---------------------------------------------------------------------------------------------------------------------*/

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


