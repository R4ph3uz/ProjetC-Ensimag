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
        widget->callback = callback;
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param)
{

}

