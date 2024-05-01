#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>
#define ASSIGN_IF_NULL(field, value,value2) if ((value) != NULL) { (field) = (value);} else {*(field)=(value2);}
#define ASSIGN_IF_NULL2(field, value,value2) if ((value) != NULL) { (field) = (value);} else {(field)=(value2);}
#define ASSIGN_IF_NULL3(field, value,value2,value3) if ((value) != NULL) { (field) = (value2);} else {(field)=(value3);}

/*-------------------------------------------------------------------------------------------------------*/

void		ei_place	(ei_widget_t		widget,
                         ei_anchor_t*		anchor,
                         int*			x,
                         int*			y,
                         int*			width,
                         int*			height,
                         float*			rel_x,
                         float*			rel_y,
                         float*			rel_width,
                         float*			rel_height)
{

    ei_geom_param_t param=malloc(sizeof(ei_geom_param_t));
    int*			default_width=malloc(sizeof(int));
    int*			requested_width =malloc(sizeof(int));
    int*			default_height=malloc(sizeof(int));
    int*			requested_height=malloc(sizeof(int));
    int*          cas1=malloc(sizeof(int));
    int*          cas2=malloc(sizeof(int));
    int*          cas3=malloc(sizeof(int));

    *requested_width  = widget->requested_size.width;
    *requested_height = widget->requested_size.height;
    *default_width  = widget->requested_size.width;
    *default_height = widget->requested_size.height;
    ASSIGN_IF_NULL(param->anchor,anchor,ei_anc_northeast);
    ASSIGN_IF_NULL(param->x,x,0);
    ASSIGN_IF_NULL(param->y,y,0);
    ASSIGN_IF_NULL(param->rel_x,rel_x,0);
    ASSIGN_IF_NULL(param->rel_y,rel_y,0);
    ASSIGN_IF_NULL(param->rel_width,rel_width,0);
    ASSIGN_IF_NULL(param->rel_height,rel_height,0);
    ASSIGN_IF_NULL(cas1,requested_width,0);
    ASSIGN_IF_NULL(cas2,default_width,0);
    ASSIGN_IF_NULL3(cas3,rel_width,cas1,cas2);
    ASSIGN_IF_NULL2(width,width,cas3);
    ASSIGN_IF_NULL(cas1,requested_height,0);
    ASSIGN_IF_NULL(cas2,default_height,0);
    ASSIGN_IF_NULL3(cas3,rel_height,cas1,cas2);
    ASSIGN_IF_NULL2(height,height,cas3);

    widget->geom_params=param;
}

/*-------------------------------------------------------------------------------------------------------*/

