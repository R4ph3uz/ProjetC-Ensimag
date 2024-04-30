#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"

/* ----------------------------------------------------------------- */

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();
    ei_widgetclass_register();
    ei_geometrymanager_register();
    hw_create_window(main_window_size, fullscreen);
}

/* ----------------------------------------------------------------- */

void ei_app_run(void)
{
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

}
