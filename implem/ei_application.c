#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "widget_class/ei_frame.h"
#include "ei_implementation.h"

/* ----------------------------------------------------------------- */

ei_impl_widget_t ARBRE_WIDGET;

ei_surface_t ROOT;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();
    ei_widgetclass_t* frame = create_frame_widgetclass();
    ei_widgetclass_register(frame);
//    ei_geometrymanager_register();
    ROOT =  hw_create_window(main_window_size, fullscreen);
}

/* ----------------------------------------------------------------- */

void ei_app_run(void)
{
    // pesudo code
    // while true:
    // iterer sur l'arbre de widget
    // dessiner le widget
    // passer à celui d'après
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
    return ROOT;
}
