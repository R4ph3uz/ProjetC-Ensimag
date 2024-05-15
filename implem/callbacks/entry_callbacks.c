#include "entry_callbacks.h"
#include <string.h>
#include <ei_entry.h>
#include "../widgetclass/ei_entry.h"
#include "ei_event.h"
#include "../utils/draw_utils.h"
#include "../utils/text_utils.h"
#include "ei_application.h"
/*------------------------------------------------------------------------------------------------------------------*/

void* ID_EVENT_ANIMATION;
void* get_id_animation(void){
    return ID_EVENT_ANIMATION;
}
void set_id_animation(void* id) {
    ID_EVENT_ANIMATION= id;
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  cette fonction gere les doubles click pour le callback simple click  (il ne faut pas la bin
 * @param entry entry
 * @param event event
 * @return nothing
 */
void handle_double_click(ei_entry_t entry, ei_event_t* event){
    entry_app_event *user_p = malloc(sizeof(entry_app_event));
    user_p->is_animation_event = false;
    user_p->is_double_click_event = true;
    user_p->param= entry;
    if (entry->is_double_clickable){
        entry->is_in_selection = true;
        entry->is_double_clickable = false;

        int debut, fin;
        find_word(entry->text, find_position_cursor_selection_entry(entry, event->param.mouse.where), &debut, &fin);

        entry->debut_selection = debut;
        entry->fin_selection = fin;
    } else{
        entry->is_in_selection = false;
        entry->is_double_clickable = true;
    }
    hw_event_schedule_app(200, user_p);
}
/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) widget;
    entry->position = find_position_cursor_selection_entry(entry, event->param.mouse.where);
    ei_bind(ei_ev_mouse_move, NULL, "all", entry_selection_mouse_move, entry);
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", entry_up_click_handler, entry);

    handle_double_click(entry, event);

    if(entry->focus == false){
        ei_entry_give_focus((ei_widget_t) entry );
        entry->debut_selection = entry->position;
        entry->fin_selection=entry->position;
        entry->is_in_selection = false;
    }


    // faire un truc si shift click
    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler_all(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    // quand on clique n'importe ou sur la page
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
    hw_event_cancel_app(get_id_animation());
    entry->focus=false;
    return true;

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_write(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) user_param;
    char* text = (char*) ei_entry_get_text(user_param);
    if(entry->is_in_selection == false){ // on n'est pas en selection
        if(event->type==ei_ev_text_input){
            if(text && (int32_t) strlen(text)+1 <= *entry->requested_char_size){
                text = insert_char(text, event->param.text, entry->position );
                ei_entry_set_text((ei_widget_t)entry,text);
                entry->position+=1;
                entry->debut_selection=entry->position; // mais dans le doute si au prochain on entre en selection je sauvegarde cette position
            }
            else{
                //on laisse le texte comme ceci
            }
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_DELETE) {
            //touche suppr

            //supprime tout un mot
            if (ei_event_has_ctrl(event)){
                char ch = text[entry->position];
                int pos1 =entry->position;
                int pos2 =pos1;
                while (((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) && (entry->position<(int32_t) strlen(text))) {
                    pos2++;
                    entry->position++;
                    ch=text[entry->position];
                }
                char *new = cut_text(text, pos1, pos2);
                ei_entry_set_text((ei_widget_t)entry,new);
                entry->debut_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->fin_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->position = (int32_t) fminf((float)pos1,(float)pos2);
            }
            else{ // supprime qu'un seul caractere
                char* new = delete_char(text, entry->position+1);
                ei_entry_set_text((ei_widget_t)entry,new);
                entry->debut_selection=entry->position;
            }
        }
        if (event->type == ei_ev_keydown && event->param.key_code==SDLK_BACKSPACE) {
            // touche backspace pas en selection

            //supprime tout un mot
            if (ei_event_has_ctrl(event)) {
                char ch = text[entry->position-1];
                int pos1 =entry->position;
                int pos2 =pos1;
                while(ch==' '){ // supprime tous les espaces après le mot
                    pos2--;
                    entry->position--;
                    ch=text[entry->position-1];
                }// puis supprime le mot
                while (((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9' )) && (pos2>0)) {
                    pos2--;
                    entry->position--;
                    ch=text[entry->position-1];
                }
                char *new = cut_text(text, pos2, pos1);
                ei_entry_set_text((ei_widget_t)entry,new);
                entry->debut_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->fin_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->position = (int32_t) fminf((float)pos1,(float)pos2);
                //gérer le décalge si ctrl+backspace
//                if (entry->decal_x > 0){
//                    int width, height;
//                    char* text_rest = restrict_text(new, entry->position);
//                    hw_text_compute_size(text_rest, *entry->text_font, &width, &height);
//                    entry->decal_x = width-entry->widget.screen_location.size.width;
//                }
            }
            else{
                char* new = delete_char(text, entry->position);
                ei_entry_set_text((ei_widget_t)entry,new);
                if (strcmp(text, new)!=0)
                    entry->position-=1;
                //calcul nouveau decalage
                if (entry->decal_x > 0){
                    int width, height;
                    char* text_rest = restrict_text(new, entry->position);
                    hw_text_compute_size(text_rest, *entry->text_font, &width, &height);
                    entry->decal_x = width-entry->widget.screen_location.size.width;
                }
                entry->debut_selection=entry->position;
                entry->fin_selection=entry->position;
            }
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_LEFT) {
            if (entry->position > 0){
                entry->position -= 1;
                if (ei_event_has_ctrl(event)){
                    char* rest_text = restrict_text(text,entry->position);
                    char ch = rest_text[(int32_t) strlen(rest_text)-1];
                    while(ch==' ' && entry->position>0){
                        entry->position--;
                        entry->fin_selection= entry->position;
                        ch=rest_text[entry->position-1];
                    }
                    while (((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) && entry->position>0 && entry->position<(int32_t) strlen(text)){
                        entry->position-=1;
                        entry->fin_selection= entry->position;
                        ch=rest_text[entry->position-1];
                    }
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->fin_selection=entry->position;
                    entry->is_in_selection=true;
                }
                else{
                    entry->debut_selection=entry->position;
                }
            }
            else{
                entry->fin_selection= 0;
            }
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_RIGHT ) {
            if(entry->position < (int32_t) strlen(entry->text)){
                entry->position +=1;
                //boutton control enfoncé
                if (ei_event_has_ctrl(event)){
                    char ch = text[entry->position];
                    while(ch==' '){
                        entry->fin_selection= entry->position;
                        entry->position++;
                        ch=text[entry->position];
                    }
                    while (((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) && (entry->position<(int32_t) strlen(text))) {
                        entry->fin_selection= entry->position;
                        entry->position++;
                        ch=text[entry->position];
                    }
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->fin_selection=entry->position;
                    entry->is_in_selection=true;
                }
                else{
                    entry->debut_selection=entry->position;
                }
            }
            else{
                entry->fin_selection= (int32_t) strlen(text);
            }
        }
    }
    else{
        //min entre debut selection et fin selection
        int pos1 = (uint8_t) fminf((float)  entry->debut_selection,
                                   (float) entry->fin_selection);
        int pos2 = (uint8_t) fmaxf((float)  entry->debut_selection,
                                   (float) entry->fin_selection);
        if (event->type == ei_ev_text_input){
//            if (pos2 <= strlen(entry->text) && pos1 > 0) {
                char *new = cut_text(text, pos1, pos2);
                text = insert_char(new, event->param.text, entry->position);
                ei_entry_set_text((ei_widget_t) entry, text);
                entry->position += 1;
                entry->is_in_selection = false;
//            }
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_DELETE) {
            // touche suppr si en selection
            char* new= cut_text(text, pos1,pos2 );
            ei_entry_set_text((ei_widget_t)entry,new);
            entry->position = pos1;
            entry->is_in_selection = false;
            entry->debut_selection=entry->position;
            entry->fin_selection=entry->position;
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_BACKSPACE) {
            // touche backspace
            char* new= cut_text(text, pos1,pos2 );
            ei_entry_set_text((ei_widget_t)entry,new);
            entry->position = pos1;
            entry->is_in_selection = false;
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_LEFT) {

            if (pos1 <= (int32_t)strlen(entry->text) && pos1 > 0){
                //boutton control enfoncé
                if (ei_event_has_ctrl(event)){
                    char* rest_text = restrict_text(text,entry->position);
                    char ch = rest_text[strlen(rest_text)-1];
                    while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')){
                        if(entry->position>0){
                            entry->position-=1;
                            ch=rest_text[entry->position-1];
                        }
                    }
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->position-=1;
                    entry->fin_selection = entry->position;
                }
                else{
                    entry->position = pos1;
                    entry->is_in_selection=false;
                    entry->debut_selection=entry->position;
                    entry->fin_selection=entry->position;
                }
            }
            else if (pos1==0){
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->fin_selection = entry->position;
                }
                else{
                    entry->position = 0;
                    entry->is_in_selection=false;
                    entry->debut_selection=entry->position;
                    entry->fin_selection=entry->position;
                }
            }

        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_RIGHT ) {
            if (pos2 <=(int32_t) strlen(entry->text) && pos2 > 0){
                //boutton control enfoncé
                if (ei_event_has_ctrl(event)){
                    char ch = text[entry->position];
                    while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')){
                        if(entry->position<(int32_t) strlen(text)){
                            entry->position+=1;
                            ch=text[entry->position];
                        }
                    }
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->position+=1;
                    entry->fin_selection = entry->position;
                }
                else {
                    entry->position = pos2;
                    entry->is_in_selection = false;
                    entry->debut_selection = entry->position;
                    entry->fin_selection = entry->position;
                }
            }
        }
    }

    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool animation_cursor(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    entry_app_event * app_event= (entry_app_event * ) event->param.application.user_param;
    if (app_event->is_animation_event){
        ei_entry_t entry = (ei_entry_t)app_event->param;
        entry->is_focus_visible = !entry->is_focus_visible ;
        set_id_animation( hw_event_schedule_app(750,user_param));
    }
    else if (app_event->is_double_click_event){
        ei_entry_t entry = (ei_entry_t)app_event->param;
        entry->is_double_clickable = false;
    }

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_selection_mouse_move(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_entry_t entry = user_param;
    entry->is_in_selection = true;
    entry->fin_selection = find_position_cursor_selection_entry(entry,event->param.mouse.where);

    if (event->param.mouse.where.x < entry->widget.screen_location.top_left.x){
        if (entry->decal_x > 0 ){
            entry->decal_x -= 5;
        }
        else if (entry->decal_x != 0)
            entry->decal_x = 0;
    }
    else if (event->param.mouse.where.x > entry->widget.screen_location.top_left.x + entry->widget.screen_location.size.width){
        entry->decal_x += 5;
    }
    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_entry_t entry = user_param;

    ei_unbind(ei_ev_mouse_move, NULL, "all", entry_selection_mouse_move, entry);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", entry_up_click_handler, entry);
    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

ei_widget_t dfs_find_first_after_entry(ei_entry_t entry,ei_widget_t node, bool* founded){
    if (node == (ei_widget_t)entry){
        *founded = true;
    }
    else if(strcmp(node->wclass->name, "entry")== 0 && *founded ){
            return node;
    }
    ei_widget_t temp_widget = node->children_head;
    while(temp_widget!=NULL){
        ei_widget_t res = dfs_find_first_after_entry(entry, temp_widget, founded);
        if(res)
            return res;
        temp_widget = temp_widget->next_sibling;
    }
    return NULL;
}

/*------------------------------------------------------------------------------------------------------------------*/

ei_widget_t dfs_find_first_entry(ei_widget_t node){
    if(strcmp(node->wclass->name, "entry")== 0){
        return node;
    }
    ei_widget_t temp_widget = node->children_head;
    while(temp_widget!=NULL){
        ei_widget_t res = dfs_find_first_entry(temp_widget);
        if(res)
            return res;
        temp_widget = temp_widget->next_sibling;
    }
    return NULL;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool handle_tab_entry(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_entry_t entry = user_param;
    if (event->param.key_code == SDLK_TAB && !ei_event_has_shift(event)){
        bool founded;
        ei_widget_t next_entry = dfs_find_first_after_entry(entry,ei_app_root_widget(),&founded );
        if (next_entry){
            ei_entry_give_focus(next_entry);
        }
        else{
            next_entry = dfs_find_first_entry(ei_app_root_widget());
            if (next_entry)
                ei_entry_give_focus(next_entry);
        }
        return true;
    }
    else if (event->param.key_code == SDLK_TAB && ei_event_has_shift(event)){
        bool founded;
        ei_widget_t next_entry = dfs_find_first_after_entry(entry,ei_app_root_widget(),&founded );
        if (next_entry){
            ei_entry_give_focus(next_entry);
        }
        else{
            next_entry = dfs_find_first_entry(ei_app_root_widget());
            if (next_entry)
                ei_entry_give_focus(next_entry);
        }
        return true;
    }
   return false;
}