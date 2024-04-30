#include "ei_geometrymanager.h"

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
                         float*			rel_height)
{

    ei_anchor_t*		anchor2;
    int*			x2;
    int*			y2;
    int*			width2;
    int*			height2;
    float*			rel_x2;
    float*			rel_y2;
    float*			rel_width2;
    float*			rel_height2;
    *anchor2=*anchor;
    if (anchor==NULL)
    {
        *anchor2= ei_anc_northwest;
    }

    if (x==NULL)
    {
        x2=0;
    }
    else
    {
        x2=x;
    }
    if (y==NULL)
    {
        y2=0;
    }
    else
    {
        y2=y;
    }

    if (rel_x==NULL)
    {
        rel_x2=0;
    }
    else
    {
        rel_x2=rel_x;
    }
    if (rel_y==NULL)
    {
        rel_y2=0;
    }
    else
    {
        rel_y2=rel_y;
    }

    if (rel_width==NULL)
    {
        rel_width2=0;
    }
    else
    {
        rel_width2=rel_width;
    }

    if (rel_height==NULL)
    {
        rel_height2=0;
    }
    else
    {
        rel_height2=rel_height;
    }

}

/*-------------------------------------------------------------------------------------------------------*/

