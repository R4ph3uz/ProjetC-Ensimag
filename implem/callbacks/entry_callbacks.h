#ifndef ENTRY_CALLBACKS_H
#define ENTRY_CALLBACKS_H

#include "../ei_implementation.h"
#include "ei_types.h"
#include "ei_event.h"

/**
 * @brief  check if clicked, then put focus or not on it
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

#endif //ENTRY_CALLBACKS_H
