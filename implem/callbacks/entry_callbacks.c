#include "entry_callbacks.h"
#include <string.h>
#include <ei_entry.h>
#include "../widgetclass/ei_entry.h"
#include "ei_event.h"
#include "../draw_utils/draw_utils.h"

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) widget;
    entry->position = find_position_cursor_entry(entry, event->param.mouse.where);
    entry->debut_selection = event->param.mouse.where;
    ei_bind(ei_ev_mouse_move, NULL, "all", entry_selection_mouse_move, entry);
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", entry_up_click_handler, entry);
    entry->is_in_selection = false;
    if(event->param.mouse.where.x> widget->screen_location.top_left.x
        && event->param.mouse.where.x< widget->screen_location.top_left.x+widget->screen_location.size.width
        && event->param.mouse.where.y> widget->screen_location.top_left.y
        && event->param.mouse.where.y< widget->screen_location.top_left.y+ widget->screen_location.size.height
        && entry->focus == false)
    {
        // je suis dans l'entry
        entry->focus=true;
        // que mettre dans user param ? ou entry ?
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
    if (event->type == ei_ev_text_input && entry->is_in_selection== false) {
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

    }
    else if (event->type == ei_ev_text_input && entry->is_in_selection){
        char symbole[2] = {event->param.text,'\0'};
        char* text = (char*) ei_entry_get_text(widget);
        int pos1 = find_position_cursor_selection_entry(entry, entry->debut_selection)+1;
        int pos2 = find_position_cursor_selection_entry(entry, entry->fin_selection)+1;

        if(pos1 <pos2){

            char* new= cut_text(text, pos1,pos2 );
            fprintf(stderr, "%d, %d test delete selection 1 avant : %s, apres %s\n",pos1,pos2, text, new);
            text = insert_char(new, event->param.text, strlen(text)-entry->position ); //fuite de mémoire here
            ei_entry_set_text((ei_widget_t)entry,text);

        }
        else{
            char* new = cut_text(text,pos2, pos1 );
            fprintf(stderr, "%d, %d test delete selection 1 avant : %s, apres %s\n",pos1,pos2, text, new);
            text = insert_char(new, event->param.text, strlen(text)-entry->position ); //fuite de mémoire here
            ei_entry_set_text((ei_widget_t)entry,text);
        }
        entry->position = 0;
        entry->is_in_selection = false;

    }
    else if(event->type == ei_ev_keydown) {
        if(event->param.key_code==SDLK_DELETE || event->param.key_code==SDLK_BACKSPACE) {
            if (!entry->is_in_selection){
                fprintf(stderr, "yeah i want do delete\n");
                char* text = (char*) ei_entry_get_text(widget);
                char* new = delete_char(text, strlen(text)-entry->position);
                ei_entry_set_text((ei_widget_t)entry,new);
            }
            else{
                fprintf(stderr, "yeah i want do delete\n");
                char* text = (char*) ei_entry_get_text(widget);
                int pos1 = find_position_cursor_selection_entry(entry, entry->debut_selection)+1;
                int pos2 = find_position_cursor_selection_entry(entry, entry->fin_selection)+1;

                if(pos1 <pos2){

                    char* new= cut_text(text, pos1,pos2 );
                    fprintf(stderr, "%d, %d test delete selection 1 avant : %s, apres %s\n",pos1,pos2, text, new);
                    ei_entry_set_text((ei_widget_t)entry,new);

                }
                else{
                    char* new = cut_text(text,pos2, pos1 );
                    fprintf(stderr, "%d, %d test delete selection 1 avant : %s, apres %s\n",pos1,pos2, text, new);
                    ei_entry_set_text((ei_widget_t)entry,new);
                }
                entry->position = 0;
                entry->is_in_selection = false;
            }

        }
        else if(event->param.key_code==SDLK_LEFT) {

            if (entry->position < strlen(entry->text))
                entry->position += 1;
            // fprintf(stderr, "%d\n", entry->position);
        }
        else if(event->param.key_code==SDLK_RIGHT ) {
            if(entry->position >0 )
                entry->position -=1 ;
            // fprintf(stderr, "%d\n", entry->position);
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

bool entry_selection_mouse_move(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_entry_t entry = user_param;
    entry->is_in_selection = true;
    entry->fin_selection = event->param.mouse.where;
    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_entry_t entry = user_param;

    ei_unbind(ei_ev_mouse_move, NULL, "all", entry_selection_mouse_move, entry);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", entry_up_click_handler, entry);
    return true;
}