#include "ei_application.h"

#include <ei_utils.h>

#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "widgetclass/ei_frame.h"
#include "ei_implementation.h"
#include "widgetclass/ei_button.h"
#include "ei_widget_attributes.h"
#include "ei_event.h"
#include "list_event.h"
#include "pick_event.h"
#include "callbacks/button_callbacks.h"
#include "callbacks/entry_callbacks.h"
#include "widgetclass/ei_toplevel.h"
#include "callbacks/toplevel_callbacks.h"
#include "widgetclass/ei_entry.h"
#include "grid.h"
#include "invalidated_rect_list.h"

/* ----------------------------------------------------------------- */

ei_impl_widget_t ARBRE_WIDGET;

static ei_surface_t ROOT_SURFACE;
static ei_widget_t ROOT_WIDGET;
static ei_surface_t PICKING_SURFACE;
static bool IS_RUNNING;
static bool CHANGEMENT_PREMIER_PLAN;

/* ----------------------------------------------------------------- */

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();
    initialize_pickid_array(); // < init dynamic array to have the widget corresponding to an id

    //définit les widget
    ei_widgetclass_t* frame = create_frame_widgetclass();
    ei_widgetclass_t* button = create_button_widgetclass();
    ei_widgetclass_t* top_level = create_toplevel_widgetclass();
    ei_widgetclass_t* entry = create_entry_widgetclass();

    //enregistre les widgets
    ei_widgetclass_register(frame);
    ei_widgetclass_register(button);
    ei_widgetclass_register(top_level);
    ei_widgetclass_register(entry);

    //bind les events pour les widgets
    ei_bind(ei_ev_mouse_buttondown,NULL,  "button",down_click_handler, NULL );
    ei_bind(ei_ev_mouse_buttonup,NULL,  "button",up_click_handler,NULL );
    ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", toplevel_down_click_handler, NULL);
    ei_bind(ei_ev_mouse_buttondown,NULL,"entry",entry_down_click_handler,NULL);

    //définit les geometry manager
    ei_geometrymanager_t* grid = create_grid_gm();
    ei_geometrymanager_t* placer = create_placer_gm();

    // enregistre les geometry manager
    ei_geometrymanager_register(grid);
    ei_geometrymanager_register(placer);


    //Création de la root
    ROOT_SURFACE =  hw_create_window(main_window_size, fullscreen);
    PICKING_SURFACE = hw_surface_create(ROOT_SURFACE, main_window_size, false);
    ROOT_WIDGET =  ei_widget_create("frame",NULL,NULL,NULL);
    ei_widget_set_requested_size(ROOT_WIDGET,main_window_size);
    ROOT_WIDGET->screen_location.top_left = ei_point_zero();
    ROOT_WIDGET->screen_location.size = main_window_size;
    ROOT_WIDGET->content_rect=&ROOT_WIDGET->screen_location;

}


/* ----------------------------------------------------------------- */


void ei_app_run(void)
{
    // Dessine l'écran
    ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),get_pick_surface(),NULL);
    hw_surface_update_rects(ROOT_SURFACE, NULL);

    //Initialise des variables utiles pour la suite
    IS_RUNNING = true;
    CHANGEMENT_PREMIER_PLAN = false;
    ei_widget_t widget = ROOT_WIDGET;
    ei_event_t* new_event = SAFE_MALLOC(sizeof(ei_event_t));
    ei_widget_t widget_last_child = NULL;

    while(IS_RUNNING){ //Tant que le programme tourne

        hw_event_wait_next(new_event);

        ei_rect_t rect_before;
        ei_rect_t rect_after;

        // Si il y a un event
        if(new_event->type == ei_ev_mouse_buttondown || new_event->type == ei_ev_mouse_buttonup )
        {
            ei_widget_t precwid=widget;
            widget= ei_widget_pick(&(new_event->param.mouse.where)); //widget devient le widget sur lequel on vient de cliquer
            widget_last_child=widget;

            //Si le widget n'est pas la root et est différent du widget précedent
            if (widget!=ROOT_WIDGET && widget!=precwid)
            {
                //widget_last_child parcours tout les parents de widget
                if (widget_last_child)
                {
                    while (widget_last_child!= ROOT_WIDGET) {
                        if (strcmp(widget_last_child->wclass->name, "toplevel")==0)  // Si c'est un toplevel
                        {
                            if (widget_last_child->parent->children_tail!=widget_last_child) // Si le toplevel n'est pas déja au premier plan
                            {
                                //on met la toplevel au premier plan (en dernier enfant)
                                CHANGEMENT_PREMIER_PLAN=true;
                                supprime_de_ses_freres(widget_last_child);
                                place_a_la_fin(widget_last_child);

                            }
                        }
                        widget_last_child = widget_last_child->parent;
                    }
                }
            }
            memcpy(&rect_before, &widget->screen_location,sizeof(ei_rect_t));
        }

        bool isModified1 = false;
        bool isModified = false;

        //parcourir la liste des callbacks et appeler si le bon type de widget et le bon type d'event
        list_callback* list_call = get_list_callback();
        while(list_call!=NULL) {

            if(list_call->eventtype== ei_ev_mouse_buttondown || list_call->eventtype== ei_ev_mouse_buttonup) {
                if (list_call->tag && list_call->eventtype == new_event->type &&
                    strcmp(list_call->tag, widget->wclass->name) == 0) {
                    isModified = (*list_call->callback)(widget, new_event, list_call->user_param) || isModified;
                }
            }
            if( isModified)
                break;

            if(list_call->tag && strcmp(list_call->tag, "all")==0 &&  list_call->eventtype == new_event->type) {
                isModified = (*list_call->callback)(widget, new_event, list_call->user_param) || isModified;

            }
            if(isModified)
                break;
            list_call = list_call->next;
        }



        ei_rect_t * union_rect = NULL;
        if(((isModified) )||CHANGEMENT_PREMIER_PLAN) {



             if (ROOT_WIDGET->children_tail){
                 //find the closest parent that is a top level, if there is none then update everything
                 ei_widget_t temp = ROOT_WIDGET->children_tail;
                 while(temp->parent!=NULL){
                     if(strcmp(temp->wclass->name, "toplevel")== 0 ){
                         break;
                     }
                     temp = temp->parent;
                 }

                 if(temp->parent && temp->geom_params){
                     ei_rect_t* test  = intersection_rectangle(hw_surface_get_rect(ROOT_SURFACE),temp->screen_location );
                     rect_before = *test;
                     temp->geom_params->manager->runfunc(temp);
                     ei_rect_t* test2  = intersection_rectangle(hw_surface_get_rect(ROOT_SURFACE),temp->screen_location );
                     rect_after = *test2;
                     ei_linked_rect_t list;
                     list.rect = rect_before;
                     list.next = NULL;
                     union_rect = union_rectangle(rect_before, rect_after);
                 }


             }
        }
        ei_rect_t* union_r = union_rectangles(get_invalidated_rect_list());
        ei_impl_widget_draw_children(ROOT_WIDGET,ei_app_root_surface(),get_pick_surface(),union_r);
        SAFE_FREE(union_r);
        hw_surface_update_rects(ROOT_SURFACE,get_invalidated_rect_list());
        //hw_surface_update_rects(ROOT_SURFACE,get_invalidated_rect_list()); //with invalidated rect (seem slower)
        reinitialize_invalidated_rect_list();
    }
    free(new_event);
    hw_quit();
}


/* ----------------------------------------------------------------- */

void ei_app_invalidate_rect(const ei_rect_t* rect)
{
    if(rect) {
        ei_linked_rect_t* list = get_invalidated_rect_list();
        ei_linked_rect_t* new_head = malloc(sizeof(ei_linked_rect_t));
        new_head->rect= *rect;
        new_head->next = list;
        set_invalidated_rect_list(new_head);
    }

}

/* ----------------------------------------------------------------- */

void ei_app_free(void)
{
    free_text_copie();
    free_list_callback();
    ei_widget_destroy(ROOT_WIDGET);
    ei_free_geometrymanager();
    ei_free_widgetclass();
    hw_surface_free(ROOT_SURFACE);
    hw_surface_free(PICKING_SURFACE);
    hw_quit();
}

/* ----------------------------------------------------------------- */


void ei_app_quit_request(void)
{
    IS_RUNNING = false;

}

/* ----------------------------------------------------------------- */

ei_surface_t ei_app_root_surface(void)
{
    return ROOT_SURFACE;
}

/* ----------------------------------------------------------------- */

ei_widget_t ei_app_root_widget(void)
{
    return ROOT_WIDGET;
}

/* ----------------------------------------------------------------- */

ei_surface_t get_pick_surface(void)
{
    return PICKING_SURFACE;
}