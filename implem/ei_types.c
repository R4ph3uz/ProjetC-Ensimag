#include "ei_geometrymanager.h"
#include "ei_widgetclass.h"

/*-------------------------------------------------------------------------------------------------------*/

struct ei_impl_widget_t {
    ei_widgetclass_t* wclass;

    ei_widget_t* parent;
    ei_widget_t* children_head;
};

/*-------------------------------------------------------------------------------------------------------*/