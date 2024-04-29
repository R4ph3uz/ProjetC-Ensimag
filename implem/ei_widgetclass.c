#include "ei_geometrymanager.h"

/*-------------------------------------------------------------------------------------------------------*/


size_t		ei_widget_struct_size()
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef char 	ei_widgetclass_name_t[20]
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef ei_widget_t (*ei_widgetclass_allocfunc_t)	()
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef void	(*ei_widgetclass_releasefunc_t)		(ei_widget_t	widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef void	(*ei_widgetclass_drawfunc_t)		(ei_widget_t		widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef void	(*ei_widgetclass_setdefaultsfunc_t)	(ei_widget_t		widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef void	(*ei_widgetclass_geomnotifyfunc_t)	(ei_widget_t		widget)
{

}

/*-------------------------------------------------------------------------------------------------------*/

typedef struct ei_widgetclass_t {
	ei_widgetclass_name_t			name;			///< The string name of this class of widget.
	ei_widgetclass_allocfunc_t		allocfunc;		///< The function that allocated instances of this class of widget.
	ei_widgetclass_releasefunc_t		releasefunc;		///< The function that releases all the resources used by an instance of this class of widget.
	ei_widgetclass_drawfunc_t		drawfunc;		///< The function that draws on screen an instance of this class of widget.
	ei_widgetclass_setdefaultsfunc_t	setdefaultsfunc;	///< The function that sets the default values to all the parameters of an instance of this class of widget.
	ei_widgetclass_geomnotifyfunc_t		geomnotifyfunc;		///< The function that is called to notify an instance of widget of this class that its geometry has changed.
	struct ei_widgetclass_t*		next;			///< A pointer to the next instance of ei_widget_class_t, allows widget class descriptions to be chained.
} ei_widgetclass_t
{

}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass)
{

}

/*-------------------------------------------------------------------------------------------------------*/

ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name)
{

}

/*-------------------------------------------------------------------------------------------------------*/

