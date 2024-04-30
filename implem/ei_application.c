#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "widget_class/ei_frame.h"
#include "ei_implementation.h"
#include "ei_widget_configure.h"
/* ----------------------------------------------------------------- */
ei_impl_widget_t ARBRE_WIDGET;

static ei_widget_t ROOT_WIDGET;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();
    ei_widgetclass_t* frame = create_frame_widgetclass();
    ei_widgetclass_register(frame);
//    ei_geometrymanager_register();
    ROOT_WIDGET =  ei_widget_create("frame",NULL,NULL,NULL);
    hw_create_window(main_window_size,fullscreen);
}

/* ----------------------------------------------------------------- */

void ei_app_run(void)
{
    // draw func de root et ça se débrouille
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

}

/* ----------------------------------------------------------------- */

ei_widget_t ei_app_root_widget(void)
{
    fprintf(stderr,"je suis dans ei_app_root_widget\n");
    return ROOT_WIDGET;
}
