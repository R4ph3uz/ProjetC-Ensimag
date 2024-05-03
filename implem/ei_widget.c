#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>
/*-------------------------------------------------------------------------------------------------------*/

static uint32_t PICKID=1;
ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                            ei_widget_t		parent,
                                            ei_user_param_t	user_data,
                                            ei_widget_destructor_t destructor)
{
    ei_widgetclass_t* wclass = ei_widgetclass_from_name(class_name);
    ei_widget_t widget = (wclass->allocfunc)();
    (*(wclass->setdefaultsfunc))(widget);

    widget->wclass = wclass;
    widget->pick_id=PICKID; // < Id of this widget in the picking offscreen
    widget->pick_color = malloc(sizeof(ei_color_t));//< pick_id encoded as a color
    widget->pick_color->red=200;
    widget->pick_color->green=150;
    widget->pick_color->blue=48;
    widget->pick_color->alpha=255;
    PICKID++;
    widget->user_data = user_data;
    widget->destructor = destructor;
//  /* Widget Hierarchy Management */
    widget->parent= parent;
    if(parent!=NULL){
        if(parent->children_head==NULL)
        {
            parent->children_head=widget;
            parent->children_tail=widget;
        }
        else
        {
            parent->children_tail->next_sibling=widget;
            parent->children_tail= widget;
        }
    }
    widget->children_head=NULL;
    widget->children_tail=NULL;
    widget->next_sibling= NULL;///< Pointer to the next child of this widget's parent widget.

//  /* Geometry Management */
    widget->geom_params = NULL;///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.

    widget->requested_size.height=100;///< See \ref ei_widget_get_requested_size.
    widget->requested_size.width=100;///< See \ref ei_widget_get_requested_size.
    widget->screen_location.top_left.x=0;///< See \ref ei_widget_get_screen_location.
    widget->screen_location.top_left.y=0;///< See \ref ei_widget_get_screen_location.
    widget->screen_location.size.width=100;///< See \ref ei_widget_get_screen_location.
    widget->screen_location.size.height=100;///< See \ref ei_widget_get_screen_location.

    widget->content_rect=NULL;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.

    /* Specific to each class */

    return widget;

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_destroy		(ei_widget_t		widget)
{
    //détruit les enfants
    while(widget->children_head){
        ei_widget_t prochain = widget->children_head->next_sibling;
        ei_widget_destroy(widget->children_head);
        widget->children_head=prochain;
    }
    ei_geometrymanager_unmap(widget);
    (*(widget->wclass->releasefunc))(widget); // ici libère la mémoire
}

/*-------------------------------------------------------------------------------------------------------*/

bool	 		ei_widget_is_displayed		(ei_widget_t		widget)
{
    // verifie si geom param est nul
    // condition suffisante mais pas nécéssaire car il existe d'autres cas où le widget est pas display mais a des
    // geomparam
    if (!widget->geom_params){
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t		ei_widget_pick			(ei_point_t*		where)
{

    return NULL; // if root window
}