#include "ei_draw.h"
#include "ei_implementation.h"
#include <string.h>
#include "ei_utils.h"

/*------------------------------------------------------------------------------*/
int max(int a, int b) {
    return (a > b) ? a : b;
}
/*------------------------------------------------------------------------------*/

void	ei_draw_text		(ei_surface_t		surface,
                             const ei_point_t*	where,
                             ei_const_string_t	text,
                             ei_font_t		font,
                             ei_color_t		color,
                             const ei_rect_t*	clipper)
{
    ei_surface_t surface_text = hw_text_create_surface(text, font, color);

    ei_rect_t rect_surface_text = hw_surface_get_rect(surface_text);


    if (where)
        rect_surface_text.top_left = *where;

    if(clipper){
        ei_rect_t* intersection = intersection_rectangle(rect_surface_text, *clipper);

        if (intersection){
            int point_debut_x = intersection->top_left.x != clipper->top_left.x ? 0: rect_surface_text.size.width-intersection->size.width;
            int point_debut_y = intersection->top_left.y != clipper->top_left.y ? 0: rect_surface_text.size.height-intersection->size.height;
            ei_rect_t intersection_for_text = ei_rect(
                    ei_point(point_debut_x,point_debut_y),
                    intersection->size);
            hw_surface_lock(surface_text);
            ei_copy_surface(surface, intersection, surface_text, &intersection_for_text, true);
            hw_surface_unlock(surface_text);
        }
        free(surface_text);
        free(intersection);
    }
    else{
        hw_surface_lock(surface_text);
        ei_copy_surface(surface, &rect_surface_text, surface_text, NULL, true);
        hw_surface_unlock(surface_text);
    }




}

/*------------------------------------------------------------------------------*/

void	ei_fill			(ei_surface_t		surface,
                            const ei_color_t*	color,
                            const ei_rect_t*	clipper)
{

}

/*------------------------------------------------------------------------------*/

int	ei_copy_surface		(ei_surface_t		destination,
                               const ei_rect_t*	dst_rect,
                               ei_surface_t		source,
                               const ei_rect_t*	src_rect,
                               bool			alpha)
{
    ei_size_t size_src = src_rect ? src_rect->size : hw_surface_get_size(source);
    ei_size_t size_dest = dst_rect ? dst_rect->size: hw_surface_get_size(destination);

    if(memcmp(&size_src, &size_dest, sizeof(ei_size_t)) != 0 ){
        return 1; //tailles différentes on ne copie pas
    }


    ei_point_t top_left_dest = dst_rect ? dst_rect->top_left : (ei_point_t){0, 0};
    ei_point_t top_left_src = src_rect ? src_rect->top_left : (ei_point_t){0, 0 };

    uint32_t* buffer_dest = (uint32_t*)hw_surface_get_buffer(destination);
    uint32_t* buffer_src  = (uint32_t*)hw_surface_get_buffer(source);



    ei_size_t real_dest_size = hw_surface_get_size(destination);
    ei_size_t real_src_size = hw_surface_get_size(source);

    for(uint32_t i = 0; i< size_dest.height; i++)
        for(uint32_t j = 0; j < size_dest.width; j++) {

            uint32_t index_dest = (j+top_left_dest.x)+(i+top_left_dest.y)*real_dest_size.width;
            uint32_t index_src = (j+top_left_src.x)+(i+top_left_src.y)*real_src_size.width;

            if(alpha) {
                uint32_t source_pixel = buffer_src[index_src];
                uint32_t dest_pixel = buffer_dest[index_dest];

                int ir,ig,ib,ia;
                hw_surface_get_channel_indices( source, &ir, &ig, &ib, &ia);
                uint8_t src_color[4];
                // Décalage de bits et masquage pour extraire chaque octet
                src_color[0] = (uint8_t)( source_pixel & 0xFF);
                src_color[1] = (uint8_t)((source_pixel >> 8) & 0xFF);
                src_color[2] = (uint8_t)((source_pixel >> 16) & 0xFF);
                src_color[3] = (uint8_t)((source_pixel >> 24) & 0xFF);

                uint8_t dest_color[4];
                // Décalage de bits et masquage pour extraire chaque octet
                dest_color[0] = (uint8_t)( dest_pixel & 0xFF);
                dest_color[1] = (uint8_t)((dest_pixel >> 8) & 0xFF);
                dest_color[2] = (uint8_t)((dest_pixel >> 16) & 0xFF);
                dest_color[3] = (uint8_t)((dest_pixel >> 24) & 0xFF);

                /*
                dest_color[ir] = (dest_color[ir]*(255-src_color[ia]) + src_color[ir]*src_color[ia])/255;
                dest_color[ig] = (dest_color[ir]*(255-src_color[ia]) + src_color[ir]*src_color[ia])/255;
                dest_color[ib] = (dest_color[ir]*(255-src_color[ia]) + src_color[ir]*src_color[ia])/255;
                 */
                unsigned int alpha_val = src_color[ia];
                dest_color[ir] =  ((255-alpha_val) * dest_color[ir] + alpha_val * src_color[ir])/255;
                dest_color[ig] =  ((255-alpha_val) * dest_color[ig] + alpha_val * src_color[ig])/255;
                dest_color[ib] =  ((255-alpha_val) * dest_color[ib] + alpha_val * src_color[ib])/255;


                uint32_t value = ((uint32_t)dest_color[0]) |
                                 ((uint32_t)dest_color[1] << 8) |
                                 ((uint32_t)dest_color[2] << 16) |
                                 ((uint32_t)dest_color[3] << 24);

                buffer_dest[index_dest] = value;
            }
            else
                buffer_dest[index_dest] =  buffer_src[index_src];


        }




    return 0;
}


