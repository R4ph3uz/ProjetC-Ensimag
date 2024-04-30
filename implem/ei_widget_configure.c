#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_application.h"
#include "widget_class/ei_frame.h"
/*-------------------------------------------------------------------------------------------------------*/

void			ei_frame_configure		(ei_widget_t		widget,
                                           ei_size_t*		requested_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_relief_t*		relief,
                                           ei_string_t*		text,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color,
                                           ei_anchor_t*		text_anchor,
                                           ei_surface_t*		img,
                                           ei_rect_ptr_t*		img_rect,
                                           ei_anchor_t*		img_anchor)
{
    widget->requested_size=*requested_size;
    ei_frame_t frame = (ei_frame_t) widget;
    frame->color=(ei_color_t*) color;
    frame->border_width= border_width;
    frame->relief = relief;
    frame->text=text;
    frame->text_font=text_font;
    frame->text_color=text_color;
    frame->text_anchor=text_anchor;
    frame->img=img;
    frame->img_rect=img_rect;
    frame->img_anchor=img_anchor;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_button_configure		(ei_widget_t		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 int*			corner_radius,
							 ei_relief_t*		relief,
							 ei_string_t*		text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_ptr_t*		img_rect,
							 ei_anchor_t*		img_anchor,
							 ei_callback_t*		callback,
							 ei_user_param_t*	user_param)
{
//    widget->requested_size=*requested_size;
//    ei_button_t button = (ei_button_t) widget;
//    button->color=(ei_color_t*) color;
//    button->border_width= border_width;
//    button->relief = relief;
//    button->text=text;
//    button->text_font=text_font;
//    button->text_color=text_color;
//    button->text_anchor=text_anchor;
//    button->img=img;
//    button->img_rect=img_rect;
//    button->img_anchor=img_anchor;
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_toplevel_configure		(ei_widget_t		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_string_t*		title,
							 bool*			closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_ptr_t*		min_size)
{

}

