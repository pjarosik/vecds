#include "Contour.h"
#include <float.h>

CContour::CContour( const string &nm, double min, double max, unsigned d, MAP_COLOR_MODES mode,bool pm, bool pc ):
    vmin( min ), vmax( max ), dn( d ), paint_map( pm ), paint_contour( pc ), name( nm ), degree(-1),paint_mode( PM_MAP )
{
    unsigned k;
    dv = ( max - min ) / (double)dn;
    for(k=0; k<=d; k++)
        contours.push_back( min + k * dv );

    conf.resize( contours.size(), DBL_MAX );
    con1.resize( contours.size(), DBL_MAX );
    con2.resize( contours.size(), DBL_MAX );

    vector< DColor > ncols;
    vector< double > sizes ;

    switch( mode )
    {
        case CM_SPECTRUM :  ncols.push_back(DColor(1.0,0.0,0.0));
                            ncols.push_back(DColor(1.0,1.0,0.0));   sizes.push_back(1.0);
                            ncols.push_back(DColor(0.0,1.0,0.0));   sizes.push_back(1.0);
                            ncols.push_back(DColor(0.0,1.0,1.0));   sizes.push_back(1.0);
                            ncols.push_back(DColor(0.0,0.0,1.0));   sizes.push_back(1.0);
                            ncols.push_back(DColor(1.0,0.0,1.0));   sizes.push_back(1.0);
                            break;

        case CM_RED_YELLOW: ncols.push_back(DColor(1.0,0.0,0.0));
                            ncols.push_back(DColor(1.0,1.0,0.0));   sizes.push_back(1.0);
                            break;

        case CM_GREEN_RED:  ncols.push_back(DColor(0.0,1.0,0.0));
                            ncols.push_back(DColor(1.0,0.0,0.0));   sizes.push_back(1.0);
                            break;

        case CM_RED_GREEN: ncols.push_back(DColor(1.0,0.0,0.0));
                           ncols.push_back(DColor(0.0,1.0,0.0));   sizes.push_back(1.0);
                           break;

        case CM_RED_BLUE:   ncols.push_back(DColor(1.0,0.0,0.0));
                            ncols.push_back(DColor(0.0,0.0,1.0));   sizes.push_back(1.0);
                            break;

        case CM_BLUE_RED:   ncols.push_back(DColor(0.0,0.0,1.0));
                            ncols.push_back(DColor(1.0,0.0,0.0));   sizes.push_back(1.0);
                            break;

        case CM_GRAY:       ncols.push_back(DColor(0.1,0.1,0.1));
                            ncols.push_back(DColor(0.9,0.9,0.9));   sizes.push_back(1.0);
                            break;
    }

    CreateContourColors( ncols, sizes, DColor(0,0,0) );
}


void CContour::CreateContourColors( const vector< DColor > &ncols, const vector< double > &sizes, const DColor &sep )
{
    unsigned k, l;
    double sum = 0.0, dx, x;
    vector< double > xs;
    xs.push_back( 0.0 );

    for(k=0; k<sizes.size(); k++)
    {
        sum += sizes[ k ];
        xs.push_back( sum );
    }

    dx = sum / (double)dn ;
    for( k = 0; k < dn; k++ )
    {
        x = k * dx + dx / 2.0;
        for (l=0; l<sizes.size(); l++ )
            if ( ( xs[ l ] < x ) && ( x < xs[ l + 1 ] ) )
            {
                CScaler rs( xs[l], xs[l+1], ncols[l].GetR(), ncols[l+1].GetR() );
                CScaler gs( xs[l], xs[l+1], ncols[l].GetG(), ncols[l+1].GetG() );
                CScaler bs( xs[l], xs[l+1], ncols[l].GetB(), ncols[l+1].GetB() );
                DColor col( rs.scale(x), gs.scale(x), bs.scale(x));
                brushes.push_back( DBrush( col, 0 ) );
            }
    }

    dx = sum /(double)dn;
    pens.push_back(  DPen( ncols[0] , 0, 0.5 )  );
    for( k = 1; k < dn ; k++ )
    {
        x = k * dx;
        for (l=0; l<sizes.size(); l++ )
            if ( (xs[ l ] <= x) && (x <= xs[ l + 1 ] ) )
            {
                CScaler rs( xs[l], xs[l+1], ncols[l].GetR(), ncols[l+1].GetR() );
                CScaler gs( xs[l], xs[l+1], ncols[l].GetG(), ncols[l+1].GetG() );
                CScaler bs( xs[l], xs[l+1], ncols[l].GetB(), ncols[l+1].GetB() );
                DColor col( rs.scale(x), gs.scale(x), bs.scale(x) );
                pens.push_back( DPen( col, 0, 0.1 ) );
            }
    }
    pens.push_back(  DPen( ncols.back(), 0, 0.5 ) );
}

void CContour::FindContours( const CContourVertex &c1, const CContourVertex &c2, vector<unsigned> &v12 )
{
    unsigned k;
    for ( k = 0; k < contours.size(); k++ )
        if ( c1.v <= contours[ k ] <= c2.v ) v12.push_back(  k  );
}

void CContour::PlotContourTriangle( CGraphDevice &GDC, const CContourVertex &c1, const CContourVertex &c2, const CContourVertex &c3  )
{
    dim = c1.x.GetDim( );
    GDC.SetNullPen();
//    const CContourVertex *ca = 0, *cb = 0, *cm = 0;
   const CContourVertex *x1 = &c1, *x2 = &c2, *x3 = &c3, *pm;

    if ( c1< c2 && c1<c3 ) { x1=&c1; x2=&c2; x3=&c3; }
    if ( c2< c1 && c2<c3 ) { x1=&c2; x2=&c3; x3=&c1; }
    if ( c3< c1 && c3<c2 ) { x1=&c3; x2=&c1; x3=&c2; }

    mvector  xp1 = c1.x, xp2( c1.x.GetDim() ), xp3( c1.x.GetDim() ), xp4( c1.x.GetDim() );

    CScaler sd12( x1->v, x2->v, 0.0, 1.0 );
    CScaler sd13( x1->v, x3->v, 0.0, 1.0 );
    CScaler sd23( x2->v, x3->v, 0.0, 1.0 );
    CScaler sd32( x3->v, x2->v, 0.0, 1.0 );

    CScaler *sful, *s1,*s2, *sm;


    int chc=-1,ppf=-1,pp1=-1,pp2=-1;
    unsigned k,l,npf=0,np1=0,np2=0;
    double pccf=-1.0;
    is_right = x2->v > x3->v;

    const CContourVertex *xf, *xm;

    if ( is_right )
    {
       sful     = &sd12;
       s1       = &sd13;
       s2       = &sd32;
       xf       = x2;
       xm       = x3;
    }
    else
    {
        sful    = &sd13;
        s1      = &sd12;
        s2      = &sd23;
        xf      = x3;
        xm      = x2;
    }

    sm       = s1;
    // aind = 0;
    double cff,cf1,cf2;
    for (k=0; k<contours.size()-1; k++)
    {
        conf[k]=DBL_MAX; con1[k]=DBL_MAX; con2[k]=DBL_MAX;
        cff = sful->scale( contours[ k ] ), cf1 = s1->scale( contours[ k ] ), cf2 = s2->scale( contours[ k ] );
        if ( cff > 0.0 && cff < 1.0 ) { conf[ k ] = cff; npf++; if (ppf==-1) ppf=k; }
        if ( cf1 > 0.0 && cf1 < 1.0 ) { con1[ k ] = cf1; np1++; if (pp1==-1) pp1=k; }
        if ( cf2 > 0.0 && cf2 < 1.0 ) { con2[ k ] = cf2; np2++; if (pp2==-1) pp2=k; }
        if ( pccf *  cff < 0.0 ) chc = k;
        pccf = cff;
    }

    if ( npf == 0 )
    {
        if ( !paint_map ) return;
        if ( chc==-1) GDC.SetBrush( cff < 0 ? brushes.back() : brushes.front() );
        else  GDC.SetBrush( brushes[ chc-1 ] );
        switch( dim )
        {
            case 2: GDC.Triangle2D( x1->x, x2->x, x3->x ); break;
            case 3: GDC.Triangle3D( x1->x, x2->x, x3->x ); break;
        }
        return;
    }

    abrush = ppf - 1;
    GDC.SetBrush( brushes[ abrush++ ] );
    xp1 = x1->x;

    if ( np1 != 0 )
    {
        xp1 = ( 1.0 - con1[ pp1 ] ) * x1->x + con1[ pp1 ] * xm->x;
        xp2 = ( 1.0 - conf[ ppf ] ) * x1->x + conf[ ppf ] * xf->x;
        PaintTriangle(GDC,x1->x,xp1,xp2);
        PaintLine(GDC,xp1,xp2);

    }
    else
    {
        xp1 = ( 1.0 - con2[ pp2 ] ) * xm->x + con2[ pp2 ] * xf->x;
        xp2 = ( 1.0 - conf[ ppf ] ) * x1->x + conf[ ppf ] * xf->x;
        PaintTriangle(GDC,x1->x,xp2,xm->x);
        PaintTriangle(GDC,xm->x,xp1,xp2);
        PaintLine(GDC,xp1,xp2);
    }

    for (k=pp1; k<pp1+np1-1; k++ )
    {
        GDC.SetBrush( brushes[ abrush++] );
        xp3 = ( 1.0 - con1[ k+1 ] ) * x1->x + con1[ k+1 ] * xm->x;
        xp4 = ( 1.0 - conf[ k+1 ] ) * x1->x + conf[ k+1 ] * xf->x;
        PaintTriangle(GDC,xp1,xp3,xp2);
        PaintTriangle(GDC,xp2,xp3,xp4);
        xp1=xp3; xp2=xp4;
        PaintLine(GDC,xp1,xp2);
    }
    if ( np1 != 0 )
    {
        GDC.SetBrush( brushes[ abrush++ ] );
        if ( np2 == 0 )
        {
            PaintTriangle(GDC,xp1,xm->x,xp2);
            PaintTriangle(GDC,xp2,xm->x,xf->x);
        }
        else
        {
            xp3 = ( 1.0 - con2[ pp2 ] ) * xm->x + con2[ pp2 ] * xf->x;
            xp4 = ( 1.0 - conf[ pp2 ] ) * x1->x + conf[ pp2 ] * xf->x;
            PaintTriangle(GDC,xp2,xp1,xm->x);
            PaintTriangle(GDC,xp2,xm->x,xp4);
            PaintTriangle(GDC,xp4,xm->x,xp3);
            xp1=xp3;
            xp2=xp4;
            PaintLine(GDC,xp1,xp2);
        }
    }

    for (k=pp2; k<pp2+np2-1; k++ )
    {
        GDC.SetBrush( brushes[ abrush++ ] );
        xp3 = ( 1.0 - con2[ k + 1 ] ) * xm->x  + con2[ k + 1 ] * xf->x;
        xp4 = ( 1.0 - conf[ k + 1  ] ) * x1->x + conf[ k + 1 ] * xf->x;
        PaintTriangle(GDC,xp1,xp3,xp2);
        PaintTriangle(GDC,xp2,xp3,xp4);
        xp1=xp3; xp2=xp4;
        PaintLine(GDC,xp1,xp2);
    }
    if ( np2 != 0 )
    {
        GDC.SetBrush( brushes[ abrush++ ] );
        PaintTriangle(GDC,xp2,xp1,xf->x);
    }

}


void CContour::PaintLine( CGraphDevice &GDC, const mvector &x1, const mvector &x2 )
{
    if ( !paint_contour || paint_mode != PM_LINES ) return;
    if ( paint_map ) GDC.SetPen( DPen( DColor(0,0,0), 0, 0.5 ) );
    else GDC.SetPen( pens[ abrush - 1 ] );
    switch( dim )
    {
        case 2: GDC.Line( x1, x2 ); break;
        case 3: GDC.Line3D( x1, x2 ); break;
    }
    GDC.SetNullPen();

}

void CContour::PaintTriangle( CGraphDevice &GDC, const mvector &x1, const mvector &x2, const mvector &x3)
{
    if (!paint_map || paint_mode != PM_MAP ) return;
    if ( is_right )
    {
        switch( dim )
        {
            case 2: GDC.Triangle2D( x1, x2, x3 ); break;
            case 3: GDC.Triangle3D( x1, x2, x3 ); break;
        }
    }
    else
    {
        switch( dim )
        {
            case 2: GDC.Triangle2D( x1, x3, x2 ); break;
            case 3: GDC.Triangle3D( x1, x3, x2 ); break;
        }
    }
}

void CContour::triangleContour( CGraphDevice &GDC, const CContourVertex &c1, const CContourVertex &c2, const CContourVertex &c3  )
{
    if ( paint_mode == PM_MAP && !paint_map ) return;
    if ( paint_mode == PM_LINES && !paint_contour ) return;

    unsigned dim = c1.x.GetDim( );
    mvector  xp1 = c1.x, xp2 = c2.x, xp3, xp4;
    int dind = c3.v > c1.v ? 1 : -1;
    double alpha = 1.0, bv = c1.v + dind * dv / 2.0;
    int ind = FindContourIndex( bv );
    if ( dind < 0 ) ind++;
    CScaler sc( c1.v, c3.v, 1.0, 0.0 );
    do
    {
        ind += dind;
        alpha = sc.scale( contours[ ind ] );
        if ( ( alpha <= 0.0 ) ) break;
        xp3 = alpha * c1.x + ( 1.0 - alpha ) * c3.x;
        xp4 = alpha * c2.x + ( 1.0 - alpha ) * c3.x;
        GDC.SetBrush( brushes[ dind > 0 ? ind-1 : ind ] );
        switch( dim )
        {
            case 2: GDC.Triangle2D( xp1, xp2, xp4 );
                    GDC.Triangle2D( xp1, xp4, xp3 );
                    break;

            case 3: GDC.Triangle3D( xp1, xp2, xp4 );
                    GDC.Triangle3D( xp1, xp4, xp3 );
                    break;
        }
        xp1  = xp3;
        xp2  = xp4;
    } while( true );
    ind -= dind;
    xp3 = c3.x;
    GDC.SetBrush( brushes[ dind > 0 ? ind-1 : ind ] );
    switch( dim )
    {
        case 2: GDC.Triangle2D( xp1, xp2, xp3 ); break;
        case 3: GDC.Triangle3D( xp1, xp2, xp3 ); break;
    }
}
