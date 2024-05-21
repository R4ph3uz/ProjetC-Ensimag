#ifndef ENTRY_CALLBACKS_H
#define ENTRY_CALLBACKS_H

#include "../ei_implementation.h"
#include "ei_types.h"
#include "ei_event.h"
#include "../widgetclass/ei_entry.h"

typedef struct entry_app_event{
    bool is_animation_event;
    bool is_double_click_event;
    void* param;
}entry_app_event;

void free_text_copie();

void free_userp();

/**
 * @brief  check if clicked, then put focus or not on it (first time clicked)
 * @param widget entry
 * @param event downclick
 * @param user_param if needed
 * @return
 */
bool entry_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief ecrit qqch dedans
 * @param widget entry
 * @param event keystrokes
 * @param user_param  if needed
 * @return
 */
bool entry_write(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief  check if clicked, then put focus or not on it
 * @param widget entry
 * @param event downclick
 * @param user_param if needed
 * @return
 */
bool entry_down_click_handler_all(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief  anime le curseur en l'affichant et le desaffichant a intervalle régulier
 * @param widget entry
 * @param event downclick
 * @param user_param if needed
 * @return true if need to redraw
 */
bool animation_cursor(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief quand on bouge a l'interieur d'une entry en ayant le clic enfoncé
 * @param widget entry
 * @param event mouse move
 * @param user_param if needed
 * @return true if need to redraw
 */
bool entry_selection_mouse_move(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief quand on relache le clic pour arreter de selectionner
 * @param widget entry
 * @param event mouse upclic
 * @param user_param if needed
 * @return true if need to redraw
 */
bool entry_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief quand on tab pour focus sur l'entry suivante
 * @param widget entry
 * @param event keyboard tab
 * @param user_param if needed
 * @return true if need to redraw
 */
bool handle_tab_entry(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 *
 * @return la valeur de l'id renvoyé par hw_schedule_app pour pouvoir le cancel quand on enleve le focus de l'entry
 */
void* get_id_animation(void);

/**
 *
 * @param id la valeur de l'id renvoyé par hw_schedule_app
 */
void set_id_animation(void* id);
/**
 * effectue le control-c
 * @param widget
 * @param event
 * @param user_param entry du control c
 */
bool controlc(ei_widget_t widget,ei_event_t* event,ei_user_param_t user_param);

/**
 * effectue le control-x
 * @param widget
 * @param event
 * @param user_param entry du control x
 */
bool controlx(ei_widget_t widget,ei_event_t* event,ei_user_param_t user_param);

/**
 * effectue le control-v
 * @param widget
 * @param event
 * @param user_param entry du control v
 */
bool controlv(ei_widget_t widget,ei_event_t* event,ei_user_param_t user_param);

/**
 *
 * @param entry the entry we want to find the next entry in the tree
 * @param node actual node, root_widget for the first call then modified by the recursive call
 * @param founded false for the first call
 * @return the first entry after (prefix order) entry, null if there is none
 */
ei_widget_t dfs_find_first_after_entry(ei_entry_t entry,ei_widget_t node, bool* founded);

/**
 *
 * @param node actual node, root_widget for the first call then modified by the recursive call
 * @return the first entry in the tree(prefix order) entry, null if there is none
 */
ei_widget_t dfs_find_first_entry(ei_widget_t node);


/**
 *
 * @param entry the entry we want to find the first before this entry
 * @param node actual node, root_widget for the first call then modified by the recursive call
 * @param temp null for the first call
 * @return the first entry after (prefix order) entry, null if there is none
 */
ei_widget_t dfs_find_last_before_entry(ei_entry_t entry,ei_widget_t node, ei_widget_t* temp);

/**
 *
 * @param node actual node, root_widget for the first call then modified by the recursive call
 * @return the last entry in the tree (except entry)
 */
ei_widget_t dfs_find_last_except_entry(ei_entry_t entry,ei_widget_t node, ei_widget_t* temp);

/**
 * renvoie l'entry qui a le focus
 * @return l'entry qui est focus
 */
ei_entry_t get_entry_focus(void);

/**
 * mets le focus à l'entry donnée en paramètres
 * @param entry entry à mettre le focus
 */
void set_entry_focus(ei_entry_t entry);

#endif //ENTRY_CALLBACKS_H
