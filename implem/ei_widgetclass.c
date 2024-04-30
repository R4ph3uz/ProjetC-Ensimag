#include "ei_geometrymanager.h"


/*-------------------------------------------------------------------------------------------------------*/
static ei_widgetclass_t* LISTE_WIDGETCLASS= NULL;

void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass)
{
    if(LISTE_WIDGETCLASS==NULL)
    {
        LISTE_WIDGETCLASS=widgetclass;
    }
    else
    {
        widgetclass->next=LISTE_WIDGETCLASS;
        LISTE_WIDGETCLASS=widgetclass;
    }
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name)
{
    ei_widgetclass_t* actual = LISTE_WIDGETCLASS;
    while(actual != NULL)
    {
        if (strcmp(actual->name, name)==0)
        {
            return actual;
        }
        actual=actual->next;
    }
    return NULL;
}

/*-------------------------------------------------------------------------------------------------------*/

