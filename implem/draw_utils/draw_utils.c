#include "draw_utils.h"
#include <math.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------*/

ei_point_t* polygon_arc(ei_point_t centre,
                        uint32_t rayon,
                        uint8_t angle_debut,
                        uint8_t angle_fin,
                        uint32_t* size_tableau)
{
    // Convert angles to radians
    double start_rad = angle_debut * M_PI / 180.0;
    double end_rad = angle_fin * M_PI / 180.0;

    // Calculate the number of points
    *size_tableau = (uint32_t)ceil(fabs(end_rad - start_rad) / (2 * M_PI / 360) + 2);

    // Allocate memory for the points
    ei_point_t* points = malloc(*size_tableau * sizeof(ei_point_t));
    if (!points) {
        return NULL;
    }

    // Generate the points
    for (uint32_t i = 0; i < *size_tableau - 1; i++) {
        double angle = start_rad + (end_rad - start_rad) * i / (*size_tableau - 2);
        points[i].x = centre.x + rayon * cos(angle);
        points[i].y = centre.y + rayon * sin(angle);
    }

    // Add the center point to the end of the list
    points[*size_tableau - 1] = centre;

    return points;
}

/*----------------------------------------------------------------------------*/

ei_point_t* rounded_frame(ei_rect_t* rectangle,
                            uint32_t rayon,
                            const rounded_frame_part part
                            )
{
    if(part==full) {

    }
}

/*----------------------------------------------------------------------------*/