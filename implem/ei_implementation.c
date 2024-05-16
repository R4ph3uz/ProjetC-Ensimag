#include "ei_implementation.h"
#include "hw_interface.h"
#include <stdlib.h>

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_runfunc(ei_widget_t widget)
{
    ei_rect_t* newscreen=malloc(sizeof (ei_rect_t));
    if(!widget->geom_params){
        return;
    }
    int res=(int)((float)*widget->geom_params->height + *(widget->geom_params->rel_height) * (float)(widget->parent->content_rect->size.height));
    newscreen->size.height =res ;
    int res2=(int)((float)*widget->geom_params->width + *(widget->geom_params->rel_width) * (float)(widget->parent->content_rect->size.width));
    newscreen->size.width = res2;
    int x =(int)(widget->parent->content_rect->top_left.x+*widget->geom_params->x+(int) ((float)(widget->parent->content_rect->size.width)**(widget->geom_params->rel_x)));
    int y =(int)(widget->parent->content_rect->top_left.y+*widget->geom_params->y+(int) ((float)(widget->parent->content_rect->size.height)**(widget->geom_params->rel_y)));

    if (*widget->geom_params->anchor==ei_anc_northwest)
    {
        newscreen->top_left.x = x;
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_north)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width/2);
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_northeast)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width);
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_west)
    {
        newscreen->top_left.x = x;
        newscreen->top_left.y = y- (int) (newscreen->size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_center)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width/2);
        newscreen->top_left.y = y- (int) (newscreen->size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_east)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width);
        newscreen->top_left.y = y- (int) (newscreen->size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_southwest)
    {
        newscreen->top_left.x = x ;
        newscreen->top_left.y = y- (int) (newscreen->size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_south)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width/2);
        newscreen->top_left.y = y- (int) (newscreen->size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_southeast)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width);
        newscreen->top_left.y = y- (int) (newscreen->size.height);
    }
    ei_geometry_run_finalize(widget,newscreen);


}

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_releasefunc(ei_widget_t widget)
{
    if (widget->children_head!=NULL)
    {
        ei_widget_t children;
        children=widget->children_head;
        while (children!=widget->children_tail)
        {
            ei_geometrymanager_unmap(children);
            children=children->next_sibling;
        }
        ei_geometrymanager_unmap(children);
    }
}

/*-------------------------------------------------------------------------------------------------------*/
ei_rect_t* intersection_rectangle(ei_rect_t rect1,ei_rect_t rect2) {
    // Find the coordinates of the top-left corner of the intersection rectangle
    int x_tl = rect1.top_left.x > rect2.top_left.x ? rect1.top_left.x : rect2.top_left.x;
    int y_tl = rect1.top_left.y > rect2.top_left.y ? rect1.top_left.y : rect2.top_left.y;

    // Find the coordinates of the bottom-right corner of the intersection rectangle
    int x_br = (rect1.top_left.x + rect1.size.width) < (rect2.top_left.x + rect2.size.width) ?
               (rect1.top_left.x + rect1.size.width) : (rect2.top_left.x + rect2.size.width);
    int y_br = (rect1.top_left.y + rect1.size.height) < (rect2.top_left.y + rect2.size.height) ?
               (rect1.top_left.y + rect1.size.height) : (rect2.top_left.y + rect2.size.height);

    // Calculate the width and height of the intersection rectangle
    int width = x_br - x_tl;
    int height = y_br - y_tl;

    // Check if the rectangles intersect
    if (width <= 0 || height <= 0) {
        // No intersection, return NULL
        return NULL;
    } else {
        // Construct and return the intersection rectangle
        ei_rect_t* intersection_rect = malloc(sizeof(ei_rect_t));
        if (intersection_rect == NULL) {
            // Allocation failed
            return NULL;
        }
        intersection_rect->top_left.x = x_tl;
        intersection_rect->top_left.y = y_tl;
        intersection_rect->size.width = width;
        intersection_rect->size.height = height;
        return intersection_rect;
    }
}

/*------------------------------------------------------------------------------------*/

ei_rect_t* union_rectangle(ei_rect_t rect1, ei_rect_t rect2)
{
    // Calculate the top-left point of the union rectangle
    int x1 = rect1.top_left.x;
    int y1 = rect1.top_left.y;
    int x2 = rect2.top_left.x;
    int y2 = rect2.top_left.y;
    int x = (x1 < x2) ? x1 : x2;
    int y = (y1 < y2) ? y1 : y2;

    // Calculate the size of the union rectangle
    int w1 = rect1.size.width;
    int h1 = rect1.size.height;
    int w2 = rect2.size.width;
    int h2 = rect2.size.height;
    int w = (x1 + w1 > x2 + w2) ? (x1 + w1 - x) : (x2 + w2 - x);
    int h = (y1 + h1 > y2 + h2) ? (y1 + h1 - y) : (y2 + h2 - y);

    // Allocate memory for the union rectangle
    ei_rect_t* rect = malloc(sizeof(ei_rect_t));
    if (!rect) {
        return NULL;
    }

    // Set the values of the union rectangle
    rect->top_left.x = x;
    rect->top_left.y = y;
    rect->size.width = w;
    rect->size.height = h;

    return rect;
}

/*------------------------------------------------------------------------------------*/

uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color){
    int ir,ig,ib,ia;
    hw_surface_get_channel_indices( surface, &ir, &ig, &ib, &ia);
    uint8_t* res = malloc(sizeof(uint8_t)*4);
    res[ir] = color.red;
    res[ig] = color.green;
    res[ib] = color.blue;
    if(ia!=-1){
        res[ia] = color.alpha;
    }
    return (uint32_t) *res;

}

/*------------------------------------------------------------------------------------*/

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                         ei_surface_t		surface,
                         ei_surface_t		pick_surface,
                         ei_rect_t*		clipper) {
    (*(widget->wclass->drawfunc))(widget,surface,pick_surface,clipper);
        ei_widget_t actuel = widget->children_head;
    if (actuel!=NULL) {
        while(actuel!=NULL) {
            // verifier surface
            if (clipper==NULL)
            {
                clipper=actuel->parent->content_rect;
            }
            else
            {
                clipper=intersection_rectangle(*clipper,*actuel->parent->content_rect);
            }
            ei_impl_widget_draw_children(actuel,surface,pick_surface,clipper);
            actuel=actuel->next_sibling;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t* create_placer_gm(){
    ei_geometrymanager_t* res = SAFE_MALLOC(sizeof(ei_geometrymanager_t));
    res->runfunc=ei_placer_runfunc;
    res->releasefunc = ei_placer_releasefunc;
    const char name[] = "placer";
    strcpy(res->name, name);
    res->next = NULL;
    return res;
}