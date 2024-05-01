#include "ei_implementation.h"
#include "hw_interface.h"
#include <stdlib.h>

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
            ei_impl_widget_draw_children(actuel,surface,pick_surface,clipper);
            actuel=actuel->next_sibling;
        }
    }
}