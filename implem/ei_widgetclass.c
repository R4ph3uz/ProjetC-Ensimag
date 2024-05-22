#include "ei_geometrymanager.h"
#include "ei_implementation.h"

/*-------------------------------------------------------------------------------------------------------*/
static ei_widgetclass_t* LISTE_WIDGETCLASS= NULL;

void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass)
{
    if(LISTE_WIDGETCLASS==NULL) // Si il y a aune classe dans la liste
    {
        LISTE_WIDGETCLASS=widgetclass; // On l'ajoute
    }
    else //Sinon
    {
        widgetclass->next=LISTE_WIDGETCLASS;
        LISTE_WIDGETCLASS=widgetclass; //On ajoute la classe en tête de liste
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void			ei_free_widgetclass	(ei_widgetclass_t widgetclass)
{
    ei_widgetclass_t* first = LISTE_WIDGETCLASS;
    if(strcmp(LISTE_WIDGETCLASS->name, widgetclass.name)==0)
    {
        LISTE_WIDGETCLASS = LISTE_WIDGETCLASS->next;
        SAFE_FREE(first);
        return;
    }
    while(LISTE_WIDGETCLASS!=NULL) {
        ei_widgetclass_t* temp = LISTE_WIDGETCLASS;
        LISTE_WIDGETCLASS=LISTE_WIDGETCLASS->next;
        if(LISTE_WIDGETCLASS && strcmp(LISTE_WIDGETCLASS->name, widgetclass.name)==0){
            temp->next = LISTE_WIDGETCLASS->next;
            free(LISTE_WIDGETCLASS);
            LISTE_WIDGETCLASS=first;
            return;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------*/

ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name)
{
    ei_widgetclass_t* actual = LISTE_WIDGETCLASS;
    while(actual != NULL) //pour tout les elements de la liste
    {
        if (strcmp(actual->name, name)==0)//Si le nom de classe de la liste est celui cherché
        {
            return actual; //renvoie la classe associé
        }
        actual=actual->next;
    }
    return NULL;
}

/*-------------------------------------------------------------------------------------------------------*/

size_t		ei_widget_struct_size ()
{
    return (sizeof(ei_impl_widget_t));
}