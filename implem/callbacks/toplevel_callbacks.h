#ifndef TOPLEVEL_CALLBACKS_H
#define TOPLEVEL_CALLBACKS_H

#include "../ei_implementation.h"
#include "ei_types.h"
#include "ei_event.h"

/**
 * @brief	handle a down click ;  vérifie sur qui est cliqué
 */
bool toplevel_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	handle a up click
 */
bool toplevel_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
/**
 * @brief	handle how resize the toplevel
 */
bool toplevel_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
/**
 * @brief stop moving the toplevel
 */
bool toplevel_move_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);
/**
 * @brief	move the toplevel
 */
bool toplevel_move_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);



#endif //TOPLEVEL_CALLBACKS_H

