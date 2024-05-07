#include "entry_callbacks.h"
#include <string.h>
#include <ei_entry.h>

#include "../widgetclass/ei_entry.h"
#include "ei_event.h"

bool entry_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) widget;
    if(event->param.mouse.where.x> widget->screen_location.top_left.x
        && event->param.mouse.where.x< widget->screen_location.top_left.x+widget->screen_location.size.width
        && event->param.mouse.where.y> widget->screen_location.top_left.y
        && event->param.mouse.where.y< widget->screen_location.top_left.y+ widget->screen_location.size.height)
    {
        // je suis dans l'entry
        entry->focus=true;
        // que mettre dans user param ? ou entry ?
        ei_bind(ei_ev_keydown,widget,NULL,entry_write,user_param); // keystroke
        ei_bind(ei_ev_text_input,widget,NULL,entry_write,user_param); // texte collé ?
        ei_bind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler,user_param); // si on clique e dehors

    }
    else {
        entry->focus=false;
        ei_unbind(ei_ev_mouse_buttondown,widget,NULL,entry_down_click_handler,user_param);
        ei_unbind(ei_ev_keydown,widget,NULL,entry_write,user_param);
        return true;
    }
    return false;
}
bool entry_write(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) widget;
    char t[2] = {event->param.text,'\0'};
    char* text = (char*) ei_entry_get_text(widget);
    strcat(text,t);
    ei_entry_set_text(widget,text);
    ei_unbind(ei_ev_keydown,widget,NULL,entry_write,user_param);
    ei_unbind(ei_ev_text_input,widget,NULL,entry_write,user_param);
    return true;
}