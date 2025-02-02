#include "toplevel_callbacks.h"
#include "../widgetclass/ei_toplevel.h"
#include "ei_event.h"
#include "ei_types.h"
#include <math.h>
#include "ei_placer.h"

/*--------------------------------------------------------------------------------------------------------------------*/

bool toplevel_down_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_toplevel_t toplevel = (ei_toplevel_t) widget;

    //On est sur le rond rouge
    if (event->param.mouse.where.x>widget->content_rect->top_left.x+6  && event->param.mouse.where.x<widget->content_rect->top_left.x+20 && event->param.mouse.where.y>widget->content_rect->top_left.y-17  && event->param.mouse.where.y<widget->content_rect->top_left.y-3)
    {

        ei_widget_destroy(widget);
        return true;
    }

    //On est sur la barre du toplevel
   if(event->param.mouse.where.y < widget->content_rect->top_left.y ) {

        toplevel->whereButtonDown = event->param.mouse.where;
        ei_bind(ei_ev_mouse_move, NULL, "all", toplevel_move_mouse_mouve_handler, toplevel);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", toplevel_move_up_click_handler, toplevel);
        return true;
    }

    //On est sur le petit carré du toplevel
    if(event->param.mouse.where.y > widget->content_rect->top_left.y+ widget->content_rect->size.height-10 &&
        event->param.mouse.where.x > widget->content_rect->top_left.x+ widget->content_rect->size.width-10 &&
            *toplevel->resizable!= 0){
        ei_bind(ei_ev_mouse_move, NULL, "all", toplevel_mouse_mouve_handler, toplevel);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", toplevel_up_click_handler, toplevel);
        toplevel->whereButtonDown = event->param.mouse.where;
        return true;
    }
    return false;

}

/*--------------------------------------------------------------------------------------------------------------------*/

// On enléve le click lorsque l'on est sur le petit carré
bool toplevel_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_toplevel_t toplevel = (ei_toplevel_t) user_param;
    ei_unbind(ei_ev_mouse_move, NULL, "all", toplevel_mouse_mouve_handler, toplevel);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", toplevel_up_click_handler, toplevel);
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

        // On se déplace lorsque l'on est sur le petit carré
bool toplevel_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_toplevel_t toplevel = (ei_toplevel_t) user_param;
    ei_anchor_t *anchor=widget->geom_params->anchor;

    //place la toplevel si elle est manager par autre chose que placer
    if (strcmp(widget->geom_params->manager->name   , "placer") != 0)
    {
        ei_place			(toplevel, NULL,
                             &(int){widget->content_rect->top_left.x-widget->parent->content_rect->top_left.x},
                             &(int){widget->content_rect->top_left.y-widget->parent->content_rect->top_left.y-30},
                             &(int){widget->content_rect->size.width},
                             &(int){widget->content_rect->size.height},
                             NULL, NULL,
                             NULL, NULL);
    }

    if (*toplevel->resizable==ei_axis_x ||*toplevel->resizable==ei_axis_both)
    {
        int ancien = *toplevel->widget.geom_params->width;
        *toplevel->widget.geom_params->width =(int) fmax((*toplevel->min_size)->width -(int) (*widget->geom_params->rel_width*(float)widget->parent->content_rect->size.width), *toplevel->widget.geom_params->width+ event->param.mouse.where.x - toplevel->whereButtonDown.x)  ;
        if ((*anchor==ei_anc_northeast) || (*anchor==ei_anc_east) || (*anchor==ei_anc_southeast))
            *toplevel->widget.geom_params->x+=*toplevel->widget.geom_params->width- ancien ;
        if ((*anchor==ei_anc_north) || (*anchor==ei_anc_center) || (*anchor==ei_anc_south))
            *toplevel->widget.geom_params->x+=(*toplevel->widget.geom_params->width- ancien)/2 ;
    }
    if (*toplevel->resizable==ei_axis_y ||*toplevel->resizable==ei_axis_both)
    {
        int ancien = *toplevel->widget.geom_params->height;
        *toplevel->widget.geom_params->height =(int) fmax((*toplevel->min_size)->height -(int) (*widget->geom_params->rel_height*(float)widget->parent->content_rect->size.height) , *toplevel->widget.geom_params->height+ event->param.mouse.where.y - toplevel->whereButtonDown.y)  ;
        if ((*anchor==ei_anc_southeast) || (*anchor==ei_anc_south) || (*anchor==ei_anc_southwest))
            *toplevel->widget.geom_params->y+=*toplevel->widget.geom_params->height- ancien ;
        if ((*anchor==ei_anc_west) || (*anchor==ei_anc_center) || (*anchor==ei_anc_east))
            *toplevel->widget.geom_params->y+=(*toplevel->widget.geom_params->height- ancien)/2 ;
    }
    toplevel->whereButtonDown = event->param.mouse.where;

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

        //On arrete de cliquer lorsque l'on est sur la barre de la toplevel
bool toplevel_move_up_click_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_toplevel_t toplevel = (ei_toplevel_t) user_param;
    ei_unbind(ei_ev_mouse_move, NULL, "all", toplevel_move_mouse_mouve_handler, toplevel);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", toplevel_move_up_click_handler, toplevel);
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

        //on se deplace lorsque l'on est sur le barre de la toplevel
bool toplevel_move_mouse_mouve_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_toplevel_t toplevel = (ei_toplevel_t) user_param;

    //place la toplevel si elle est manager par autre chose que placer
    if (strcmp(widget->geom_params->manager->name   , "placer") != 0)
    {
        ei_place			(toplevel, NULL,
                             &(int){widget->content_rect->top_left.x-widget->parent->content_rect->top_left.x},
                             &(int){widget->content_rect->top_left.y-widget->parent->content_rect->top_left.y-30},
                             &(int){widget->content_rect->size.width},
                             &(int){widget->content_rect->size.height},
                             NULL, NULL,
                             NULL, NULL);
    }

        *toplevel->widget.geom_params->x += event->param.mouse.where.x - toplevel->whereButtonDown.x  ;
        *toplevel->widget.geom_params->y += event->param.mouse.where.y - toplevel->whereButtonDown.y;
    toplevel->whereButtonDown = event->param.mouse.where;
    return true;
}