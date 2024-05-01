#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>
#define ASSIGN_IF_NULL(field, value,value2) if ((value) != NULL) { (field) = (value);} else {*(field)=(value2);}
#define ASSIGN_IF_NULL2(field, value,value2) if ((value) != NULL) { (field) = (value);} else {(field)=(value2);}
#define ASSIGN_IF_NULL3(field, value,value2,value3) if ((value) != NULL) { (field) = (value2);} else {(field)=(value3);}

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_runfunc(ei_widget_t widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_releasefunc(ei_widget_t widget)
{

}

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
    // int*			default_width=malloc(sizeof(int));
    int*			requested_width =malloc(sizeof(int));
    // int*			default_height=malloc(sizeof(int));
    int*			requested_height=malloc(sizeof(int));
    int*          cas1=malloc(sizeof(int));
    int*          cas2=malloc(sizeof(int));
    int*          cas3=malloc(sizeof(int));

    char name[] = "placer";
    strcpy(param->manager->name,name);
    param->manager->runfunc=ei_placer_runfunc;
    param->manager->releasefunc=ei_placer_releasefunc;
    param->manager->next=NULL;
    ASSIGN_IF_NULL2(param->anchor,anchor,ei_anc_northeast);
    if (anchor)
    {param->anchor=anchor;}
    else
    {
       param->anchor = malloc(sizeof(ei_anchor_t));
       *param->anchor=ei_anc_northeast;
    }
    if (y)
    {param->y=y;}
    else
    {
        param->y = malloc(sizeof(int));
        *param->y = 0;
    }
    if (x)
    {param->x=x;}
    else
    {
        param->x = malloc(sizeof(int));
        *param->x = 0;
    }
    if (rel_y)
    {param->rel_y=rel_y;}
    else
    {
        param->rel_y = malloc(sizeof(float));
        *param->rel_y = 0;
    }
    if (rel_x)
    {param->rel_x=rel_x;}
    else
    {
        param->rel_x = malloc(sizeof(float));
        *param->rel_x = 0;
    }
    if (rel_width)
    {param->rel_width=rel_width;}
    else
    {
        param->rel_width = malloc(sizeof(float));
        *param->rel_width = 0;
    }


    // if (requested_width)
    // {  cas1=requested_width;}
    // else
    // {
    //     *cas1 = 0;
    // }
    // if (default_width)
    // {
    //     cas2=default_width;
    // }
    // else
    // {
    //     *cas2 = 0;
    // }
    //
    // if (rel_width)
    // { cas3=cas1;}
    // else
    // {
    //     cas3 = cas1 ;
    // }
    // if (width)
    // {  param->width=width;}
    // else
    // {
    //     param->width = malloc(sizeof(int));
    //     param->width=cas3;
    // }
    //
    // if (requested_height)
    // { cas1=requested_height;}
    // else
    // {
    //     cas1 = malloc(sizeof(int));
    //     *cas1 = 0;
    // }
    // if (default_height)
    // { cas2=default_height;}
    // else
    // {
    //     cas2 = malloc(sizeof(int));
    //     *cas2 = 0;
    // }

    // if (rel_height)
    // { cas3=cas1;}
    // else
    // {
    //     cas3 = cas1 ;
    // }
    // if (height)
    // {  param->height=height;}
    // else
    // {
    //     param->height = malloc(sizeof(int));
    //     param->height=cas3;
    // }

    *requested_width  = widget->requested_size.width;
    *requested_height = widget->requested_size.height;
    int* default_width  = widget->requested_size.width;
    int* default_height = widget->requested_size.height;
    param->width=requested_width;
    param->height=requested_height;

    if (rel_height)
    {param->rel_height=rel_height;}
    else
    {
        param->rel_height = malloc(sizeof(float));
        *param->rel_height = 0;
    }
    widget->geom_params=param;
}

/*-------------------------------------------------------------------------------------------------------*/
