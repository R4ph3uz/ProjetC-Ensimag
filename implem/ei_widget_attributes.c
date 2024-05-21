#include "ei_geometrymanager.h"
#include "ei_implementation.h"
/*-------------------------------------------------------------------------------------------------------*/

ei_widgetclass_t*	ei_widget_get_class		(ei_widget_t		widget)
{
    return widget->wclass;
}

/*-------------------------------------------------------------------------------------------------------*/

const ei_color_t*	ei_widget_get_pick_color	(ei_widget_t		widget)
{
    return widget->pick_color;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t 		ei_widget_get_parent		(ei_widget_t		widget)
{
    return widget->parent;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t 		ei_widget_get_first_child	(ei_widget_t		widget)
{
    return widget->children_head;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t 		ei_widget_get_last_child	(ei_widget_t		widget)
{
    return widget->children_tail;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t 		ei_widget_get_next_sibling	(ei_widget_t		widget)
{
    return widget->next_sibling;
}

/*-------------------------------------------------------------------------------------------------------*/

void*			ei_widget_get_user_data		(ei_widget_t		widget)
{
    return widget->user_data;
}

/*-------------------------------------------------------------------------------------------------------*/

const ei_size_t*	ei_widget_get_requested_size	(ei_widget_t		widget)
{
    const ei_size_t* res = &widget->requested_size;
    return res;
}

/*-------------------------------------------------------------------------------------------------------*/

void	 		ei_widget_set_requested_size	(ei_widget_t		widget, ei_size_t 		requested_size)
{
    widget->requested_size.width = requested_size.width;
    widget->requested_size.height = requested_size.height;
    if (( widget->geom_params) && *widget->geom_params->is_reconfigurable)
    {       //Si le widget est reconfigurable (la taille du widget depend de la requested) ,alors, l'actualiser
        *widget->geom_params->height = requested_size.height;
        *widget->geom_params->width = requested_size.width;
        widget->geom_params->manager->runfunc(widget);
    }
}

/*-------------------------------------------------------------------------------------------------------*/

const ei_rect_t*	ei_widget_get_screen_location	(ei_widget_t		widget)
{
    const ei_rect_t* res = &widget->screen_location;
    return res;
}

/*-------------------------------------------------------------------------------------------------------*/

const ei_rect_t*	ei_widget_get_content_rect	(ei_widget_t		widget)
{
    return widget->content_rect;
}

/*-------------------------------------------------------------------------------------------------------*/

void	 		ei_widget_set_content_rect	(ei_widget_t		widget, const ei_rect_t*	content_rect)
{
    if(widget->content_rect!=&widget->screen_location)
        SAFE_FREE(widget->content_rect);
    if(widget->content_rect==NULL)
    {
        widget->content_rect=SAFE_MALLOC(sizeof(ei_rect_t));
    }
    memcpy(widget->content_rect,content_rect, sizeof (ei_rect_t));

}