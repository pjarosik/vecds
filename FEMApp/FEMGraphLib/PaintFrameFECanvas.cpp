#include "PaintFrameFECanvas.h"
#include "../GraphLib/GraphDeviceSizer.h"
#include "../GraphLib/color.h"
#include "../FEMLib/ResultApprox.h"

CPaintFrameFECanvas::CGeomObjectDist::CGeomObjectDist( const mvector &pv, CGeometricObject *g  ):go(g)
{
    mvector Xs( 3 );
    unsigned l, count = 0.0;
    for (l=0; l<g->GetOwned().size(); l++ )
    {
        mvector X;
        if ( go->GetOwned()[ l ]->IsPoint() )
        {
            CGeometricObjectPoint *point = dynamic_cast< CGeometricObjectPoint* >( go->GetOwned()[ l ] );
            point->GetX( X );
            Xs += X;
            count++;
        }
    }
    Xs *=  1.0 / (double)count;
    mvector r = pv - Xs;
    d   = r.GetNorm();
}


CPaintFrameFECanvas::CPaintFrameFECanvas( const CFEMProject &fp, const CPaintSetup &s,  double x, double y, double w, double h ):CPaintFrame(x,y,w,h), FP( fp ),
    m_setup( s ), m_contour(0), m_max_disp(0.0), m_diagonal(0), m_max(3), m_min(3), pr_depth(0.0), pr_height(0.0), pr_xp(0.0), m_spr(0,0,0)

{
        m_dim = FP.GetGeometricDomain().DetermineSpaceDimension();
}

CPaintFrameFECanvas::~CPaintFrameFECanvas()
{
    delete m_contour;
}

void CPaintFrameFECanvas::PaintNodes( CGraphDevice &gDV, const vector< CFENode* > &nodes ) const
{
    unsigned k;
    for (k=0; k<nodes.size(); k++)
        PaintGeometricObject( gDV, *nodes[ k ] );
}

void CPaintFrameFECanvas::PaintFiniteElements( CGraphDevice &gDV, const vector< CFiniteElementInstance* > &fes )
{
    unsigned k;
    for (k=0; k<fes.size(); k++)
        PaintGeometricObject( gDV, *fes[ k ] );
}

void CPaintFrameFECanvas::CalculateContentSize( double &x, double &y, double &fw, double &fh )
{
    mvector size( 2 );
    CGraphDeviceSizer sizer( m_spaceview );

    sizer.MakeContext();
    Paint( sizer );
    sizer.DeleteContext();

    x = sizer.GetMin2D()[ 0 ];
    y = sizer.GetMin2D()[ 1 ];

    sizer.GetSizes2D( size );

    m_min = sizer.GetMin3D();
    m_max = sizer.GetMax3D();
    m_diagonal = sizer.GetDiagonal3D();

    fw = size[ 0 ];
    fh = size[ 1 ];


}

void CPaintFrameFECanvas::EnableDeformation( bool en )
{
    ComputeDispMax();
    if ( m_max_disp <= 0.0 ) return;

    unsigned k;
    double sc = en ? 0.1 * m_diagonal / m_max_disp : 0.0;
    if ( m_max_disp > 0.0 )
    for (k=0; k<FP.GetNodes().size(); k++)
        FP.GetNodes()[ k ]->SetDefScale( sc );
}

void CPaintFrameFECanvas::SetupPointView()
{
    if ( m_dim == 3 )
    {
        if ( m_setup.xp == DBL_MAX ) pr_xp = m_max[ 0 ] + 0.5 *( m_max[ 0 ] - m_min[ 0 ] );
        else pr_xp = m_setup.xp;

        if ( m_setup.height == DBL_MAX ) pr_height = m_max[ 2 ] + 0.5 *( m_max[ 2 ] - m_min[ 2 ] );
        else pr_height = m_setup.height;

        if ( m_setup.depth == DBL_MAX ) pr_depth = m_diagonal;
        else pr_depth = m_setup.depth;
    }
    else
    {
        if ( m_setup.xp == DBL_MAX ) pr_xp = m_max[ 0 ] + 0.5 *( m_max[ 0 ] - m_min[ 0 ] );
        else pr_xp = m_setup.xp;

        if ( m_setup.height == DBL_MAX ) pr_height =  0.5 * pr_xp;
        else pr_height = m_setup.height;

        if ( m_setup.depth == DBL_MAX ) pr_depth = m_diagonal;
        else pr_depth = m_setup.depth;
    }

    m_spr = CSpacePerspective( pr_depth, pr_height, pr_xp );

    switch ( m_setup.projection ) {
        case PR_AXONOMETRY:                     m_spaceview = &m_spax; break;
        case PR_PERSPECTIVE:                    m_spaceview = &m_spr;  break;
    }

}

void CPaintFrameFECanvas::HideGeometricObjects()
{

    EnableDeformation( m_setup.deformed );
    GO_sorted.clear();

    const vector< CGeometricObject* > &objects = FP.GetGeometricDomain().GetAllGeometricObjects();

    mvector pointviev(3);

    pointviev[ 0 ] =  pr_xp;
    pointviev[ 1 ] = -pr_depth;
    pointviev[ 2 ] =  pr_height;

    if ( m_dim == 3 && !m_setup.hollow )
    {
        unsigned k;
        for ( k=0; k<objects.size(); k++ ) {
            bool to_paint = true;
            if ( !objects[ k ]->IsBoundary() && m_setup.boundary_only ) to_paint = false;
            if ( to_paint && objects[ k ]->GetObjectDim() == 2  )
                if ( objects[ k ]->IsBoundary() ) GO_sorted.push_back( CGeomObjectDist( pointviev, objects[ k ] ) );

        }

        sort( GO_sorted.begin(), GO_sorted.end() );
        for ( k=0; k<GO_sorted.size(); k++ )
        {
            unsigned l;
            for (l=0; l<GO_sorted[k].go->GetOwned().size(); l++ )
            {
                CGeometricObject *obj =  GO_sorted[ k ].go->GetOwned()[ l ];
                obj->selected_owner = GO_sorted[ k ].go;
            }
        }

    }

}

void CPaintFrameFECanvas::SetUpMap(  )
{
    unsigned k,l;
        delete m_contour; m_contour = 0;
        double  maxres = -1.0E300, minres = 1.0E300;
        for (k=0; k<FP.GetNodes().size(); k++)
        {
              const map< string, double > &rs = FP.GetNodes()[ k ]->GetResults();
              map< string, double >::const_iterator i = rs.find( m_setup.map );
              if ( i != rs.end() )
              {
                if ( i->second > maxres ) maxres = i->second;
                if ( i->second < minres ) minres = i->second;
              }
              const CDOFVector &ndofs = FP.GetNodes()[ k ]->GetDofs();
              for ( l = 0; l<ndofs.size(); l++ )
                  if ( ndofs[ l ]->DofName() == m_setup.map )
                  {
                      if ( ndofs[ l ]->GetSolution() > maxres ) maxres = ndofs[l]->GetSolution();
                      if ( ndofs[ l ]->GetSolution() < minres ) minres = ndofs[l]->GetSolution();
                  }

        }
        m_contour = new CContour( m_setup.map, minres, maxres, m_setup.contour_number, m_setup.colormode, m_setup.paint_map, m_setup.paint_contours );

}

void CPaintFrameFECanvas::ComputeDispMax( )
{
    unsigned k, l, m;
    m_max_disp = -1.0;
    for (l=0; l<FP.GetNodes().size(); l++)
    {
        CFENode* node =  FP.GetNodes()[l];
        if ( node )
        {
            for (m=0; m <node->GetDofs().size(); m++)
            if ( node->GetDofs()[m]->GetType() == DOF_UX && fabs( node->GetDofs()[m]->GetSolution()) > m_max_disp ) m_max_disp = fabs( node->GetDofs()[m]->GetSolution());
                    else if ( node->GetDofs()[m]->GetType() == DOF_UY && fabs( node->GetDofs()[m]->GetSolution()) > m_max_disp ) m_max_disp = fabs( node->GetDofs()[m]->GetSolution());
                    else if ( node->GetDofs()[m]->GetType() == DOF_UZ && fabs( node->GetDofs()[m]->GetSolution()) > m_max_disp ) m_max_disp = fabs( node->GetDofs()[m]->GetSolution());
            }
        }
}


void CPaintFrameFECanvas::PaintGeometricObject( CGraphDevice &GDC, const CGeometricObject &go, CContour *contour ) const
{
    const CSpace *os = go.GetSpace();
    const string name = go.GetAttribute("name");
    if ( go.IsPoint( ) )
    {
        double nsize = 3;
        const CGeometricObjectPoint *pt = dynamic_cast< const CGeometricObjectPoint* >( &go );
        mvector X;
        pt->GetX( X );
        switch( X.GetDim() )
        {
           case 2 : GDC.Rectangle( X, nsize , nsize );
                    break;
           case 3 : GDC.Point( X, nsize );
                    break;
        }
        if ( name != "" )
        {
            GDC.WriteText( X, name );
        }

    }
    else if ( contour )
    {
        contour->SetPaintMode( PM_MAP );
        if ( os ) PaintShape( GDC, name, os, contour );
        contour->SetPaintMode( PM_LINES );
    }
    else if ( os ) PaintShape( GDC, name, os, contour );
}


 void CPaintFrameFECanvas::PaintShape( CGraphDevice &GDC, const string &name, const CSpace *vss, CContour *contour ) const
 {
    unsigned sdim = vss->Getxdim(), dg, k, l, odim = vss->GetXdim();
    double dx;

    mvector ksi1(sdim), ksi2(sdim), ksi3(sdim), ksi4(sdim), ksis(sdim), x1(odim), x2(odim), x3(odim), x4(odim), xs(odim), resv(1);

    SHAPE_TYPES st = ST_RECTANGLE;
    const CSpaceShapeApproxPtr<CFENode> *ssh = dynamic_cast< const CSpaceShapeApproxPtr< CFENode > * >( vss );
    CResultApprox *resappr = 0;
    if ( ssh )
    {
        st = ssh->GetShapeSpace().GetShapeType();
        if (contour) resappr = new CResultApprox( *ssh, contour->GetName() );
    }

    ksi2 = ksi1 = -1.0;
    switch( sdim )
    {

        case 1 : vss->Transform( ksi1, x1 );
                 dg = 10;
                 if ( vss->GetGlobalDegree() != -1 ) dg = 2 * vss->GetGlobalDegree();
                 dx = 2.0 / dg;
                for( k=1; k<=dg; k++)
                {
                    ksi2 = k * dx - 1.0;
                    vss->Transform(ksi2,x2);
                    if ( odim == 3 ) GDC.Line3D(x1,x2);
                    else if ( odim == 2 ) GDC.Line(x1,x2);
                    x1 = x2;
                }
                if ( name != "" )
                {
                    ksi1 = 0;
                    vss->Transform( ksi1, x1 );
                    GDC.WriteText( x1, name );
                }
                break;
         case 2 :
                dg = 2;
                if ( vss->GetGlobalDegree() != -1 ) dg = 2 * vss->GetGlobalDegree();
                if ( resappr && resappr->IsValid() && contour->GetDegree() != -1 ) dg = contour->GetDegree();
                switch (st)
                {
                    case ST_RECTANGLE:
                                        dx = 2.0 / dg;
                                        for( k=0; k<dg; k++)
                                        for( l=0; l<dg; l++)
                                        {
                                            ksi1[0] = k * dx - 1.0;
                                            ksi1[1] = l * dx - 1.0;

                                            ksi2[0] = (k+1) * dx - 1.0;
                                            ksi2[1] = l * dx - 1.0;

                                            ksi3[0] = (k+1) * dx - 1.0;
                                            ksi3[1] = (l+1) * dx - 1.0;

                                            ksi4[0] = k * dx - 1.0;
                                            ksi4[1] = (l+1) * dx - 1.0;

                                            ksis[0] = k * dx + dx / 2.0 - 1.0;
                                            ksis[1] = l * dx + dx / 2.0 - 1.0;

                                            vss->Transform(ksi1,x1);
                                            vss->Transform(ksi2,x2);
                                            vss->Transform(ksi3,x3);
                                            vss->Transform(ksi4,x4);
                                            vss->Transform(ksis,xs);

                                            if ( resappr && resappr->IsValid() )
                                            {
                                                CContour::CContourVertex c1(x1,0.0), c2(x2,0.0), c3(x3,0.0), c4(x4,0.0),cs(xs,0.0);
                                                resappr->Transform( ksi1, resv ); c1.v=resv[0];
                                                resappr->Transform( ksi2, resv ); c2.v=resv[0];
                                                resappr->Transform( ksi3, resv ); c3.v=resv[0];
                                                resappr->Transform( ksi4, resv ); c4.v=resv[0];
                                                resappr->Transform( ksis, resv ); cs.v=resv[0];

                                                contour->PlotContourTriangle(GDC,c1,c2,cs);
                                                contour->PlotContourTriangle(GDC,c2,c3,cs);
                                                contour->PlotContourTriangle(GDC,c3,c4,cs);
                                                contour->PlotContourTriangle(GDC,c4,c1,cs);

                                            }
                                            else
                                            {
                                                if ( x1.GetDim() == 3 )
                                                {
                                                    GDC.Triangle3D(x1,x2,xs);
                                                    GDC.Triangle3D(x2,x3,xs);
                                                    GDC.Triangle3D(x3,x4,xs);
                                                    GDC.Triangle3D(x4,x1,xs);
                                                }
                                                else if ( x1.GetDim() == 2 )
                                                {
                                                    GDC.Triangle(x1,x2,xs);
                                                    GDC.Triangle(x2,x3,xs);
                                                    GDC.Triangle(x3,x4,xs);
                                                    GDC.Triangle(x4,x1,xs);
                                                }
                                            }
                                        }
                                        if ( name != "" )
                                        {
                                            ksi1 = 0;
                                            vss->Transform( ksi1, x1 );
                                            GDC.WriteText( x1, name );
                                        }
                                        break;
                    case ST_TRIANGLE :
                                        ksi1[0] = 1.0;
                                        ksi1[1] = 0.0;

                                        ksi2[0] = 0.0;
                                        ksi2[1] = 1.0;

                                        ksi3[0] = 0.0;
                                        ksi3[1] = 0.0;
                                        vss->Transform(ksi1,x1);
                                        vss->Transform(ksi2,x2);
                                        vss->Transform(ksi3,x3);

                                        if ( resappr->IsValid() )
                                        {
                                            CContour::CContourVertex c1(x1,0.0), c2(x2,0.0), c3(x3,0.0);
                                            resappr->Transform( ksi1, resv ); c1.v=resv[0];
                                            resappr->Transform( ksi2, resv ); c2.v=resv[0];
                                            resappr->Transform( ksi3, resv ); c3.v=resv[0];
                                            contour->PlotContourTriangle(GDC,c1,c2,c3);
                                        }
                                        else
                                        {
                                            if ( x1.GetDim() == 3 )
                                                GDC.Triangle3D(x1,x2,x3);
                                            else if ( x1.GetDim() == 2 )
                                                GDC.Triangle(x1,x2,x3);
                                        }
                                        break;
                }

            break;
    }
    delete resappr;
 }

void CPaintFrameFECanvas::PaintGeometricObjects( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, int dim , bool on_boundary , CContour *contour ) const
{
    unsigned k;
    for ( k=0; k<objects.size(); k++ )
    {
        bool is_proper_dim = ( dim == -1 ) || dim == objects[k]->GetObjectDim();
        bool is_proper_boundary = on_boundary ? objects[k]->IsBoundary() : true;
        if (  is_proper_dim && is_proper_boundary ) PaintGeometricObject( GDC, *objects[ k ], contour );
    }

}

void CPaintFrameFECanvas::PaintPoints( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, bool on_boundary ) const
{
    PaintGeometricObjects( GDC, objects, 0, on_boundary );
}

void CPaintFrameFECanvas::PaintLines( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, bool on_boundary  ) const
{
    PaintGeometricObjects( GDC, objects, 1, on_boundary );
}

void CPaintFrameFECanvas::PaintFaces( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, bool on_boundary, CContour *contour  ) const
{
    PaintGeometricObjects( GDC, objects, 2, on_boundary );
}

void CPaintFrameFECanvas::PaintFEScheme( CGraphDevice &GDC, const vector< CGeometricObject* > &objects, CContour *contour  ) const
{
    if ( m_setup.paint_faces )
    {
        GDC.SetBrush( DBrush( DColor( m_setup.m_facecolor ), 0 ) );
        PaintFaces( GDC, objects, m_setup.boundary_only, contour );
    }

    if ( m_setup.paint_edges )
    {
        GDC.SetBrush( DBrush( DColor( m_setup.m_edgecolor ), 0 ) );
        PaintLines( GDC, objects, m_setup.boundary_only );
    }

    if ( m_setup.paint_nodes )
    {
        GDC.SetBrush( DBrush( DColor( m_setup.m_nodecolor ), 0 ) );
        PaintPoints( GDC, objects, m_setup.boundary_only );
    }


}

void CPaintFrameFECanvas::PaintDescriptions( CGraphDevice &GDC )
{
    if ( m_setup.paint_node_numbers ) {

    }

    if ( m_setup.paint_fe_numbers ) {

    }
}

void CPaintFrameFECanvas::PaintHidedStructure( CGraphDevice &GDC, CContour *contour )
{
    unsigned k;
    for ( k=0; k<GO_sorted.size(); k++ )
    {
        if ( m_setup.paint_faces )
        {
            GDC.SetBrush( DBrush( DColor( m_setup.m_facecolor ), 0 ) );
            PaintGeometricObject( GDC, *GO_sorted[k].go, contour );
        }
        unsigned l;
        vector< CGeometricObject* > edges, points;
        if ( m_setup.paint_edges )
        {
            unsigned l;
            for (l=0; l<GO_sorted[ k ].go->GetOwned().size(); l++ )
            {
                CGeometricObject *go =  GO_sorted[ k ].go->GetOwned()[ l ];
                if ( go->IsLine() ) edges.push_back( go );
            }

        }

        if ( m_setup.paint_nodes )
        {
            unsigned l;
            for (l=0; l<GO_sorted[ k ].go->GetOwned().size(); l++ )
            {
                CGeometricObject *go =  GO_sorted[ k ].go->GetOwned()[ l ];
                if ( go->IsPoint() ) points.push_back( go );

            }

        }
        GDC.SetPen( DPen( DColor( m_setup.m_edgecolor ), 0, 0.5 ) );
        PaintLines( GDC, edges, m_setup.boundary_only );
        GDC.SetPen( DPen( DColor( m_setup.m_nodecolor ), 0, 1.0 ) );
        GDC.SetBrush( DBrush( DColor( m_setup.m_nodecolor ), 0 ) );
        PaintPoints( GDC, points, m_setup.boundary_only );
    }

}

void CPaintFrameFECanvas::PaintDeformedStructure( CGraphDevice &gDV ) const
{
    unsigned k;
    gDV.SetPen( DPen( m_setup.m_deformed_color, 0, 1.0 ) );
    for ( k=0; k<GO_sorted.size(); k++ )
    {
        unsigned l;
        vector< CFENode* > ndh;
        for (l=0; l<GO_sorted[ k ].go->GetOwned().size(); l++ )
        {
            CGeometricObject *go =  GO_sorted[ k ].go->GetOwned()[l];
            if ( go->IsBoundary() && go->GetObjectDim() == 1 && go->selected_owner == GO_sorted[ k ].go ) PaintGeometricObject( gDV, *go );
            if ( go->IsBoundary() && go->GetObjectDim() == 0 && go->selected_owner == GO_sorted[ k ].go ) ndh.push_back( dynamic_cast< CFENode* >( go ) );
        }


    }

    gDV.SetPen( DPen( m_setup.red_color, 0, 1.0 )  );

    if ( GO_sorted.size() == 0 )
    {
        if ( m_setup.paint_edges ) PaintLines( gDV, FP.GetGeometricDomain().GetAllGeometricObjects(), m_setup.boundary_only );
        if ( m_setup.paint_nodes ) PaintNodes( gDV, FP.GetNodes() );
    }

}

void CPaintFrameFECanvas::Paint( CGraphDevice &gDV )
{
    unsigned k;

    if ( m_setup.map.size() > 0 ) SetUpMap( );


    HideGeometricObjects();

    if ( GO_sorted.size() == 0 )  PaintFEScheme( gDV, FP.GetGeometricDomain().GetAllGeometricObjects(), m_contour );
    else PaintHidedStructure( gDV, m_contour );


    if ( m_setup.deformed )
    {
        EnableDeformation( true );
        PaintDeformedStructure( gDV );
    }

    PaintDescriptions( gDV );

}
