#include "ei_button.h"
#include "../draw_utils/draw_utils.h"
#include "ei_application.h"
#include "ei_utils.h"
/*--------------------------------------------------------------------------------*/

ei_widget_t button_allocfunc() {
    ei_impl_button_t* button = malloc(sizeof(ei_impl_button_t));

    button->color=malloc(sizeof(ei_color_t));

    button->border_width=malloc(sizeof(int));
    button->corner_radius = malloc(sizeof(int));
    button->relief=malloc(sizeof(ei_relief_t));
    button->text = malloc(sizeof(ei_string_t));
    button->text_font = malloc(sizeof(ei_font_t));
    button->text_color = malloc(sizeof(ei_color_t));
    button->text_anchor = malloc(sizeof(ei_anchor_t));
    button->img = malloc(sizeof(ei_surface_t));
    button->img_rect = malloc(sizeof(ei_rect_ptr_t));
    button->img_anchor = malloc(sizeof(ei_anchor_t));
    // que mettre au callback ?
    button->callback=malloc(sizeof(ei_callback_t));
    button->user_param =malloc(sizeof(ei_user_param_t));
    return (ei_widget_t) button;
}

/*--------------------------------------------------------------------------------*/

void button_releasefunc(ei_widget_t widget) {
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    free(button->color);
    free(button->border_width);
    free(button->corner_radius);
    free(button->relief);
    free(button->text);
    free(button->text_font);
    free(button->text_color);
    free(button->text_anchor);
    free(button->img);
    free(button->img_rect);
    free(button->img_anchor);

    free(button->callback);
    free(button->user_param);
}

/*--------------------------------------------------------------------------------*/

void button_setdefaultsfunc(ei_widget_t widget) {
    ei_button_t button = (ei_button_t) widget;

    button->widget.requested_size.width = 100;
    button->widget.requested_size.height =10;
    button->color->alpha=255;
    button->color->red= 180;
    button->color->blue= 180;
    button->color->green= 180;

    *button->border_width=1;
    *button->corner_radius = 1;
    *button->relief=ei_relief_raised;
    char texte[]="";
    strcpy((char *) button->text, texte);
    ei_fontstyle_t style = ei_style_normal;
    ei_const_string_t name = "misc/font.ttf";
    *button->text_font = hw_text_font_create(name, style, 20);
    *button->text_color= (ei_color_t) {10,10,10, 255};
    *button->text_anchor =ei_anc_northwest;
    button->img=NULL;
    button->img_rect=NULL;
    button->img_anchor=NULL;
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
        draw_button(surface,widget->screen_location,*button->corner_radius,*button->color,*button->relief,clipper);
    draw_button(pick_surface, widget->screen_location,*button->corner_radius,*button->widget.pick_color,ei_relief_none, clipper );

    // if(button->text){
    //     ei_surface_t surface_text = hw_text_create_surface(*button->text_font, *button->text_font, *button->text_color);
    //     ei_rect_t rect_surface_text = hw_surface_get_rect(surface_text);
    //
    //     uint32_t decal_x = widget->screen_location.size.width/2 - rect_surface_text.size.width/2;
    //     uint32_t decal_y = widget->screen_location.size.height/2 - rect_surface_text.size.height/2;
    //     ei_point_t place = {widget->screen_location.top_left.x+decal_x,widget->screen_location.top_left.y+decal_y};
    //     ei_draw_text(surface, &place, *button->text, *button->text_font, *button->text_color, clipper);
    //
    // }
//    ei_surface_t img = hw_image_load("misc/klimt.jpg",ei_app_root_surface() );
//    ei_rect_t test = hw_surface_get_rect(img);
//
//
//    hw_surface_lock(img);
//    ei_copy_surface(surface, &test, img, NULL, true);
//    hw_surface_unlock(img);

    hw_surface_unlock(pick_surface);
    if(button->img){
        // Si il y a un image a afficher (pour l'instant ignoré)
        ei_point_t place = {widget->screen_location.top_left.x,widget->screen_location.top_left.y};
        if(button->img_rect == NULL) {
            button->img_rect = malloc(sizeof(ei_rect_ptr_t));
            *button->img_rect = malloc(sizeof(ei_rect_t));
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

}

/*--------------------------------------------------------------------------------*/

ei_widgetclass_t* create_button_widgetclass() {
    ei_widgetclass_t* res = malloc(sizeof(ei_widgetclass_t));
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
