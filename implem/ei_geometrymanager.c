#include "ei_geometrymanager.h"
#include "ei_implementation.h"

/*-------------------------------------------------------------------------------------------------------*/

static ei_geometrymanager_t* LISTE_GEOMETRYMANAGER= NULL;


/*-------------------------------------------------------------------------------------------------------*/

size_t		ei_geom_param_size()
{
    return (sizeof(ei_impl_geom_param_t));
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location)
{
    //Si différent

    if (widget->content_rect==NULL || (widget->screen_location.size.height != new_screen_location->size.height) || (
            widget->screen_location.size.width != new_screen_location->size.width) || (
            widget->screen_location.top_left.x != new_screen_location->top_left.x) || (
            widget->screen_location.top_left.y != new_screen_location->top_left.y))
    {
        // widget->screen_location=*new_screen_location;
        memcpy(&widget->screen_location,new_screen_location,sizeof(widget->screen_location));
        // Il faut schedule un redraw  d'après la doc, a voir comment faire vu que pour l'instant, cette fonction est appeler par draw , donc jsp trop
        widget->wclass->geomnotifyfunc(widget);
        if (widget->children_tail)
        {
            ei_widget_t enfant=widget->children_head;
            while(enfant!=NULL)
            {
                if (enfant->geom_params)
                enfant->geom_params->manager->runfunc(enfant);
                enfant=enfant->next_sibling;
            }
        }
    }
    free(new_screen_location);
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager)
{
    // pour mieux comprendre voir ei_widgetclass
    if(LISTE_GEOMETRYMANAGER==NULL)
    {
        LISTE_GEOMETRYMANAGER=geometrymanager;
    }
    else
    {
        geometrymanager->next=LISTE_GEOMETRYMANAGER;
        LISTE_GEOMETRYMANAGER=geometrymanager;
    }
}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name)
{
    ei_geometrymanager_t* actual = LISTE_GEOMETRYMANAGER;
    while(actual != NULL)
    {
        if (strcmp(actual->name, name)==0)
        {
            return actual;
        }
        actual=actual->next;
    }
    return NULL;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometrymanager_unmap	(ei_widget_t widget)
{
    if (widget->geom_params)
    {
        widget->geom_params->manager->releasefunc(widget);
        free(widget->geom_params->rel_y);
        free(widget->geom_params->width);
        free(widget->geom_params->height);
        free(widget->geom_params->rel_height);
        free(widget->geom_params->rel_width);
        free(widget->geom_params->anchor);
        free(widget->geom_params);
        widget->geom_params=NULL;
    }


    widget->screen_location.size.height=0;
    widget->screen_location.size.width=0;
    widget->screen_location.top_left.x=0;
    widget->screen_location.top_left.y=0;
    *widget->content_rect=widget->screen_location;

}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget)
{
    if (widget->geom_params)
    {
        if (widget->geom_params->manager) {
            return widget->geom_params->manager;
        }
        else
            return NULL;
    }
    else
    {

        return NULL;
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager)
{
    widget->geom_params->manager = manager;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_geom_param_t		ei_widget_get_geom_params	(ei_widget_t widget)
{
    return widget->geom_params;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_set_geom_params	(ei_widget_t widget, ei_geom_param_t geom_param)
{
    widget->geom_params=geom_param;

}




