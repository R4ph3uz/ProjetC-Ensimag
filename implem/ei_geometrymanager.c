#include "ei_geometrymanager.h"

/*-------------------------------------------------------------------------------------------------------*/

struct ei_impl_geom_param_t
{
    ei_anchor_t*		anchor;
    int*			x;
    int*			y;
    int*			width;
    int*			height;
    float*			rel_x;
    float*			rel_y;
    float*			rel_width;
    float*			rel_height;
};


/*-------------------------------------------------------------------------------------------------------*/

size_t		ei_geom_param_size()
{
    size_t i=9;
    return i;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location)
{

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager)
{

}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name)
{

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometrymanager_unmap	(ei_widget_t widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager)
{

}

/*-------------------------------------------------------------------------------------------------------*/

ei_geom_param_t		ei_widget_get_geom_params	(ei_widget_t widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_set_geom_params	(ei_widget_t widget, ei_geom_param_t geom_param)
{

}




