#include "ei_button.h"
#include "../utils/draw_utils.h"
#include "ei_application.h"
#include "ei_utils.h"
/*--------------------------------------------------------------------------------*/

ei_widget_t button_allocfunc() {
    ei_impl_button_t* button = SAFE_CALLOC(sizeof(ei_impl_button_t));


    return (ei_widget_t) button;
}

/*--------------------------------------------------------------------------------*/

void button_releasefunc(ei_widget_t widget) {
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    SAFE_FREE(button->color);
    SAFE_FREE(button->border_width);
    SAFE_FREE(button->corner_radius);
    SAFE_FREE(button->relief);
    if(button->text!=NULL) {
        if(*button->text)
            SAFE_FREE(*(button->text));
        SAFE_FREE(button->text);
    }
    SAFE_FREE(button->text_font);
    SAFE_FREE(button->text_color);
    SAFE_FREE(button->text_anchor);
    if(button->img)
        // hw_surface_unlock(button->img);
    {
        hw_surface_free(button->img);
        button->img = NULL;
    }
    if(button->img_rect && *button->img_rect)
        SAFE_FREE(*button->img_rect)
    SAFE_FREE(button->img_rect);
    SAFE_FREE(button->img_anchor);

    SAFE_FREE(button->callback);
    SAFE_FREE(button);
}

/*--------------------------------------------------------------------------------*/

void button_setdefaultsfunc(ei_widget_t widget) {
    ei_button_t button = (ei_button_t) widget;
    button->color=SAFE_MALLOC(sizeof(ei_color_t));
    button->border_width=SAFE_MALLOC(sizeof(int));
    button->corner_radius = SAFE_MALLOC(sizeof(int));
    button->relief=SAFE_MALLOC(sizeof(ei_relief_t));
    button->text_font = SAFE_MALLOC(sizeof(ei_font_t));
    button->text_color = SAFE_MALLOC(sizeof(ei_color_t));
    button->text_anchor = SAFE_MALLOC(sizeof(ei_anchor_t));
    button->img_anchor = SAFE_MALLOC(sizeof(ei_anchor_t));

    button->widget.requested_size.width = 100;
    button->widget.requested_size.height =100;
    button->color->alpha=255;
    button->color->red= 180;
    button->color->blue= 180;
    button->color->green= 180;

    *button->border_width=3;
    *button->corner_radius = 10;
    *button->relief=ei_relief_raised;
    button->text = NULL;
    *button->text_font = ei_default_font;
    *button->text_color= (ei_color_t) {10,10,10, 255};
    *button->text_anchor =ei_anc_center;
    button->img=NULL;
    button->img_rect=NULL;
    *button->img_anchor=ei_anc_center;
    button->callback=NULL;
    button->user_param =NULL;
}

/*--------------------------------------------------------------------------------*/

void button_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper) {
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_button_t button = (ei_button_t) widget;
    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    if(!button->img)
        draw_button(surface,*widget->content_rect,*button->corner_radius,*button->color,*button->relief,clipper);
    draw_button(pick_surface, *widget->content_rect,*button->corner_radius,*button->widget.pick_color,ei_relief_none, clipper );

     if(button->text && *button->text){
         int width, height;
         hw_text_compute_size(*button->text, *button->text_font, &width,&height);


         uint32_t decal_x =0;
         uint32_t decal_y =0;
         if (!button->text_anchor)
         {
             decal_x = widget->content_rect->size.width/2-width/2;
             decal_y = widget->content_rect->size.height/2-height/2;
         }
         else if (*button->text_anchor==ei_anc_north)
         {
             decal_x = widget->content_rect->size.width/2-width/2;
         }
         else if (*button->text_anchor==ei_anc_northeast)
         {
             decal_x = widget->content_rect->size.width-width;
         }
         else if (*button->text_anchor==ei_anc_west)
         {
             decal_y = widget->content_rect->size.height/2-height/2;
         }
         else if (*button->text_anchor==ei_anc_center)
         {
             decal_x = widget->content_rect->size.width/2-width/2;
             decal_y = widget->content_rect->size.height/2-height/2;
         }
         else if (*button->text_anchor==ei_anc_east)
         {
             decal_x = widget->content_rect->size.width-width;
             decal_y = widget->content_rect->size.height/2-height/2;
         }
         else if (*button->text_anchor==ei_anc_southwest)
         {
             decal_y = widget->content_rect->size.height-height;
         }
         else if (*button->text_anchor==ei_anc_south)
         {
             decal_x = widget->content_rect->size.width/2-width/2;
             decal_y = widget->content_rect->size.height-height;
         }
         else if (*button->text_anchor==ei_anc_southeast)
         {
             decal_x = widget->content_rect->size.width-width;
             decal_y = widget->content_rect->size.height-height;
         }

         if (*button->relief == ei_relief_sunken){
             decal_x += 5 * widget->content_rect->size.height/100;
             decal_y += 5 * widget->content_rect->size.height/100;
         }
         ei_point_t place = {widget->content_rect->top_left.x+decal_x,widget->content_rect->top_left.y+decal_y};
         ei_draw_text(surface, &place, *button->text, *button->text_font, *button->text_color, clipper);

     }

    hw_surface_unlock(pick_surface);
    if(button->img){
        // Si il y a un image a afficher (pour l'instant ignoré)
        ei_point_t place = {0,0};
        ei_rect_t rect=hw_surface_get_rect(*button->img);
        if (widget->content_rect->size.width<rect.size.width ||widget->content_rect->size.height<rect.size.height)
            *button->img_anchor=ei_anc_northwest;
        if(!button->img_anchor)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else if (*button->img_anchor==ei_anc_northwest)
        {
            place.x=widget->content_rect->top_left.x;
            place.y=widget->content_rect->top_left.y;

        }
        else if (*button->img_anchor==ei_anc_north)
        {
            place.x=widget->content_rect->top_left.x;
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else if (*button->img_anchor==ei_anc_northeast)
        {
            place.x=widget->content_rect->top_left.x;
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height))-(int)((float)rect.size.height);
        }
        else if (*button->img_anchor==ei_anc_west)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y;
        }
        else if (*button->img_anchor==ei_anc_center)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else if (*button->img_anchor==ei_anc_east)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width)/2)-(int)((float)rect.size.width/2);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height))-(int)((float)rect.size.height);
        }
        else if (*button->img_anchor==ei_anc_southwest)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width))-(int)((float)rect.size.width);
            place.y=widget->content_rect->top_left.y;
        }
        else if (*button->img_anchor==ei_anc_south)
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width))-(int)((float)rect.size.width);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height)/2)-(int)((float)rect.size.height/2);
        }
        else
        {
            place.x=widget->content_rect->top_left.x+(int)((float)(widget->content_rect->size.width))-(int)((float)rect.size.width);
            place.y=widget->content_rect->top_left.y+(int)((float)(widget->content_rect->size.height))-(int)((float)rect.size.height);
        }
        if(button->img_rect == NULL) {
            button->img_rect = SAFE_MALLOC(sizeof(ei_rect_ptr_t));
            *button->img_rect = SAFE_MALLOC(sizeof(ei_rect_t));
            **button->img_rect = hw_surface_get_rect(*button->img);
        }

        ei_rect_t test = ei_rect(place,(*button->img_rect)->size);
        hw_surface_lock(*button->img);
        ei_copy_surface(surface, &test, *button->img, *button->img_rect, true);
        hw_surface_unlock(*button->img);
    }

    hw_surface_unlock(surface);


}

/*--------------------------------------------------------------------------------*/

void button_geomnotifyfunc(ei_widget_t widget) {
    if(&widget->screen_location!=widget->content_rect)
        SAFE_FREE(widget->content_rect);
    widget->content_rect=&widget->screen_location;
}

/*--------------------------------------------------------------------------------*/

ei_widgetclass_t* create_button_widgetclass() {
    ei_widgetclass_t* res = SAFE_MALLOC(sizeof(ei_widgetclass_t));
    res->allocfunc = button_allocfunc;
    res->releasefunc = button_releasefunc;
    res->drawfunc = button_drawfunc;
    res->geomnotifyfunc= button_geomnotifyfunc;
    res->setdefaultsfunc = button_setdefaultsfunc;

    const char name[] = "button";
    strcpy(res->name, name);

    res->next = NULL;
    return res;
}
