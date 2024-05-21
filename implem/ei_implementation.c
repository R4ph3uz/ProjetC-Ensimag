#include "ei_implementation.h"
#include "hw_interface.h"
#include <stdlib.h>
#include "ei_application.h"
#include "widgetclass/ei_toplevel.h"
/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_runfunc(ei_widget_t widget)
{
    ei_rect_t* newscreen=SAFE_MALLOC(sizeof (ei_rect_t));   //Nouvelle localisation de l'ecran
    if(!widget->geom_params){
        return;
    }

    int height= *widget->geom_params->height ;
    float rel_height= *(widget->geom_params->rel_height);
    int parent_height= widget->parent->content_rect->size.height;

    int width= *widget->geom_params->width ;
    float rel_width= *(widget->geom_params->rel_width);
    int parent_width= widget->parent->content_rect->size.width;

    int new_height=(int)((float)height + rel_height * (float)parent_height);
    newscreen->size.height =new_height ;

    int new_width=(int)((float)width + rel_width * (float)parent_width);
    newscreen->size.width =new_width ;

    int x =(int)(widget->parent->content_rect->top_left.x
            + *widget->geom_params->x+(int)((float)(widget->parent->content_rect->size.width)
            * *(widget->geom_params->rel_x)));
    int y =(int)(widget->parent->content_rect->top_left.y + *widget->geom_params->y
            + (int)((float)(widget->parent->content_rect->size.height)
            * *(widget->geom_params->rel_y)));

    if (*widget->geom_params->anchor==ei_anc_northwest) //localisation du top left en fonction de l'ancre
    {
        newscreen->top_left.x = x;
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_north)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width/2);
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_northeast)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width);
        newscreen->top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_west)
    {
        newscreen->top_left.x = x;
        newscreen->top_left.y = y- (int) (newscreen->size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_center)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width/2);
        newscreen->top_left.y = y- (int) (newscreen->size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_east)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width);
        newscreen->top_left.y = y- (int) (newscreen->size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_southwest)
    {
        newscreen->top_left.x = x ;
        newscreen->top_left.y = y- (int) (newscreen->size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_south)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width/2);
        newscreen->top_left.y = y- (int) (newscreen->size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_southeast)
    {
        newscreen->top_left.x = x - (int) (newscreen->size.width);
        newscreen->top_left.y = y- (int) (newscreen->size.height);
    }
    ei_geometry_run_finalize(widget,newscreen);
    SAFE_FREE(newscreen);
}

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_releasefunc(ei_widget_t widget)
{
    if (widget->children_head!=NULL) // Si widget a un enfant ou +
    {
        ei_widget_t children;
        children=widget->children_head;
        while (children!=widget->children_tail) //Pour chacun de ses enfants
        {
            ei_geometrymanager_unmap(children); //Appelle a unmap sur cet enfant
            children=children->next_sibling;
        }
        ei_geometrymanager_unmap(children);
    }

}

/*-------------------------------------------------------------------------------------------------------*/
ei_rect_t* intersection_rectangle(ei_rect_t rect1,ei_rect_t rect2) {
    // Find the coordinates of the top-left corner of the intersection rectangle
    int x_tl = rect1.top_left.x > rect2.top_left.x ? rect1.top_left.x : rect2.top_left.x;
    int y_tl = rect1.top_left.y > rect2.top_left.y ? rect1.top_left.y : rect2.top_left.y;

    // Find the coordinates of the bottom-right corner of the intersection rectangle
    int x_br = (rect1.top_left.x + rect1.size.width) < (rect2.top_left.x + rect2.size.width) ?
               (rect1.top_left.x + rect1.size.width) : (rect2.top_left.x + rect2.size.width);
    int y_br = (rect1.top_left.y + rect1.size.height) < (rect2.top_left.y + rect2.size.height) ?
               (rect1.top_left.y + rect1.size.height) : (rect2.top_left.y + rect2.size.height);

    // Calculate the width and height of the intersection rectangle
    int width = x_br - x_tl;
    int height = y_br - y_tl;

    // Check if the rectangles intersect
    if (width <= 0 || height <= 0) {
        // No intersection, return NULL
        return NULL;
    } else {
        // Construct and return the intersection rectangle
        ei_rect_t* intersection_rect = SAFE_MALLOC(sizeof(ei_rect_t));
        intersection_rect->top_left.x = x_tl;
        intersection_rect->top_left.y = y_tl;
        intersection_rect->size.width = width;
        intersection_rect->size.height = height;
        return intersection_rect;
    }
}

/*------------------------------------------------------------------------------------*/

ei_rect_t* union_rectangle(ei_rect_t rect1, ei_rect_t rect2)
{
    // Calculate the top-left point of the union rectangle
    int x1 = rect1.top_left.x;
    int y1 = rect1.top_left.y;
    int x2 = rect2.top_left.x;
    int y2 = rect2.top_left.y;
    int x = (x1 < x2) ? x1 : x2;
    int y = (y1 < y2) ? y1 : y2;

    // Calculate the size of the union rectangle
    int w1 = rect1.size.width;
    int h1 = rect1.size.height;
    int w2 = rect2.size.width;
    int h2 = rect2.size.height;
    int w = (x1 + w1 > x2 + w2) ? (x1 + w1 - x) : (x2 + w2 - x);
    int h = (y1 + h1 > y2 + h2) ? (y1 + h1 - y) : (y2 + h2 - y);

    // Allocate memory for the union rectangle
    ei_rect_t* rect = SAFE_MALLOC(sizeof(ei_rect_t));
    if (!rect) {
        return NULL;
    }

    // Set the values of the union rectangle
    rect->top_left.x = x;
    rect->top_left.y = y;
    rect->size.width = w;
    rect->size.height = h;

    return rect;
}

/*------------------------------------------------------------------------------------*/

uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color){
    int ir,ig,ib,ia;
    hw_surface_get_channel_indices( surface, &ir, &ig, &ib, &ia);
    uint8_t* res = SAFE_MALLOC(sizeof(uint8_t)*4);
    res[ir] = color.red;
    res[ig] = color.green;
    res[ib] = color.blue;
    if(ia!=-1){
        res[ia] = color.alpha;
    }
    return (uint32_t) *res;

}

/*------------------------------------------------------------------------------------*/

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                         ei_surface_t		surface,
                         ei_surface_t		pick_surface,
                         ei_rect_t*		clipper) {
    (*(widget->wclass->drawfunc))(widget,surface,pick_surface,clipper);
        ei_widget_t actuel = widget->children_head;
    if (actuel!=NULL) { //Si widget a des enfants
        while(actuel!=NULL) { //Pour chaque enfant

            if (!widget->parent) // Si le widget est la root
            {
                clipper=actuel->parent->content_rect; // le clipper est le content rect de la root
                if(clipper)
                    ei_impl_widget_draw_children(actuel, surface, pick_surface, clipper); // On appelle recursivement la fonciton sur l'enfant
            }
            else if(clipper) { //Si un clipper existe déja
                //le nouveau clipper est l'intersection de ce clipper et du content rect du parent
                clipper=intersection_rectangle(*clipper,*actuel->parent->content_rect);
                if (clipper){
                    ei_impl_widget_draw_children(actuel, surface, pick_surface, clipper);
                    SAFE_FREE(clipper);
                }
            }
            actuel = actuel->next_sibling;
        }
    }
    if (!strcmp(widget->wclass->name, "toplevel")){
        ei_point_t* carre_bas_droite = SAFE_MALLOC(sizeof(ei_point_t)*4);
        ei_color_t color_plus_fonce;
        color_plus_fonce.red = 87;
        color_plus_fonce.green = 93;
        color_plus_fonce.blue = 100;
        color_plus_fonce.alpha = 255;

        ei_rect_t nouveau_rect = widget->screen_location;
        carre_bas_droite[0] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-1,nouveau_rect.top_left.y + nouveau_rect.size.height-1 };
        carre_bas_droite[1] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-10,nouveau_rect.top_left.y + nouveau_rect.size.height-1 };
        carre_bas_droite[2] = (ei_point_t){nouveau_rect.top_left.x+ nouveau_rect.size.width-10, nouveau_rect.top_left.y + nouveau_rect.size.height-10};
        carre_bas_droite[3] = (ei_point_t){ nouveau_rect.top_left.x+ nouveau_rect.size.width-1,nouveau_rect.top_left.y + nouveau_rect.size.height-10 };

        ei_toplevel_t toplevel= (ei_toplevel_t ) widget;
        if (*toplevel->resizable!=ei_axis_none)
        {
            ei_draw_polygon(surface, carre_bas_droite, 4, color_plus_fonce, clipper);
            ei_draw_polygon(get_pick_surface(), carre_bas_droite, 4, *widget->pick_color, clipper);

        }
        ei_point_t* border_line = SAFE_MALLOC(sizeof(ei_point_t)*5);
        ei_rect_t content_rect = *widget->content_rect;
        border_line[0] = content_rect.top_left;
        border_line[1]= (ei_point_t){content_rect.top_left.x+ content_rect.size.width-1, content_rect.top_left.y };
        border_line[2] = (ei_point_t){content_rect.top_left.x+ content_rect.size.width-1, content_rect.top_left.y + content_rect.size.height-1};
        border_line[3]= (ei_point_t){content_rect.top_left.x, content_rect.top_left.y + content_rect.size.height-1 };
        border_line[4] = content_rect.top_left;
        ei_draw_polyline(surface, border_line, 5, color_plus_fonce,clipper);
    }
}

/*-------------------------------------------------------------------------------------------------------*/

ei_geometrymanager_t* create_placer_gm(){
    ei_geometrymanager_t* res = SAFE_MALLOC(sizeof(ei_geometrymanager_t));
    res->runfunc=ei_placer_runfunc;
    res->releasefunc = ei_placer_releasefunc;
    const char name[] = "placer";
    strcpy(res->name, name);
    res->next = NULL;
    return res;
}

/*-------------------------------------------------------------------------------------------------------*/
void supprime_de_ses_freres(ei_widget_t widget)
{
    if(widget->parent){ //Si widget != Root (normalement on appelle pas cette fonction sur la root)
        ei_widget_t prec=widget->parent;
        ei_widget_t suiv=widget->next_sibling; //suiv est le widget frere de widget
        if (prec->children_head!=widget) //Si le widget n'est pas le premier fils de son frère
        {
            prec=prec->children_head;
            while (prec->next_sibling!=widget) //Cherche widget dans ses freres
            {
                prec=prec->next_sibling; //trouve le widget precedent a widget
            }
            prec->next_sibling=suiv; // suiv devient le frére de prec
            if(suiv==NULL) // Si widget est le dernier fils de son parent
            {
                prec->parent->children_tail=prec; //La tail devient prec
            }
        }
        else // si widget est le premier fils de son pere
        {
            prec->children_head=suiv; //le premier fils devient suiv
            if(suiv==NULL)// si suiv nul
            {
                prec->children_tail=NULL; //actualise la tail
            }
        }
    }
}
/*-------------------------------------------------------------------------------------------------------*/

void place_a_la_fin(ei_widget_t widget2) {
    if (widget2->parent->children_tail == NULL) { //si le parent n'as pas d'enfant
        widget2->parent->children_tail = widget2;
        widget2->parent->children_head = widget2;
    } else { //si le parent a des enfants
        widget2->parent->children_tail->next_sibling = widget2;
        widget2->parent->children_tail = widget2;
        widget2->next_sibling = NULL;
    }
}

/*-------------------------------------------------------------------------------------------------------*/
