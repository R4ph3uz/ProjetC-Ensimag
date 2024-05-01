#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>

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
                         float*			rel_height) {
    widget->geom_params=malloc(sizeof(ei_geom_param_t));

    widget->geom_params->manager = malloc(sizeof(ei_geometrymanager_t));
    widget->geom_params->manager->runfunc=ei_placer_runfunc;
    widget->geom_params->manager->releasefunc=ei_placer_releasefunc;
    widget->geom_params->manager->next=NULL;
    const char naame[] = "placer";
    strcpy(widget->geom_params->manager->name,naame);

    widget->geom_params->anchor = malloc(sizeof(ei_anchor_t));
    if (anchor) {
        *widget->geom_params->anchor=*anchor;
    }
    else
    {
       *widget->geom_params->anchor= ei_anc_northeast;
    }


    widget->geom_params->x=malloc(sizeof(int));
    if (x) {
        *widget->geom_params->x=*x;
    }
    else
    {
        *widget->geom_params->x = 0;
    }

    widget->geom_params->y=malloc(sizeof(int));
    if (y) {
        *(widget->geom_params->y)=*y;
    }
    else
    {
        *(widget->geom_params->y) = 0;
    }

    widget->geom_params->rel_y=malloc(sizeof(float));
    if (rel_y) {
        *widget->geom_params->rel_y=*rel_y;
    }
    else
    {
        *widget->geom_params->rel_y = 0;
    }

    widget->geom_params->rel_x=malloc(sizeof(float));
    if (rel_x) {
        *widget->geom_params->rel_x=*rel_x;
    }
    else
    {
        *widget->geom_params->rel_x = 0;
    }

    widget->geom_params->rel_width=malloc(sizeof(float));
    if (rel_width) {
        *widget->geom_params->rel_width=*rel_width;
    }
    else
    {
        *widget->geom_params->rel_width = 0;
    }

    widget->geom_params->rel_height=malloc(sizeof(float));
    if (rel_height) {
        *widget->geom_params->rel_height=*rel_height;
    }
    else
    {
        *widget->geom_params->rel_height = 0;
    }


    // passer par des pointeurs ?
    int			requested_width = widget->requested_size.width;
    int			requested_height= widget->requested_size.height;
    int         default_width = requested_width;
    int         default_height = requested_height;

    widget->geom_params->width = malloc(sizeof(int));
    if(width) {
        *widget->geom_params->width=*width;
    }
    else {
        if(requested_width) {
            *widget->geom_params->width=requested_width;
        }
        else {
            if(rel_width == NULL) {
                *widget->geom_params->width = default_width;
            }
            else {
                *widget->geom_params->width=0;
            }
        }
    }

    widget->geom_params->height = malloc(sizeof(int));
    if(height) {
        *widget->geom_params->height=*height;
    }
    else {
        if(requested_height) {
            *widget->geom_params->height=requested_height;
        }
        else {
            if(rel_height == NULL) {
                *widget->geom_params->height = default_height;
            }
            else {
                *widget->geom_params->height=0;
            }
        }
    }

}


// POUR PLUS TARD
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



/*-------------------------------------------------------------------------------------------------------*/
