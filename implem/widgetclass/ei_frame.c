#include "ei_frame.h"
#include <stdlib.h>
#include <string.h>
#include "../utils/draw_utils.h"
#include "ei_types.h"
#include "ei_utils.h"
/*--------------------------------------------------------------------------------*/

ei_widget_t frame_allocfunc()
{
    ei_impl_frame_t* frame = SAFE_CALLOC(sizeof(ei_impl_frame_t));


    return (ei_widget_t) frame;
}

/*--------------------------------------------------------------------------------*/

void frame_releasefunc(ei_widget_t widget)
{
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    SAFE_FREE(frame->color);
    SAFE_FREE(frame->border_width);
    SAFE_FREE(frame->relief);
    if(frame->text){
        SAFE_FREE(*frame->text);
        SAFE_FREE(frame->text);
    }


    SAFE_FREE(frame->text_color);
    SAFE_FREE(frame->text_font);
    SAFE_FREE(frame->text_anchor);
    if(frame->img){
        hw_surface_free(*frame->img);
        SAFE_FREE(frame->img);
    }
    SAFE_FREE(frame->img_rect);


    SAFE_FREE(frame->img_anchor);

    // free trucs spécifique aux widgets ?


    SAFE_FREE(frame);
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

    int top_left_x = widget->content_rect->top_left.x;
    int top_left_y = widget->content_rect->top_left.y;


    ei_point_t* points = SAFE_MALLOC(4*sizeof(ei_point_t));
    points[0] = (ei_point_t) {top_left_x, top_left_y };
    points[1] = (ei_point_t) {top_left_x+widget->content_rect->size.width, top_left_y };
    points[2] = (ei_point_t) {top_left_x+widget->content_rect->size.width, top_left_y+widget->content_rect->size.height };
    points[3] = (ei_point_t) {top_left_x, top_left_y+widget->content_rect->size.height };
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
            ei_point_t* border = SAFE_MALLOC(4*sizeof(ei_point_t));
            border[0] = (ei_point_t) {top_left_x-*frame->border_width, top_left_y -*frame->border_width};
            border[1] = (ei_point_t) {top_left_x+widget->content_rect->size.width+*frame->border_width, top_left_y-*frame->border_width };
            border[2] = (ei_point_t) {top_left_x+widget->content_rect->size.width+*frame->border_width, top_left_y+widget->content_rect->size.height+*frame->border_width };
            border[3] = (ei_point_t) {top_left_x-*frame->border_width, top_left_y+widget->content_rect->size.height+*frame->border_width };
            ei_color_t color = (ei_color_t) {0,0,0, 255};
            ei_draw_polygon(surface, border, nb_points, color, clipper);
            ei_draw_polygon(surface, points, nb_points, *frame->color, clipper);
            SAFE_FREE(border);
        }
    }

    ei_draw_polygon(pick_surface, points, nb_points, *frame->widget.pick_color, clipper);

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);

    if (frame->text){
        // Si il a du texte a afficher (pour l'instant ignoré)
        int width, height;
        hw_text_compute_size(*frame->text, *frame->text_font, &width,&height);
        uint32_t decal_x =0;
        uint32_t decal_y =0;
        if (!frame->text_anchor)
        {
            decal_x = widget->content_rect->size.width/2-width/2;
            decal_y = widget->content_rect->size.height/2-height/2;
        }
        else if (*frame->text_anchor==ei_anc_north)
        {
            decal_x = widget->content_rect->size.width/2-width/2;
        }
        else if (*frame->text_anchor==ei_anc_northeast)
        {
            decal_x = widget->content_rect->size.width-width;
        }
        else if (*frame->text_anchor==ei_anc_west)
        {
            decal_y = widget->content_rect->size.height/2-height/2;
        }
        else if (*frame->text_anchor==ei_anc_center)
        {
            decal_x = widget->content_rect->size.width/2-width/2;
            decal_y = widget->content_rect->size.height/2-height/2;
        }
        else if (*frame->text_anchor==ei_anc_east)
        {
            decal_x = widget->content_rect->size.width-width;
            decal_y = widget->content_rect->size.height/2-height/2;
        }
        else if (*frame->text_anchor==ei_anc_southwest)
        {
            decal_y = widget->content_rect->size.height-height;
        }
        else if (*frame->text_anchor==ei_anc_south)
        {
            decal_x = widget->content_rect->size.width/2-width/2;
            decal_y = widget->content_rect->size.height-height;
        }
        else if (*frame->text_anchor==ei_anc_southeast)
        {
            decal_x = widget->content_rect->size.width-width;
            decal_y = widget->content_rect->size.height-height;
        }





        ei_point_t place = {widget->content_rect->top_left.x+decal_x,widget->content_rect->top_left.y+decal_y};
        hw_surface_lock(surface);
        ei_draw_text(surface, &place, *frame->text, *frame->text_font, *frame->text_color, clipper);
        hw_surface_unlock(surface);

    }
    if(frame->img)
    {
        ei_point_t place = {0,0};
        ei_rect_t rect=hw_surface_get_rect(*frame->img);
        if (widget->content_rect->size.width<rect.size.width ||widget->content_rect->size.height<rect.size.height)
            *frame->img_anchor=ei_anc_northwest;
        if(!frame->img_anchor)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else if (*frame->img_anchor==ei_anc_northwest)
        {
            place.x=widget->content_rect->top_left.x;
            place.y=widget->content_rect->top_left.y;

        }
        else if (*frame->img_anchor==ei_anc_north)
        {
            place.x=widget->content_rect->top_left.x;
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else if (*frame->img_anchor==ei_anc_northeast)
        {
            place.x=widget->content_rect->top_left.x;
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height))-(int)((float)rect.size.height);
        }
        else if (*frame->img_anchor==ei_anc_west)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y;
        }
        else if (*frame->img_anchor==ei_anc_center)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else if (*frame->img_anchor==ei_anc_east)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height))-(int)((float)rect.size.height);
        }
        else if (*frame->img_anchor==ei_anc_southwest)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width))-(int)((float)rect.size.width);
            place.y=widget->content_rect->top_left.y;
        }
        else if (*frame->img_anchor==ei_anc_south)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width))-(int)((float)rect.size.width);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width))-(int)((float)rect.size.width);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height))-(int)((float)rect.size.height);
        }
        if(frame->img_rect == NULL) {
            frame->img_rect = SAFE_MALLOC(sizeof(ei_rect_ptr_t));
            *frame->img_rect = SAFE_MALLOC(sizeof(ei_rect_t));
            **frame->img_rect = hw_surface_get_rect(*frame->img);
        }

        ei_rect_t test = ei_rect(place,(*frame->img_rect)->size);
        hw_surface_lock(surface);
        hw_surface_lock(*frame->img);
        ei_copy_surface(surface, &test, *frame->img, *frame->img_rect, true);
        hw_surface_unlock(*frame->img);

        hw_surface_unlock(surface);
    }

    SAFE_FREE(points);
}

/*--------------------------------------------------------------------------------*/

void frame_geomnotifyfunc(ei_widget_t widget)
{
    if(&widget->screen_location!=widget->content_rect)
        SAFE_FREE(widget->content_rect);
    if(&widget->screen_location!=widget->content_rect){
        SAFE_FREE(widget->content_rect);
        widget->content_rect=&widget->screen_location;
    }
}

/*--------------------------------------------------------------------------------*/

void frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_frame_t frame = (ei_frame_t) widget;

    /* Suite spécifique à une  frame*/
    frame->color = SAFE_MALLOC(sizeof(ei_color_t));
    frame->relief = SAFE_MALLOC(sizeof(ei_relief_t));
    frame->text_font = SAFE_MALLOC(sizeof(ei_font_t));
    frame->text_color = SAFE_MALLOC(sizeof(ei_color_t));
    frame->text_anchor = SAFE_MALLOC(sizeof(ei_anchor_t));
    frame->img_anchor = SAFE_MALLOC(sizeof(ei_anchor_t));
    frame->border_width = SAFE_MALLOC(sizeof(int));
    frame->color->alpha = 255;
    frame->color->red = 149;
    frame->color->green = 149;
    frame->color->blue = 149;
    *frame->border_width =0;
    *frame->relief = ei_relief_none;

    frame->text = NULL;
    *frame->text_font = ei_default_font;
    *frame->text_color= (ei_color_t) {0,0,0, 0 };
    *frame->text_anchor =ei_anc_center;
    frame->img = NULL;
    *frame->img_anchor = ei_anc_center;
    frame->img_rect = NULL;
}

/*--------------------------------------------------------------------------------*/

ei_widgetclass_t* create_frame_widgetclass()
{
    ei_widgetclass_t* res = SAFE_MALLOC(sizeof(ei_widgetclass_t));
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