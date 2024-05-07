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

    widget->requested_size.height=100;///< See \ref ei_widget_get_requested_size.
    widget->requested_size.width=100;///< See \ref ei_widget_get_requested_size.
    widget->screen_location.top_left.x=0;///< See \ref ei_widget_get_screen_location.
    widget->screen_location.top_left.y=0;///< See \ref ei_widget_get_screen_location.
    widget->screen_location.size.width=100;///< See \ref ei_widget_get_screen_location.
    widget->screen_location.size.height=100;///< See \ref ei_widget_get_screen_location.

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
    if(widget->parent){
        ei_widget_t parent = widget->parent;
        ei_widget_t head = parent->children_head;
        if(widget == head && widget==parent->children_tail){
            parent->children_head = NULL;
            parent->children_tail = NULL;
            fprintf(stderr, "if\n");
        }
        else if(widget == head ) {
            parent->children_head = head->next_sibling;
            fprintf(stderr, "else if\n");
        }
        else{
            fprintf(stderr, "else\n");
            while(head->next_sibling){
                if (head->next_sibling == widget){
                    fprintf(stderr, "found widget");
                    head->next_sibling = head->next_sibling->next_sibling;
                    break;
                }
                head = head->next_sibling;
            }
        }

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

    return NULL; // if root window
}