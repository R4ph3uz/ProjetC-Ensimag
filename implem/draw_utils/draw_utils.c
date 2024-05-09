#include "draw_utils.h"
#include <math.h>
#include <stdlib.h>
#include "ei_draw.h"
#include "ei_utils.h"
#include "ei_types.h"

/*------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* polygon_arc(ei_point_t centre,
                        uint32_t rayon,
                        uint32_t angle_debut,
                        uint32_t angle_fin,
                        size_t* size_tableau)
{
    // Convert angles to radians
    double start_rad = angle_debut * M_PI / 180.0;
    double end_rad = angle_fin * M_PI / 180.0;

    // Calculate the number of points
    *size_tableau = (uint32_t)ceil(fabs(end_rad - start_rad) / (2 * M_PI / 360) +1);

    // Allocate memory for the points
    ei_point_t* points = malloc(*size_tableau * sizeof(ei_point_t));
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
        return conc3;
    }
    else if(part==high){
        points1 = polygon_arc(top_left, rayon, 225, 270, &nb_points1);
        points = polygon_arc(top_right, rayon, 270, 360, &nb_points);
        points2 = polygon_arc(bottom_right, rayon, 0, 45, &nb_points2);
        int h;

        h= (rectangle->size.width > rectangle->size.height ) ? rectangle->size.height /2 : rectangle->size.width/2;
        *nb_concat = nb_points1+nb_points+nb_points2+2;
        conc1 = concatene_points(points1, points, nb_points1, nb_points);
        conc2 = concatene_points(conc1, points2, nb_points1+nb_points, nb_points2);
        ei_point_t* intermediare = malloc(sizeof(ei_point_t)*2);
        intermediare[1] = (ei_point_t){rectangle->top_left.x+h,rectangle->top_left.y+h};
        intermediare[0] = (ei_point_t){rectangle->top_left.x-h +rectangle->size.width ,rectangle->top_left.y-h+rectangle->size.height};


        conc3 = concatene_points(conc2,intermediare, nb_points1+nb_points+nb_points2, 2);

        return conc3;
    }
    else{
        points1 = polygon_arc(top_left, rayon, 180, 225, &nb_points1);
        points2 = polygon_arc(bottom_right, rayon, 45, 90, &nb_points2);
        points3 = polygon_arc(bottom_left, rayon, 90, 180, &nb_points3);
        conc1 = concatene_points(points2, points3, nb_points2, nb_points3);
        conc2 = concatene_points(conc1, points1, nb_points2+nb_points3, nb_points1);
        *nb_concat = nb_points1+ nb_points2 + nb_points3+2;

        int h;

        h= (rectangle->size.width > rectangle->size.height ) ? rectangle->size.height /2 : rectangle->size.width/2;

        ei_point_t* intermediare = malloc(sizeof(ei_point_t)*2);
        intermediare[0] = (ei_point_t){rectangle->top_left.x+h,rectangle->top_left.y+h};
        intermediare[1] = (ei_point_t){rectangle->top_left.x-h +rectangle->size.width ,rectangle->top_left.y-h+rectangle->size.height};


        conc3 = concatene_points(conc2,intermediare, nb_points1+nb_points3+nb_points2, 2);

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
    nouveau_rect.top_left.x= rectangle.top_left.x+radius;
    nouveau_rect.top_left.y= rectangle.top_left.y+radius;
    nouveau_rect.size.height = rectangle.size.height-2*radius;
    nouveau_rect.size.width = rectangle.size.width-2*radius;


    conc3 = rounded_frame(&nouveau_rect, 20, low, &nb_concat);
    conc2 = rounded_frame(&nouveau_rect, 20, high, &nb_concat);
    nouveau_rect.size.width -= nouveau_rect.size.height/10;
    nouveau_rect.size.height -= nouveau_rect.size.height/10;
    nouveau_rect.top_left.x += nouveau_rect.size.height/20;
    nouveau_rect.top_left.y += nouveau_rect.size.height/20;
    conc1 = rounded_frame(&nouveau_rect, 20, full, &nb_points1);

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


    ei_draw_polygon(surface, conc1, nb_points1, color, clipper);
}

/*------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* concatene_points(ei_point_t* points1, ei_point_t* points2, size_t size1, size_t size2){
    ei_point_t *res = malloc(sizeof(ei_point_t)*(size1+size2));
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

    ei_point_t top_left_corrected = {top_left.x -rayon, top_left.y-rayon};
    ei_point_t top_right_corrected = (ei_point_t){top_left.x+rectangle->size.width+rayon, top_left.y-rayon};
    ei_point_t bottom_left_corrected = (ei_point_t){top_left.x-rayon, top_left.y+rectangle->size.height+rayon};
    ei_point_t bottom_right_corrected = (ei_point_t){top_right.x+rayon, bottom_left.y+rayon};


    if(top) {
        points1 = polygon_arc(top_left, rayon, 180, 270, &nb_points1);
        points = polygon_arc(top_right, rayon, 270, 360, &nb_points);
        conc1 = concatene_points(points1, points, nb_points1, nb_points);

        conc2 = concatene_points(conc1, &bottom_right_corrected, nb_points1+nb_points, 1);
        conc3 = concatene_points(conc2, &bottom_left_corrected, nb_points1+nb_points+ 1, 1);
        *nb_concat = nb_points1+nb_points + 2;
        return conc3;
    }

    points2 = polygon_arc(bottom_right, rayon, 0, 90, &nb_points2);
    points3 = polygon_arc(bottom_left, rayon, 90, 180, &nb_points3);
    conc1 = concatene_points(points2, points3, nb_points2, nb_points3);

    conc2 = concatene_points(conc1, &top_left_corrected, nb_points2+nb_points3, 1);
    conc3 = concatene_points(conc2, &top_right_corrected, nb_points2+nb_points3+ 1, 1);
    *nb_concat = nb_points2+nb_points3 + 2;
    return conc3;

}

/*--------------------------------------------------------------------------------------------------------------------------*/

ei_point_t* circle(ei_point_t centre, int radius, size_t* size_tableau)
{
    // Calculate the number of points based on the radius
    double points_per_unit = 0.7; // Adjust this value to change the density of points
    *size_tableau = (size_t)(2 * M_PI * radius * points_per_unit);

    // Allocate memory for the points
    ei_point_t* points = malloc(*size_tableau * sizeof(ei_point_t));
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
    ei_point_t* conc2 =  malloc(sizeof(ei_point_t)*4);
;   ei_point_t* carre_bas_droite = malloc(sizeof(ei_point_t)*4);
    ei_point_t* conc3 = NULL;

    size_t nb_concat;
    ei_color_t color_plus_fonce;

    ei_rect_t nouveau_rect = rectangle;
    nouveau_rect.top_left.x= rectangle.top_left.x+radius;
    nouveau_rect.top_left.y= rectangle.top_left.y+radius-20;
    nouveau_rect.size.height = -10;
    nouveau_rect.size.width = rectangle.size.width-2*radius;


    conc3 = demi_rounded_frame(&nouveau_rect, 20, true, &nb_concat);

    nouveau_rect.top_left.x= rectangle.top_left.x-radius;
    nouveau_rect.top_left.y= rectangle.top_left.y;
    nouveau_rect.size.height = rectangle.size.height ;
    nouveau_rect.size.width = rectangle.size.width+ 2*radius;

    conc2[0] = nouveau_rect.top_left;
    conc2[1]= (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width, nouveau_rect.top_left.y };
    conc2[2] = (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width, nouveau_rect.top_left.y + nouveau_rect.size.height};
    conc2[3]= (ei_point_t){nouveau_rect.top_left.x, nouveau_rect.top_left.y + nouveau_rect.size.height };

    color_plus_fonce.red = 40;
    color_plus_fonce.green = 40;
    color_plus_fonce.blue = 40;
    color_plus_fonce.alpha = 255;

    carre_bas_droite[0] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width,nouveau_rect.top_left.y + nouveau_rect.size.height };
    carre_bas_droite[1] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-10,nouveau_rect.top_left.y + nouveau_rect.size.height };
    carre_bas_droite[2] = (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-10, nouveau_rect.top_left.y + nouveau_rect.size.height-10};
    carre_bas_droite[3] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width,nouveau_rect.top_left.y + nouveau_rect.size.height-10 };

    size_t nb_circle;
    ei_point_t* circle_p = circle((ei_point_t) {rectangle.top_left.x+5, rectangle.top_left.y-15}, 6, &nb_circle);

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
        ei_draw_polyline(surface, conc2, 4, color_plus_fonce,clipper);
        ei_draw_polygon(surface, circle_p, nb_circle, red, clipper);
        if (*resizable!=ei_axis_none)
        {
            ei_draw_polygon(surface, carre_bas_droite, 4, color_plus_fonce, clipper);
        }
    }



}

/*--------------------------------------------------------------------------------------------------------------------------*/

char* restrict_text(char* text, uint8_t taille) {
    char* res = malloc(sizeof(char)*(strlen(text)-taille) ) ;
    for(int i = 0 ; i< strlen(text)-taille ; i++)
        res[i] = text[i];
    res[strlen(text)-taille] = '\0';
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/
char* insert_char(char* text, char character, uint8_t where) {
    int taille = strlen(text)+1;
    char* res = malloc(sizeof(char)*(taille+1));

    for(int i = 0 ; i <where; i++)
        res[i] = text[i];
    res[where] = character;
    for(int i = where+1; i< taille;i++)
        res[i] = text[i-1];
    res[taille] = '\0';

    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

char * delete_char(char *text, uint8_t where) {
    int taille = strlen(text)-1;
    if(where >0) {
        char* res =malloc(sizeof(char)*(taille+1));

        for(int i = 0 ; i <where-1; i++)
            res[i] = text[i];
        for(int i = where-1; i< taille;i++)
            res[i] = text[i+1];
        res[taille] = '\0';

        return res;
    }
    return text;


}

/*--------------------------------------------------------------------------------------------------------------------------*/
