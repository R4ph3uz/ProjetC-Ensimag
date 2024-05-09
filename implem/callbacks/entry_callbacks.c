#include "entry_callbacks.h"
#include <string.h>
#include <ei_entry.h>
#include "../widgetclass/ei_entry.h"
#include "ei_event.h"

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) widget;
    fprintf(stderr,"test");
    if(event->param.mouse.where.x> widget->screen_location.top_left.x
        && event->param.mouse.where.x< widget->screen_location.top_left.x+widget->screen_location.size.width
        && event->param.mouse.where.y> widget->screen_location.top_left.y
        && event->param.mouse.where.y< widget->screen_location.top_left.y+ widget->screen_location.size.height
        && entry->focus == false)
    {
        // je suis dans l'entry
        fprintf(stderr, "yeah");
        entry->focus=true;
        // que mettre dans user param ? ou entry ?
        ei_bind(ei_ev_keydown,NULL,"all",entry_write,entry); // keystroke
        ei_bind(ei_ev_text_input,NULL,"all",entry_write,entry); // texte collé ?
        ei_bind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,entry); // si on clique e dehors
        return true;

    }

    return false;

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler_all(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) user_param;
    fprintf(stderr,"test");
    if(event->param.mouse.where.x> entry->widget.screen_location.top_left.x
        && event->param.mouse.where.x< entry->widget.screen_location.top_left.x+entry->widget.screen_location.size.width
        && event->param.mouse.where.y> entry->widget.screen_location.top_left.y
        && event->param.mouse.where.y< entry->widget.screen_location.top_left.y+ entry->widget.screen_location.size.height)
    {
        // je suis dans l'entry
        fprintf(stderr, "yeah");
        return false;

    }

    ei_unbind(ei_ev_keydown,NULL,"all",entry_write,entry); // keystroke
    ei_unbind(ei_ev_text_input,NULL,"all",entry_write,entry); // texte collé ?
    ei_unbind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,entry); // si on clique e dehors
    entry->focus=false;
    return true;

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_write(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) user_param;
    if (event->type == ei_ev_text_input) {
        char t[2] = {event->param.text,'\0'};
        char* text = (char*) ei_entry_get_text(widget);
        strcat(text,t);
        ei_entry_set_text((ei_widget_t)entry,text);
    }
    else if(event->type == ei_ev_keydown) {
        fprintf(stderr, "aie\n");
        if(event->param.key_code==SDLK_DELETE || event->param.key_code==SDLK_BACKSPACE) {
            fprintf(stderr, "yeah i want do delete\n");
            char* text = (char*) ei_entry_get_text(widget);
            text[strlen(text)-1] = '\0';
            ei_entry_set_text((ei_widget_t)entry,text);
        }
    }

    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/
