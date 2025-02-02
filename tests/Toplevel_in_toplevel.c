//
// Created by Léonard Havet on 21/8/24.
//

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
bool button_press(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    printf("Click !\n");
    return true;
}

/*
 * default_handler --
 *
 *	Callback called to handle keypress and window close events.
 */
bool default_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	if ( (event->type == ei_ev_close) ||
	    ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_ESCAPE))) {
		ei_app_quit_request();
		return true;
	} else
		return false;
}

int main(int argc, char** argv)
{
	ei_widget_t	top2;
    ei_widget_t	top1;

	ei_widget_t	window;
    ei_widget_t	window2;

	/* Create the application and change the color of the background. */
	ei_app_create			((ei_size_t){800, 600}, false);
	ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	/* Create, configure and place a toplevel window on screen. */
	window = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure		(window, &(ei_size_t){320,240},
					      	 &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
						 &(int){2},
						 &(ei_string_t){"♥☺☺♥"}, NULL, NULL, NULL);
	ei_place_xy			(window, 30, 10);
    window2 = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure		(window2, &(ei_size_t){320,240},
					      	 &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
						 &(int){2},
						 &(ei_string_t){"♣♣♣♣"}, NULL, NULL, NULL);
	ei_place_xy			(window2, 30, 100);

	/* Create, configure and place a button as a descendant of the toplevel window. */
	top2 = ei_widget_create	("toplevel", window, NULL, NULL);
	ei_toplevel_configure		(top2, &(ei_size_t){120,140},
                                  &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
                                  &(int){2},
                                  &(ei_string_t){"2"}, NULL, NULL, NULL);
	ei_place			(top2, &(ei_anchor_t){ei_anc_southeast},
					 	&(int){-20}, &(int){-20}, NULL, NULL,
					 	&(float){1.0f}, &(float){1.0f},
					 	&(float){0.5f}, NULL);

    top1 = ei_widget_create	("toplevel", window, NULL, NULL);
    ei_toplevel_configure		(top1, &(ei_size_t){120,140},
                                  &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
                                  &(int){2},
                                  &(ei_string_t){"1"}, NULL, NULL, NULL);
    ei_place			(top1, &(ei_anchor_t){ei_anc_northwest},
                         &(int){0}, &(int){0}, NULL, NULL,
                         &(float){0}, &(float){0},
                         &(float){0.5f}, NULL);

	ei_bind(ei_ev_keydown, NULL, "all", default_handler, NULL);
	ei_bind(ei_ev_close, NULL, "all", default_handler, NULL);

	ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", default_handler, NULL);
	ei_unbind(ei_ev_close, NULL, "all", default_handler, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}
