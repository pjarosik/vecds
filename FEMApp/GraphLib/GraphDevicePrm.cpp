#include "GraphDevicePrm.h"
#include "../XMLLib/XML.h"

// 2D functions

//void Rectangle( const mvector &p, double a, double b );
void CGraphDeviceStorage::Rectangle( double x1, double y1, double x2, double y2 )
{

    triangles2D[ brush ].push_back( tr2d( x1, y1, x2, y2, x1, y2 ) );
    triangles2D[ brush ].push_back( tr2d( x1, y1, x2, y1, x2, y2 ) );
}

void CGraphDeviceStorage::Triangle2D( double x1, double y1, double x2, double y2, double x3, double y3 )
{
    triangles2D[ brush ].push_back( tr2d( x1, y1, x2, y2, x3, y3 ) );
}

//void Triangle( const mvector&x1, const mvector &x2 ,const mvector&x3 );
void CGraphDeviceStorage::Line2D( double x1, double y1, double x2, double y2 )
{
    lines2D[ pen ].push_back( ln2d( x1, y1, x2, y2 ) );
}

//void Line( const mvector&x1, const mvector &x2 );
void CGraphDeviceStorage::Polygon2D( vector<DPoint2D> &dp )
{
    double xs, ys;
    vector<DPoint2D>::iterator i;
    for (i=dp.begin(); i!=dp.end(); i++)
    {
        xs += i->GetX();
        ys += i->GetY();
    }
    xs /= (double)dp.size();
    ys /= (double)dp.size();
    unsigned k;
    for (k=1; k<dp.size(); k++)
        triangles2D[ brush ].push_back( tr2d( dp[ k-1 ].GetX(), dp[ k-1 ].GetY(), dp[ k ].GetX(), dp[ k ].GetY(), xs, ys ) );
}

void CGraphDeviceStorage::Polyline2D( vector<DPoint2D> &dp )
{
    unsigned k;
    for (k=1; k<dp.size(); k++)
        lines2D[ pen ].push_back( ln2d( dp[ k-1 ].GetX(), dp[ k-1 ].GetY(), dp[ k ].GetX(), dp[ k ].GetY() ) );
}

void CGraphDeviceStorage::Ellipse( double,double,double,double )
{

}

//    void WriteInBox2D(double,double,double,double,const string &str, TextJustify tj = TJ_LC );
//    void WriteInBox2D(const DRect &r,const string &str, TextJustify tj = TJ_LC ) { WriteInBox2D(r.GetX(), r.GetY(), r.GetX()+r.GetL(), r.GetY()+r.GetH(), str, tj); }
//void Triangle2D( const mvector &x1, const mvector &x2, const mvector &x3 );

// 3D functions

void CGraphDeviceStorage::Triangle3D( const mvector &p1, const mvector &p2, const mvector &p3 )
{
    triangles3D[ brush ].push_back( tr3d(    p1[0], p1[1], p1[2],
                                    p2[0], p2[1], p2[2],
                                    p3[0], p3[1], p3[2] )
            );
}

void CGraphDeviceStorage::Line3D( const mvector &p1, const mvector &p2 )
{
    lines3D[ pen ].push_back( ln3d(  p1[0], p1[1], p1[2],
                            p2[0], p2[1], p2[2] )
                );
}

void CGraphDeviceStorage::Line3D( double x1, double y1, double z1, double x2, double y2, double z2 )
{
    lines3D[ pen ].push_back( ln3d( x1,y1,z1,
                           x2,y2,z2 )
                    );
}

void CGraphDeviceStorage::Point( const mvector &p, double size )
{
    points3D[ pen ].push_back( pt3d( p[0], p[1], p[2] ) );
}

void CGraphDeviceStorage::WriteText( const mvector &p, const string &tx )
{
    strX.push_back( p );
    strS.push_back(tx);
}


// Atributes


void CGraphDeviceStorage::SetPen( const DPen &p )
{
    pen = p;
}

void CGraphDeviceStorage::SetPredefinedPen( unsigned int i )
{
    pen = penarray[i];
}

void CGraphDeviceStorage::CreatePenArray( const vector<DPen> &pa )
{
    penarray = pa;
}

void CGraphDeviceStorage::SelectArrayPen( unsigned int i )
{
    pen = penarray[i];
}

void CGraphDeviceStorage::SetNullPen()
{

}

void CGraphDeviceStorage::SetBrush( const DBrush &br )
{
    brush = br;
}

void CGraphDeviceStorage::SetPredefinedBrush( unsigned int )
{

}

void CGraphDeviceStorage::CreateBrushArray( const vector<DBrush> &ba )
{
    brusharray = ba;
}

void CGraphDeviceStorage::SelectArrayBrush( unsigned int i )
{
    brush = brusharray[ i ];
}

void CGraphDeviceStorage::SetHollowBrush( )
{

}

void CGraphDeviceStorage::SetAspect( double, double )
{

}

void CGraphDeviceStorage::MakeContext()
{
    Reset();
}

void CGraphDeviceStorage::FlushToTxT()
{
   ofstream fout( (name + "_graph.txt").c_str() );

   map< DPen, vector<pt2d> >::const_iterator ip2;
   fout << "GRAPH_DEVICE" << endl;
   fout << points2D.size() << endl;
   for ( ip2 = points2D.begin(); ip2 != points2D.end(); ip2++ )
   {
           const DPen &pen=ip2->first;
           fout <<  pen.GetStyle() << " " <<
                    pen.GetThickness() << " " <<
                    pen.GetColor().GetR() << " " <<
                    pen.GetColor().GetG() << " " <<
                    pen.GetColor().GetB() << " " <<
                    pen.GetColor().GetAlpha() << endl;

           const vector<pt2d> &pts = ip2->second;
           vector<pt2d>::const_iterator i;
           fout << pts.size() << endl;
           for (i=pts.begin(); i!=pts.end(); i++)
               fout << i->x() << " " << i->y() << endl;
   }

   fout << endl;

   map< DPen, vector<pt3d> >::const_iterator ip3;
   fout << points3D.size() << endl;
   for ( ip3 = points3D.begin(); ip3 != points3D.end(); ip3++ )
   {
           const DPen &pen=ip3->first;
           fout <<  pen.GetStyle() << " " <<
                    pen.GetThickness() << " " <<
                    pen.GetColor().GetR() << " " <<
                    pen.GetColor().GetG() << " " <<
                    pen.GetColor().GetB() << " " <<
                    pen.GetColor().GetAlpha() << endl;

           const vector<pt3d> &pts3 = ip3->second;
           vector<pt3d>::const_iterator i;
           fout << pts3.size() << endl;
           for (i=pts3.begin(); i!=pts3.end(); i++)
               fout << i->x() << " " << i->y() << " " << i->z() << endl;
   }

   fout << endl;

   map< DPen, vector<ln2d> >::const_iterator il;
   fout << lines2D.size() << endl;
   for ( il = lines2D.begin(); il != lines2D.end(); il++ )
   {
        const DPen &pen=il->first;

        fout <<  pen.GetStyle() << " " <<
                 pen.GetThickness() << " " <<
                 pen.GetColor().GetR() << " " <<
                 pen.GetColor().GetG() << " " <<
                 pen.GetColor().GetB() << " " <<
                 pen.GetColor().GetAlpha() << endl;

               const vector< ln2d > &lns2d = il->second;
               vector< ln2d >::const_iterator i;
               fout << lns2d.size() << endl;
               for (i=lns2d.begin(); i!=lns2d.end(); i++)
                   fout << i->p1().x() << " " << i->p1().y() << "     "
                        << i->p2().x() << " " << i->p2().y() << endl;

    }

    fout << endl;

   map< DPen, vector<ln3d> >::const_iterator il3d;
   fout << lines3D.size() << endl;
   for ( il3d = lines3D.begin(); il3d != lines3D.end(); il3d++ )
   {
        const DPen &pen=il3d->first;
        fout <<  pen.GetStyle() << " " <<
                 pen.GetThickness() << " " <<
                 pen.GetColor().GetR() << " " <<
                 pen.GetColor().GetG() << " " <<
                 pen.GetColor().GetB() << " " <<
                 pen.GetColor().GetAlpha() << endl;

               const vector< ln3d > &lns3d = il3d->second;
               vector< ln3d >::const_iterator i;
               fout << lns3d.size() << endl;
               for (i=lns3d.begin(); i!=lns3d.end(); i++)
                   fout << i->p1().x() << " " << i->p1().y() << " " << i->p1().z() << "     "
                        << i->p2().x() << " " << i->p2().y() << " " << i->p2().z() << endl;

    }

    fout << endl;

    map< DBrush, vector<tr2d> >::const_iterator it2d;
    fout << triangles2D.size() << endl;
    for ( it2d = triangles2D.begin(); it2d != triangles2D.end(); it2d++ )
       {
           const DBrush &brush=it2d->first;
           fout << brush.GetStyle()  << " " <<
                   brush.GetColor().GetR() << " " <<
                   brush.GetColor().GetG() << " " <<
                   brush.GetColor().GetB() << " " <<
                   brush.GetColor().GetAlpha() << endl;
               const vector<tr2d> &trs2d = it2d->second;
               fout << trs2d.size() << endl;
               vector<tr2d>::const_iterator i;
               for (i=trs2d.begin(); i!=trs2d.end(); i++)
                   fout << i->p1().x() << " " << i->p1().y() << "     "
                        << i->p2().x() << " " << i->p2().y() << "     "
                        << i->p3().x() << " " << i->p3().y() << endl;
       }

    fout << endl;

    map< DBrush, vector<tr3d> >::const_iterator it3d;
    fout << triangles3D.size() << endl;
    for ( it3d = triangles3D.begin(); it3d != triangles3D.end(); it3d++ )
       {
           const DBrush &brush=it3d->first;
           fout << brush.GetStyle()  << " " <<
                   brush.GetColor().GetR() << " " <<
                   brush.GetColor().GetG() << " " <<
                   brush.GetColor().GetB() << " " <<
                   brush.GetColor().GetAlpha() << endl;
               const vector<tr3d> &trs3d = it3d->second;
               vector<tr3d>::const_iterator i;
               fout << trs3d.size() << endl;
               for (i=trs3d.begin(); i!=trs3d.end(); i++)
                   fout << i->p1().x() << " " << i->p1().y() << " " << i->p1().z() << "     "
                        << i->p2().x() << " " << i->p2().y() << " " << i->p2().z()<< "     "
                        << i->p3().x() << " " << i->p3().y() << " " << i->p3().z() << endl;
       }

     fout << endl;

     fout << strX.size() << endl;
     unsigned k;
     for (k=0; k<strX.size(); k++)
     {
        fout << strX[k][0] << " " << strX[k][1] << " " << strX[k][2] << " " << strS[k] << endl;
     }
}

void CGraphDeviceStorage::DeleteContext()
{
    if ( !to_memory ) FlushToTxT();
}

void CGraphDeviceStorage::FlushToXml()
{

    CXML xml;
    CXML_OutputDevice od( name + ".xml" );
    CXML_Creator creator( od );

    creator.AddHeader("version=\"1.0\"");
    creator.OpenNode("GraphDevicePRM");
        creator.OpenNode("points2D");
            map< DPen, vector<pt2d> >::const_iterator ip2;
            for ( ip2 = points2D.begin(); ip2 != points2D.end(); ip2++ )
            {
                const DPen &pen=ip2->first;
                creator.OpenNode("Points2D_Set");
                    creator.AddParameter( "style", pen.GetStyle() );
                    creator.AddParameter( "width", pen.GetThickness() );
                    creator.AddParameter( "R", pen.GetColor().GetR());
                    creator.AddParameter( "G", pen.GetColor().GetG());
                    creator.AddParameter( "B", pen.GetColor().GetB());
                    creator.AddParameter( "A", pen.GetColor().GetAlpha());
                    const vector<pt2d> &pts = ip2->second;
                    vector<pt2d>::const_iterator i;
                    for (i=pts.begin(); i!=pts.end(); i++)
                    {
                        creator.OpenNode("point2D");
                            creator.AddParameter("x",i->x() );
                            creator.AddParameter("y",i->y() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }

        creator.CloseNode();

        creator.OpenNode("Points3D");
            map< DPen, vector<pt3d> >::const_iterator ip3;
            for ( ip3 = points3D.begin(); ip3 != points3D.end(); ip3++ )
            {
                const DPen &pen=ip3->first;
                creator.OpenNode("Points3D_Set");
                    creator.AddParameter( "style", pen.GetStyle() );
                    creator.AddParameter( "width", pen.GetThickness() );
                    creator.AddParameter( "R", pen.GetColor().GetR());
                    creator.AddParameter( "G", pen.GetColor().GetG());
                    creator.AddParameter( "B", pen.GetColor().GetB());
                    creator.AddParameter( "A", pen.GetColor().GetAlpha());
                    const vector<pt3d> &pts = ip3->second;
                    vector<pt3d>::const_iterator i;
                    for (i=pts.begin(); i!=pts.end(); i++)
                    {
                        creator.OpenNode("point3D");
                            creator.AddParameter("x",i->x() );
                            creator.AddParameter("y",i->y() );
                            creator.AddParameter("z",i->z() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }

        creator.CloseNode();

        creator.OpenNode("lines2D");
            map< DPen, vector<ln2d> >::const_iterator il;
            for ( il = lines2D.begin(); il != lines2D.end(); il++ )
            {
                const DPen &pen=il->first;
                creator.OpenNode("Lines2D_Set");
                    creator.AddParameter( "style", pen.GetStyle() );
                    creator.AddParameter( "width", pen.GetThickness() );
                    creator.AddParameter( "R", pen.GetColor().GetR());
                    creator.AddParameter( "G", pen.GetColor().GetG());
                    creator.AddParameter( "B", pen.GetColor().GetB());
                    creator.AddParameter( "A", pen.GetColor().GetAlpha());
                    const vector<ln2d> &pts = il->second;
                    vector<ln2d>::const_iterator i;
                    for (i=pts.begin(); i!=pts.end(); i++)
                    {
                        creator.OpenNode("line2D");
                            creator.AddParameter("x1",i->p1().x() );
                            creator.AddParameter("y1",i->p1().y() );
                            creator.AddParameter("x2",i->p2().x() );
                            creator.AddParameter("y2",i->p2().y() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }

        creator.CloseNode();

        creator.OpenNode("lines3D");
            map< DPen, vector<ln3d> >::const_iterator il3d;
            for ( il3d = lines3D.begin(); il3d != lines3D.end(); il3d++ )
            {
                const DPen &pen = il3d->first;
                creator.OpenNode("Lines3D_Set");
                    creator.AddParameter( "style", pen.GetStyle() );
                    creator.AddParameter( "width", pen.GetThickness() );
                    creator.AddParameter( "R", pen.GetColor().GetR());
                    creator.AddParameter( "G", pen.GetColor().GetG());
                    creator.AddParameter( "B", pen.GetColor().GetB());
                    creator.AddParameter( "A", pen.GetColor().GetAlpha());
                    const vector<ln3d> &pts = il3d->second;
                    vector<ln3d>::const_iterator i;
                    for (i=pts.begin(); i!=pts.end(); i++)
                    {
                        creator.OpenNode("line3D");
                            creator.AddParameter("x1",i->p1().x() );
                            creator.AddParameter("y1",i->p1().y() );
                            creator.AddParameter("z1",i->p1().z() );
                            creator.AddParameter("x2",i->p2().x() );
                            creator.AddParameter("y2",i->p2().y() );
                            creator.AddParameter("z2",i->p2().z() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }

        creator.CloseNode();

        creator.OpenNode("triangles2D");
            map< DBrush, vector<tr2d> >::const_iterator tr;
            for ( tr = triangles2D.begin(); tr != triangles2D.end(); tr++ )
            {
                const DBrush &brush=tr->first;
                creator.OpenNode("Triangles2D_Set");
                    creator.AddParameter( "style", brush.GetStyle() );
                    creator.AddParameter( "R", brush.GetColor().GetR());
                    creator.AddParameter( "G", brush.GetColor().GetG());
                    creator.AddParameter( "B", brush.GetColor().GetB());
                    creator.AddParameter( "A", brush.GetColor().GetAlpha());
                    const vector<tr2d> &trs = tr->second;
                    vector<tr2d>::const_iterator i;
                    for (i=trs.begin(); i!=trs.end(); i++)
                    {
                        creator.OpenNode("triangle2D");
                            creator.AddParameter("x1",i->p1().x() );
                            creator.AddParameter("y1",i->p1().y() );
                            creator.AddParameter("x2",i->p2().x() );
                            creator.AddParameter("y2",i->p2().y() );
                            creator.AddParameter("x3",i->p3().x() );
                            creator.AddParameter("y3",i->p3().y() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }

        creator.CloseNode();

        creator.OpenNode("triangles3D");
            map< DBrush, vector<tr3d> >::const_iterator tr3;
            for ( tr3 = triangles3D.begin(); tr3 != triangles3D.end(); tr3++ )
            {
                const DBrush &brush=tr->first;
                creator.OpenNode("Triangles3D_Set");
                    creator.AddParameter( "style", brush.GetStyle() );
                    creator.AddParameter( "R", brush.GetColor().GetR());
                    creator.AddParameter( "G", brush.GetColor().GetG());
                    creator.AddParameter( "B", brush.GetColor().GetB());
                    creator.AddParameter( "A", brush.GetColor().GetAlpha());
                    const vector<tr3d> &trs = tr3->second;
                    vector<tr3d>::const_iterator i;
                    for (i=trs.begin(); i!=trs.end(); i++)
                    {
                        creator.OpenNode("triangle2D");
                            creator.AddParameter("x1",i->p1().x() );
                            creator.AddParameter("y1",i->p1().y() );
                            creator.AddParameter("z1",i->p1().z() );

                            creator.AddParameter("x2",i->p2().x() );
                            creator.AddParameter("y2",i->p2().y() );
                            creator.AddParameter("z2",i->p2().z() );

                            creator.AddParameter("x3",i->p3().x() );
                            creator.AddParameter("y3",i->p3().y() );
                            creator.AddParameter("z3",i->p3().z() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }

        creator.CloseNode();
    creator.CloseNode();
    xml.Create(creator);
}


void CGraphDeviceStorage::AssignFont( const char *nm, double ws )
{

}

double CGraphDeviceStorage::GetFontHeight()
{

}

double CGraphDeviceStorage::GetTextWidth( const char *tx )
{

}

void CGraphDeviceStorage::SetFontHeight( double, bool absolute  )
{

}

double CGraphDeviceStorage::TranslateTextHeight( double )
{

}

void CGraphDeviceStorage::Write2D( double,double,const char* )
{

}

void CGraphDeviceStorage::WriteVerically2D( double,double,const char* )
{

}

void CGraphDeviceStorage::Reset()
{
    points2D.clear();
    points3D.clear();

    lines2D.clear();
    lines3D.clear();

    triangles2D.clear();
    triangles3D.clear();
    strX.clear();
    strS.clear();
}
