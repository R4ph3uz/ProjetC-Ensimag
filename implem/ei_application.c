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
#include "list_event.h"
#include "pick_event.h"
#include "callbacks/button_callbacks.h"
/* ----------------------------------------------------------------- */

ei_impl_widget_t ARBRE_WIDGET;

static ei_surface_t ROOT_SURFACE;
static ei_widget_t ROOT_WIDGET;
static ei_surface_t PICKING_SURFACE;
static bool IS_RUNNING;

/* ----------------------------------------------------------------- */

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();
    initialize_pickid_array(); // < init dynamic array to have the widget corresponding to an id
    ei_widgetclass_t* frame = create_frame_widgetclass();
    ei_widgetclass_t* button = create_button_widgetclass();
    ei_widgetclass_register(frame);
    ei_bind(ei_ev_mouse_buttondown,NULL,  "button",down_click_handler, NULL );
    ei_bind(ei_ev_mouse_buttonup,NULL,  "button",up_click_handler,NULL );
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
    PICKING_SURFACE = hw_surface_create(ROOT_SURFACE, main_window_size, false);
    ROOT_WIDGET =  ei_widget_create("frame",NULL,NULL,NULL);
    ei_widget_set_requested_size(ROOT_WIDGET,(ei_size_t) main_window_size);
    ROOT_WIDGET->screen_location.size=main_window_size;
}


/* ----------------------------------------------------------------- */

uint32_t get_pick_id(ei_surface_t pick_surface, ei_point_t point) {
    hw_surface_lock(pick_surface);
    uint32_t* buffer = (uint32_t*)hw_surface_get_buffer(pick_surface);
    ei_size_t size = hw_surface_get_size(pick_surface);

    hw_surface_unlock(pick_surface);
    uint32_t index = (point.x)+(point.y)*size.width;
    uint8_t color[4];
    // Décalage de bits et masquage pour extraire chaque octet
    color[0] = (uint8_t)( buffer[index] & 0xFF);
    color[1] = (uint8_t)((buffer[index] >> 8) & 0xFF);
    color[2] = (uint8_t)((buffer[index] >> 16) & 0xFF);
    color[3] = (uint8_t)((buffer[index] >> 24) & 0xFF);

    int ir,ig,ib,ia;
    hw_surface_get_channel_indices( pick_surface, &ir, &ig, &ib, &ia);

    uint32_t value = ((uint32_t)color[ir]) |
                     ((uint32_t)color[ig] << 8) |
                     ((uint32_t)color[ib] << 16) ;

    return value;

}

/* ----------------------------------------------------------------- */

void ei_app_run(void)
{
    // draw func de root et ça se débrouille
    ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),get_pick_surface(),NULL);
    // (*(ROOT_WIDGET->wclass->drawfunc))(ROOT_WIDGET, ei_app_root_surface(), NULL, NULL);
    IS_RUNNING = true;
    while(IS_RUNNING){
        ei_event_t *new_event = malloc(sizeof(ei_event_t));
        hw_event_wait_next(new_event);
        ei_widget_t widget = get_widget_by_pickid(get_pick_id(PICKING_SURFACE,new_event->param.mouse.where ));
        if(new_event->type == ei_ev_mouse_buttondown || new_event->type == ei_ev_mouse_buttonup || new_event->type == ei_ev_mouse_move)
            if(new_event->type == ei_ev_mouse_buttondown)
                fprintf(stderr, "Click %s \n",widget->wclass->name);

        //parcourir la liste des callbacks et appeler si le bon type de widget et le bon type d'event
        list_callback* list_call = get_list_callback();
        while(list_call!=NULL) {
            if(list_call->eventtype == new_event->type && strcmp(list_call->tag, widget->wclass->name)==0 ) {
                fprintf(stderr, "test2");
                list_call->callback(widget,new_event,list_call->user_param);
            }
            list_call = list_call->next;
        }

        if (new_event->type == ei_ev_close){
            ei_app_quit_request();
        }
    }

    hw_quit();
}


/* ----------------------------------------------------------------- */

void ei_app_invalidate_rect(const ei_rect_t* rect) {

}

/* ----------------------------------------------------------------- */

void ei_app_free(void)
{

}

/* ----------------------------------------------------------------- */


void ei_app_quit_request(void)
{
    IS_RUNNING = false;
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

/* ----------------------------------------------------------------- */

ei_surface_t get_pick_surface(void)
{
    return PICKING_SURFACE;
}