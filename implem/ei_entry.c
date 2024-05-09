#include "ei_entry.h"
#include "widgetclass/ei_entry.h"
#include <ei_utils.h>

#include "ei_draw.h"
#include "ei_application.h"

/*------------------------------------------------------------------------------*/

#define COPY_IF_NOT_NULL(field, value) if ((value) != NULL) { if ((field) == NULL){(field) = malloc(sizeof(*(field)));} memcpy((field), (value),sizeof(*(field)) );}

/*------------------------------------------------------------------------------*/



void			ei_entry_configure		(ei_widget_t		widget,
                                           int*			requested_char_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color)
{
    ei_entry_t entry = (ei_entry_t) widget;
    widget->requested_size =ei_size(150,20);
    COPY_IF_NOT_NULL(entry->requested_char_size,requested_char_size);
    COPY_IF_NOT_NULL(entry->color,color);
    COPY_IF_NOT_NULL(entry->border_width,border_width);
    COPY_IF_NOT_NULL(entry->text_font,text_font);
    COPY_IF_NOT_NULL(entry->text_color,text_color);
}

/*------------------------------------------------------------------------------*/

void			ei_entry_set_text		(ei_widget_t		widget,
                                          ei_const_string_t 	text)
{
    ei_entry_t entry = (ei_entry_t) widget;
    entry->text= malloc(sizeof(char)* strlen(text));
    strcpy( entry->text,text);
}

/*------------------------------------------------------------------------------*/

ei_const_string_t 	ei_entry_get_text		(ei_widget_t		widget)
{
    ei_entry_t entry = (ei_entry_t) widget;
    return entry->text;
}

/*------------------------------------------------------------------------------*/

void			ei_entry_give_focus		(ei_widget_t		widget)
{
    ei_entry_t entry = (ei_entry_t) widget;
    entry->focus =true;
}
