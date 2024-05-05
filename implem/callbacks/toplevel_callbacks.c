#include "toplevel_callbacks.h"

#include "../widgetclass/ei_top_level.h"

/*--------------------------------------------------------------------------------------------------------------------*/

bool toplevel_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_top_level_t toplevel = (ei_top_level_t) widget;
    if(event->param.mouse.where.y < widget->screen_location.top_left.y +20) {
        toplevel->isButtonDownOnTop = true;
        toplevel->whereButtonDown = event->param.mouse.where;
        return true;
    }
    return false;

}

/*--------------------------------------------------------------------------------------------------------------------*/


bool toplevel_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_top_level_t toplevel = (ei_top_level_t) widget;
    if(toplevel->isButtonDownOnTop) {
        toplevel->isButtonDownOnTop = false;
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------------------------------------------------*/


bool toplevel_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_top_level_t toplevel = (ei_top_level_t) widget;
    if(toplevel->isButtonDownOnTop) {
        *widget->geom_params->x += event->param.mouse.where.x - toplevel->whereButtonDown.x  ;
        *widget->geom_params->y += event->param.mouse.where.y - toplevel->whereButtonDown.y  ;
        toplevel->whereButtonDown = event->param.mouse.where;
        return true;
    }
    return false;
}