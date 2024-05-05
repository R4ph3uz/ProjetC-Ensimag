#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "ei_types.h"
#include "hw_interface.h"


typedef enum rounded_frame_part {
    full,
    low,
    high
} rounded_frame_part;

/**
 * \brief	Create a tab of points representing an arc
 *
 * @param	centre	Centre de l'arc
 * @param   rayon   Rayon de l'arc
 * @param	angle_debut		angle du début
 * @param	angle_fin		angle de la fin
 * @param	size_tableau taille du tableau de fin (pour connaitre la taille du tableau après avoir appelé la fonction)
 *
*/
ei_point_t* polygon_arc(ei_point_t centre,
                        uint32_t rayon,
                        uint32_t angle_debut,
                        uint32_t angle_fin,
                        size_t* size_tableau);



/**
 * \brief	crée un tableau de points définissant un cadre aux bords arrondis
 *
 * @param	rectangle	Centre de l'arc
 * @param	rayon		rayon de l'arrondi
 * @param	part	    génère uniquement la partie haute, ou basse, ou bien la totalité de la forme
 * @param nb_concat met a *nb_concat la taille du tableau renvoyé
 *
 */
ei_point_t* rounded_frame(ei_rect_t* rectangle,
                            uint32_t rayon,
                            rounded_frame_part part,
                            size_t* nb_concat
                            );

/**
 * \brief	crée un tableau de points en ajoutant les points de points1 a points2, alloc un nouveau tableau de size1 + size2
 *
 * @param	points1 tableau de point 1
 * @param	points2		tableau de point 2
 * @param	size1	    taille de tableau point1
 * @param   size2   taille de tableau point2
 *
 */
ei_point_t* concatene_points(ei_point_t* points1, ei_point_t* points2, size_t size1, size_t size2);

/**
 * \brief	dessine un bouton (change selon le relief, la taille du rectangle et sa place)
 *          Surface must be lock before calling the function
 *
 * @param	surface surface sur laquelle dessiner
 * @param	rectangle	rectangle dans lequel inclure le bouton
 * @param radius rayon de l'arrondi
 * @param	color	    couleur du bouton
 * @param   relief  relief du bouton
 * @param   clipper clipper
 */
void draw_button(ei_surface_t surface, ei_rect_t rectangle,int radius,ei_color_t color,ei_relief_t  relief, ei_rect_t* clipper);



/**
* \brief	crée un tableau de points définissant un cadre aux bords arrondis
*
* @param	rectangle	Centre de l'arc
* @param	rayon		rayon de l'arrondi
* @param	top	    les bords sont rounded en haut si true, en bas si false
* @param nb_concat met a *nb_concat la taille du tableau renvoyé
*
*/
ei_point_t* demi_rounded_frame(ei_rect_t* rectangle,
                            uint32_t rayon,
                            bool top,
                            size_t* nb_concat
                            );


/**
 * \brief	dessine une toplevel (change selon la taille du rectangle et sa place)
 *          Surface must be lock before calling the function
 *
 * @param	surface surface sur laquelle dessiner
 * @param	rectangle	rectangle dans lequel inclure le bouton
 * @param	color	    couleur du bouton
 * @param radius rayon de l'arrondi
 * @param   clipper clipper
 */
void draw_toplevel(ei_surface_t surface, ei_rect_t rectangle,int radius ,ei_color_t color, ei_rect_t* clipper, bool isPicking);

#endif //DRAW_UTILS_H
