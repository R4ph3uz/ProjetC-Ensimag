#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>
#include "pick_event.h"
#include "ei_application.h"
#include "list_event.h"
#include "callbacks/entry_callbacks.h"
/*-------------------------------------------------------------------------------------------------------*/

static uint32_t PICKID=0;

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                            ei_widget_t		parent,
                                            ei_user_param_t	user_data,
                                            ei_widget_destructor_t destructor)
{
    ei_widgetclass_t* wclass = ei_widgetclass_from_name(class_name);
    ei_widget_t widget = (wclass->allocfunc)();
    widget->wclass = wclass;
    (*(wclass->setdefaultsfunc))(widget);

    widget->pick_id=PICKID; // < Id of this widget in the picking offscreen
    widget->pick_color = SAFE_MALLOC(sizeof(ei_color_t));//< pick_id encoded as a color
    widget->pick_color->red=(uint8_t)( PICKID & 0xFF);
    widget->pick_color->green=(uint8_t)((PICKID >> 8) & 0xFF);
    widget->pick_color->blue=(uint8_t)((PICKID >> 16) & 0xFF);
    widget->pick_color->alpha=255;

    add_widget_pickid_array(widget);
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



    widget->content_rect=NULL;
    ///< See ei_widget_get_content_rect. By defaults, points to the screen_location.

    widget->callback= NULL;

    widget->isChildIgnoreAddInvalidateRect=false;

    return widget;

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_destroy		(ei_widget_t		widget)
{
    //détruit les enfants

    supprime_de_ses_freres(widget);
    while(widget->children_head){
        ei_widget_t prochain = widget->children_head->next_sibling;
        ei_widget_destroy(widget->children_head);
        widget->children_head=prochain;
    }
    ei_app_invalidate_rect(&widget->screen_location);
    ei_geometrymanager_unmap(widget);
    SAFE_FREE(widget->pick_color);
    widget->callback = NULL;
//    ei_unbind(ei_ev_mouse_buttondown, widget, NULL, NULL,NULL);
    if(widget->content_rect!=&widget->screen_location)
        SAFE_FREE(widget->content_rect);
    if (widget->destructor)
        widget->destructor(widget);

    //set entry focus a null si on destroy l'entry
    if((ei_widget_t)get_entry_focus() == widget) {
        set_entry_focus(NULL);
    }
    (*(widget->wclass->releasefunc))(widget); // ici libère la mémoire
}

/*-------------------------------------------------------------------------------------------------------*/

bool	 		ei_widget_is_displayed		(ei_widget_t		widget)
{
    if (widget->geom_params){
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widget_t		ei_widget_pick			(ei_point_t*		where)
{
    widget_array* PICK_DYNARRAY = get_pick_PICK_DYNARRAY();
    ei_surface_t pick_surface = get_pick_surface();
    hw_surface_lock(pick_surface);
    uint32_t* buffer = (uint32_t*)hw_surface_get_buffer(pick_surface);
    ei_size_t size = hw_surface_get_size(pick_surface);

    hw_surface_unlock(pick_surface);
    uint32_t index = (where->x)+(where->y)*size.width;
    uint8_t color[4];
    // Décalage de bits et masquage pour extraire chaque octet
    color[0] = (uint8_t)( buffer[index] & 0xFF);
    color[1] = (uint8_t)((buffer[index] >> 8) & 0xFF);
    color[2] = (uint8_t)((buffer[index] >> 16) & 0xFF);
    color[3] = (uint8_t)((buffer[index] >> 24) & 0xFF);

    int ir,ig,ib,ia;
    hw_surface_get_channel_indices( pick_surface, &ir, &ig, &ib, &ia);

    uint32_t value = ((uint32_t)color[ir]) |
                     ((uint32_t)color[ig] << 8) |
                     ((uint32_t)color[ib] << 16) ;

    if (value <= PICK_DYNARRAY->number_element) {
        return PICK_DYNARRAY->widgets[value];
    }
    fprintf(stderr, "erreur index out of bounds");
    return NULL;
}