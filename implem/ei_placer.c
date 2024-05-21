#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>

// placer_release func déplacé dans implémentation
// placer run func déplacé dans implémentation
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
    if (widget->geom_params && (strcmp(widget->geom_params->manager->name,"placer")!=0))
    {
        //Si il y a un geometry manager différent de placer, supprime ce geometry manager via unmap
        ei_geometrymanager_unmap(widget);
    }

    if (widget->geom_params==NULL) {        //Si le widget n'as pas de geometry manager, on lui affecte placer
        widget->geom_params = SAFE_MALLOC(sizeof(ei_impl_geom_param_t));
        widget->geom_params->manager = ei_geometrymanager_from_name("placer");
        widget->geom_params->is_reconfigurable = SAFE_MALLOC(sizeof(bool));
        *widget->geom_params->is_reconfigurable=true;
        widget->geom_params->anchor = SAFE_MALLOC(sizeof(ei_anchor_t));

        if (anchor) {       //initialise l'ancre
            *widget->geom_params->anchor = *anchor;
        } else {
            *widget->geom_params->anchor = ei_anc_northwest;
        }


        widget->geom_params->x = SAFE_MALLOC(sizeof(int));
        if (x) { //initialise x
            *widget->geom_params->x = *x;
        } else {
            *widget->geom_params->x = 0;
        }

        widget->geom_params->y = SAFE_MALLOC(sizeof(int));
        if (y) {    //initialise y
            *(widget->geom_params->y) = *y;
        } else {
            *(widget->geom_params->y) = 0;
        }

        widget->geom_params->rel_y = SAFE_MALLOC(sizeof(float));
        if (rel_y) {    //initialise rel_y
            *widget->geom_params->rel_y = *rel_y;
        } else {
            *widget->geom_params->rel_y = 0;
        }

        widget->geom_params->rel_x = SAFE_MALLOC(sizeof(float));
        if (rel_x) {    //initialise rel_x
            *widget->geom_params->rel_x = *rel_x;
        } else {
            *widget->geom_params->rel_x = 0;
        }

        widget->geom_params->rel_width = SAFE_MALLOC(sizeof(float));
        if (rel_width) {    //initialise rel_width
            *widget->geom_params->rel_width = *rel_width;
        } else {
            *widget->geom_params->rel_width = 0;
        }

        widget->geom_params->rel_height = SAFE_MALLOC(sizeof(float));
        if (rel_height) {    //initialise rel_height
            *widget->geom_params->rel_height = *rel_height;
        } else {
            *widget->geom_params->rel_height = 0;
        }



        int requested_width = widget->requested_size.width;
        int requested_height = widget->requested_size.height;
        int default_width = widget->parent->content_rect->size.width;
        int default_height = widget->parent->content_rect->size.height;

        widget->geom_params->width = SAFE_MALLOC(sizeof(int));
        if (width) {    //initialise width
            *widget->geom_params->width = *width;
            *widget->geom_params->is_reconfigurable=false; //Détecte si la width est définit avec la requested/default (true par défaut)
        } else {
            if (rel_width) {
                *widget->geom_params->width = 0;
                *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
            } else {
                if (requested_width) {
                    *widget->geom_params->width = requested_width;
                } else {
                    *widget->geom_params->width = default_width;
                }
            }
        }

        widget->geom_params->height = SAFE_MALLOC(sizeof(int));
        if (height) {    //initialise height
            *widget->geom_params->height = *height;
            *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
        } else {
            if (rel_height) {
                *widget->geom_params->height = 0;
                *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
            } else {
                if (requested_height) {
                    *widget->geom_params->height = requested_height;
                } else {
                    *widget->geom_params->height = default_height;
                }
            }
        }
        widget->geom_params->manager->runfunc(widget);
    }
    else
    {       //Si le widget était déja manager par placer
        if (anchor) { //Change l'ancre si non nul en entrée
            *widget->geom_params->anchor = *anchor;
        }
        if (x) {        //Change x si non nul en entrée
            *widget->geom_params->x = *x;
        }
        if (y) {        //Change y si non nul en entrée
            *(widget->geom_params->y) = *y;
        }
        if (rel_y) {        //Change rel_y si non nul en entrée
            *widget->geom_params->rel_y = *rel_y;
        }
        if (rel_x) {        //Change rel_x si non nul en entrée
            *widget->geom_params->rel_x = *rel_x;
        }
        if (rel_width) {        //Change rel_width si non nul en entrée
            *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
            *widget->geom_params->rel_width = *rel_width;
        }
        if (rel_height) {        //Change rel_height si non nul en entrée
            *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
            *widget->geom_params->rel_height = *rel_height;
        }
        if (width) {        //Change width si non nul en entrée
            *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
            *widget->geom_params->width = *width;
        }
        if (height) {        //Change height si non nul en entrée
            *widget->geom_params->is_reconfigurable=false;//Détecte si la width est définit avec la requested/default
            *widget->geom_params->height = *height;
        }
    }
}


/*-------------------------------------------------------------------------------------------------------*/
