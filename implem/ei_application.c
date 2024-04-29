//
// Created by legrandh on 29/04/24.
//

#include "ei_application.h"
#include "hw_interface.h"
#include "ei_geometrymanager.h"

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
    hw_init();
//    ei_widgetclass_register();
//    ei_geometrymanager_register();
    hw_create_window(main_window_size, fullscreen);


}

/* ----------------------------------------------------------------- */

void ei_app_run(void){
    getchar();
    hw_quit();
}

/* ----------------------------------------------------------------- */

void ei_app_free(void){

}
/* ----------------------------------------------------------------- */

void ei_app_quit_request(void){
    hw_quit();
}


/* ----------------------------------------------------------------- */

ei_widget_t ei_app_root_widget(void) {

}

/* ----------------------------------------------------------------- */

ei_surface_t ei_app_root_surface(void){

}



