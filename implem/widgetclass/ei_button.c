#include "ei_button.h"
#include "../draw_utils/draw_utils.h"
/*--------------------------------------------------------------------------------*/

ei_widget_t button_allocfunc() {
    ei_impl_button_t* button = malloc(sizeof(ei_impl_button_t));

    button->requested_size = malloc(sizeof(ei_size_t));
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
    free(button->requested_size);
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

    button->requested_size->width = 30;
    button->requested_size->height =30;
    // button->color->alpha=255;
    // button->color->red= 180;
    // button->color->blue= 180;
    // button->color->green= 180;

    *button->border_width=1;
    *button->corner_radius = 1;
    *button->relief=ei_relief_none;
    char texte[]="";
    strcpy((char *) button->text, texte);
    ei_fontstyle_t style = ei_style_normal;
    ei_const_string_t name = "misc/font.ttf";
    *button->text_font = hw_text_font_create(name, style, 20);
    *button->text_color= (ei_color_t) {0,0,0};
    *button->text_anchor =ei_anc_northwest;
    button->img=NULL;
    button->img_rect=NULL;
    button->img_anchor=NULL;
    // que mettre au callback ?
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

    draw_button(surface,widget->screen_location,*button->corner_radius,*button->color,*button->relief,clipper);
    draw_button(pick_surface, widget->screen_location,*button->corner_radius,*button->widget.pick_color,ei_relief_none, clipper );

    if(button->text){
        uint32_t decal_x = widget->screen_location.size.width/10;
        uint32_t decal_y = widget->screen_location.size.height/2;
        ei_point_t place = {widget->screen_location.top_left.x+decal_x,widget->screen_location.top_left.y+decal_y};
        ei_draw_text(surface, &place, *button->text, *button->text_font, *button->text_color, clipper);

    }
    // ei_surface_t surface_image = hw_image_load("misc/bomb.png",surface);
    // ei_rect_t rect_surface_image = hw_surface_get_rect(surface_image);

    hw_surface_unlock(pick_surface);
    // hw_surface_lock(surface_image);
    // ei_copy_surface(surface, &rect_surface_image, surface_image, NULL, true);
    // hw_surface_unlock(surface_image);

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
