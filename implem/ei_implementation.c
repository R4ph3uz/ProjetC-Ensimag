#include "ei_implementation.h"
#include "hw_interface.h"
#include <stdlib.h>

/*------------------------------------------------------------------------------------*/


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

