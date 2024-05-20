#ifndef BUTTON_CALLBACKS_H
#define BUTTON_CALLBACKS_H

#include "../ei_implementation.h"
#include "ei_types.h"
#include "ei_event.h"

/**
 * gere le up click du boutton
 * @param widget widget
 * @param event evenement up-click
 * @param user_param boutton en question
 * @return
 */
bool up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * gere le down click du boutton
 * @param widget widget
 * @param event evenement up-click
 * @param user_param boutton en question
 * @return
 */
bool down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * gere si on se déplace aux alentours du boutton
 * @param widget widget
 * @param event evenement up-click
 * @param user_param boutton en question
 * @return
 */

bool button_mouse_move(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif //BUTTON_CALLBACKS_H
