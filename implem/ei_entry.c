#include "ei_entry.h"
#include "widgetclass/ei_entry.h"
#include <ei_utils.h>
#include "ei_widget_attributes.h"
#include "ei_draw.h"
#include "ei_application.h"
#include "callbacks/entry_callbacks.h"

/*------------------------------------------------------------------------------*/

#define COPY_IF_NOT_NULL(field, value) if ((value) != NULL) { if ((field) == NULL){(field) = malloc(sizeof(*(field)));} memcpy((field), (value),sizeof(*(field)) );}

ei_entry_t ENTRY_FOCUS;
/*------------------------------------------------------------------------------*/



void			ei_entry_configure		(ei_widget_t		widget,
                                           int*			requested_char_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color)
{
    ei_entry_t entry = (ei_entry_t) widget;
    // mets la width a "a"*requested_char_size
    if(requested_char_size){
        char* falsestring=(char*) malloc((*requested_char_size+1)*sizeof(char));
        memset(falsestring,97,*requested_char_size);
        falsestring[*requested_char_size]='\0';
        // fprintf(stderr,"%s\n",falsestring);
        ei_surface_t surfaceee=hw_text_create_surface(falsestring,*entry->text_font,*entry->text_color);
        int width= hw_surface_get_rect(surfaceee).size.width;
        ei_widget_set_requested_size(widget,ei_size(width,20));
        free(falsestring);
    }
    else if (!widget->geom_params)
        ei_widget_set_requested_size(widget,ei_size(50,20));
    // sinon classique
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
    entry->text= malloc(sizeof(char)* (strlen(text)+1));
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
    if(ENTRY_FOCUS){
        ENTRY_FOCUS->focus = false;
        ei_unbind(ei_ev_keydown,NULL,"all",entry_write,ENTRY_FOCUS); // keystroke
        ei_unbind(ei_ev_text_input,NULL,"all",entry_write,ENTRY_FOCUS); // texte collé ?
        ei_unbind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,ENTRY_FOCUS); // si on clique e dehors
        hw_event_schedule_app(500,NULL);
        ei_unbind(ei_ev_app,NULL, "all", animation_cursor,ENTRY_FOCUS);
    }


    entry->focus =true;
    ENTRY_FOCUS = entry;
    ei_bind(ei_ev_keydown,NULL,"all",entry_write,entry); // keystroke
    ei_bind(ei_ev_text_input,NULL,"all",entry_write,entry); // texte collé ?
    ei_bind(ei_ev_mouse_buttondown,NULL,"all",entry_down_click_handler_all,entry); // si on clique e dehors
    hw_event_schedule_app(500,NULL);
    ei_bind(ei_ev_app,NULL, "all", animation_cursor,entry);
}
