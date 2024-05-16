
#ifndef PROJETC_IG_GRID_H
#define PROJETC_IG_GRID_H
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>
/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 *
 *		The placer computes a widget's geometry relative to its parent *content_rect*.
 *
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 *		Arguments that are NULL don't change the current value of the parameter.
 *
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then either the requested size of the widget is used if one was provided,
 *		or the default size is used.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width or
 * 				the default width of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height or
 *				the default height of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the parent, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the parent, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the parent (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the parent (defaults to 0.0).
 */
void		grid	(ei_widget_t		widget,
                         ei_anchor_t*		anchor);

/*-------------------------------------------------------------------------------------------------------*/

/**
 * @brief	Crée un geometry manager de type placer
 */
        ei_geometrymanager_t* create_grid_gm(void);

/*-------------------------------------------------------------------------------------------------------*/

/**
 * @brief	unmap le widget et ses enfants pour le geometry manager grid
 *
 * @param	widget		Le widget a unmap
 */
void ei_grid_releasefunc(ei_widget_t widget);

/*-------------------------------------------------------------------------------------------------------*/

/**
 * @brief	A function that runs the geometry computation for this widget.
 * 		Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void ei_grid_runfunc(ei_widget_t widget);

/*-------------------------------------------------------------------------------------------------------*/











#endif //PROJETC_IG_GRID_H
