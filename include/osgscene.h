// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along      with      this      program.       If      not,      see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#ifndef OSGSCENE_H
#define OSGSCENE_H

//#include <QGLWidget>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/StateSet>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PointSprite>
//#include <osg/BlendFunc>
#include <osg/StateAttribute>
#include <osg/Point>
//#include <osg/ClearNode>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osgUtil/Optimizer>
#include <osg/Depth>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgText/Text>
#include <osgSim/ColorRange>

#include "miscfunc.h"
#include "atoms.h"
#include "adds.h"
#include "lattice.h"
#include "picking.h"
/*
//#include "../FEMAppLib/FEMProject.h"
#include "../FEMApp/FEMAppLib/FEMAppInterface.h"
#include "../FEMApp/MolecularLib/MException.h"
#include "../FEMApp/XMLLib/xml_exceptions.h"
#include "../FEMApp/FEMLib/FEException.h"
#include "../FEMApp/MolecularLib/MException.h"
#include "../FEMApp/LangLib/LangException.h"
//#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1]) // inline double xsect(int p1, int p2) { return (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1]);  }
//#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1]) // inline double ysect(int p1, int p2) { return (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1];  }
*/
class OsgScene : public QObject, public osg::Group
{
    Q_OBJECT
public:
    OsgScene();
    ~OsgScene();
    osg::ref_ptr<osg::Group> getScene() { return m_scene.get(); }
    const osg::ref_ptr<osg::Group> getScene() const { return m_scene.get(); }
    void displayPlane(glm::dmat3 rotTens, int nA);
    void showOneAtom(int ind);
    void displayAdds(bool sw);
    void displayMarked();
    
private:
   
    osg::ref_ptr<osg::Geode> geode0;
    osg::ref_ptr<osg::MatrixTransform> m_worldAdds;
    osg::ref_ptr<osg::Group> createScene();
    osg::ref_ptr<osg::Group> createSceneLight();
    osg::ref_ptr<osg::MatrixTransform> createAtoms();
    osg::ref_ptr<osg::MatrixTransform> createImage();
    osg::ref_ptr<osg::MatrixTransform> createFem();
    osg::ref_ptr<osg::MatrixTransform> createRes();
    
    //int conrec(double **d, int ilb, int iub, int jlb, int jub, double *x, double *y, int nc, double *z);
    //int conrec(double **d, int ilb, int iub, int jlb, int jub, double *x, double *y, int nc, double *z, osg::ref_ptr<osg::Vec3Array> vertL, osg::ref_ptr<osg::Vec3Array> vertT, osg::ref_ptr<osg::Vec4Array> colorT, CGeometricObjectShapeNodal* face);
    //int conrec(double **d, int imax, double *t, int nc, double *z, CGeometricObjectShapeNodal* face);
    
    inline double xsect(int p1, int p2) {
       return (conrec_h[p2]*conrec_xh[p1]-conrec_h[p1]*conrec_xh[p2])/(conrec_h[p2]-conrec_h[p1]);  
    }
    inline double ysect(int p1, int p2) {
       return (conrec_h[p2]*conrec_yh[p1]-conrec_h[p1]*conrec_yh[p2])/(conrec_h[p2]-conrec_h[p1]);  
    }
    double conrec_h[5], conrec_xh[5], conrec_yh[5];
    osg::ref_ptr<osgText::Text> createText(const osg::Vec3& pos, const std::string& content, float size, osg::Vec4 color);
    osg::ref_ptr<osg::MatrixTransform> createNum();

    osg::ref_ptr<osg::Geometry> makeSphere(double radius, int slices, int stacks, osg::Vec4 col);    
    osg::ref_ptr<osg::MatrixTransform> createAxis();
    osg::ref_ptr<osg::MatrixTransform> createAdds();
    osg::ref_ptr<osg::MatrixTransform> drawArrow(osg::Vec3 point, osg::Vec3 dir, float l, float r, float prop1, float prop2, osg::Vec4 color);
    osg::ref_ptr<osg::MatrixTransform> drawBond(osg::Vec3 point1, osg::Vec3 point2, float r, osg::Vec4 color);
    osg::StateSet* makeStateSet(float size);
    
    osg::ref_ptr<osg::MatrixTransform> m_worldReferenceFrame;
    osg::ref_ptr<osg::MatrixTransform> m_worldAt;
    osg::ref_ptr<osg::Group> m_switchRoot;//osg::Switch *m_switchRoot;

    osg::TessellationHints *hints;
    osg::ref_ptr<osg::Group> m_scene;
    double scDim;
    
    osg::ref_ptr<osg::Vec4Array> colorL;
    double **value;
    double *tablica;
    int ncol;
    int ndiv;
    double *wart;
    osg::ref_ptr<osg::Geode> geo;
/*      
public slots:

    void SL_doRotation(osg::Matrix *mat);
*/
};


#endif // OSGSCENE_H
