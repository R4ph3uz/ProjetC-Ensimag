#include "ei_geometrymanager.h"

#include <ei_application.h>

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


    if (widget->content_rect==NULL || (widget->screen_location.size.height != new_screen_location->size.height) || (
            widget->screen_location.size.width != new_screen_location->size.width) || (
            widget->screen_location.top_left.x != new_screen_location->top_left.x) || (
            widget->screen_location.top_left.y != new_screen_location->top_left.y )) // Si new_screen est différent de l'ancien screen location
    {
        // widget->screen_location=*new_screen_location;
        if(!widget->isChildIgnoreAddInvalidateRect){
            ei_rect_t* rectangle_intersecte = intersection_rectangle(widget->screen_location, widget->parent->screen_location);
            ei_app_invalidate_rect(rectangle_intersecte);
            SAFE_FREE(rectangle_intersecte);
            ei_rect_t* rectangle_intersecte2 =intersection_rectangle(*new_screen_location, widget->parent->screen_location);
            ei_app_invalidate_rect(rectangle_intersecte2);
            SAFE_FREE(rectangle_intersecte2);
        }
        else{
            widget->isChildIgnoreAddInvalidateRect = false;
        }

        memcpy(&widget->screen_location,new_screen_location,sizeof(widget->screen_location));
        widget->wclass->geomnotifyfunc(widget); //Notifie du changement du screen_location
        if (widget->children_tail) //Si widget a un enfant
        {
            ei_widget_t enfant=widget->children_head;
            while(enfant!=NULL) //Pour chaque enfant
            {
                if (enfant->geom_params){
                    enfant->isChildIgnoreAddInvalidateRect = true;
                    enfant->geom_params->manager->runfunc(enfant);
                }

                enfant=enfant->next_sibling;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager)
{
    if(LISTE_GEOMETRYMANAGER==NULL) //Si il y a aucun geometrymanager dans la liste
    {
        LISTE_GEOMETRYMANAGER=geometrymanager; //On l'y ajoute
    }
    else //Sinon
    {
        geometrymanager->next=LISTE_GEOMETRYMANAGER;
        LISTE_GEOMETRYMANAGER=geometrymanager; //On ajoute le geometry manager en tête de la liste
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_free_geometrymanager	()
{
    while(LISTE_GEOMETRYMANAGER!=NULL) {
        ei_geometrymanager_t* temp= LISTE_GEOMETRYMANAGER;
        LISTE_GEOMETRYMANAGER=LISTE_GEOMETRYMANAGER->next;
        free(temp);
    }
}

/*-------------------------------------------------------------------------------------------------------*/


ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name)
{
    ei_geometrymanager_t* actual = LISTE_GEOMETRYMANAGER;
    while(actual != NULL) //pour chaque element de la liste des geometrymanager
    {
        if (strcmp(actual->name, name)==0) //Si le nom du geometry manager de la liste est celui cherché
        {
            return actual; //On renvoie le geometry manager associé
        }
        actual=actual->next;
    }
    return NULL;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_geometrymanager_unmap	(ei_widget_t widget)
{
    if (widget->geom_params) //si il a un manager
    { //free et supprimme ce qui est associé a geom params
        widget->geom_params->manager->releasefunc(widget);
        SAFE_FREE(widget->geom_params->rel_y);
        SAFE_FREE(widget->geom_params->rel_x);
        SAFE_FREE(widget->geom_params->width);
        SAFE_FREE(widget->geom_params->height);
        SAFE_FREE(widget->geom_params->rel_height);
        SAFE_FREE(widget->geom_params->rel_width);
        SAFE_FREE(widget->geom_params->anchor);
        SAFE_FREE(widget->geom_params->is_reconfigurable);
        SAFE_FREE(widget->geom_params->x);
        SAFE_FREE(widget->geom_params->y);
        SAFE_FREE(widget->geom_params);
    }


    widget->screen_location.size.height=0;
    widget->screen_location.size.width=0;
    widget->screen_location.top_left.x=0;
    widget->screen_location.top_left.y=0;      //remet  a 0 la screen location
    if(widget->content_rect!=&widget->screen_location)
        SAFE_FREE(widget->content_rect);
    widget->content_rect=&widget->screen_location;

}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget)
{
    if (widget->geom_params)
    {
        if (widget->geom_params->manager) {//si widget a un geometry manager
            return widget->geom_params->manager; //renvoie son manager
        }
        else
            return NULL; //sinon renvoie NULL
    }
    else
    {

        return NULL;//sinon renvoie NULL
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




