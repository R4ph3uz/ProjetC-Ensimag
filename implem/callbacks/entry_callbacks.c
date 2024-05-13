#include "entry_callbacks.h"
#include <string.h>
#include <ei_entry.h>
#include "../widgetclass/ei_entry.h"
#include "ei_event.h"
#include "../draw_utils/draw_utils.h"

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) widget;
    if(event->param.mouse.where.x> widget->screen_location.top_left.x
        && event->param.mouse.where.x< widget->screen_location.top_left.x+widget->screen_location.size.width
        && event->param.mouse.where.y> widget->screen_location.top_left.y
        && event->param.mouse.where.y< widget->screen_location.top_left.y+ widget->screen_location.size.height
        && entry->focus == false)
    {
        // je suis dans l'entry
        entry->focus=true;
        ei_bind(ei_ev_keydown,NULL,"all",entry_write,entry); // keystroke
        ei_bind(ei_ev_text_input,NULL,"all",entry_write,entry); // texte collé ?
        ei_bind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,entry); // si on clique e dehors
        hw_event_schedule_app(500,NULL);
        ei_bind(ei_ev_app,NULL, "all", animation_cursor,entry);
        return true;

    }

    return false;

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler_all(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) user_param;
    if(event->param.mouse.where.x> entry->widget.screen_location.top_left.x
        && event->param.mouse.where.x< entry->widget.screen_location.top_left.x+entry->widget.screen_location.size.width
        && event->param.mouse.where.y> entry->widget.screen_location.top_left.y
        && event->param.mouse.where.y< entry->widget.screen_location.top_left.y+ entry->widget.screen_location.size.height)
    {
        // je suis dans l'entry
        return false;

    }

    ei_unbind(ei_ev_keydown,NULL,"all",entry_write,entry); // keystroke
    ei_unbind(ei_ev_text_input,NULL,"all",entry_write,entry); // texte collé ?
    ei_unbind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,entry); // si on clique e dehors
    ei_unbind(ei_ev_app,NULL, "all", animation_cursor,entry);
    entry->focus=false;
    return true;

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_write(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) user_param;
    if (event->type == ei_ev_text_input) {
        char symbole[2] = {event->param.text,'\0'};
        char* text = (char*) ei_entry_get_text(widget);
        if(text && strlen(text)+1<=*entry->requested_char_size)
            {
            //segfault avant le resquested size
                text = insert_char(text, event->param.text, strlen(text)-entry->position ); //fuite de mémoire here
                ei_entry_set_text((ei_widget_t)entry,text);
            }
        else{
             //on laisse le texte comme ceci
        }
    // else
    //     ei_entry_set_text((ei_widget_t)entry, symbole);

    }
    else if(event->type == ei_ev_keydown) {
        if(event->param.key_code==SDLK_DELETE || event->param.key_code==SDLK_BACKSPACE) {
            fprintf(stderr, "yeah i want do delete\n");
            char* text = (char*) ei_entry_get_text(widget);
            char* new = delete_char(text, strlen(text)-entry->position);
            ei_entry_set_text((ei_widget_t)entry,new);
        }
        else if(event->param.key_code==SDLK_LEFT) {

            if (entry->position < strlen(entry->text))
                entry->position += 1;
            fprintf(stderr, "%d\n", entry->position);
        }
        else if(event->param.key_code==SDLK_RIGHT ) {
            if(entry->position >0 )
                entry->position -=1 ;
            fprintf(stderr, "%d\n", entry->position);
        }
    }

    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool animation_cursor(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = user_param;
    entry->is_focus_visible = !entry->is_focus_visible ;
    hw_event_schedule_app(500,NULL);

}

/*------------------------------------------------------------------------------------------------------------------*/
