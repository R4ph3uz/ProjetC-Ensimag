#include "entry_callbacks.h"
#include <string.h>
#include <ei_entry.h>
#include "../widgetclass/ei_entry.h"
#include "ei_event.h"
#include "../utils/draw_utils.h"
#include "../utils/text_utils.h"
#include "ei_application.h"
/*------------------------------------------------------------------------------------------------------------------*/
ei_entry_t ENTRY_FOCUS;
char* TEXTE_COPIE;
void* ID_EVENT_ANIMATION;
void* get_id_animation(void){
    return ID_EVENT_ANIMATION;
}
void set_id_animation(void* id) {
    ID_EVENT_ANIMATION= id;
}

/*------------------------------------------------------------------------------------------------------------------*/

#define SAFE_REALLOC(ptr, size) \
    ({ \
        void *new_ptr = realloc(ptr, size); \
        if (!new_ptr && (size) != 0) { \
            fprintf(stderr, "Memory reallocation failed, exiting application.\n"); \
            exit(EXIT_FAILURE); \
        } \
        new_ptr; \
    })

/*------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  cette fonction gere les doubles click pour le callback simple click  (il ne faut pas la bin
 * @param entry entry
 * @param event event
 * @return nothing
 */
void handle_double_click(ei_entry_t entry, ei_event_t* event){
    entry_app_event *user_p = SAFE_MALLOC(sizeof(entry_app_event));
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
    entry->fin_selection=entry->debut_selection;
    entry->position = find_position_cursor_selection_entry(entry, event->param.mouse.where);
    entry->debut_selection = find_position_cursor_selection_entry(entry,event->param.mouse.where);
    ei_bind(ei_ev_mouse_move, NULL, "all", entry_selection_mouse_move, entry);
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", entry_up_click_handler, entry);

    handle_double_click(entry, event);
    if(!entry->focus){
        ei_entry_give_focus((ei_widget_t) entry );
        entry->debut_selection = entry->position;
        entry->fin_selection=entry->position;
        entry->is_in_selection = false;
    }
    // faire un truc si shift click
    if(entry->focus && !entry->is_in_selection && ei_event_has_shift(event)){
        int32_t pos1 = (int32_t) fminf((float) entry->debut_selection,
                                       (float) entry->fin_selection);
        int32_t pos2 = (int32_t) fmaxf((float) entry->debut_selection,
                                       (float) entry->fin_selection);
        entry->debut_selection = pos1;
        entry->fin_selection=pos2;
        entry->is_in_selection = true;
    }

    return true;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_down_click_handler_all(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    // quand on clique n'importe ou sur la page
    ei_entry_t entry = (ei_entry_t) user_param;
    if(event->param.mouse.where.x> entry->widget.content_rect->top_left.x
        && event->param.mouse.where.x< entry->widget.content_rect->top_left.x+entry->widget.content_rect->size.width
        && event->param.mouse.where.y> entry->widget.content_rect->top_left.y
        && event->param.mouse.where.y< entry->widget.content_rect->top_left.y+ entry->widget.content_rect->size.height)
    {
        // je suis dans l'entry
        return false;

    }

    ei_unbind(ei_ev_keydown,NULL,  "all",controlc, entry);
    ei_unbind(ei_ev_keydown,NULL,  "all",controlx, entry);
    ei_unbind(ei_ev_keydown,NULL,  "all",controlv, entry);
    ei_unbind(ei_ev_keydown,NULL,"all",entry_write,entry); // keystroke
    ei_unbind(ei_ev_text_input,NULL,"all",entry_write,entry); // texte collé ?
    ei_unbind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,entry); // si on clique e dehors
    ei_unbind(ei_ev_app,NULL, "all", animation_cursor,NULL);
    set_entry_focus(NULL);
    hw_event_cancel_app(get_id_animation());
    free_userp();
    entry->focus=false;
    return true;

}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_write(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_entry_t entry = (ei_entry_t) user_param;
    char* text = (char*) ei_entry_get_text(user_param);
    if(entry->is_in_selection == false){ // on n'est pas en selection
        if(event->type==ei_ev_text_input){
            text = insert_char(text, event->param.text, entry->position );
            ei_entry_set_text((ei_widget_t)entry,text);
            entry->position+=1;
            entry->debut_selection=entry->position; // mais dans le doute si au prochain on entre en selection je sauvegarde cette position
            SAFE_FREE(text);
            return true;
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_DELETE) {
            //touch suppr

            //supprime tout un mot
            if (ei_event_has_ctrl(event)){
                char ch = text[entry->position];
                int pos1 =entry->position;
                int pos2 = bypass_control(entry->text,entry->position,1);
                char *new = cut_text(text, pos1, pos2);
                ei_entry_set_text((ei_widget_t)entry,new);
                SAFE_FREE(new);
                entry->debut_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->fin_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->position = (int32_t) fminf((float)pos1,(float)pos2);
            }
            else{ // supprime qu'un seul caractere
                if(entry->position>=strlen(text)){
                    entry->position=strlen(text);
                    return false;
                }
                char* new = delete_char(text, entry->position+1);
                ei_entry_set_text((ei_widget_t)entry,new);
                SAFE_FREE(new);
                entry->debut_selection=entry->position;
            }
            return true;
        }
        if (event->type == ei_ev_keydown && event->param.key_code==SDLK_BACKSPACE) {
            // touch backspace pas en selection

            //supprime tout un mot
            if (ei_event_has_ctrl(event)) {
                int pos1 =entry->position;
                int pos2 = bypass_control(entry->text,entry->position,-1);
                char *new = cut_text(text, pos2, pos1);
                //gérer le décalge si ctrl+backspace
                if (entry->decal_x > 0 && strcmp(text, new)!=0){
                    text=entry->text;
                    int old_width, old_height;
                    char* old_text = restrict_text(text, entry->position+1);
                    hw_text_compute_size(old_text, *entry->text_font, &old_width, &old_height);
                    int decalage = entry->decal_x-old_width+entry->widget.screen_location.size.width;

                    int width, height;
                    char* text_rest = restrict_text(new, entry->position);
                    hw_text_compute_size(text_rest, *entry->text_font, &width, &height);
                    entry->decal_x = width-entry->widget.screen_location.size.width +decalage;
                    if (width <entry->widget.screen_location.size.width )
                        entry->decal_x =0;
                    SAFE_FREE(old_text);
                    SAFE_FREE(text_rest);
                }
                ei_entry_set_text((ei_widget_t)entry,new);
                SAFE_FREE(new);
                entry->debut_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->fin_selection = (int32_t) fminf((float)pos1,(float)pos2);
                entry->position = (int32_t) fminf((float)pos1,(float)pos2);
            }
            else{
                if(entry->position<=0){
                    entry->position=0;
                    entry->debut_selection=entry->position;
                    entry->fin_selection=entry->position;
                    return false;
                }
                char* new = delete_char(text, entry->position);
                //calcul nouveau decalage
                if (entry->decal_x > 0){
                    int old_width, old_height;
                    char* old_text = restrict_text(text, entry->position+1);
                    hw_text_compute_size(old_text, *entry->text_font, &old_width, &old_height);
                    int decalage = entry->decal_x-old_width+entry->widget.screen_location.size.width;

                    int width, height;
                    char* text_rest = restrict_text(new, entry->position);
                    hw_text_compute_size(text_rest, *entry->text_font, &width, &height);
                    entry->decal_x = width-entry->widget.screen_location.size.width +decalage;
                    if (width <entry->widget.screen_location.size.width )
                        entry->decal_x =0;
                    SAFE_FREE(old_text);
                    SAFE_FREE(text_rest);
                }
                if (strcmp(text, new)!=0){
                    ei_entry_set_text((ei_widget_t)entry,new);
                    entry->position-=1;
                    SAFE_FREE(new);
                }
                entry->debut_selection=entry->position;
                entry->fin_selection=entry->position;

            }
            return true;
        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_LEFT) {
            if (entry->position > 0){
                entry->position -= 1;
                if (ei_event_has_ctrl(event)){
                    entry->fin_selection = entry->position = bypass_control(entry->text,entry->position,-1);
                }
                // pour les buttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->fin_selection=entry->position;
                    entry->is_in_selection=true;
                }
                else{
                    entry->debut_selection=entry->position;
                }
                return true;
            }
            else{
                entry->fin_selection= 0;
                return false;
            }

        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_RIGHT ) {
            if(entry->position < (int32_t) strlen(entry->text)){
                entry->position +=1;
                //boutton control enfoncé
                if (ei_event_has_ctrl(event)){
                    entry->fin_selection = entry->position = bypass_control(entry->text,entry->position,1);
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->fin_selection=entry->position;
                    entry->is_in_selection=true;
                }
                else{
                    entry->debut_selection=entry->position;
                }
                return true;
            }
            else{
                entry->fin_selection= entry->position =(int32_t) strlen(text);
                return false;
            }
        }
    }
    else {
        //min entre debut selection et fin selection
        int32_t pos1 = (int32_t) fminf((float) entry->debut_selection,
                                   (float) entry->fin_selection);
        int32_t pos2 = (int32_t) fmaxf((float) entry->debut_selection,
                                   (float) entry->fin_selection);
        if (event->type == ei_ev_text_input){
            entry->position=pos1;
            char *new = cut_text(text, pos1, pos2);
            char* text2 = insert_char(new, event->param.text, entry->position);
            ei_entry_set_text((ei_widget_t) entry, text2);
            entry->position += 1;
            entry->debut_selection=entry->fin_selection =entry->position;
            entry->is_in_selection = false;
            SAFE_FREE(new);
            SAFE_FREE(text2);
            return true;
        }
        if (event->type == ei_ev_keydown && event->param.key_code == SDLK_DELETE) {
            // touche suppr si en selection
            char *new = cut_text(text, pos1, pos2);
            //calcul nouveau decalage
            if (entry->decal_x > 0 && strcmp(text, new)!=0){
                int old_width, old_height;
                char* old_text = restrict_text(text, entry->position+1);
                hw_text_compute_size(old_text, *entry->text_font, &old_width, &old_height);
                int decalage = entry->decal_x-old_width+entry->widget.screen_location.size.width;

                int width, height;
                char* text_rest = restrict_text(new, entry->position);
                hw_text_compute_size(text_rest, *entry->text_font, &width, &height);
                entry->decal_x = width-entry->widget.screen_location.size.width +decalage;
                if (width <entry->widget.screen_location.size.width )
                    entry->decal_x =0;
                SAFE_FREE(old_text);
                SAFE_FREE(text_rest);
            }
            if (strcmp(text, new)!=0){
                ei_entry_set_text((ei_widget_t)entry,new);
                entry->position-=1;
                SAFE_FREE(new);
            }
            entry->position = pos1;
            entry->is_in_selection = false;
            entry->debut_selection = entry->position;
            entry->fin_selection = entry->position;
            return true;
        }
        if (event->type == ei_ev_keydown && event->param.key_code == SDLK_BACKSPACE) {
            // touch backspace
            char *new = cut_text(text, pos1, pos2);
            //calcul nouveau decalage
            if (entry->decal_x > 0 && strcmp(text, new)!=0){
                int old_width, old_height;
                char* old_text = restrict_text(text, entry->position+1);
                hw_text_compute_size(old_text, *entry->text_font, &old_width, &old_height);
                int decalage = entry->decal_x-old_width+entry->widget.screen_location.size.width;

                int width, height;
                char* text_rest = restrict_text(new, entry->position);
                hw_text_compute_size(text_rest, *entry->text_font, &width, &height);
                entry->decal_x = width-entry->widget.screen_location.size.width +decalage;
                if (width <entry->widget.screen_location.size.width )
                    entry->decal_x =0;
                SAFE_FREE(old_text);
                SAFE_FREE(text_rest);
            }
            if (strcmp(text, new)!=0){
                ei_entry_set_text((ei_widget_t)entry,new);
                entry->position=pos1;
                SAFE_FREE(new);
                entry->is_in_selection = false;
                return true;
            }
            return false;
        }
        if (event->type == ei_ev_keydown && event->param.key_code == SDLK_LEFT) {
            if (entry->position > 0) {
                entry->position -= 1;
                //boutton control enfoncé
                if (ei_event_has_ctrl(event)) {
                    entry->fin_selection = entry->position= bypass_control(entry->text,entry->position,-1);
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if (ei_event_has_shift(event)) {
                    entry->fin_selection = entry->position;
                } else {
                    entry->position = pos1;
                    entry->is_in_selection = false;
                    entry->debut_selection = entry->position;
                    entry->fin_selection = entry->position;
                }
            } else if (pos1 == 0) {
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if (ei_event_has_shift(event)) {
                    entry->fin_selection = 0;
                    entry->position = 0;
                    return false;
                } else {
                    entry->position = 0;
                    entry->is_in_selection = false;
                    entry->debut_selection = entry->position;
                    entry->fin_selection = entry->position;
                }
            }
            return true;

        }
        if(event->type == ei_ev_keydown && event->param.key_code==SDLK_RIGHT ) {
            if(entry->position < (int32_t) strlen(entry->text)){
                entry->position +=1;
                //boutton control enfoncé
                if (ei_event_has_ctrl(event)){
                    pos2= entry->position = bypass_control(text,entry->position,1);
                }
                // pour les bouttons LSHIFT et RSHIFT enfoncés
                if(ei_event_has_shift(event)){
                    entry->fin_selection = entry->position;
                    return true;
                }
                else {
                    entry->position = pos2;
                    entry->is_in_selection = false;
                    entry->debut_selection = entry->position;
                    entry->fin_selection = entry->position;
                }
                return true;
            }
            else{
                if (ei_event_has_shift(event)){
                    entry->position=(int32_t) strlen(text);
                    entry->fin_selection=(int32_t) strlen(text);
                    return false;
                }
                else{
                    entry->position=entry->debut_selection=entry->fin_selection=(int32_t) strlen(text);
                    return true;
                }
            }
        }
    }
    return false;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool animation_cursor(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    entry_app_event * app_event= (entry_app_event * ) event->param.application.user_param;
    if (app_event->is_animation_event){
        ei_entry_t entry = (ei_entry_t)app_event->param;
        if (entry){
            entry->is_focus_visible = !entry->is_focus_visible ;
            set_id_animation( hw_event_schedule_app(600,event->param.application.user_param));
            return true;
        }

    }
    else if (app_event->is_double_click_event){

            ei_entry_t entry = (ei_entry_t) app_event->param;
        if (entry) {
            entry->is_double_clickable = false;
            SAFE_FREE(app_event);
            return true;
        }
    }
    return false;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool entry_selection_mouse_move(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
    ei_entry_t entry = user_param;
    entry->is_in_selection = true;
    entry->fin_selection = find_position_cursor_selection_entry(entry,event->param.mouse.where);
    entry->position=entry->fin_selection;

    if (event->param.mouse.where.x < entry->widget.content_rect->top_left.x){
        if (entry->decal_x > 0 ){
            entry->decal_x -= 5;
        }
        else if (entry->decal_x != 0)
            entry->decal_x = 0;
    }
    else if (event->param.mouse.where.x > entry->widget.content_rect->top_left.x + entry->widget.content_rect->size.width){
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
        bool founded = false;
        ei_widget_t next_entry = dfs_find_first_after_entry(entry,ei_app_root_widget(),&founded );
        if (next_entry){
            ei_entry_give_focus(next_entry);
            ei_widget_t widget_last_child = (ei_widget_t) next_entry;
            while (widget_last_child!= ei_app_root_widget()) {
                if (strcmp(widget_last_child->wclass->name, "toplevel")==0)  // Si c'est un toplevel
                {
                    if (widget_last_child->parent->children_tail!=widget_last_child) // Si le toplevel n'est pas déja au premier plan
                    {
                        supprime_de_ses_freres(widget_last_child);
                        place_a_la_fin(widget_last_child);

                    }
                }
                widget_last_child = widget_last_child->parent;
            }
        }
        else{
            next_entry = dfs_find_first_entry(ei_app_root_widget());
            if (next_entry){
                ei_entry_give_focus(next_entry);
                ei_widget_t widget_last_child = (ei_widget_t) next_entry;
                while (widget_last_child!= ei_app_root_widget()) {
                    if (strcmp(widget_last_child->wclass->name, "toplevel")==0)  // Si c'est un toplevel
                    {
                        if (widget_last_child->parent->children_tail!=widget_last_child) // Si le toplevel n'est pas déja au premier plan
                        {
                            supprime_de_ses_freres(widget_last_child);
                            place_a_la_fin(widget_last_child);

                        }
                    }
                    widget_last_child = widget_last_child->parent;
                }

            }

        }
        return true;
    }
    else if (event->param.key_code == SDLK_TAB && ei_event_has_shift(event)){
        ei_widget_t temp = NULL;
        ei_widget_t next_entry = dfs_find_last_before_entry(entry,ei_app_root_widget(),&temp );
        if (next_entry){
            ei_entry_give_focus(next_entry);
        }
        else{
            dfs_find_last_except_entry(entry,ei_app_root_widget(),&temp);
            next_entry = temp;
            if (next_entry){
                ei_entry_give_focus(next_entry);
            }

        }
        return true;
    }
   return false;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool controlc(ei_widget_t widget,ei_event_t* event,ei_user_param_t user_param){
    ei_entry_t entry = (ei_entry_t) user_param;
    if (ei_event_has_ctrl(event) && event->param.key_code=='c') {
        if (entry->is_in_selection) {
            int pos1 = (uint8_t) fminf((float) entry->debut_selection,
                                       (float) entry->fin_selection);
            int pos2 = (uint8_t) fmaxf((float) entry->debut_selection,
                                       (float) entry->fin_selection);
            char *text = entry->text;
            char *new_text = texte_selectionne(text, pos1, pos2);
            if (TEXTE_COPIE==NULL) {
                TEXTE_COPIE = SAFE_MALLOC(sizeof(char) * (strlen(new_text)+1));
                strcpy(TEXTE_COPIE, new_text);
            } else {
                TEXTE_COPIE = SAFE_REALLOC(TEXTE_COPIE, sizeof(char) * (strlen(new_text)+1));
                strcpy(TEXTE_COPIE, new_text);
            }
            SAFE_FREE(new_text);
            return true;
        } else {
            return false;
        }
    }
    else
        return false;
}
/*------------------------------------------------------------------------------------------------------------------*/

bool controlx(ei_widget_t widget,ei_event_t* event,ei_user_param_t user_param){
    ei_entry_t entry = (ei_entry_t) user_param;
    if (ei_event_has_ctrl(event) && event->param.key_code=='x') {
        if (entry->is_in_selection){
            char* text = entry->text;
            int pos1 = (uint8_t) fminf((float) entry->debut_selection,
                                       (float) entry->fin_selection);
            int pos2 = (uint8_t) fmaxf((float) entry->debut_selection,
                                       (float) entry->fin_selection);
            char* new_text = texte_selectionne(text,pos1,pos2);
            char* new = cut_text(text, pos1, pos2);
            ei_entry_set_text((ei_widget_t) entry, new);
            SAFE_FREE(new);
            entry->position = pos1;
            entry->is_in_selection = false;
            entry->debut_selection = entry->position;
            entry->fin_selection = entry->position;
            if(TEXTE_COPIE==NULL){
                TEXTE_COPIE=SAFE_MALLOC(sizeof(char)*(strlen(new_text)+1));
                strcpy(TEXTE_COPIE,new_text);
            }
            else{
                TEXTE_COPIE = SAFE_REALLOC(TEXTE_COPIE,sizeof(char)*(strlen(new_text)+1));
                strcpy(TEXTE_COPIE,new_text);
            }

            SAFE_FREE(new_text);
            return true;
        }
        else{
            return false;
        }
    }
    else
        return false;
}
/*------------------------------------------------------------------------------------------------------------------*/
bool controlv(ei_widget_t widget,ei_event_t* event,ei_user_param_t user_param){
    ei_entry_t entry = (ei_entry_t) user_param;
    if (ei_event_has_ctrl(event) && event->param.key_code=='v') {
        if(entry->is_in_selection){
            int32_t pos1 = (int32_t) fminf((float) entry->debut_selection,
                                           (float) entry->fin_selection);
            int32_t pos2 = (int32_t) fmaxf((float) entry->debut_selection,
                                           (float) entry->fin_selection);
            char* text =entry->text;
            char* text2= cut_text(text,pos1,pos2);
            ei_entry_set_text((ei_widget_t) entry,text2);
            SAFE_FREE(text2);
            entry->is_in_selection=false;
            entry->position=entry->debut_selection=entry->fin_selection=pos1;
        }
        char* text = entry->text;
        if(!TEXTE_COPIE){
            return false;
        }
        else{
            char* text2 = insert_word(text,TEXTE_COPIE,entry->position);
            ei_entry_set_text((ei_widget_t) entry, text2);
            SAFE_FREE(text2);
            entry->position+=strlen(TEXTE_COPIE);
            return true;
        }
    }
    else
        return false;
}
/*------------------------------------------------------------------------------------------------------------------*/

void free_text_copie() {
    SAFE_FREE(TEXTE_COPIE);
}

/*------------------------------------------------------------------------------------------------------------------*/

ei_widget_t dfs_find_last_before_entry(ei_entry_t entry,ei_widget_t node, ei_widget_t* temp){
    if (node == (ei_widget_t)entry){
        return *temp;
    }
    else if(strcmp(node->wclass->name, "entry")== 0){

        *temp = node;
    }
    ei_widget_t temp_widget = node->children_head;
    while(temp_widget!=NULL){
        ei_widget_t res = dfs_find_last_before_entry(entry, temp_widget, temp);
        if(res)
            return res;
        temp_widget = temp_widget->next_sibling;
    }
    return NULL;
}

/*------------------------------------------------------------------------------------------------------------------*/

ei_widget_t dfs_find_last_except_entry(ei_entry_t entry,ei_widget_t node,ei_widget_t* temp){
    if(strcmp(node->wclass->name, "entry")== 0){
        *temp = node;
    }
    ei_widget_t temp_widget = node->children_head;
    while(temp_widget!=NULL){
        ei_widget_t res = dfs_find_last_except_entry(entry, temp_widget, temp);
        if(res)
            return res;
        temp_widget = temp_widget->next_sibling;
    }
    return NULL;
}

/*------------------------------------------------------------------------------------------------------------------*/
ei_entry_t get_entry_focus(void) {
    return ENTRY_FOCUS;
}

void set_entry_focus(ei_entry_t entry) {
    ENTRY_FOCUS=entry;
}