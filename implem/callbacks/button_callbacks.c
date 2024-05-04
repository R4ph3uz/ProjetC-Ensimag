
#include "button_callbacks.h"
#include "ei_event.h"
#include "../widgetclass/ei_button.h"
#include "../ei_implementation.h"
#include "ei_application.h"

/*-----------------------------------------------------------------------------------------------------------------------------------------*/

bool up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    ei_button_t button = (ei_button_t) widget;
    *button->relief = ei_relief_sunken;

    ei_impl_widget_draw_children(ei_app_root_widget(),ei_app_root_surface(),get_pick_surface(),NULL);
    return true;

}

/*-----------------------------------------------------------------------------------------------------------------------------------------*/

bool down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    ei_button_t button = (ei_button_t) widget;
    *button->relief = ei_relief_raised;
    fprintf(stderr, "nowup");
    ei_impl_widget_draw_children(ei_app_root_widget(),ei_app_root_surface(),get_pick_surface(),NULL);
    return true;
}