#include "ei_frame.h"
#include <stdlib.h>
#include <string.h>
#include "../draw_utils/draw_utils.h"
#include "ei_types.h"
#include "ei_utils.h"
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
//    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
//    free(frame->color);
//    free(frame->border_width);
//    free(frame->relief);
//    free(frame->text);
//    free(frame->text_font);
//    free(frame->text_color);
//    free(frame->text_anchor);
//    free(frame->img);
//    free(frame->img_rect);
//    free(frame->img_anchor);
//
//    // free trucs spécifique aux widgets ?
//
//
//    free(frame);
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

    int top_left_x = widget->screen_location.top_left.x;
    int top_left_y = widget->screen_location.top_left.y;


    ei_point_t* points = malloc(4*sizeof(ei_point_t));
    points[0] = (ei_point_t) {top_left_x, top_left_y };
    points[1] = (ei_point_t) {top_left_x+widget->screen_location.size.width, top_left_y };
    points[2] = (ei_point_t) {top_left_x+widget->screen_location.size.width, top_left_y+widget->screen_location.size.height };
    points[3] = (ei_point_t) {top_left_x, top_left_y+widget->screen_location.size.height };
    size_t nb_points = 4;

    /* Afficher le cadre */
    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);
    if(frame->border_width!=NULL) { // doit etre fait avant de dessiner la frame (vu que en dessous)
        if (frame->relief!=NULL){
            //dessine le relief
            draw_button(surface, frame->widget.screen_location, 0, *frame->color, *frame->relief, clipper);
        }
        else{
            ei_point_t* border = malloc(4*sizeof(ei_point_t));
            border[0] = (ei_point_t) {top_left_x-*frame->border_width, top_left_y -*frame->border_width};
            border[1] = (ei_point_t) {top_left_x+widget->screen_location.size.width+*frame->border_width, top_left_y-*frame->border_width };
            border[2] = (ei_point_t) {top_left_x+widget->screen_location.size.width+*frame->border_width, top_left_y+widget->screen_location.size.height+*frame->border_width };
            border[3] = (ei_point_t) {top_left_x-*frame->border_width, top_left_y+widget->screen_location.size.height+*frame->border_width };
            ei_color_t color = (ei_color_t) {0,0,0, 255};
            ei_draw_polygon(surface, border, nb_points, color, clipper);
            ei_draw_polygon(surface, points, nb_points, *frame->color, clipper);
            free(border);
        }
    }

    ei_draw_polygon(pick_surface, points, nb_points, *frame->widget.pick_color, clipper);

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);

    if (frame->text){
        // Si il a du texte a afficher (pour l'instant ignoré)
        int width, height;
        hw_text_compute_size(*frame->text, *frame->text_font, &width,&height);
        uint32_t decal_x = widget->screen_location.size.width/2-width/2;
        uint32_t decal_y = widget->screen_location.size.height/2-height/2;
        ei_point_t place = {widget->screen_location.top_left.x+decal_x,widget->screen_location.top_left.y+decal_y};
        hw_surface_lock(surface);
        ei_draw_text(surface, &place, *frame->text, *frame->text_font, *frame->text_color, clipper);
        hw_surface_unlock(surface);

    }
    if(frame->img){
        // Si il y a un image a afficher (pour l'instant ignoré)

            // Si il y a un image a afficher (pour l'instant ignoré)
        ei_point_t place = {widget->screen_location.top_left.x,widget->screen_location.top_left.y};
        if(frame->img_rect == NULL) {
            frame->img_rect = malloc(sizeof(ei_rect_ptr_t));
            *frame->img_rect = malloc(sizeof(ei_rect_t));
            **frame->img_rect = hw_surface_get_rect(*frame->img);
        }

        ei_rect_t test = ei_rect(place,(*frame->img_rect)->size);
        hw_surface_lock(surface);
        hw_surface_lock(*frame->img);
        ei_copy_surface(surface, &test, *frame->img, *frame->img_rect, true);
        hw_surface_unlock(*frame->img);

        hw_surface_unlock(surface);
    }

    free(points);
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

    frame->color->alpha = 255;
    frame->color->red = 149;
    frame->color->green = 149;
    frame->color->blue = 149;
    *frame->border_width =0;
    *frame->relief = ei_relief_none;

    frame->text = NULL;
    ei_const_string_t name = "misc/font.ttf";
    ei_fontstyle_t style = ei_style_normal;
    *frame->text_font = hw_text_font_create(name, style, 20);
    *frame->text_color= (ei_color_t) {0,0,0};
    *frame->text_anchor =ei_anc_northwest;
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