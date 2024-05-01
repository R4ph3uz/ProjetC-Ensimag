#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "ei_types.h"


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
                        uint8_t angle_debut,
                        uint8_t angle_fin,
                        uint32_t* size_tableau);



/**
 * \brief	crée un tableau de points définissant un cadre aux bords arrondis
 *
 * @param	rectangle	Centre de l'arc
 * @param	rayon		rayon de l'arrondi
 * @param	part	    génère uniquement la partie haute, ou basse, ou bien la totalité de la forme
 *
 */
ei_point_t* rounded_frame(ei_rect_t* rectangle,
                            uint32_t rayon,
                            const rounded_frame_part part
                            );

#endif //DRAW_UTILS_H
