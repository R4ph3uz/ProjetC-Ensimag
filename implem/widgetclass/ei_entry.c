#include "ei_widgetclass.h"
#include "ei_types.h"
#include "../ei_implementation.h"
#include "ei_entry.h"

#include <ei_entry.h>
#include <ei_utils.h>
#include <stdlib.h>
#include "../utils/draw_utils.h"
#include "../utils/text_utils.h"

/*---------------------------------------------------------------------------------------------------------------------*/

ei_widget_t entry_allocfunc(){
    ei_impl_entry_t* entry = SAFE_CALLOC(sizeof(ei_impl_entry_t));

    return (ei_widget_t) entry;
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_releasefunc(ei_widget_t widget){
    ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
    SAFE_FREE(entry->color);
    SAFE_FREE(entry->border_width);
    SAFE_FREE(entry->text_font);
    SAFE_FREE(entry->text_color);
    SAFE_FREE(entry->requested_char_size);
    SAFE_FREE(entry->text);
    SAFE_FREE(entry);
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_setdefaultsfunc(ei_widget_t widget){
    ei_entry_t entry = (ei_entry_t) widget;

    /* Suite spécifique à une  entry*/
    entry->color = SAFE_MALLOC(sizeof(ei_color_t));
    entry->requested_char_size = SAFE_MALLOC(sizeof(int));
    entry->text_font = SAFE_MALLOC(sizeof(ei_font_t));
    entry->text_color = SAFE_MALLOC(sizeof(ei_color_t));
    entry->border_width = SAFE_MALLOC(sizeof(int));
    entry->text= SAFE_MALLOC(sizeof(char));
    entry->color->alpha = 255;
    entry->color->blue = 255;
    entry->color->green = 255;
    entry->color->red= 255;
    *entry->border_width = 1;
    *entry->text_font = ei_default_font;
    *entry->text_color= ei_font_default_color;
    *entry->requested_char_size= 100;
    entry->focus=false;
    entry->position= 0 ;
    *entry->text = '\0';
    entry->decal_x =0;
    entry->is_double_clickable = false;
    entry->is_in_selection = false;
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_entry_t entry = (ei_entry_t) widget;

    int top_left_x = widget->content_rect->top_left.x;
    int top_left_y = widget->content_rect->top_left.y;

    ei_point_t* points = SAFE_MALLOC(4*sizeof(ei_point_t));
    points[0] = (ei_point_t) {top_left_x, top_left_y };
    points[1] = (ei_point_t) {top_left_x+widget->content_rect->size.width, top_left_y };
    points[2] = (ei_point_t) {top_left_x+widget->content_rect->size.width, top_left_y+widget->content_rect->size.height };
    points[3] = (ei_point_t) {top_left_x, top_left_y+widget->content_rect->size.height };
    size_t nb_points = 4;

    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);
    if(entry->border_width!=NULL) { // doit etre fait avant de dessiner la entry (vu que en dessous
        ei_point_t* border = SAFE_MALLOC(4*sizeof(ei_point_t));
        if (entry->focus)
            *entry->border_width = 2;
        else
            *entry->border_width= 1;
        border[0] = (ei_point_t) {top_left_x - (*entry->border_width), top_left_y - *entry->border_width};
        border[1] = (ei_point_t) {top_left_x + widget->content_rect->size.width + *entry->border_width, top_left_y - *entry->border_width };
        border[2] = (ei_point_t) {top_left_x + widget->content_rect->size.width + *entry->border_width, top_left_y + widget->content_rect->size.height + *entry->border_width };
        border[3] = (ei_point_t) {top_left_x - (*entry->border_width), top_left_y + widget->content_rect->size.height + *entry->border_width };
        ei_color_t color = (ei_color_t) {0,0,0, 255};
        ei_draw_polygon(surface, border, 4, color, clipper);
        SAFE_FREE(border);
    }
    ei_draw_polygon(surface, points, nb_points, *entry->color, clipper);
    ei_draw_polygon(pick_surface, points, nb_points, *entry->widget.pick_color, clipper);

    if(entry->text){
        /* test place cursor pour voir s'il est en dehors de l'entry */
        char* entry_text_restreint = restrict_text(entry->text,entry->position);
        int width, height;
        hw_text_compute_size(entry_text_restreint, *entry->text_font,&width, &height);
        SAFE_FREE(entry_text_restreint);
        if(width > entry->widget.content_rect->size.width + entry->decal_x) { // si dépasse a droite
            entry->decal_x = width-entry->widget.content_rect->size.width;
        }
        if (width < entry->decal_x) { //si dépasse a gauche
            entry_text_restreint = restrict_text(entry->text,entry->position);
            hw_text_compute_size(entry_text_restreint, *entry->text_font,&width, &height);
            SAFE_FREE(entry_text_restreint);
            entry->decal_x = width;
        }
        ei_point_t place = {entry->widget.content_rect->top_left.x-entry->decal_x,entry->widget.content_rect->top_left.y};
        ei_rect_t clipper_text = entry->widget.screen_location;
        ei_rect_t *clip = intersection_rectangle(clipper_text,*clipper);
        if(clip!=NULL)
        ei_draw_text(surface, &place, entry->text, *entry->text_font, *entry->text_color, clip);
        SAFE_FREE(clip);
    }
    // place une border autour de l'entry
    if(entry->focus) {
        //mets un curseur si focus
        uint32_t decal_y = 0;//widget->content_rect->size.height/2;

        //calcul de la place du curseur |
        ei_point_t* place_cursor ;
        if (entry->text) {
            const char* entry_text_restreint = restrict_text(entry->text, entry->position);
            int width, height;
            hw_text_compute_size(entry_text_restreint, *entry->text_font,&width, &height);
            place_cursor = &(ei_point_t){
                entry->widget.content_rect->top_left.x - entry->decal_x + width -5,
                entry->widget.content_rect->top_left.y - 5,
            };
            SAFE_FREE(entry_text_restreint);
        }
        else
        //No text
            place_cursor = &(ei_point_t){
                widget->content_rect->top_left.x - 10,
                widget->content_rect->top_left.y + decal_y - 5
            };


        //draw cursor au bonne endroit
        ei_const_string_t cursor_text = "|";
        if (entry->is_focus_visible && !entry->is_in_selection)
            ei_draw_text(surface,place_cursor, cursor_text, *entry->text_font,(ei_color_t){0,0,0,255} , clipper );

        if(entry->is_in_selection){
            ei_point_t* rect = SAFE_MALLOC(sizeof(ei_point_t)* 4);
            int l_rect_bleu_x,r_rect_bleu_x;
            int size_text,h;
            char* restricted_text = restrict_text(entry->text,(uint8_t) fminf((float)entry->debut_selection,(float)entry->fin_selection));
            hw_text_compute_size(restricted_text,*entry->text_font,&size_text,&h);
            l_rect_bleu_x=entry->widget.content_rect->top_left.x-entry->decal_x+size_text;
            rect[0] = ei_point(l_rect_bleu_x , entry->widget.content_rect->top_left.y); // top left
            rect[1] = ei_point(l_rect_bleu_x, entry->widget.content_rect->top_left.y + entry->widget.content_rect->size.height); //bottom left
            SAFE_FREE(restricted_text);

            char* restricted_text2 =restrict_text(entry->text,(uint8_t) fmaxf((float)entry->debut_selection,(float)entry->fin_selection));
            hw_text_compute_size(restricted_text2,*entry->text_font,&size_text,&h);
            r_rect_bleu_x=entry->widget.content_rect->top_left.x-entry->decal_x+size_text;
            rect[2] = ei_point(r_rect_bleu_x, entry->widget.content_rect->top_left.y + entry->widget.content_rect->size.height);
            rect[3] = ei_point(r_rect_bleu_x, entry->widget.content_rect->top_left.y);
            SAFE_FREE(restricted_text2);

            ei_color_t select_color = (ei_color_t) {25, 25, 200, 100};
            ei_draw_polygon(surface, rect ,4, select_color, &entry->widget.screen_location);
            SAFE_FREE(rect);
        }
    }
    SAFE_FREE(points);

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

/*---------------------------------------------------------------------------------------------------------------------*/


void entry_geomnotifyfunc(ei_widget_t widget){
    if(&widget->screen_location!=widget->content_rect){
        SAFE_FREE(widget->content_rect);
        widget->content_rect=&widget->screen_location;
    }
}

/*---------------------------------------------------------------------------------------------------------------------*/

ei_widgetclass_t* create_entry_widgetclass(){
    ei_widgetclass_t* res = SAFE_MALLOC(sizeof(ei_widgetclass_t));
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


