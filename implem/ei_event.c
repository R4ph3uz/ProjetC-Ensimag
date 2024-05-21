#include "ei_event.h"

#include "ei_implementation.h"
#include "list_event.h"
#include "pick_event.h"


/*-------------------------------------------------------------------------------------------------------*/

void		ei_bind			(ei_eventtype_t		eventtype,
                                ei_widget_t		widget,
                                ei_tag_t		tag,
                                ei_callback_t		callback,
                                void*			user_param)
{
    //Choix: on bind les généraux (avec un tag) dans une liste, et les spécifiques avec leur widget correspondant dans leur structure
    if (tag) {
        add_list_callback(callback, tag, eventtype, user_param);
    }
    else {
        //assiger au widget le callback et l'eventtype
        list_widget_callback* list_cb= SAFE_MALLOC(sizeof(list_widget_callback));
        list_cb->callback = callback;
        list_cb->eventtype = eventtype;
        list_cb->user_param = user_param;
        list_cb->next=NULL;
        if(widget->callback == NULL){
            widget->callback = list_cb;
        }
        else{
           list_cb->next=widget->callback;
           widget->callback=list_cb;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param)
{
    if(tag) {
        remove_list_callback(callback, tag, eventtype, user_param);
    }
    else
        while(widget->callback !=NULL){
            list_widget_callback* temp = widget->callback;
            widget->callback = widget->callback->next;
            SAFE_FREE(temp);
        }
}

