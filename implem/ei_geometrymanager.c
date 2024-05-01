#include "ei_geometrymanager.h"
#include "ei_implementation.h"

/*-------------------------------------------------------------------------------------------------------*/

static ei_geometrymanager_t* LISTE_GEOMETRYMANAGER= NULL;


/*-------------------------------------------------------------------------------------------------------*/

size_t		ei_geom_param_size()
{
    size_t i=10;
    return i;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location)
{

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

}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget)
{
    ei_geometrymanager_t* geommanager = (ei_geometrymanager_t*) widget;
    return geommanager;
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




