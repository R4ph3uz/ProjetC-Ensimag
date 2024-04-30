#include "ei_draw.h"

/*------------------------------------------------------------------------------*/

void	ei_draw_text		(ei_surface_t		surface,
                             const ei_point_t*	where,
                             ei_const_string_t	text,
                             ei_font_t		font,
                             ei_color_t		color,
                             const ei_rect_t*	clipper)
{

}

/*------------------------------------------------------------------------------*/

void	ei_fill			(ei_surface_t		surface,
                            const ei_color_t*	color,
                            const ei_rect_t*	clipper)
{
    hw_surface_lock(surface);

    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, NULL);
}

/*------------------------------------------------------------------------------*/

int	ei_copy_surface		(ei_surface_t		destination,
                               const ei_rect_t*	dst_rect,
                               ei_surface_t		source,
                               const ei_rect_t*	src_rect,
                               bool			alpha)
{
    return 0;
}


