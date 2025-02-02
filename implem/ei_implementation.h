/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 * 
 */

#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H

#include <ei_event.h>
#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

//gère si les malloc fail
#define SAFE_MALLOC(size) \
    ({ \
        void *ptr = malloc(size); \
        if (!ptr) { \
            fprintf(stderr, "Memory allocation failed, exiting application.\n"); \
            exit(EXIT_FAILURE);\
        } \
        ptr; \
    })

//gère si les calloc fail
#define SAFE_CALLOC(size) \
    ({ \
        void *ptr = calloc(size,1); \
        if (!ptr) { \
            fprintf(stderr, "Memory allocation failed, exiting application.\n"); \
            exit(EXIT_FAILURE);\
        } \
        ptr; \
    })

#define SAFE_FREE(ptr) if (ptr) { free(ptr); ptr = NULL; }

/**
 * @brief	unmap le widget et ses enfants pour le geometry manager placer
 *
 * @param	widget		Le widget a unmap
 */
void ei_placer_releasefunc(ei_widget_t widget);


void ei_free_geometrymanager(ei_geometrymanager_t manager);
void ei_free_widgetclass(ei_widgetclass_t widclass);


/**
 * @brief	A function that runs the geometry computation for this widget.
 * 		Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void ei_placer_runfunc(ei_widget_t widget);

typedef struct list_widget_callback {
    ei_callback_t callback;
    ei_eventtype_t eventtype;
    void* user_param;
    struct list_widget_callback* next;
}list_widget_callback;

/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_impl_widget_t {
	ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
	uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
	ei_color_t*		pick_color;	///< pick_id encoded as a color.
	void*			user_data;	///< Pointer provided by the programmer for private use. May be NULL.
	ei_widget_destructor_t	destructor;	///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

	/* Widget Hierarchy Management */
	ei_widget_t		parent;		///< Pointer to the parent of this widget.
	ei_widget_t		children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	ei_widget_t		children_tail;	///< Pointer to the last child of this widget.
	ei_widget_t		next_sibling;	///< Pointer to the next child of this widget's parent widget.

	/* Geometry Management */
	ei_geom_param_t		geom_params;	///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	ei_size_t		requested_size;	///< See \ref ei_widget_get_requested_size.
	ei_rect_t		screen_location;///< See \ref ei_widget_get_screen_location.
	ei_rect_t*		content_rect;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.

	/* Callback Management */
    ei_callback_t callback; ///< if the widget have a callback just for him

    /*Champ*/
    bool isChildIgnoreAddInvalidateRect;

} ei_impl_widget_t;



/**
 * @brief	Draws the children of a widget.
 * 		The children are draw withing the limits of the clipper and
 * 		the widget's content_rect.
 *
 * @param	widget		The widget which children are drawn.
 * @param	surface		A locked surface where to draw the widget's childre n.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void		ei_impl_widget_draw_children	(ei_widget_t		widget,
						 ei_surface_t		surface,
						 ei_surface_t		pick_surface,
						 ei_rect_t*		clipper);




/************** **************/


/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */
uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);



/**
 * @brief	Calcule le rectangle d'union de deux rectangles
 *
 * @param	rect1	Premier rectangle
 * @param	rect2	Deuxième rectangle
 */
ei_rect_t* union_rectangle(ei_rect_t rect1, ei_rect_t rect2);




/**
 * \brief	Fields common to all geometry managers. Every geometry manager specializes this by adding its own fields.
 */
typedef struct ei_impl_geom_param_t {
	ei_geometrymanager_t*		manager;
	ei_anchor_t*		anchor;
	int*			x;
	int*			y;
	int*			width;
	int*			height;
	float*			rel_x;
	float*			rel_y;
	float*			rel_width;
	float*			rel_height;
    bool *           is_reconfigurable; //True si la width/height dépend de la requested size
} ei_impl_geom_param_t;




/**
 * \brief	Returns the pick surface
 */
ei_surface_t get_pick_surface(void);




/**
 * @brief	Calcule le rectangle d'intersection de deux rectangles
 *
 * @param	rect1	Premier rectangle
 * @param	rect2	Deuxième rectangle
 */
ei_rect_t* intersection_rectangle(ei_rect_t rect1 , ei_rect_t rect2);


/**
 * @brief	Crée un geometry manager de type placer
 */
ei_geometrymanager_t* create_placer_gm(void);


/**
 * @brief	Supprime le widget parmis ses frères et soeur
 *
 * @param	widget le widget
 */
void supprime_de_ses_freres(ei_widget_t widget);

/**
 * @brief	place le widget a la fin de la liste de frères
 *
 * @param	widget le widget
 */
void place_a_la_fin(ei_widget_t widget);


ei_rect_t* union_rectangles(const ei_linked_rect_t* tete);

#endif

