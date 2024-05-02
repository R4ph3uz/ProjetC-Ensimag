#include "ei_frame.h"
#include "../ei_implementation.h"
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------------*/

ei_widget_t frame_allocfunc()
{
    ei_impl_frame_t* frame = malloc(sizeof(ei_impl_frame_t));

    frame->color = malloc(sizeof(ei_color_t));
    frame->relief = malloc(sizeof(ei_relief_t));
    frame->text = malloc(sizeof(ei_string_t));
    frame->text_font = malloc(sizeof(ei_font_t));
    frame->text_color = malloc(sizeof(ei_color_t));
    frame->text_anchor = malloc(sizeof(ei_anchor_t));
    frame->img = malloc(sizeof(ei_surface_t));
    frame->img_rect = malloc(sizeof(ei_rect_ptr_t));
    frame->img_anchor = malloc(sizeof(ei_anchor_t));
    frame->border_width = malloc(sizeof(int));
    return (ei_widget_t) frame;
}

/*--------------------------------------------------------------------------------*/

void frame_releasefunc(ei_widget_t widget)
{
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
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

    // free trucs spécifique aux widgets ?


    free(frame);
}


/*--------------------------------------------------------------------------------*/

void frame_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper)
{
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_frame_t frame = (ei_frame_t) widget;

    //Si requested size est bien géré
    int top_left_x = widget->screen_location.top_left.x;
    int top_left_y = widget->screen_location.top_left.y;


     ei_point_t* points = malloc(4*sizeof(ei_point_t));
     points[0] = (ei_point_t) {top_left_x, top_left_y };
     points[1] = (ei_point_t) {top_left_x+widget->screen_location.size.width, top_left_y };
     points[2] = (ei_point_t) {top_left_x+widget->screen_location.size.width, top_left_y+widget->screen_location.size.height };
     points[3] = (ei_point_t) {top_left_x, top_left_y+widget->screen_location.size.height };

//    // Toujours en haut a gauche
//    ei_point_t* points = malloc(4*sizeof(ei_point_t));
//    points[0] = (ei_point_t) {0, 0 };
//    points[1] = (ei_point_t) {widget->requested_size.width, 0 };
//    points[2] = (ei_point_t) {widget->requested_size.width, widget->requested_size.height };
//    points[3] = (ei_point_t) {0, widget->requested_size.height };

    /* Afficher le cadre */
    hw_surface_lock(surface);
    ei_draw_polygon(surface, points, 4, *frame->color, clipper);
    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, NULL);


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

    frame->color->alpha = 100;
    frame->color->blue = 255;
    frame->color->green = 100;
    frame->color->red= 0;
    *frame->border_width = 2;
    *frame->relief = ei_relief_none;

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

    const char name[] = "frame";
    strcpy(res->name, name);
    
    res->next = NULL;
    return res;
}