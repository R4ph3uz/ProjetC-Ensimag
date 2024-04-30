#include "ei_frame.h"
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------------*/

ei_widget_t frame_allocfunc()
{
    ei_impl_frame_t* frame = malloc(sizeof(ei_impl_frame_t));
    return (ei_widget_t) frame;
}

/*--------------------------------------------------------------------------------*/

void frame_releasefunc(ei_widget_t widget)
{
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    free(frame);
}


/*--------------------------------------------------------------------------------*/

void frame_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper)
{
    ei_frame_t frame = (ei_frame_t) widget;

}

/*--------------------------------------------------------------------------------*/

void frame_geomnotifyfunc(ei_widget_t widget)
{

}

/*--------------------------------------------------------------------------------*/

void frame_setdefaultsfunc(ei_widget_t widget)
{

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

    char name[] = "frame";
    strcpy(res->name, name);
    
    res->next = NULL;
    return res;
}