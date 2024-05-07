#include "ei_toplevel.h"
#include "../draw_utils/draw_utils.h"
#include "ei_types.h"

/*-----------------------------------------------------------------------------------------------*/

ei_widget_t toplevel_allocfunc() {
    ei_impl_toplevel_t* top_level = malloc(sizeof(ei_impl_toplevel_t));

    top_level->color = malloc(sizeof(ei_color_t));
    top_level->border_width = malloc(sizeof(int));
    top_level->title = malloc(sizeof(ei_string_t));
    top_level->closable = malloc(sizeof(bool));
    top_level->resizable = malloc(sizeof(ei_axis_set_t));
    top_level->min_size = malloc(sizeof(ei_size_t*));
    (*top_level->min_size)= malloc(sizeof(ei_size_t));
    ei_widget_t widget = (ei_widget_t) top_level;
    return widget;
}

/*-----------------------------------------------------------------------------------------------*/

void toplevel_releasefunc(ei_widget_t widget){
    ei_impl_toplevel_t* top_level = (ei_impl_toplevel_t*) widget;
    free(top_level->color);
    free(top_level->border_width);
    free(top_level->title);
    free(top_level->closable);
    free(top_level->resizable);
    free(top_level->min_size);
    // free trucs spécifique aux widgets ?
    free(top_level);
}

/*-----------------------------------------------------------------------------------------------*/

void toplevel_setdefaultsfunc(ei_widget_t widget) {
    ei_toplevel_t top_level = (ei_toplevel_t) widget;

    *top_level->border_width= 1;
    *top_level->closable = true;
    *top_level->resizable = ei_axis_both;
    const char* title = "title";
    strcpy((char*) top_level->title ,title);
    (*top_level->min_size)->width = 100;
    (*top_level->min_size)->height = 100;
}

/*-----------------------------------------------------------------------------------------------*/

void toplevel_drawfunc(ei_widget_t widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper){
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_toplevel_t top_level = (ei_toplevel_t) widget;
    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    draw_toplevel(surface,widget->screen_location,10,*top_level->color,clipper, false, top_level->resizable);
    draw_toplevel(pick_surface, widget->screen_location,10,*top_level->widget.pick_color, clipper, true, top_level->resizable );
    hw_surface_unlock(pick_surface);
    if(top_level->title)
    {
        uint32_t decal_x = 20;
        uint32_t decal_y = -25;
        ei_point_t place = {widget->screen_location.top_left.x+decal_x,widget->screen_location.top_left.y+decal_y};
        ei_color_t	blanc	= { 0xdf, 0xdf, 0xdf, 0xff };
        void* test= hw_text_font_create("misc/font.ttf",ei_style_normal,20);
        ei_draw_text(surface, &place, *top_level->title, test, blanc, clipper);

    }


    hw_surface_unlock(surface);
}

/*-----------------------------------------------------------------------------------------------*/

void toplevel_geomnotifyfunc(ei_widget_t widget) {

}

/*-----------------------------------------------------------------------------------------------*/

ei_widgetclass_t* create_toplevel_widgetclass(){
    ei_widgetclass_t* res = malloc(sizeof(ei_widgetclass_t));
    res->allocfunc = toplevel_allocfunc;
    res->releasefunc = toplevel_releasefunc;
    res->drawfunc = toplevel_drawfunc;
    res->geomnotifyfunc= toplevel_geomnotifyfunc;
    res->setdefaultsfunc = toplevel_setdefaultsfunc;

    const char name[] = "toplevel";
    strcpy(res->name, name);

    res->next = NULL;
    return res;
}