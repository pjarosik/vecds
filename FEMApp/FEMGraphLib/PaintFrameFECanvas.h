#ifndef PAINTFRAMEFECANVAS_H
#define PAINTFRAMEFECANVAS_H

#include "PaintSetup.h"
#include "../GraphLib/PaintFrame.h"
#include "../FEMAppLib/FEMProject.h"
#include "../MathLib/GeometricObject.h"
#include "../MathLib/SpacePerspective.h"
#include "../MathLib/SpaceAxonometry.h"


class CPaintFrameFECanvas : public CPaintFrame
{

    class CGeomObjectDist {
    public:
        CGeometricObject *go, *parent;
        double d;
        CGeomObjectDist( const mvector &pv, CGeometricObject *g  );
        bool operator >( const CGeomObjectDist &gd ) const { return d < gd.d; }
        bool operator <( const CGeomObjectDist &gd ) const { return d > gd.d; }
    };

public:

    CPaintFrameFECanvas( const CFEMProject &fp, const CPaintSetup &s, double x=0.0, double y=0.0, double w=0.0, double h=0.0 );
    ~CPaintFrameFECanvas();

    virtual void Paint( CGraphDevice &gDV );

    void PaintFiniteElements( CGraphDevice &GDC, const vector< CFiniteElementInstance* > &fes );
    void PaintNodes( CGraphDevice &GDC, const vector< CFENode* > &nodes ) const;

    void PaintFEScheme(CGraphDevice &GDC, const vector< CGeometricObject* > &objects , CContour *contour = 0 ) const;
    void PaintGeometricObjects( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, int dim = -1, bool on_boundary = true, CContour *contour = 0 ) const;
    void PaintPoints( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, bool on_boundary = true ) const;
    void PaintLines( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, bool on_boundary = true ) const;
    void PaintFaces( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, bool on_boundary = true, CContour *contour = 0 ) const;

    CSpace* GetSpaceView() { return m_spaceview; }
    double GetMaxDisp() const { return m_max_disp; }
    void SetupPointView( );

    void ComputeDispMax( );
    virtual void CalculateContentSize ( double &x, double &y, double &fw, double &fh );

protected:

    unsigned m_dim;
    const CPaintSetup &m_setup;
    const CFEMProject &FP;
    CContour *m_contour;
    vector< CGeomObjectDist > GO_sorted;
    mvector m_max, m_min;
    double m_diagonal,  m_max_disp;
    double pr_depth, pr_height, pr_xp;
    CSpacePerspective m_spr;
    CSpaceAxonometry m_spax;

    void SetUpMap( );
    void EnableDeformation( bool en );
    void HideGeometricObjects( );

    void PaintGeometricObject( CGraphDevice &GDC, const CGeometricObject &go,  CContour *contour = 0 ) const;
    void PaintShape( CGraphDevice &GDC, const string &name, const CSpace *vss, CContour *contour = 0 ) const;
    void PaintHidedStructure( CGraphDevice &GDC, CContour *contour = 0 );
    void PaintDeformedStructure( CGraphDevice &gDV ) const;
    void PaintDescriptions( CGraphDevice &GDC );

};

#endif // PAINTFRAMEFECANVAS_H
