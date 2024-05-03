#include "ei_button.h"

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
    button->relief=ei_relief_none;
    button->text=NULL;
    button->text_font=NULL;
    button->text_color=NULL;
    button->text_anchor =NULL;
    button->img=NULL;
    button->img_rect=NULL;
    button->img_anchor=NULL;
    // que mettre au callback ?
    button->callback=NULL;
    button->user_param =NULL;
}

void button_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper) {

}

void button_geomnotifyfunc(ei_widget_t widget) {

}

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
