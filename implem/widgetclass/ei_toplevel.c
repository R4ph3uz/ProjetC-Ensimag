#include "ei_toplevel.h"
#include "../utils/draw_utils.h"
#include "ei_types.h"

/*-----------------------------------------------------------------------------------------------*/

ei_widget_t toplevel_allocfunc() {
    ei_impl_toplevel_t* top_level = SAFE_CALLOC(sizeof(ei_impl_toplevel_t));


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

void toplevel_drawfunc(ei_widget_t widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper){
    if(widget->geom_params)
        widget->geom_params->manager->runfunc(widget);
    ei_toplevel_t top_level = (ei_toplevel_t) widget;
    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    draw_toplevel(surface,*widget->content_rect,10,*top_level->color,clipper, false, top_level->resizable);
    draw_toplevel(pick_surface, *widget->content_rect, 10, *top_level->widget.pick_color, clipper, true,
                  top_level->resizable);
    hw_surface_unlock(pick_surface);
    if(top_level->title)
    {
        uint32_t decal_x = 20;
        int32_t decal_y = -25;
        ei_point_t place = {widget->content_rect->top_left.x+decal_x,widget->content_rect->top_left.y+decal_y};
        ei_color_t	blanc	= { 0xdf, 0xdf, 0xdf, 0xff };
        void* test= hw_text_font_create("misc/font.ttf",ei_style_normal,20);
        ei_rect_t* text_clipper = intersection_rectangle(*clipper, top_level->widget.screen_location);
        ei_draw_text(surface, &place, *top_level->title, test, blanc, text_clipper);
        free(test);
        SAFE_FREE(text_clipper);
    }


    hw_surface_unlock(surface);
}

/*-----------------------------------------------------------------------------------------------*/

void toplevel_geomnotifyfunc(ei_widget_t widget) {
    if(&widget->screen_location==widget->content_rect)
        widget->content_rect = SAFE_MALLOC(sizeof(ei_rect_t ));
    widget->content_rect->top_left.x=widget->screen_location.top_left.x;
    widget->content_rect->top_left.y=widget->screen_location.top_left.y+30;
    widget->content_rect->size.width=widget->screen_location.size.width;
    widget->content_rect->size.height=widget->screen_location.size.height;
    widget->screen_location.size.height+=30;
}

/*-----------------------------------------------------------------------------------------------*/

void toplevel_setdefaultsfunc(ei_widget_t widget) {
    ei_toplevel_t top_level = (ei_toplevel_t) widget;
    top_level->color = SAFE_MALLOC(sizeof(ei_color_t));
    top_level->border_width = SAFE_MALLOC(sizeof(int));
    top_level->title = SAFE_MALLOC(sizeof(ei_string_t));
    top_level->closable = SAFE_MALLOC(sizeof(bool));
    top_level->resizable = SAFE_MALLOC(sizeof(ei_axis_set_t));
    top_level->min_size = SAFE_MALLOC(sizeof(ei_size_t*));
    (*top_level->min_size)= SAFE_MALLOC(sizeof(ei_size_t));
    *top_level->border_width= 1;
    *top_level->closable = true;
    *top_level->resizable = ei_axis_both;
    const char* title = "title";
    strcpy((char*) top_level->title ,title);
    (*top_level->min_size)->width = 160;
    (*top_level->min_size)->height = 120;
    top_level->color->alpha = 255;
    top_level->color->red = 149;
    top_level->color->green = 149;
    top_level->color->blue = 149;

}
/*-----------------------------------------------------------------------------------------------*/

ei_widgetclass_t* create_toplevel_widgetclass(){
    ei_widgetclass_t* res = SAFE_MALLOC(sizeof(ei_widgetclass_t));
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