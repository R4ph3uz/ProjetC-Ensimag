#include "ei_geometrymanager.h"
#include "ei_implementation.h"
/*-------------------------------------------------------------------------------------------------------*/

static int PICKID=0;
ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                            ei_widget_t		parent,
                                            ei_user_param_t	user_data,
                                            ei_widget_destructor_t destructor)
{
    ei_impl_widget_t* widget=malloc(sizeof(ei_impl_widget_t));
    widget->wclass = ei_widgetclass_from_name(class_name);
    widget->pick_id=PICKID; // < Id of this widget in the picking offscreen.
    ei_color_t* couleur= malloc(sizeof(ei_color_t));
    couleur->red=(PICKID*100)%255;
    couleur->green=(PICKID*100)%255;
    couleur->blue=(PICKID*100)%255;
    couleur->alpha=(PICKID*100)%255;
    PICKID++;
    widget->pick_color = couleur;//< pick_id encoded as a color
    widget->user_data = user_data;
    widget->destructor = destructor;

//  /* Widget Hierarchy Management */
    widget->parent= parent;
    if(parent!=NULL){
    parent->children_tail->next_sibling=widget;
    parent->children_tail= widget;
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
    return widget;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_destroy		(ei_widget_t		widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

bool	 		ei_widget_is_displayed		(ei_widget_t		widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t		ei_widget_pick			(ei_point_t*		where)
{

}