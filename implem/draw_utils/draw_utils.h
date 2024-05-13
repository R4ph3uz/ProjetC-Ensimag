#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "ei_types.h"
#include "hw_interface.h"
#include "../widgetclass/ei_entry.h"

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
 * @param resizable resizable
 */
void draw_toplevel(ei_surface_t surface, ei_rect_t rectangle,int radius ,ei_color_t color, ei_rect_t* clipper, bool isPicking, ei_axis_set_t* resizable);

/**
 * \brief Generates points lying on the circumference of a circle given its center coordinates and radius.
 *
 * @param centre The center point of the circle.
 * @param radius The radius of the circle.
 * @param size_tableau A pointer to a size_t variable that will store the size of the array of points generated by the function.
 * @return A pointer to an array of ei_point_t structures representing the points lying on the circumference of the circle. Returns NULL if memory allocation fails.
 */
ei_point_t* circle(ei_point_t centre, int radius, size_t* size_tableau);


/**
 * @brief Coupe la fin d'un texte
 * @param text texte a restreindre
 * @param taille taille a restreindre, doit être supérieur a 0 et inférieur a la taille (0 renvoie text)
 * @return
 */
char* restrict_text(char* text, uint8_t taille);

/**
 * @brief
 * @param text texte auquel on ajoute le charactère, malloc un nouveau text de la bonne taille
 * @param character charactere à insérer dans le texte
 * @param where indice (en partant du debut ou ajouter le caractère
 * @return
 */
char* insert_char(char* text, char character, uint8_t where);

/**
 * @brief
 * @param text texte auquel supprimer le caractère
 * @param where index du caractère à supprimer
 * @return
 */
char* delete_char(char* text, uint8_t where);

/**
 * @brief prend une entrée et une position ou a eu lieu un click pour replacer le curseur
 * @param entry widget entry ou a eu lieu le click pour changer la position du curseur
 * @param position point ou le click a eu lieu
 * @return
 */
int find_position_cursor_entry(ei_entry_t entry,ei_point_t position );

/**
 * @brief prend une entrée et une largeur pour dessiner le selection
 * @param entry widget entry ou a eu lieu le click pour changer la position du curseur
 * @param position point ou le click a eu lieu
 * @return
 */
int find_selection_entry(ei_entry_t entry, ei_point_t position);

/**
 * @brief coupe une partie du  texte de debut a fin non inclus (prend des indices en paramatres)
 * @param text texte a couper
 * @param debut point ou le click a eu lieu
 * @param fin fin non inclus
 * @return
 */
char* cut_text(char* text, uint8_t debut, uint8_t fin);


int find_position_cursor_selection_entry(ei_entry_t entry, ei_point_t position);

#endif //DRAW_UTILS_H
