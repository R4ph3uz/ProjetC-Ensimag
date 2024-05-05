#ifndef BUTTON_CALLBACKS_H
#define BUTTON_CALLBACKS_H

#include "../ei_implementation.h"
#include "ei_types.h"
#include "ei_event.h"

bool up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif //BUTTON_CALLBACKS_H
