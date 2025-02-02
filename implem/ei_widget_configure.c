#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "widgetclass/ei_frame.h"
#include "widgetclass/ei_button.h"
#include "widgetclass/ei_toplevel.h"
#include "ei_event.h"
#include <string.h>
#include "ei_utils.h"
#include "ei_placer.h"
#include "ei_widget_attributes.h"
#define COPY_IF_NOT_NULL(field, value) if ((value) != NULL) { if ((field) == NULL){(field) = SAFE_MALLOC(sizeof(*(field)));} memcpy((field), (value),sizeof(*(field)) );}

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
    ei_frame_t frame = (ei_frame_t) widget;
    if (requested_size)
        ei_widget_set_requested_size(widget,*requested_size);
    else if (!widget->geom_params && !text)
        ei_widget_set_requested_size(widget, ei_size(40,30));
    else if (!widget->geom_params ){
        int width, height;
        hw_text_compute_size(*text, *frame->text_font, &width, &height);
        ei_widget_set_requested_size(widget, ei_size(width,height));
    }
	COPY_IF_NOT_NULL(frame->color,color);
    COPY_IF_NOT_NULL(frame->border_width, border_width);
    COPY_IF_NOT_NULL(frame->relief, relief);
    if (text != NULL) {
        if(frame->text==NULL) {
            frame->text = SAFE_MALLOC(sizeof(ei_string_t ));
            *frame->text =  SAFE_MALLOC(sizeof(char)*30);
        }
        if(*text == NULL){
            SAFE_FREE(*frame->text);
            SAFE_FREE(frame->text);
            frame->text =NULL;
        }
        strcpy(*frame->text,*text);
    }
    if(text_font){
        *frame->text_font =  *text_font;
    }

    COPY_IF_NOT_NULL(frame->text_color, text_color);
    COPY_IF_NOT_NULL(frame->text_anchor, text_anchor);
    COPY_IF_NOT_NULL(frame->img_anchor, img_anchor);

    if(img != NULL) {
        if(frame->img == NULL)
            frame->img = SAFE_MALLOC(sizeof(ei_surface_t));

        *frame->img = hw_surface_create(*img, hw_surface_get_rect(*img).size,true  );
        hw_surface_lock(*img);
        hw_surface_lock(*frame->img );
        ei_copy_surface(*frame->img, NULL,*img,NULL,false);
        hw_surface_unlock(*img);
        hw_surface_unlock(*frame->img );
    }
    if (img_rect != NULL) {
        if (frame->img_rect==NULL){
            frame->img_rect=SAFE_MALLOC(sizeof(ei_rect_ptr_t));
        }
        *frame->img_rect = SAFE_MALLOC(sizeof(ei_rect_t));
        memcpy(*frame->img_rect, *img_rect, sizeof(ei_rect_t));

    }


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
							 ei_user_param_t*	user_param) {


    ei_button_t button = (ei_button_t) widget;

    if (requested_size)
        ei_widget_set_requested_size(widget,*requested_size);
    else if (!widget->geom_params && text){
        int width, height;
        hw_text_compute_size(*text, *button->text_font,&width, &height );
        ei_widget_set_requested_size(widget, ei_size(width,height));
    }
    else if(!widget->geom_params)
        ei_widget_set_requested_size(widget, ei_size(50,20));

    if ((ei_color_t *) color != NULL) {
        if (!button->color) {
            button->color = SAFE_MALLOC(sizeof(*button->color));
        }
        memcpy(button->color, (ei_color_t *) color, sizeof(*button->color));
    }
    COPY_IF_NOT_NULL(button->border_width, border_width);
    if (corner_radius != NULL) {
        if (!button->corner_radius) {
            button->corner_radius = SAFE_MALLOC(sizeof(*button->corner_radius));
        }
        memcpy(button->corner_radius, corner_radius, sizeof(*button->corner_radius));
    }
    if (relief != NULL) {
        if (!button->relief) {
            button->relief = SAFE_MALLOC(sizeof(*button->relief));
        }
        *button->relief = *relief;
    }
    if (text != NULL) {
        if(button->text==NULL) {
            button->text = SAFE_MALLOC(sizeof(ei_string_t ));
            *button->text =  SAFE_MALLOC(sizeof(char)*60);
        }
        if(*text == NULL){
            SAFE_FREE(*button->text);
            SAFE_FREE(button->text);
        }
        else{
            strcpy(*button->text,*text);
        }

    }
    if(text_font){
        *button->text_font = *text_font;
    }
    if (text_color != NULL) {
        if (!button->text_color) {
            button->text_color = SAFE_MALLOC(sizeof(*button->text_color));
        }
        memcpy(button->text_color, text_color, sizeof(*button->text_color));
    }
    COPY_IF_NOT_NULL(button->text_anchor, text_anchor);
    if (img != NULL) {

        if (*img == NULL)
            button->img = NULL;
        else {
            button->img = SAFE_MALLOC(sizeof(ei_surface_t));
            *button->img = hw_surface_create(*img, hw_surface_get_rect(*img).size, true);
            hw_surface_lock(*img);
            hw_surface_lock(*button->img);
            ei_copy_surface(*button->img, NULL, *img, NULL, false);
            hw_surface_unlock(*img);
            hw_surface_unlock(*button->img);
        }

    }
    if (img_rect != NULL) {
        if (button->img_rect == NULL) {
            button->img_rect = SAFE_MALLOC(sizeof(ei_rect_ptr_t));
        }
        *button->img_rect = SAFE_MALLOC(sizeof(ei_rect_t));
        memcpy(*button->img_rect, *img_rect, sizeof(ei_rect_t));

    }
    COPY_IF_NOT_NULL(button->img_anchor, img_anchor);
    if (callback != NULL)
        ei_bind(ei_ev_mouse_buttonup, widget, NULL, *callback, user_param);
    if (user_param != NULL) {
        button->widget.user_data = *user_param;
    }



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
        ei_widget_set_requested_size(widget,*requested_size);
    else if (!widget->geom_params)
        ei_widget_set_requested_size(widget, ei_size(500,500));

    ei_toplevel_t toplevel = (ei_toplevel_t) widget;
    COPY_IF_NOT_NULL(toplevel->color,(ei_color_t*) color);
    COPY_IF_NOT_NULL(toplevel->border_width, border_width);
    COPY_IF_NOT_NULL(toplevel->title,title);
    COPY_IF_NOT_NULL(toplevel->closable,closable);
    COPY_IF_NOT_NULL(toplevel->resizable,resizable);
    COPY_IF_NOT_NULL(toplevel->min_size,min_size);


}

