//
// Created by raphael on 5/8/24.
//
#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_entry.h"
#include "ei_placer.h"


int main(int argc, char** argv)
{
    ei_widget_t	entry;

    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t){600, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

    /* Create, configure and place the frame on screen. */
    ei_widget_t entries		= ei_widget_create	("entry", ei_app_root_widget(), NULL, NULL);
    ei_entry_configure	(entries, &(int){30}, &(ei_color_t){0xff, 0xff, 0xff, 0xff}, NULL, NULL, NULL);
    ei_entry_set_text(entries, "OUUUAAAIIISSS");
    ei_place		(entries, &(ei_anchor_t){ei_anc_northwest}, &(int){200}, &(int){200},
                                 &(int){200}, NULL, NULL, NULL, NULL, NULL);


    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
