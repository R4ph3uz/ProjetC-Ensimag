#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>
#include "pick_event.h"
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
    (*(wclass->setdefaultsfunc))(widget);

    widget->wclass = wclass;
    widget->pick_id=PICKID; // < Id of this widget in the picking offscreen
    widget->pick_color = malloc(sizeof(ei_color_t));//< pick_id encoded as a color
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

    widget->requested_size.height=10;///< See \ref ei_widget_get_requested_size.
    widget->requested_size.width=10;///< See \ref ei_widget_get_requested_size.

    widget->content_rect=NULL;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.

    widget->callback= NULL;

    return widget;

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widget_destroy		(ei_widget_t		widget)
{
    //détruit les enfants

    ei_widget_t prec=widget->parent;
    ei_widget_t suiv=widget->next_sibling;
    if (prec->children_head!=widget)
    {
        prec=prec->children_head;
        while (prec->next_sibling!=widget)
        {
            prec=prec->next_sibling;
        }
        prec->next_sibling=suiv;
        if(suiv==NULL)
        {
            prec->parent->children_tail=prec;
        }
    }
    else
    {
        prec->children_head=suiv;
        if(suiv==NULL)
        {
            prec->children_tail=NULL;
        }
    }
    while(widget->children_head){
        ei_widget_t prochain = widget->children_head->next_sibling;
        ei_widget_destroy(widget->children_head);
        widget->children_head=prochain;
    }


    ei_geometrymanager_unmap(widget);
//    (*(widget->wclass->releasefunc))(widget); // ici libère la mémoire
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