
#include "button_callbacks.h"
#include "ei_event.h"
#include "../widgetclass/ei_button.h"
#include "../ei_implementation.h"
#include "ei_application.h"

/*-----------------------------------------------------------------------------------------------------------------------------------------*/

bool up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    ei_button_t button = (ei_button_t) widget;
    *button->relief = ei_relief_raised;
    ei_unbind(ei_ev_mouse_move, NULL, "all", button_mouse_move,button);
    return true;

}

/*-----------------------------------------------------------------------------------------------------------------------------------------*/

bool down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    ei_button_t button = (ei_button_t) widget;
    ei_bind(ei_ev_mouse_move, NULL, "all", button_mouse_move,button);
    *button->relief = ei_relief_sunken;
    button->is_clicked = true;
    return true;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------*/

bool button_mouse_move(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    ei_button_t button = (ei_button_t) user_param;
    if(event->param.mouse.where.x> button->widget.content_rect->top_left.x
       && event->param.mouse.where.x< button->widget.content_rect->top_left.x+button->widget.content_rect->size.width
       && event->param.mouse.where.y> button->widget.content_rect->top_left.y
       && event->param.mouse.where.y< button->widget.content_rect->top_left.y+ button->widget.content_rect->size.height)
    {
        button->is_clicked = true;
        *button->relief = ei_relief_sunken;
    }
    else{
        button->is_clicked = false;
        *button->relief = ei_relief_raised;
    }

    return true;
}