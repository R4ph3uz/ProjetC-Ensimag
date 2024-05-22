//#include "ei_geometrymanager.h"
//#include "ei_implementation.h"
//#include "ei_types.h"
//#include <stdlib.h>
//
//
//
//
//
//
//
//
///*-------------------------------------------------------------------------------------------------------*/
//
//void ei_grid_runfunc(ei_widget_t widget) {
//    ei_rect_t *newscreen = SAFE_MALLOC(sizeof(ei_rect_t));
//    if (!widget->geom_params) {
//        return;
//    }
//    int res = (int) ((0.2f) * (float) (widget->parent->content_rect->size.height));
//    newscreen->size.height = res;
//    int res2 = (int) ((0.2f) * (float) (widget->parent->content_rect->size.width));
//    newscreen->size.width = res2;
//    int x =(int)  ((float)(widget->parent->content_rect->size.width) * (0.2f));
//    int y =(int)  ((float)(widget->parent->content_rect->size.height) * (0.2f));
//
//    if (*widget->geom_params->anchor == ei_anc_northwest) {
//        newscreen->top_left.x = x;
//        newscreen->top_left.y = y;
//    }
//    if (*widget->geom_params->anchor == ei_anc_north) {
//        newscreen->top_left.x = x - (int) (newscreen->size.width / 2);
//        newscreen->top_left.y = y;
//    }
//    if (*widget->geom_params->anchor == ei_anc_northeast) {
//        newscreen->top_left.x = x - (int) (newscreen->size.width);
//        newscreen->top_left.y = y;
//    }
//    if (*widget->geom_params->anchor == ei_anc_west) {
//        newscreen->top_left.x = x;
//        newscreen->top_left.y = y - (int) (newscreen->size.height / 2);
//    }
//    if (*widget->geom_params->anchor == ei_anc_center) {
//        newscreen->top_left.x = x - (int) (newscreen->size.width / 2);
//        newscreen->top_left.y = y - (int) (newscreen->size.height / 2);
//    }
//    if (*widget->geom_params->anchor == ei_anc_east) {
//        newscreen->top_left.x = x - (int) (newscreen->size.width);
//        newscreen->top_left.y = y - (int) (newscreen->size.height / 2);
//    }
//    if (*widget->geom_params->anchor == ei_anc_southwest) {
//        newscreen->top_left.x = x;
//        newscreen->top_left.y = y - (int) (newscreen->size.height);
//    }
//    if (*widget->geom_params->anchor == ei_anc_south) {
//        newscreen->top_left.x = x - (int) (newscreen->size.width / 2);
//        newscreen->top_left.y = y - (int) (newscreen->size.height);
//    }
//    if (*widget->geom_params->anchor == ei_anc_southeast) {
//        newscreen->top_left.x = x - (int) (newscreen->size.width);
//        newscreen->top_left.y = y - (int) (newscreen->size.height);
//    }
//    ei_geometry_run_finalize(widget, newscreen);
//}
///*-------------------------------------------------------------------------------------------------------*/
//void ei_grid_releasefunc(ei_widget_t widget)
//{
//
//
//}
///*-------------------------------------------------------------------------------------------------------*/
//
//    ei_geometrymanager_t* create_grid_gm(){
////        ei_geometrymanager_t* res = SAFE_MALLOC(sizeof(ei_geometrymanager_t));
//        ei_geometrymanager_t* res;
//        res->runfunc=ei_grid_runfunc;
//        res->releasefunc = ei_grid_releasefunc;
//        const char name[] = "grid";
//        strcpy(res->name, name);
//        res->next = NULL;
//        return res;
//    }
///*-------------------------------------------------------------------------------------------------------*/
//void		grid	(ei_widget_t		widget,
//                     ei_anchor_t*		anchor) {
//    if (widget->geom_params==NULL) {
//        widget->geom_params = SAFE_MALLOC(sizeof(ei_impl_geom_param_t));
//
//        widget->geom_params->manager = ei_geometrymanager_from_name("grid");
//        widget->geom_params->is_reconfigurable = SAFE_MALLOC(sizeof(bool));
//        *widget->geom_params->is_reconfigurable=SAFE_MALLOC(sizeof(bool));
//        widget->geom_params->anchor = SAFE_MALLOC(sizeof(ei_anchor_t));
//        if (anchor) {
//            *widget->geom_params->anchor = *anchor;
//        } else {
//            *widget->geom_params->anchor = ei_anc_northwest;
//        }
//
//
//        widget->geom_params->manager->runfunc(widget);
//    }
//    else
//    {
//
//    }
//}
