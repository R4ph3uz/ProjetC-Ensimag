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

    ei_impl_geom_param_t* param=malloc(sizeof(ei_geom_param_t));

    ei_geometrymanager_t* manager = malloc(sizeof(ei_geometrymanager_t));
    manager->runfunc=ei_placer_runfunc;
    manager->releasefunc=ei_placer_releasefunc;
    manager->next=NULL;
    char name[] = "placer";
    strcpy(manager->name,name);
    memcpy(param->manager,manager, sizeof(ei_geometrymanager_t));
    free(manager);




    param->anchor = malloc(sizeof(ei_anchor_t));
    if (anchor) {
        *param->anchor=*anchor;
    }
    else
    {
       *param->anchor= ei_anc_northeast;
    }


    param->x=malloc(sizeof(int));
    if (x) {
        *param->x=*x;
    }
    else
    {
        *param->x = 0;
    }

    param->y=malloc(sizeof(int));
    if (y) {
        *(param->y)=*y;
    }
    else
    {
        *(param->y) = 0;
    }

    param->rel_y=malloc(sizeof(float));
    if (rel_y) {
        *param->rel_y=*rel_y;
    }
    else
    {
        *param->rel_y = 0;
    }

    param->rel_x=malloc(sizeof(float));
    if (rel_x) {
        param->rel_x=rel_x;
    }
    else
    {
        *param->rel_x = 0;
    }

    param->rel_width=malloc(sizeof(float));
    if (rel_width) {
        param->rel_width=rel_width;
    }
    else
    {
        *param->rel_width = 0;
    }

    param->rel_height=malloc(sizeof(float));
    if (rel_height) {
        param->rel_height=rel_height;
    }
    else
    {
        *param->rel_height = 0;
    }


    // passer par des pointeurs ?
    int			requested_width = widget->requested_size.width;
    int			requested_height= widget->requested_size.height;
    int         default_width = requested_width;
    int         default_height = requested_height;

    int* largeur = malloc(sizeof(int));
    if(width) {
        largeur=width;
    }
    else {
        if(requested_width) {
            *largeur=requested_width;
        }
        else {
            if(rel_width == NULL) {
                *largeur = default_width;
            }
            else {
                *largeur=0;
            }
        }
    }
    param->width=largeur;

    int* hauteur = malloc(sizeof(int));
    if(height) {
        hauteur=height;
    }
    else {
        if(requested_height) {
            *hauteur=requested_height;
        }
        else {
            if(rel_height == NULL) {
                *hauteur = default_height;
            }
            else {
                *hauteur=0;
            }
        }
    }
    param->height=hauteur;

    widget->geom_params=param;
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
