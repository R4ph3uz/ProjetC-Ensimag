#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_application.h"
#include "widgetclass/ei_frame.h"
#include <string.h>

#define COPY_IF_NOT_NULL(field, value) if ((value) != NULL) { memcpy((field), (value),sizeof(*(field)) );}

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

    if (requested_size)
        widget->requested_size= *requested_size;

    ei_frame_t frame = (ei_frame_t) widget;

	if (color) {
		frame->color->alpha = color->alpha;
		frame->color->blue = color->blue;
		frame->color->red = color->red;
		frame->color->green = color->green;

	}
    COPY_IF_NOT_NULL(frame->border_width, border_width);
    COPY_IF_NOT_NULL(frame->relief, relief);
    COPY_IF_NOT_NULL(frame->text, text);
    COPY_IF_NOT_NULL(frame->text_font, text_font);
    COPY_IF_NOT_NULL(frame->text_color, text_color);
    COPY_IF_NOT_NULL(frame->text_anchor, text_anchor);
    COPY_IF_NOT_NULL(frame->img, img);
    COPY_IF_NOT_NULL(frame->img_rect, img_rect);
    COPY_IF_NOT_NULL(frame->img_anchor, img_anchor);

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
	if (requested_size)
		widget->requested_size=(ei_size_t )*requested_size;
//    ei_button_t button = (ei_button_t) widget;
//    ASSIGN_IF_NOT_NULL(button->color,(ei_color_t*) color);
//    ASSIGN_IF_NOT_NULL(button->border_width, border_width);
//    ASSIGN_IF_NOT_NULL(button->relief , relief);
//    ASSIGN_IF_NOT_NULL(button->text,text);
//    ASSIGN_IF_NOT_NULL(button->text_font,text_font);
//    ASSIGN_IF_NOT_NULL(button->text_color,text_color);
//    ASSIGN_IF_NOT_NULL(button->text_anchor,text_anchor);
//    ASSIGN_IF_NOT_NULL(button->img,img);
//    ASSIGN_IF_NOT_NULL(button->img_rect,img_rect);
//    ASSIGN_IF_NOT_NULL(button->img_anchor,img_anchor);
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
	if (requested_size)
		widget->requested_size=(ei_size_t )*requested_size;
	//    ei_toplevel_t toplevel = (ei_toplevel_t) widget;
	//    ASSIGN_IF_NOT_NULL(toplevel->color,(ei_color_t*) color);
	//    ASSIGN_IF_NOT_NULL(toplevel->border_width, border_width);
	//    ASSIGN_IF_NOT_NULL(toplevel->title,title);
	//    ASSIGN_IF_NOT_NULL(toplevel->closable,closable);
	//    ASSIGN_IF_NOT_NULL(toplevel->resizable,resizable);
	//    ASSIGN_IF_NOT_NULL(toplevel->text_anchor,text_anchor);
	//    ASSIGN_IF_NOT_NULL(toplevel->min_size,min_size);
}

