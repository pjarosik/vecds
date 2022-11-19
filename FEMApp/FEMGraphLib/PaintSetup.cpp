#include "PaintSetup.h"
#include <float.h>

CPaintSetup::CPaintSetup():m_nodecolor( 0.0, 0.8, 0.0 ),  m_edgecolor( 0.0, 0.0, 0.8 ), m_facecolor( 0.9, 0.9, 0.9 ),
    m_modelnodecolor( 0.9, 0.0, 0.0 ), m_modeledgecolor( 0.0, 0.0, 0.4 ), m_modelfacecolor( 0.85, 0.8, 0.8 ),
    white_color( 1.0, 1.0, 1.0 ), red_color( 1.0, 0.0, 0.0 ), green_color( 0.0, 1.0, 0.0 ), blue_color( 0.0, 0.0, 0.8 ), m_deformed_color(0.8, 0.0, 0.0)
{

    Rx = Ry = Rz        = 0.0;
    paint_model         = false;
    paint_nodes         = true;
    paint_edges         = true;
    paint_faces         = true;
    paint_bcs           = true;
    hollow              = false;
    boundary_only       = true;
    contour             = true;
    paint_node_numbers  = false;
    paint_fe_numbers    = false;
    deformed            = false;
    deformation         = false;
    ax                  = 3;
    ay                  = 2;
    xp                  = DBL_MAX;
    height              = DBL_MAX;
    depth               = DBL_MAX;
    contour_number      = 10;
    contour_degree      = -1;
    colormode           = CM_RED_YELLOW;
    paint_contours      = true;
    paint_map           = true;
    projection          = PR_PERSPECTIVE;

}

void CPaintSetup::SetAllState( bool st )
{

}
