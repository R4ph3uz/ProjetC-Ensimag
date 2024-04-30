#include "ei_implementation.h"
#include "hw_interface.h"

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