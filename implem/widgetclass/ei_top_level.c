#include "ei_top_level.h"
#include "../draw_utils/draw_utils.h"
#include "ei_types.h"

/*-----------------------------------------------------------------------------------------------*/

ei_widget_t top_level_allocfunc() {
    ei_impl_top_level_t* top_level = malloc(sizeof(ei_impl_top_level_t));

    top_level->color = malloc(sizeof(ei_color_t));
    top_level->border_width = malloc(sizeof(int));
    top_level->title = malloc(sizeof(ei_string_t));
    top_level->closable = malloc(sizeof(bool));
    top_level->resizable = malloc(sizeof(ei_axis_set_t));
    top_level->min_size = malloc(sizeof(ei_size_t));
}

/*-----------------------------------------------------------------------------------------------*/

void top_level_releasefunc(ei_widget_t widget){
    ei_impl_top_level_t* top_level = (ei_impl_top_level_t*) widget;
    free(top_level->color);
    free(top_level->border_width);
    free(top_level->title);
    free(top_level->closable);
    free(top_level->resizable);
    free(top_level->min_size);
}

/*-----------------------------------------------------------------------------------------------*/

void top_level_setdefaultsfunc(ei_widget_t widget) {
    ei_top_level_t top_level = (ei_top_level_t) widget;

    *top_level->border_width= 1;
    *top_level->closable = true;
    *top_level->resizable = ei_axis_both;
    strcpy(*top_level->title ,"title");
    // memccpy(top_level->min_size ,&(ei_size_t){50,50}, sizeof(ei_size_t));
}

/*-----------------------------------------------------------------------------------------------*/

void top_level_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_top_level_t top_level = (ei_top_level_t) widget;
    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    draw_toplevel(surface,widget->screen_location,10,*top_level->color,clipper);
    draw_toplevel(pick_surface, widget->screen_location,10,*top_level->widget.pick_color, clipper );

    hw_surface_unlock(pick_surface);

    hw_surface_unlock(surface);
    hw_surface_update_rects(surface,NULL);
}

/*-----------------------------------------------------------------------------------------------*/

void top_level_geomnotifyfunc(ei_widget_t widget) {

}

/*-----------------------------------------------------------------------------------------------*/

ei_widgetclass_t* create_top_level_widgetclass(){
    ei_widgetclass_t* res = malloc(sizeof(ei_widgetclass_t));
    res->allocfunc = top_level_allocfunc;
    res->releasefunc = top_level_releasefunc;
    res->drawfunc = top_level_drawfunc;
    res->geomnotifyfunc= top_level_geomnotifyfunc;
    res->setdefaultsfunc = top_level_setdefaultsfunc;

    const char name[] = "top_level";
    strcpy(res->name, name);

    res->next = NULL;
    return res;
}