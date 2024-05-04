#include "ei_implementation.h"
#include "hw_interface.h"
#include <stdlib.h>

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_runfunc(ei_widget_t widget)
{
    ei_rect_t* newscreen=malloc(sizeof (ei_rect_t));
    int res=(int)((float)*widget->geom_params->height + *(widget->geom_params->rel_height) * (float)(widget->parent->screen_location.size.height));
    newscreen->size.height =res ;
    int res2=(int)((float)*widget->geom_params->width + *(widget->geom_params->rel_width) * (float)(widget->parent->screen_location.size.width));
    newscreen->size.width = res2;
    int x =(int)(widget->parent->screen_location.top_left.x+*widget->geom_params->x+(int) ((float)(newscreen->size.width)**(widget->geom_params->rel_x)));
    int y =(int)(widget->parent->screen_location.top_left.y+*widget->geom_params->y+(int) ((float)(newscreen->size.height)**(widget->geom_params->rel_y)));

    if (*widget->geom_params->anchor==ei_anc_northwest)
    {
        newscreen->top_left.x = x;
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_north)
    {
        newscreen->top_left.x = x - (int) (widget->screen_location.size.width/2);
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_northeast)
    {
        newscreen->top_left.x = x - (int) (widget->screen_location.size.width);
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_west)
    {
        newscreen->top_left.x = x;
        newscreen->top_left.y = y- (int) (widget->screen_location.size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_center)
    {
        newscreen->top_left.x = x - (int) (widget->screen_location.size.width/2);
        newscreen->top_left.y = y- (int) (widget->screen_location.size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_east)
    {
        newscreen->top_left.x = x - (int) (widget->screen_location.size.width);
        newscreen->top_left.y = y- (int) (widget->screen_location.size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_southwest)
    {
        newscreen->top_left.x = x ;
        newscreen->top_left.y = y- (int) (widget->screen_location.size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_south)
    {
        newscreen->top_left.x = x - (int) (widget->screen_location.size.width/2);
        newscreen->top_left.y = y- (int) (widget->screen_location.size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_southeast)
    {
        newscreen->top_left.x = x - (int) (widget->screen_location.size.width);
        newscreen->top_left.y = y- (int) (widget->screen_location.size.height);
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
ei_rect_t* intersection_rectangle(ei_rect_t rect1 , ei_rect_t rect2)
{

    ei_rect_t* intersect= malloc(sizeof (ei_rect_t));
    ei_rect_t grandy;
    ei_rect_t grandx;
    ei_rect_t petity;
    ei_rect_t petitx;
    if (rect1.top_left.x<rect2.top_left.x)
    {
        petitx=rect1;
        grandx=rect2;
    }
    else
    {
        petitx=rect2;
        grandx=rect1;
    }
    if (rect1.top_left.y<rect2.top_left.y)
    {
        petity=rect1;
        grandy=rect2;
    }
    else
    {
        petity=rect2;
        grandy=rect1;
    }
    if ((grandx.top_left.x-petitx.top_left.x>petitx.size.width)||(grandy.top_left.y-petity.top_left.y>petitx.size.height))
    {
        intersect->top_left.x=0;
        intersect->top_left.y=0;
        intersect->size.height=0;
        intersect->size.width=0;
    }
    else
    {

        intersect->top_left.x=(int) fmax((double)rect1.top_left.x,(double)rect2.top_left.x);
        intersect->top_left.y=(int) fmax((double)rect1.top_left.y,(double)rect2.top_left.y);
        intersect->size.height=(int) fmin((double)grandy.size.height,petity.size.height-(grandy.top_left.y-petity.top_left.y));
        intersect->size.width=(int) fmin((double)grandx.size.width,petitx.size.width-(grandx.top_left.x-petitx.top_left.x));
    }
    return intersect;
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
                clipper=&actuel->parent->screen_location;
            }
            else
            {
                clipper=intersection_rectangle(*clipper,actuel->parent->screen_location);
            }
            ei_impl_widget_draw_children(actuel,surface,pick_surface,clipper);
            actuel=actuel->next_sibling;
        }
    }
}

