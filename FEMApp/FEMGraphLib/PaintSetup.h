#ifndef PAINTSETUP_H
#define PAINTSETUP_H
#include <string>
#include "../FEMGraphLib/Contour.h"
#include "../GraphLib/color.h"
using namespace std;

enum PROJECTION
{
    PR_PERSPECTIVE,
    PR_AXONOMETRY,
    PR_XY,
    PR_YZ,
    PR_XZ
};

enum FILE_TYPE
{
    FL_PS,
    FL_SVG,
    FL_XML,
    FL_HTML,
    FL_PRM,
    FL_VTK,
    FL_MEM
};


class CPaintSetup
{
public :

    DColor m_nodecolor, m_edgecolor, m_facecolor;
    DColor m_modelnodecolor,  m_modeledgecolor,  m_modelfacecolor;
    DColor white_color, red_color, green_color, blue_color;
    DColor m_deformed_color;

    CPaintSetup( );
    bool paint_model, paint_nodes, paint_edges, paint_faces, paint_bcs, paint_node_numbers, paint_fe_numbers, deformed, deformation, boundary_only, hollow, contour;
    bool paint_contours, paint_map;
    double Rx, Ry, Rz, xp, height, depth, ax, ay;
    unsigned contour_number;
    int contour_degree;
    MAP_COLOR_MODES colormode;
    string map;
    PROJECTION projection;
    FILE_TYPE type;
    void SetAllState( bool st );
};

#endif // PAINTSETUP_H
