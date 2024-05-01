#include "ei_button.h"

ei_widget_t button_allocfunc() {
    return NULL;
}

void button_releasefunc(ei_widget_t widget) {

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
