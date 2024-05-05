#ifndef TOPLEVEL_CALLBACKS_H
#define TOPLEVEL_CALLBACKS_H

#include "../ei_implementation.h"
#include "ei_types.h"
#include "ei_event.h"

bool toplevel_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool toplevel_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool toplevel_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool toplevel_move_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool toplevel_move_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);



#endif //TOPLEVEL_CALLBACKS_H

