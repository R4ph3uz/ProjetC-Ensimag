#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_application.h"
#include "widgetclass/ei_frame.h"
#include "widgetclass/ei_button.h"
#include <string.h>

#define COPY_IF_NOT_NULL(field, value) if ((value) != NULL) { if ((field) == NULL){(field) = malloc(sizeof(*(field)));} memcpy((field), (value),sizeof(*(field)) );}

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
	COPY_IF_NOT_NULL(frame->color,color);
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
    ei_button_t button = (ei_button_t) widget;
    if ((ei_color_t*) color != NULL) {
        if (!button->color){
            button->color=malloc(sizeof(*button->color));
        }
        memcpy(button->color, (ei_color_t*) color,sizeof(*button->color) );
    }
    COPY_IF_NOT_NULL(button->border_width, border_width);
    if (corner_radius != NULL) {
        memcpy(button->corner_radius, corner_radius,sizeof(*button->corner_radius) );
    }
    if (relief != NULL) {
        *button->relief= *relief;
    }
    if (text != NULL) {
        memcpy(button->text, text,sizeof(*button->text) );
    }
    COPY_IF_NOT_NULL(button->text_font,text_font);
    if (text_color != NULL) {
        memcpy(button->text_color, text_color,sizeof(*button->text_color) );
    }
    COPY_IF_NOT_NULL(button->text_anchor,text_anchor);
    COPY_IF_NOT_NULL(button->img,img);
    if (img_rect != NULL) {
        memcpy(button->img_rect, img_rect,sizeof(*button->img_rect) );
    }
    COPY_IF_NOT_NULL(button->img_anchor,img_anchor);
    if (callback != NULL) {
        memcpy(button->callback, callback,sizeof(*button->callback) );
    }
    COPY_IF_NOT_NULL(button->user_param,user_param);
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

