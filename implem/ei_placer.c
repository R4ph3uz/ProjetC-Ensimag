#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_types.h"
#include <stdlib.h>

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_runfunc(ei_widget_t widget)
{
    int res=(int)((float)*widget->geom_params->height + *(widget->geom_params->rel_height) * (float)(widget->parent->screen_location.size.height));
    widget->screen_location.size.height =res ;
    int res2=(int)((float)*widget->geom_params->width + *(widget->geom_params->rel_width) * (float)(widget->parent->screen_location.size.width));
    widget->screen_location.size.width = res2;
    int x =(int)(widget->parent->screen_location.top_left.x+*widget->geom_params->x+(int) ((float)(widget->parent->screen_location.size.width)**(widget->geom_params->rel_x)));
    int y =(int)(widget->parent->screen_location.top_left.y+*widget->geom_params->y+(int) ((float)(widget->parent->screen_location.size.height)**(widget->geom_params->rel_y)));
    if (*widget->geom_params->anchor==ei_anc_northwest)
    {
        widget->screen_location.top_left.x = x;
        widget->screen_location.top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_north)
    {
        widget->screen_location.top_left.x = x - (int) (widget->screen_location.size.width/2);
        widget->screen_location.top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_northeast)
    {
        widget->screen_location.top_left.x = x - (int) (widget->screen_location.size.width);
        widget->screen_location.top_left.y = y;
    }
    if (*widget->geom_params->anchor==ei_anc_west)
    {
        widget->screen_location.top_left.x = x;
        widget->screen_location.top_left.y = y- (int) (widget->screen_location.size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_center)
    {
        widget->screen_location.top_left.x = x - (int) (widget->screen_location.size.width/2);
        widget->screen_location.top_left.y = y- (int) (widget->screen_location.size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_east)
    {
        widget->screen_location.top_left.x = x - (int) (widget->screen_location.size.width);
        widget->screen_location.top_left.y = y- (int) (widget->screen_location.size.height/2);
    }
    if (*widget->geom_params->anchor==ei_anc_southwest)
    {
        widget->screen_location.top_left.x = x ;
        widget->screen_location.top_left.y = y- (int) (widget->screen_location.size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_south)
    {
        widget->screen_location.top_left.x = x - (int) (widget->screen_location.size.width/2);
        widget->screen_location.top_left.y = y- (int) (widget->screen_location.size.height);
    }
    if (*widget->geom_params->anchor==ei_anc_southeast)
    {
        widget->screen_location.top_left.x = x - (int) (widget->screen_location.size.width);
        widget->screen_location.top_left.y = y- (int) (widget->screen_location.size.height);
    }



}

/*-------------------------------------------------------------------------------------------------------*/

void ei_placer_releasefunc(ei_widget_t widget)
{
 if (widget->children_head!=NULL)
 {
     ei_widget_t children;
     children=widget->children_head;
     while (children!=widget->children_tail)
     {
         ei_geometrymanager_unmap(children);
         children=children->next_sibling;
     }
     ei_geometrymanager_unmap(children);
 }
}

/*-------------------------------------------------------------------------------------------------------*/

void		ei_place	(ei_widget_t		widget,
                         ei_anchor_t*		anchor,
                         int*			x,
                         int*			y,
                         int*			width,
                         int*			height,
                         float*			rel_x,
                         float*			rel_y,
                         float*			rel_width,
                         float*			rel_height) {
    widget->geom_params=malloc(sizeof(ei_impl_geom_param_t));

    widget->geom_params->manager = malloc(sizeof(ei_geometrymanager_t));
    widget->geom_params->manager->runfunc=ei_placer_runfunc;
    widget->geom_params->manager->releasefunc=ei_placer_releasefunc;
    widget->geom_params->manager->next=NULL;
    const char naame[] = "placer";
    strcpy(widget->geom_params->manager->name,naame);

    widget->geom_params->anchor = malloc(sizeof(ei_anchor_t));
    if (anchor) {
        *widget->geom_params->anchor=*anchor;
    }
    else
    {
       *widget->geom_params->anchor= ei_anc_northwest;
    }


    widget->geom_params->x=malloc(sizeof(int));
    if (x) {
        *widget->geom_params->x=*x;
    }
    else
    {
        *widget->geom_params->x = 0;
    }

    widget->geom_params->y=malloc(sizeof(int));
    if (y) {
        *(widget->geom_params->y)=*y;
    }
    else
    {
        *(widget->geom_params->y) = 0;
    }

    widget->geom_params->rel_y=malloc(sizeof(float));
    if (rel_y) {
        *widget->geom_params->rel_y=*rel_y;
    }
    else
    {
        *widget->geom_params->rel_y = 0;
    }

    widget->geom_params->rel_x=malloc(sizeof(float));
    if (rel_x) {
        *widget->geom_params->rel_x=*rel_x;
    }
    else
    {
        *widget->geom_params->rel_x = 0;
    }

    widget->geom_params->rel_width=malloc(sizeof(float));
    if (rel_width) {
        *widget->geom_params->rel_width=*rel_width;
    }
    else
    {
        *widget->geom_params->rel_width = 0;
    }

    widget->geom_params->rel_height=malloc(sizeof(float));
    if (rel_height) {
        *widget->geom_params->rel_height=*rel_height;
    }
    else
    {
        *widget->geom_params->rel_height = 0;
    }


    // passer par des pointeurs ?
    int			requested_width = widget->requested_size.width;
    int			requested_height= widget->requested_size.height;
    int         default_width = widget->parent->screen_location.size.width;
    int         default_height = widget->parent->screen_location.size.height;

    widget->geom_params->width = malloc(sizeof(int));
    if(width) {
        *widget->geom_params->width=*width;
    }
    else {
        if(requested_width) {
            *widget->geom_params->width=requested_width;
        }
        else {
            if(rel_width == NULL) {
                *widget->geom_params->width = default_width;
            }
            else {
                *widget->geom_params->width=0;
            }
        }
    }

    widget->geom_params->height = malloc(sizeof(int));
    if(height) {
        *widget->geom_params->height=*height;
    }
    else {
        if(requested_height) {
            *widget->geom_params->height=requested_height;
        }
        else {
            if(rel_height == NULL) {
                *widget->geom_params->height = default_height;
            }
            else {
                *widget->geom_params->height=0;
            }
        }
    }

}





/*-------------------------------------------------------------------------------------------------------*/
