#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "widgetclass/ei_frame.h"
#include "ei_implementation.h"
#include "ei_widget_configure.h"
#include "widgetclass/ei_frame.h"
#include "widgetclass/ei_button.h"
#include "ei_widget_attributes.h"
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

void ei_app_run(void)
{
    // draw func de root et ça se débrouille
    ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),NULL,NULL);
    // (*(ROOT_WIDGET->wclass->drawfunc))(ROOT_WIDGET, ei_app_root_surface(), NULL, NULL);
    getchar();
    hw_quit();
}

/* ----------------------------------------------------------------- */

void ei_app_free(void)
{

}

/* ----------------------------------------------------------------- */

void ei_app_quit_request(void)
{

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
