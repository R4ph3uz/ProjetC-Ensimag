#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "widgetclass/ei_frame.h"
#include "ei_implementation.h"
#include "ei_widget_configure.h"
#include "widgetclass/ei_frame.h"
#include "widgetclass/ei_button.h"
#include "ei_widget_attributes.h"
#include "ei_event.h"
/* ----------------------------------------------------------------- */

ei_impl_widget_t ARBRE_WIDGET;

static ei_surface_t ROOT_SURFACE;
static ei_widget_t ROOT_WIDGET;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();
    ei_widgetclass_t* frame = create_frame_widgetclass();
    ei_widgetclass_t* button = create_button_widgetclass();
    ei_widgetclass_register(frame);
    ei_widgetclass_register(button);

    //défini le geometry manager
    ei_geometrymanager_t* placer = malloc(sizeof(ei_geometrymanager_t));
    placer->runfunc = ei_placer_runfunc;
    placer->releasefunc = ei_placer_releasefunc;

    const char name[] = "button";
    strcpy(placer->name, name);

    placer->next = NULL;
    // enregistre
    ei_geometrymanager_register(placer);
    ROOT_SURFACE =  hw_create_window(main_window_size, fullscreen);
    ROOT_WIDGET =  ei_widget_create("frame",NULL,NULL,NULL);
    ei_widget_set_requested_size(ROOT_WIDGET,(ei_size_t) main_window_size);
    ROOT_WIDGET->screen_location.size=main_window_size;
}

/* ----------------------------------------------------------------- */
bool point_in_rect(ei_point_t point, ei_rect_t rectangle){
    bool top = point.y> rectangle.top_left.y;
    bool left = point.x >rectangle.top_left.x;
    bool bottom = point.y < rectangle.top_left.y + rectangle.size.height;
    bool right = point.x < rectangle.top_left.x + rectangle.size.width;
    return top && left && bottom && right;

}


void ei_app_run(void)
{
    // draw func de root et ça se débrouille
    ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),NULL,NULL);
    // (*(ROOT_WIDGET->wclass->drawfunc))(ROOT_WIDGET, ei_app_root_surface(), NULL, NULL);
    while(true){
        ei_event_t *new_event = malloc(sizeof(ei_event_t));
        ei_button_t button = (ei_button_t) ROOT_WIDGET->children_head;
        hw_event_wait_next(new_event);
        if(new_event->type == ei_ev_mouse_buttondown && point_in_rect(new_event->param.mouse.where, button->widget.screen_location)){
            *button->relief = ei_relief_sunken;
            ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),NULL,NULL);
        }
        else if(new_event->type == ei_ev_mouse_buttonup && point_in_rect(new_event->param.mouse.where, button->widget.screen_location)){
            *button->relief = ei_relief_raised;
            ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),NULL,NULL);
        }
        else if (new_event->type == ei_ev_close){
            ei_app_quit_request();
            break;
        }
    }

    hw_quit();
}



/* ----------------------------------------------------------------- */

void ei_app_free(void)
{

}

/* ----------------------------------------------------------------- */

void ei_app_quit_request(void)
{
    ei_app_free();
}

/* ----------------------------------------------------------------- */

ei_surface_t ei_app_root_surface(void)
{
    return ROOT_SURFACE;
}

/* ----------------------------------------------------------------- */

ei_widget_t ei_app_root_widget(void)
{
    return ROOT_WIDGET;
}
