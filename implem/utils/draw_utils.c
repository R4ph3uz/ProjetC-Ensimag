#include "draw_utils.h"
#include <math.h>
#include <stdlib.h>
#include "ei_draw.h"
#include "ei_utils.h"
#include "ei_types.h"
#include <string.h>

/*------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* polygon_arc(ei_point_t centre,
                        uint32_t rayon,
                        uint32_t angle_debut,
                        uint32_t angle_fin,
                        size_t* size_tableau)
{
    if(rayon == 0){
        *size_tableau = 1;
        ei_point_t* points = SAFE_MALLOC(sizeof(ei_point_t ));
        points[0] = centre;
        return points;
    }
    // Convert angles to radians
    double start_rad = angle_debut * M_PI / 180.0;
    double end_rad = angle_fin * M_PI / 180.0;

    // Calculate the number of points
    *size_tableau = (uint32_t)ceil(fabs(end_rad - start_rad) / (2 * M_PI / 360) +1);

    // Allocate memory for the points
    ei_point_t* points = SAFE_MALLOC(*size_tableau * sizeof(ei_point_t));
    if (!points) {
        return NULL;
    }

    // Generate the points
    for (uint32_t i = 0; i < *size_tableau ; i++) {
        double angle = start_rad + (end_rad - start_rad) * i / (*size_tableau - 2);
        points[i].x = centre.x + rayon * cos(angle);
        points[i].y = centre.y + rayon * sin(angle);
    }

    return points;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* rounded_frame(ei_rect_t* rectangle,
                            uint32_t rayon,
                            rounded_frame_part part,
                            size_t* nb_concat
                            )
{
    ei_point_t* points, *points1 , *points2 ,*points3;

    ei_point_t* conc1 , *conc2 , *conc3 ;

    size_t nb_points1, nb_points2, nb_points3, nb_points;

    ei_point_t top_left, top_right, bottom_right, bottom_left;
    top_left= ei_point_add(rectangle->top_left, ei_point(rayon,rayon));
    top_right = ei_point(rectangle->top_left.x+rectangle->size.width-rayon, rectangle->top_left.y+rayon);
    bottom_left = ei_point(rectangle->top_left.x+rayon, rectangle->top_left.y+rectangle->size.height-rayon);
    bottom_right= ei_point(rectangle->top_left.x+rectangle->size.width-rayon, rectangle->top_left.y+rectangle->size.height-rayon);



    if(part==full) {
        points1 = polygon_arc(top_left, rayon, 180, 270, &nb_points1);
        points2 = polygon_arc(bottom_right, rayon, 0, 90, &nb_points2);
        points3 = polygon_arc(bottom_left, rayon, 90, 180, &nb_points3);
        points = polygon_arc(top_right, rayon, 270, 360, &nb_points);
        conc1 = concatene_points(points1, points, nb_points1, nb_points);

        conc2 = concatene_points(conc1, points2, nb_points1+nb_points, nb_points2);
        conc3 = concatene_points(conc2, points3, nb_points1+nb_points+ nb_points2, nb_points3);
        *nb_concat = nb_points1+nb_points+ nb_points2 + nb_points3;
        free(points1);
        free(points2);
        free(points3);
        free(points);
        free(conc1);
        free(conc2);
        return conc3;
    }
    else if(part==high){
        points3 = polygon_arc(bottom_left, rayon, 135, 180, &nb_points3);
        points1 = polygon_arc(top_left, rayon, 180, 270, &nb_points1);
        points = polygon_arc(top_right, rayon, 270, 315, &nb_points);
        int h;

        h= (rectangle->size.width > rectangle->size.height ) ? rectangle->size.height /2 : rectangle->size.width/2;
        *nb_concat = nb_points1+nb_points+nb_points3+2;
        conc1 = concatene_points(points3, points1, nb_points3, nb_points1);
        conc2 = concatene_points(conc1, points, nb_points1+nb_points3, nb_points);
        ei_point_t* intermediaire = malloc(sizeof(ei_point_t)*2);
        intermediaire[1] = (ei_point_t){rectangle->top_left.x+h,rectangle->top_left.y+h};
        intermediaire[0] = (ei_point_t){rectangle->top_left.x-h +rectangle->size.width ,rectangle->top_left.y-h+rectangle->size.height};


        conc3 = concatene_points(conc2,intermediaire, nb_points1+nb_points+nb_points3, 2);
        free(points1);
        free(points3);
        free(points);
        free(conc1);
        free(conc2);
        free(intermediaire);
        return conc3;
    }
    else{
        points = polygon_arc(top_right, rayon, 315, 360, &nb_points);
        points2 = polygon_arc(bottom_right, rayon, 0, 90, &nb_points2);
        points3 = polygon_arc(bottom_left, rayon, 90, 135, &nb_points3);

        conc1 = concatene_points(points, points2, nb_points, nb_points2);
        conc2 = concatene_points(conc1, points3, nb_points2+nb_points, nb_points3);
        *nb_concat = nb_points+ nb_points2 + nb_points3+2;

        int h;

        h= (rectangle->size.width > rectangle->size.height ) ? rectangle->size.height /2 : rectangle->size.width/2;

        ei_point_t* intermediaire = SAFE_MALLOC(sizeof(ei_point_t)*2);
        intermediaire[0] = (ei_point_t){rectangle->top_left.x+h,rectangle->top_left.y+h};
        intermediaire[1] = (ei_point_t){rectangle->top_left.x-h +rectangle->size.width ,rectangle->top_left.y-h+rectangle->size.height};

        conc3 = concatene_points(conc2,intermediaire, nb_points+nb_points3+nb_points2, 2);
        free(points);
        free(points2);
        free(points3);
        free(conc1);
        free(conc2);
        free(intermediaire);
        return conc3;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------*/

void draw_button(ei_surface_t surface, ei_rect_t rectangle,int radius ,ei_color_t color,ei_relief_t  relief, ei_rect_t* clipper){
    ei_point_t* conc1 = NULL;
    ei_point_t* conc2 = NULL;
    ei_point_t* conc3 = NULL;

    size_t nb_points1;
    size_t nb_concat;
    ei_color_t color_plus_fonce;
    ei_color_t color_plus_clair;

    ei_rect_t nouveau_rect = rectangle;


    conc3 = rounded_frame(&nouveau_rect, radius, low, &nb_concat);
    conc2 = rounded_frame(&nouveau_rect, radius, high, &nb_concat);
    nouveau_rect.size.width -= nouveau_rect.size.height/10;
    nouveau_rect.size.height -= nouveau_rect.size.height/10;
    nouveau_rect.top_left.x += nouveau_rect.size.height/20;
    nouveau_rect.top_left.y += nouveau_rect.size.height/20;
    conc1 = rounded_frame(&nouveau_rect, radius, full, &nb_points1);

    color_plus_fonce.red = color.red-10;
    color_plus_fonce.green = color.green-10;
    color_plus_fonce.blue =color.blue-10;
    color_plus_fonce.alpha = color.alpha;

    color_plus_clair.red = color.red+10;
    color_plus_clair.green = color.green+10;
    color_plus_clair.blue =color.blue+10;
    color_plus_clair.alpha = color.alpha;

    if(relief == ei_relief_raised){
        ei_draw_polygon(surface, conc3, nb_concat, color_plus_fonce, clipper);
        ei_draw_polygon(surface, conc2, nb_concat, color_plus_clair, clipper);
    }
    else if (relief == ei_relief_sunken){
        ei_draw_polygon(surface, conc3, nb_concat, color_plus_clair, clipper);
        ei_draw_polygon(surface, conc2, nb_concat, color_plus_fonce, clipper);
    }
    else if(relief== ei_relief_none){
        conc1 = rounded_frame(&rectangle, radius, full, &nb_points1);
    }

    ei_draw_polygon(surface, conc1, nb_points1, color, clipper);
    free(conc1);
    free(conc2);
    free(conc3);
}

/*------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* concatene_points(ei_point_t* points1, ei_point_t* points2, size_t size1, size_t size2){
    ei_point_t *res = SAFE_MALLOC(sizeof(ei_point_t)*(size1+size2));
    memcpy(res, points1,sizeof(ei_point_t)*size1);
    memcpy(&res[size1], points2,sizeof(ei_point_t)*size2);
    return res;

}

/*--------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* demi_rounded_frame(ei_rect_t* rectangle,
                            uint32_t rayon,
                            bool top,
                            size_t* nb_concat
                            )
{
    ei_point_t* points, *points1 , *points2 ,*points3;

    ei_point_t* conc1 , *conc2 , *conc3 ;

    size_t nb_points1, nb_points2, nb_points3, nb_points;

    ei_point_t top_left, top_right, bottom_right, bottom_left;
    top_left= rectangle->top_left;
    top_right = (ei_point_t){top_left.x+rectangle->size.width, top_left.y};
    bottom_left = (ei_point_t){top_left.x, top_left.y+rectangle->size.height};
    bottom_right= (ei_point_t){top_right.x, bottom_left.y};

    ei_point_t top_left_corrected = {top_left.x +rayon, top_left.y+rayon};
    ei_point_t top_right_corrected = (ei_point_t){top_left.x+rectangle->size.width-rayon, top_left.y+rayon};


    points1 = polygon_arc(top_left_corrected, rayon, 180, 270, &nb_points1);
    points = polygon_arc(top_right_corrected, rayon, 270, 360, &nb_points);
    conc1 = concatene_points(points1, points, nb_points1, nb_points);

    conc2 = concatene_points(conc1, &bottom_right, nb_points1+nb_points, 1);
    conc3 = concatene_points(conc2, &bottom_left, nb_points1+nb_points+ 1, 1);
    *nb_concat = nb_points1+nb_points + 2;
    free(conc1);
    free(conc2);
    free(points1);
    free(points);
    return conc3;

    return conc3;

}

/*--------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* circle(ei_point_t centre, int radius, size_t* size_tableau)
{
    // Calculate the number of points based on the radius
    double points_per_unit = 0.7; // Adjust this value to change the density of points
    *size_tableau = (size_t)(2 * M_PI * radius * points_per_unit);

    // Allocate memory for the points
    ei_point_t* points = SAFE_MALLOC(*size_tableau * sizeof(ei_point_t));
    if (!points) {
        return NULL;
    }

    // Calculate the angle between adjacent points
    double angle_step = 2 * M_PI / *size_tableau;

    // Generate the points
    for (size_t i = 0; i < *size_tableau; i++) {
        double angle = i * angle_step;
        points[i].x = centre.x + radius * cos(angle);
        points[i].y = centre.y + radius * sin(angle);
    }

    return points;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void draw_toplevel(ei_surface_t surface, ei_rect_t rectangle,int radius ,ei_color_t color, ei_rect_t* clipper, bool isPicking, ei_axis_set_t* resizable ) {
    ei_point_t* conc2 =  SAFE_MALLOC(sizeof(ei_point_t)*4);
;   ei_point_t* carre_bas_droite = SAFE_MALLOC(sizeof(ei_point_t)*4);
    ei_point_t* border_line = SAFE_MALLOC(sizeof(ei_point_t)*5);
    ei_point_t* conc3 = NULL;

    size_t nb_concat;
    ei_color_t color_plus_fonce;

    ei_rect_t nouveau_rect = rectangle;
    nouveau_rect.top_left.x= rectangle.top_left.x;
    nouveau_rect.top_left.y= rectangle.top_left.y-30;
    nouveau_rect.size.height = 30;
    nouveau_rect.size.width = rectangle.size.width-1;


    conc3 = demi_rounded_frame(&nouveau_rect, 20, true, &nb_concat);

    nouveau_rect.top_left.x= rectangle.top_left.x;
    nouveau_rect.top_left.y= rectangle.top_left.y;
    nouveau_rect.size.height = rectangle.size.height ;
    nouveau_rect.size.width = rectangle.size.width;

    conc2[0] = nouveau_rect.top_left;
    conc2[1]= (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-1, nouveau_rect.top_left.y };
    conc2[2] = (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-1, nouveau_rect.top_left.y + nouveau_rect.size.height-1};
    conc2[3]= (ei_point_t){nouveau_rect.top_left.x, nouveau_rect.top_left.y + nouveau_rect.size.height-1 };

    border_line[0] = nouveau_rect.top_left;
    border_line[1]= (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-1, nouveau_rect.top_left.y };
    border_line[2] = (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-1, nouveau_rect.top_left.y + nouveau_rect.size.height-1};
    border_line[3]= (ei_point_t){nouveau_rect.top_left.x, nouveau_rect.top_left.y + nouveau_rect.size.height-1 };
    border_line[4] = nouveau_rect.top_left;

    color_plus_fonce.red = 87;
    color_plus_fonce.green = 93;
    color_plus_fonce.blue = 100;
    color_plus_fonce.alpha = 255;

    carre_bas_droite[0] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-1,nouveau_rect.top_left.y + nouveau_rect.size.height-1 };
    carre_bas_droite[1] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-10,nouveau_rect.top_left.y + nouveau_rect.size.height-1 };
    carre_bas_droite[2] = (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-10, nouveau_rect.top_left.y + nouveau_rect.size.height-10};
    carre_bas_droite[3] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-1,nouveau_rect.top_left.y + nouveau_rect.size.height-10 };

    size_t nb_circle;
    ei_point_t* circle_p = circle((ei_point_t) {rectangle.top_left.x+12, rectangle.top_left.y-10}, 6, &nb_circle);

    ei_color_t red;
    red.red = 230;
    red.green = 40;
    red.blue = 40;
    red.alpha = 255;



    if (isPicking) {
        ei_draw_polygon(surface, conc2, 4, color, clipper);
        ei_draw_polygon(surface, conc3, nb_concat, color, clipper);
    }
    else {
        ei_draw_polygon(surface, conc3, nb_concat, color_plus_fonce, clipper);
        ei_draw_polygon(surface, conc2, 4, color, clipper);
        ei_draw_polyline(surface, border_line, 5, color_plus_fonce,clipper);
        ei_draw_polygon(surface, circle_p, nb_circle, red, clipper);
        if (*resizable!=ei_axis_none)
        {
            ei_draw_polygon(surface, carre_bas_droite, 4, color_plus_fonce, clipper);
        }
    }

    free(circle_p);
    free(conc2);
    free(carre_bas_droite);
    free(conc3);
}

