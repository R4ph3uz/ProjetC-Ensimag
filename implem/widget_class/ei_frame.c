#include "ei_frame.h"
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------------*/

ei_widget_t frame_allocfunc()
{
    ei_impl_frame_t* frame = malloc(sizeof(ei_impl_frame_t));
    return (ei_widget_t) frame;
}

/*--------------------------------------------------------------------------------*/

void frame_releasefunc(ei_widget_t widget)
{
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;

    free(frame->requested_size);

    free(frame->color);
    free(frame->border_width);
    free(frame->relief);
    free(frame->text);
    free(frame->text_font);
    free(frame->text_color);
    free(frame->text_anchor);
    free(frame->img);
    free(frame->img_rect);
    free(frame->img_anchor);

    free(frame);
}


/*--------------------------------------------------------------------------------*/

void frame_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper)
{
    ei_frame_t frame = (ei_frame_t) widget;

    /* Afficher le cadre */
    hw_surface_lock(surface);
    ei_draw_polygon();
    hw_surface_unlock(surface);


    if (frame->text){
        // Si il a du texte a afficher (pour l'instant ignoré)
    }
    if(frame->img){
        // Si il y a un image a afficher (pour l'instant ignoré)
    }

}

/*--------------------------------------------------------------------------------*/

void frame_geomnotifyfunc(ei_widget_t widget)
{

}

/*--------------------------------------------------------------------------------*/

void frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_frame_t frame = (ei_frame_t) widget;

    /* Suite spécifique à une  frame*/
    ei_size_t* requested_size = malloc(sizeof(ei_size_t ));
    requested_size->height = 100;
    requested_size->width = 100;

    ei_color_t* color = malloc(sizeof(ei_color_t ));
    color->alpha = 100;
    color->blue = 255;
    color->green = 100;
    color->red= 0;

    int* border_width = malloc(sizeof(int));
    *border_width = 2;

    ei_relief_t * relief = malloc(sizeof(ei_relief_t));
    *relief = ei_relief_none;


    frame->requested_size = requested_size ;
    frame->color = color;
    frame->border_width = border_width;
    frame->relief = relief;

    frame->text = NULL;
    frame->text_font = NULL;
    frame->text_color = NULL;
    frame->text_anchor = NULL;
    frame->img = NULL;
    frame->img_rect = NULL;
    frame->img_anchor = NULL;
}

/*--------------------------------------------------------------------------------*/

ei_widgetclass_t* create_frame_widgetclass()
{
    ei_widgetclass_t* res = malloc(sizeof(ei_widgetclass_t));
    res->allocfunc = frame_allocfunc;
    res->releasefunc = frame_releasefunc;
    res->drawfunc = frame_drawfunc;
    res->geomnotifyfunc= frame_geomnotifyfunc;
    res->setdefaultsfunc = frame_setdefaultsfunc;

    char name[] = "frame";
    strcpy(res->name, name);
    
    res->next = NULL;
    return res;
}