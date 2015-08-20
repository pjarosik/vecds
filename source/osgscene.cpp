// --------------------------------------------- -------
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

#include "osgscene.h"
//#include "osgv.h"

//namespace M_SC {

extern Atoms *AT;
extern Lattice *LATT;
extern Internal *INT;
extern Adds *ADDS;

//geom->setUseDisplayList(false);
//geom->setUseVertexBufferObjects(true); 

OsgScene::OsgScene()
{
 qWarning("osgScene 0");
    hints = new osg::TessellationHints;
    hints->setDetailRatio(INT->detRatio);
    m_switchRoot = new osg::Group;//Switch();
    //m_switchRoot->setName("rootSwitch");
    if ( INT->lightOn ) m_switchRoot->addChild(createSceneLight());
    osg::StateSet* state = m_switchRoot->getOrCreateStateSet();
    state->setMode( GL_LIGHTING, osg::StateAttribute::ON );
    osg::ref_ptr<osg::Material> mat = new osg::Material;
// Just use the object's primary color for ambient and
//   diffuse (uses the OpenGL color material feature).
  //qWarning("Scene 1");
  qWarning("osgScene 0-1");
    mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    mat->setDiffuse(osg::Material::FRONT, INT->matDiffuse);//osg::Vec4( .6f, .4f, 0.3f, 1.f ) );
    mat->setSpecular(osg::Material::FRONT, INT->matSpecular);//osg::Vec4( .6f, .6f, .5f, 1.f ) );
    mat->setShininess(osg::Material::FRONT, INT->matShiness);//15.f);//20.f );    
    if ( INT->materialOn ) state->setAttribute( mat.get() );    
//    state->setMode( GL_LIGHTING,osg::StateAttribute::OFF |osg::StateAttribute::PROTECTED );
    state->setMode( GL_BLEND, osg::StateAttribute::ON );
    m_worldReferenceFrame = new osg::MatrixTransform;
    m_worldReferenceFrame->setMatrix(osg::Matrix::rotate(osg::Y_AXIS, osg::Z_AXIS));
  qWarning("osgScene 0-2"); 
    m_scene = createScene();
 qWarning("osgScene - END");
}

OsgScene::~OsgScene()
{
//  delete FEMi;
}

osg::ref_ptr<osg::Group> OsgScene::createScene()
{
qWarning("createScene 0");
    if ( INT->refrAtoms  && !INT->atName.empty() )	INT->m_worldAt = createAtoms();
    
    if ( INT->refrFem && (!INT->fem.empty() || !INT->compFem.empty()) ) INT->m_fem = createFem();    

    if ( INT->refrImage && !INT->image.empty() ) INT->m_image = createImage();
    
//    if ( INT->refrAdds && ADDS->n_adds>0 ) INT->m_worldAdds = createAdds();	
        
    if ( !INT->atName.empty() || !INT->fem.empty() || !INT->compFem.empty() ) {
        
	if ( !INT->atName.empty() )                            m_worldReferenceFrame.get()->addChild(INT->m_worldAt.get());
	if ( !INT->fem.empty() || !INT->compFem.empty() )      m_worldReferenceFrame.get()->addChild(INT->m_fem.get());
	if ( !INT->image.empty() )                             m_worldReferenceFrame.get()->addChild(INT->m_image.get());
	if ( INT->axInd!=0 )                                   m_worldReferenceFrame.get()->addChild(createAxis().get());
 	if ( INT->showNum && !INT->atName.empty() )            m_worldReferenceFrame.get()->addChild(createNum().get());
    //qWarning("-------------------------   +++++++++++++++++++++++++++++++     1");
	if ( (!INT->fem.empty() || !INT->compFem.empty()) && INT->refrRes && !INT->choosen_value.empty() )  m_worldReferenceFrame.get()->addChild(createRes().get());
    //qWarning("-------------------------   +++++++++++++++++++++++++++++++     2");
    }
    m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
    if ( INT->optim ) {
       osgUtil::Optimizer optimizer;
       optimizer.optimize( m_switchRoot.get() );
    }
    if ( INT->showAdds && ADDS->n_adds>0 )                 displayAdds(true);        
 qWarning("-------------------------   createScene  - END  +++++++++++++++++++++++++++++++ ");
    return m_switchRoot.get();
}

osg::ref_ptr<osg::MatrixTransform> OsgScene::createAtoms()
{
 qWarning("createAtoms 0");
    osg::ref_ptr<osg::MatrixTransform> m_worldAt = new osg::MatrixTransform;
    osg::StateSet* set = m_worldAt->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON); 
    if ( INT->fem.empty() ) {
       INT->xMin = LATT->xMin;  INT->xMax = LATT->xMax;
       INT->yMin = LATT->yMin;  INT->yMax = LATT->yMax;
       INT->zMin = LATT->zMin;  INT->zMax = LATT->zMax;
    } /*
    } else {
       INT->xMin = std::min(fxMin, LATT->xMin);  INT->xMax = std::max(fxMax, LATT->xMax);
       INT->yMin = std::min(fyMin, LATT->yMin);  INT->yMax = std::max(fyMax, LATT->yMax);
       INT->zMin = std::min(fzMin, LATT->zMin);  INT->zMax = std::max(fzMax, LATT->zMax);
    } */
    INT->scDim = std::max(INT->xMax-INT->xMin, std::max(INT->yMax-INT->yMin, INT->zMax-INT->zMin));    
    osg::Vec4 color = INT->bondColor;
    if ( INT->alphaAt<1.0 ) color.w() = INT->alphaAt;
    if ( INT->bondFactor>0. && INT->alphaB>0. ) {
       color[3] = INT->alphaB;
       float rCyl = static_cast<float>(INT->bondFactor * AT->a_rad1[0]*INT->radFactor);  //---------------------------------
    //std::cout << " rCyl=" << rCyl << std::endl;
       for (int i=0; i<LATT->n_bonds; i++) {
         int i1 = LATT->bond1.get()->at(i) - 1;
	 int i2 = LATT->bond2.get()->at(i) - 1;
 	 osg::Vec3 p1 = LATT->coords.get()->at(i1);
         osg::Vec3 p2 = LATT->coords.get()->at(i2);
         m_worldAt->addChild(drawBond(p1, p2, rCyl, color));
       }
    }  
    for (int i=0; i<LATT->n_k; i++) {
        int ak = LATT->nK.get()->at(i);
    	float r = INT->radFactor * AT->a_rad1[ak];
	QString aN = AT->namea.at(ak);
	osg::Vec4 color = AT->a_colors[ak];
	if ( INT->alphaAt<1.0 ) color.w() = INT->alphaAt;
     //std::cout << " ak=" << ak << "   r=" << r << "     red=" << color.x() << "     alfa=" << color.w() << std::endl;
	osg::ref_ptr<osg::Geometry> draw = makeSphere(r, INT->sphSlices, INT->sphStacks, color);
	for (int j=0; j<LATT->n_atoms; j++) {
	     int akk = LATT->nAt.get()->at(j);
	     if ( akk==ak ) {
	         osg::ref_ptr<osg::Geode> geo = new osg::Geode;
	         geo->addDrawable(draw.get());
	         QString str1;
	         str1.setNum(j+1);
	         QString str = QString("%1.%2").arg(aN).arg(str1);
		 geo->setName(str.toStdString());
		 osg::Vec3 pos = LATT->coords.get()->at(j);
		 osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
		 mt->setMatrix(osg::Matrix::translate(pos));
		 mt->addChild(geo.get());
		 m_worldAt->addChild(mt.get());
	     }
	}
    }
    INT->refrAtoms = false;
    INT->m_worldAt = m_worldAt;
    return m_worldAt.get();
}

void OsgScene::displayAdds(bool sw)
{
    if ( !sw ) {
   std::cout << "Remove m_worldAdds" << std::endl;
      m_worldReferenceFrame.get()->removeChild(m_worldAdds.get());
      m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
      return;  
    }
    m_worldAdds = new osg::MatrixTransform;
    osg::StateSet* set = m_worldAdds->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON); 
    for (int i=0; i<ADDS->n_adds; i++) {
       //glm::dvec3 bV = ADDS->rotTens.at(i) * ADDS->fracts.at(i) * (&(INT->structList[ADDS->crCNum.at(i)]))->c2o * ADDS->millerVs.at(i);
       glm::dvec3 bV = ADDS->rotTens.at(i) * ADDS->fracts.at(i) * (INT->structList[ADDS->crCNum.at(i)]).c2o * ADDS->millerVs.at(i);
       float length = glm::length(bV); 
       float radius = INT->axRad * length;
       //osg::ref_ptr<osg::Geode> geod = new osg::Geode;
       //geod->addDrawable(ADDS->markers[0].get());
       osg::Vec3 pos = ADDS->pos.get()->at(i);//MiscFunc::convert(a.pos);
       //osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
       //mt->setMatrix(osg::Matrix::translate(pos));
       m_worldAdds->addChild(drawArrow(pos, osg::X_AXIS, length, radius, INT->axPr1, INT->axPr2, osg::Vec4(0.5, 0.5, 0.5, 0.5))); //(geod.get());
//       m_worldAdds->addChild(mt.get());
   }
   m_worldReferenceFrame.get()->addChild(m_worldAdds.get());
   m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
}

void OsgScene::displayPlane(glm::dmat3 rotTens, int nA)
{
   if ( nA<0 )  {
   std::cout << "Remove geode0" << std::endl;
      m_worldReferenceFrame.get()->removeChild(geode0.get());
      m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
      return;  
   }
   geode0 = new osg::Geode();   //osg::ref_ptr<osg::Billboard> geode0 = new osg::Billboard();
   osg::ref_ptr<osg::Geometry> square = new osg::Geometry();
   osg::Vec3Array* squareVertices = new osg::Vec3Array;
   osg::ref_ptr<osg::Geometry> square1 = new osg::Geometry();
   osg::Vec3Array* squareVertices1 = new osg::Vec3Array;//std::cout << " ++==++==++  displayPlane 1" << std::endl;
   glm::dvec3 p1, p2, p3, p4;
   MiscFunc::ComputeRect(LATT->coords, rotTens, nA, p1, p2, p3, p4);
   glm::dvec3 pp1 = p1 * rotTens;
   glm::dvec3 pp2 = p2 * rotTens;
   glm::dvec3 pp3 = p3 * rotTens;
   glm::dvec3 pp4 = p4 * rotTens; 
   glm::dvec3 ppCentr = 0.25*(pp1+pp2+pp3+pp4);
   glm::dvec3 pp0 = MiscFunc::convert(LATT->coords.get()->at(nA)) *  rotTens;//INT->actPoint * rotTens;
   int ak = LATT->nAt.get()->at(nA); //std::cout << "DisplayPlane:   ak=" << ak << std::endl;//float r = INT->radFactor * AT->a_rad1[ak];   
   double fact = double(2.85 * INT->radFactor * AT->a_rad1[ak]) / glm::length(pp1-pp3);
   glm::dvec3 pp11 = pp0 + fact*(pp1-ppCentr);
   glm::dvec3 pp12 = pp0 + fact*(pp2-ppCentr);
   glm::dvec3 pp13 = pp0 + fact*(pp3-ppCentr);
   glm::dvec3 pp14 = pp0 + fact*(pp4-ppCentr);
// std::cout << " fxMin=" << fxMin << " fyMin=" << fyMin << " fzMin=" << fzMin << std::endl;//std::cout << " fxMax=" << fxMax << " fyMax=" << fyMax << std::endl;
   squareVertices->push_back(MiscFunc::convert(pp1));
   squareVertices->push_back(MiscFunc::convert(pp2));
   squareVertices->push_back(MiscFunc::convert(pp3));
   squareVertices->push_back(MiscFunc::convert(pp4));
   square->setVertexArray(squareVertices);
   square->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, 4));
   osg::ref_ptr<osg::Vec4Array> color0 = new osg::Vec4Array;
   color0->push_back(osg::Vec4(1.0, 1.0, 0.5, 0.35));    
   square->setColorArray(color0);
   square->setColorBinding(osg::Geometry::BIND_OVERALL);

   squareVertices1->push_back(MiscFunc::convert(pp11));
   squareVertices1->push_back(MiscFunc::convert(pp12));
   squareVertices1->push_back(MiscFunc::convert(pp13));
   squareVertices1->push_back(MiscFunc::convert(pp14));
   square1->setVertexArray(squareVertices1);
   square1->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, 4));
   osg::ref_ptr<osg::Vec4Array> color1 = new osg::Vec4Array;
   color1->push_back(osg::Vec4(1.0, 0.5, 1.0, 0.35));    
   square1->setColorArray(color1);
   square1->setColorBinding(osg::Geometry::BIND_OVERALL);
   
   osg::ref_ptr<osg::StateSet> stateset = square->getOrCreateStateSet();//qWarning("osgScene 0");
   stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    // enable texture transparency
   stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
   stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

   geode0->setStateSet(stateset);
   geode0->addDrawable(square.get());
   geode0->addDrawable(square1.get());
   m_worldReferenceFrame.get()->addChild(geode0.get());

   m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
 std::cout << " ++==++==++  displayPlane 6" << std::endl;
}

void OsgScene::showOneAtom(int ind)
{
   osg::ref_ptr<osg::MatrixTransform> m_worldNum = new osg::MatrixTransform;
   int ak = LATT->nAt.get()->at(ind);
   float r = INT->radFactor * AT->a_rad1[ak];
   osg::Vec3 pos1 = LATT->coords.get()->at(ind) + osg::Vec3d(r, 0.0, r);
   QString str1;
   str1.setNum(ind+1);
   osg::ref_ptr<osgText::Text> text = createText(pos1, str1.toStdString(), INT->sizeTxt, INT->colBlack);
   osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
   textGeode->addDrawable(text);//(text.get());
   m_worldNum->addChild(textGeode.get());
    //return m_worldNum.get();
   m_worldReferenceFrame.get()->addChild(m_worldNum.get());//(m_worldAt.get());
   m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
} 

osg::ref_ptr<osg::Geometry> OsgScene::makeSphere(double radius, int slices, int stacks, osg::Vec4 col)
{
    float rho, drho, dtheta;
    float nsign = 1.0;

    drho = osg::PI / (float) stacks;
    dtheta = 2.*osg::PI / (float) slices;
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    osg::Vec3Array* vl = new osg::Vec3Array;
    osg::Vec3Array* nl = new osg::Vec3Array;
    osg::Vec4Array* cl = new osg::Vec4Array;
    cl->push_back(col);
	
    /* build slices as quad strips */
    for (int i = 0; i < stacks; i++ ) {
      	rho = i * drho;
	  for (int j = 0; j <= slices; j++ ) {
  	     double theta = (j == slices) ? 0.0 : j * dtheta;
	     double x = -sin(theta) * sin(rho);
	     double y = cos(theta) * sin(rho);
	     double z = nsign * cos(rho);
             osg::Vec3 normal(x*nsign, y*nsign, z*nsign);
	     normal.normalize();
	     nl->push_back(normal);
	     vl->push_back(osg::Vec3(x*radius, y*radius, z*radius));
	     x = -sin(theta) * sin(rho+drho);
	     y = cos(theta) * sin(rho+drho);
	     z = nsign * cos(rho+drho);
             normal = osg::Vec3(x*nsign, y*nsign, z*nsign);
	     normal.normalize();
	     nl->push_back(normal);
	     vl->push_back(osg::Vec3(x*radius, y*radius, z*radius));
	  }
    }
    //geometry->setUseDisplayList(false);
    //geometry->setUseVertexBufferObjects(true);
    geometry->setVertexArray(vl);
    geometry->setNormalArray(nl);
    geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        //geometry->setTexCoordArray(0, tl);
    geometry->setColorArray(cl);
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, vl->size()));
    return geometry.get();
}

osg::ref_ptr<osg::MatrixTransform> OsgScene::createNum()
{
    osg::ref_ptr<osg::MatrixTransform> m_worldNum = new osg::MatrixTransform;
//std::cout << "   marked.size() = " << LATT->marked.size() << std::endl;    
    for (int i=0; i<LATT->marked.size(); i++ ) {
        int ind = LATT->marked.at(i);
	int ak = LATT->nAt.get()->at(ind);
        float r = INT->radFactor * AT->a_rad1[ak];
        QString aN = AT->namea.at(ak);
        osg::Vec3 pos1 = LATT->coords.get()->at(ind) + osg::Vec3d(r, 0.0, r);
        QString str1;
        str1.setNum(ind+1);
        QString str = QString("%1.%2").arg(aN).arg(str1);	
        osg::ref_ptr<osgText::Text> text = createText(pos1, str.toStdString(), INT->sizeTxt, INT->colBlack);
        osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
        textGeode->addDrawable(text);//(text.get());
        m_worldNum->addChild(textGeode.get());
    }
    return m_worldNum.get();
} 

osg::ref_ptr<osg::MatrixTransform> OsgScene::createImage()
{
    if ( INT->fem.empty() && INT->atName.empty() ) { std::cout << "-------------- No atoms, no FEs" << std::endl;  return NULL; }
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(INT->image);
    if( !image.valid() ) { std::cout << "-------------- Bad image" << std::endl;  return NULL; }

    osg::ref_ptr<osg::MatrixTransform> m_image = new osg::MatrixTransform;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    // protect from being optimized away as static state
    texture->setUnRefImageDataAfterApply(true);
    //texture->setInternalFormat(GL_RGBA);
    //texture->setDataVariance(osg::Object::DYNAMIC);
    texture->setImage(image);

    osg::ref_ptr<osg::Geometry> square = new osg::Geometry();
    // draw the square
    osg::Vec3Array* squareVertices = new osg::Vec3Array;
    double zCent = 0.5*(LATT->zMin + LATT->zMax);
    squareVertices->push_back( osg::Vec3( INT->xMin, INT->yMin, zCent ) );
    squareVertices->push_back( osg::Vec3( INT->xMax, INT->yMin, zCent ) );
    squareVertices->push_back( osg::Vec3( INT->xMax, INT->yMax, zCent ) );
    squareVertices->push_back( osg::Vec3( INT->xMin, INT->yMax, zCent ) );
 qWarning("  image  - xMin, xMax, yMin, yMax =  %g, %g, %g, %g", INT->xMin, INT->xMax, INT->yMin, INT->yMax);
    
    square->setVertexArray(squareVertices);
    square->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, 4));

    // create an arraw for texture coordinates
    osg::Vec2Array* texCoords = new osg::Vec2Array(4);
    (*texCoords)[0].set( 0.0f, 0.0f );
    (*texCoords)[1].set( 1.0f, 0.0f );
    (*texCoords)[2].set( 1.0f, 1.0f );
    (*texCoords)[3].set( 0.0f, 1.0f );
    square->setTexCoordArray(0,texCoords);
    
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
    square->setNormalArray(normals);
    square->setNormalBinding(osg::Geometry::BIND_OVERALL);
            
    osg::ref_ptr<osg::StateSet> stateset = square->getOrCreateStateSet();//qWarning("osgScene 0");

    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    // enable texture transparency
    stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    
    osg::Material* material = new osg::Material;
    //material->setAmbient(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
    //material->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
    material->setAlpha(osg::Material::FRONT_AND_BACK, INT->alphaIm);
    stateset->setAttribute(material,osg::StateAttribute::ON);    
    
    stateset->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON); 

    stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    
    geode->addDrawable(square.get());

    m_image->addChild(geode.get());
    INT->refrImage = false;
    INT->m_image = m_image;
    return m_image.get();
}

osg::ref_ptr<osgText::Text> OsgScene::createText(const osg::Vec3& pos, const std::string& content, float size, osg::Vec4 color)
{
  osg::ref_ptr<osgText::Text> text = new osgText::Text;
  text->setDataVariance(osg::Object::DYNAMIC);
  //text->setFont( g_font.get() );
  text->setCharacterSize(size);
  text->setColor(color);
  //text->setFontResolution(40,40);
  text->setAxisAlignment(osgText::TextBase::XY_PLANE);
  text->setAutoRotateToScreen(true);
  text->setPosition(pos);
  text->setText(content);
  return text;//.release();
}

osg::ref_ptr<osg::Group> OsgScene::createSceneLight()
{
    osg::ref_ptr<osg::Group> grp = new osg::Group;
    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
    lightSource->setLocalStateSetModes( osg::StateAttribute::ON );

    lightSource->getLight()->setAmbient(INT->ls1Ambient);//(osg::Vec4(0.4, 0.4, 0.4, 1.0));
    lightSource->getLight()->setDiffuse(INT->ls1Diffuse);//(osg::Vec4(0.45, 0.45, 0.45, 1.0));
    lightSource->getLight()->setSpecular(INT->ls1Specular);//(osg::Vec4(0.6, 0.6, 0.6, 1.0));//(1., 1., 1., 1.)); 
    lightSource->getLight()->setPosition(INT->ls1Position);//(osg::Vec4(0.9, -0.9, 0.8, 0.));
    grp->addChild(lightSource.get()); 
    osg::ref_ptr<osg::LightSource> lightSource1 = new osg::LightSource;
    lightSource1->setLocalStateSetModes( osg::StateAttribute::ON );

    lightSource1->getLight()->setAmbient(INT->ls2Ambient);//(osg::Vec4(0.4, 0.4, 0.4, 1.0));
    lightSource1->getLight()->setDiffuse(INT->ls2Diffuse);//(osg::Vec4(0.45, 0.45, 0.45, 1.0));
    lightSource1->getLight()->setSpecular(INT->ls2Specular);//(osg::Vec4(0.6, 0.6, 0.6, 1.0));
    lightSource1->getLight()->setPosition(INT->ls2Position);
    grp->addChild(lightSource1.get()); 
    return grp;
}

osg::ref_ptr<osg::MatrixTransform> OsgScene::createAxis()
{
  //std::cout << " createAxis 0" << std::endl;
    osg::ref_ptr<osg::MatrixTransform> m_worldAx = new osg::MatrixTransform;
    osg::StateSet* set = m_worldAx->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);     
    float length = static_cast<float>(INT->axL * INT->scDim);
  std::cout << " length=" << length << std::endl;
    float radius = INT->axRad * length;
    //float proportion = INT->axPr;
    osg::Vec3 point = (INT->axInd==1) ?
                 osg::Vec3(0.5*(INT->xMax+INT->xMin), 0.5*(INT->yMax+INT->yMin), 0.5*(INT->zMax+INT->zMin))  :
                 osg::Vec3(1.05 * INT->xMax, 1.05 * INT->yMin, 1.01 * INT->zMin);
    osg::Vec4 colB = INT->colBlue;  osg::Vec4 colG = INT->colGreen;  osg::Vec4 colR = INT->colRed;
    if ( INT->alphaAx<1.0 ) { colR.w() = INT->alphaAx;  colG.w() = INT->alphaAx;  colB.w() = INT->alphaAx; }
    m_worldAx->addChild(drawArrow(point, osg::Z_AXIS, length, radius, INT->axPr1, INT->axPr2, colB));
    m_worldAx->addChild(drawArrow(point, osg::Y_AXIS, length, radius, INT->axPr1, INT->axPr2, colG));
    m_worldAx->addChild(drawArrow(point, osg::X_AXIS, length, radius, INT->axPr1, INT->axPr2, colR));
    return m_worldAx.get();
}
 
osg::ref_ptr<osg::MatrixTransform> OsgScene::drawArrow(osg::Vec3 point, osg::Vec3 dir, float l, float r, float prop1, float prop2, osg::Vec4 color)
{
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
    float l_con = l * prop1;
    float l_cyl = l - l_con;
    osg::ref_ptr<osg::ShapeDrawable> cyl = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0., 0., 0.5*l_cyl), r, l_cyl), hints);
    cyl->setColor(color);
    osg::ref_ptr<osg::ShapeDrawable> cone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0., 0., l_cyl), r*prop2, l_con), hints);
    cone->setColor(color);
    osg::ref_ptr<osg::Geode> geo = new osg::Geode;
    geo->addDrawable(cyl.get());
    geo->addDrawable(cone.get());
    mt->setMatrix(osg::Matrix::rotate(osg::Vec3(0.0, 0.0, 1.0), dir)*osg::Matrix::translate(point));
    mt->addChild(geo.get());	
    return mt.get();
}

osg::ref_ptr<osg::MatrixTransform> OsgScene::drawBond(osg::Vec3 point1, osg::Vec3 point2, float r, osg::Vec4 color) //, osg::TessellationHints)
{
    osg::ref_ptr<osg::MatrixTransform>  mt = new osg::MatrixTransform();
    osg::Vec3 dir = point2 - point1;
    float l = dir.length();
    osg::ref_ptr<osg::ShapeDrawable> cyl = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0., 0., 0.5*l), r, l), hints);
    cyl->setColor(color);
    osg::ref_ptr<osg::Geode> geo = new osg::Geode;
    geo->addDrawable(cyl.get());
    mt->setMatrix(osg::Matrix::rotate(osg::Vec3(0.0, 0.0, 1.0), dir)*osg::Matrix::translate(point1));
    mt->addChild(geo.get());	
    return mt.get();
}
/*
void OsgScene::SL_doRotation(osg::Matrix *mat)
{
    m_worldReferenceFrame->setMatrix(*mat);
}
*/
osg::StateSet* OsgScene::makeStateSet(float size)
{
    QString cd0 = INT->currDir;
    cd0.append("/data/internal/particle.rgb");
    osg::Texture2D *tex = new osg::Texture2D();
// The texture for the sprites
    tex->setImage(osgDB::readImageFile(cd0.toStdString()));
    if ( !tex ) { qWarning("-------- texture not found");  return 0; }
    osg::StateSet *set = new osg::StateSet();
// Setup cool blending
    //set->setMode(GL_BLEND, osg::StateAttribute::ON);
    //osg::BlendFunc *fn = new osg::BlendFunc();
    //fn->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::DST_ALPHA);
    //set->setAttributeAndModes(fn, osg::StateAttribute::ON);
// Setup the point sprites
    osg::PointSprite *sprite = new osg::PointSprite();
    set->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);
// Give some size to the points to be able to see the sprite
    osg::Point *point = new osg::Point();
    point->setSize(size);
    set->setAttribute(point);
    //point->setDistanceAttenuation(osg::Vec3(0.0, 1.00, 0.00000000));
   //point->setDistanceAttenuation(_point->getDistanceAttenuation()*scale);
// Disable depth test to avoid sort problems and Lighting
    set->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    set->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
    return set;
}
//===============================================================================================================================================================================
osg::ref_ptr<osg::MatrixTransform> OsgScene::createFem()
{
  qWarning("....createFem 0");  
/*  
   osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
     
   if ( INT->newcalc ) {
      INT->results.clear();
      INT->Femi = new CFEMAppInterface;    
//qWarning("....createFem 1");       
      try{
         if ( INT->whichFem==2 )  INT->Femi->RunCalculations(INT->compFem);
         if ( INT->whichFem==1 )  INT->Femi->ImportFEAP(INT->fem);

 // qWarning("....createFem 2");         
         unsigned nnodes = INT->Femi->GetNodes().size();  qWarning("  nnodes=%d", nnodes);
         vector<string> rnames = INT->Femi->GetElements()[0]->GetResultFieldsNames(); // pobranie nazw funkcji dla zerowego elementu.
         for ( unsigned i=0; i<rnames.size(); i++ ) INT->results << QString::fromStdString(rnames[i]); //cout << "  i=" << i << "    " << rnames[i] << endl;
   //qWarning("....createFem 3");            
         int ixmin, iymin;
         for (unsigned k=0; k<nnodes; k++) {
            mvector xx(3);
       	    INT->Femi->GetNode(k).GetX(xx);
	        double x = xx[0];  double y = xx[1];  double z = xx[2];
	        vertices->push_back(osg::Vec3 (x, y, z));
	        if ( x<fxMin ) {fxMin = x; ixmin=int(k);}  if ( x>fxMax ) fxMax = x;
	        if ( y<fyMin ) {fyMin = y; iymin=int(k);}  if ( y>fyMax ) fyMax = y;
	        if ( z<fzMin ) fzMin = z;   if ( z>fzMax ) fzMax = z;
         }	 
         std::cout << "  fxMin=" << fxMin << "   fxMax=" << fxMax << "  fyMin=" << fyMin << "   fyMax=" << fyMax << "  fzMin=" << fzMin << "   fzMax=" << fzMax << std::endl; 
	 std::cout << "  ixmin=" << ixmin << "   iymin=" << iymin << std::endl;
      } //try
      catch( CFEException &e )  {
        cout << "FEM EXCEPTION :" << e.GetMessage() << endl;
      }
      catch( CMException &e )  {
        cout << "MOLECULAR EXCEPTION :" << e.GetMessage() << endl;
      }
      catch( CLangException &e )  {
        cout << "LANGUAGE EXCEPTION :" << e.GetErrorMessage() << endl;
      }
      catch( CMathException &e )  {
        cout << "MATH EXCEPTION :" << e.GetExceptionMessage() << endl;
      }
      catch( CXML_Exception &e )  {
        cout << "XML EXCEPTION :" << e.GetExceptionMessage() << endl;
      }   // try - catch
    //QString qstr = QString::fromStdString(str);
      if ( INT->atName.empty() ) {
         INT->xMin = fxMin;  INT->xMax = fxMax;
         INT->yMin = fyMin;  INT->yMax = fyMax;
         INT->zMin = fzMin;  INT->zMax = fzMax;
      } else {
         INT->xMin = std::min(fxMin, LATT->xMin);  INT->xMax = std::max(fxMax, LATT->xMax);
         INT->yMin = std::min(fyMin, LATT->yMin);  INT->yMax = std::max(fyMax, LATT->yMax);
         INT->zMin = std::min(fzMin, LATT->zMin);  INT->zMax = std::max(fzMax, LATT->zMax);
      }
      INT->scDim = std::max(INT->xMax-INT->xMin, std::max(INT->yMax-INT->yMin, INT->zMax-INT->zMin));  std::cout << " INT->scDim=" << INT->scDim << std::endl;
      
      INT->newcalc = false;      

    }   // newcalc

    osg::ref_ptr<osg::MatrixTransform> m_fem = new osg::MatrixTransform;
    osg::StateSet* set = m_fem->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());
    geometry->setVertexArray (vertices.get()); 
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertices->size()));
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

    colors->push_back(INT->ColorP);  //(osg::Vec4(0.5f,0.5f,0.0f,0.3f));

    geometry->setColorArray(colors.get());
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);       
    geometry->setStateSet(makeStateSet(INT->pointSize));   //(6.0f));
    geode->addDrawable (geometry.get());
//-----------------------       
    unsigned ne = INT->Femi->GetElements().size();
  //qWarning("  ........ ne= %d", ne);
    //double vmin, vmax;
   // INT->choosen_value = rnames[0];
    //INT->Femi->GetNodalValuesMinMax(INT->choosen_value, vmin, vmax);
  qWarning("  ........ ne= %d", ne);//       vmin=%g       vmax=%g", ne, vmin, vmax);
    int badEdges = 0;
    for (unsigned k=0; k<ne; k++)  {  // pętla po elementach
         vector< CGeometricObjectShapeNodal* > edges;       
	 INT->Femi->GetElements()[k]->GetEdges(edges, true);
	 osg::ref_ptr<osg::Geometry> linesGeom = new osg::Geometry();
         osg::ref_ptr<osg::Vec4Array> colorL = new osg::Vec4Array;
	 
	 colorL->push_back(INT->ColorL);   //(osg::Vec4(1.0, 1.0, 0.0, 1.0));
         
	 osg::ref_ptr<osg::Vec3Array> vertL = new osg::Vec3Array;
      if ( edges.size()<=0 )   badEdges++; //{  qWarning( " ..EDGES..   elem=%d      edges.size()= %d", k, edges.size());   badEdge++;  }
         mvector x(1),X(3);
	 for (unsigned l=0; l<edges.size(); l++)    { 
	    x[0] = -1.;  edges[l]->GetX(x, X);  vertL->push_back(osg::Vec3(X[0], X[1], X[2]));
	    x[0] = 0.;   edges[l]->GetX(x, X);  vertL->push_back(osg::Vec3(X[0], X[1], X[2]));
  	    vertL->push_back(osg::Vec3(X[0], X[1], X[2]));
            x[0] = 1.;   edges[l]->GetX(x, X);  vertL->push_back(osg::Vec3(X[0], X[1], X[2]));
         }
      	 linesGeom->setVertexArray (vertL.get()); 
	 linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertL->size()));
	 linesGeom->setColorArray(colorL.get());
	 linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	 geode->addDrawable (linesGeom.get());
    }
  qWarning(" badEdges=%d", badEdges);
    INT->refrFem = false;
    m_fem->addChild(geode.get());
    INT->m_fem = m_fem;
    return m_fem.get();
*/
}

osg::ref_ptr<osg::MatrixTransform> OsgScene::createRes()
{
  qWarning("....createRes 0    choosen_value=%s",  INT->choosen_value.c_str());  
/*  
    //CFEMProject FP;       
    osg::ref_ptr<osg::MatrixTransform> m_res = new osg::MatrixTransform;
    osg::StateSet* set = m_res->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);  
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    unsigned ne = INT->Femi->GetElements().size();
    double vmin, vmax;
    INT->Femi->GetNodalValuesMinMax(INT->choosen_value, vmin, vmax);
  qWarning("  ........ ne= %d       vmin=%g       vmax=%g", ne, vmin, vmax);
    //void GetNodalValuesMinMax(const string &value, double &min, double &max, bool boundary = false );
    geo = new osg::Geode;

    colorL = new osg::Vec4Array;
    colorL->push_back(INT->ColorResL); //colorL->push_back(osg::Vec4(0.0, 0.0, 0.0, 1.0));    //    ndiv = 16;//    ncol = 5;
 
    tablica = new double[INT->ndiv];
    double stepdiv = 2. / double(INT->ndiv-1);
    for ( int i=0; i<INT->ndiv; i++ )  tablica[i] = -1. + i * stepdiv;
    
    wart = new double[INT->ncol];
    double stepcol = (vmax - vmin) / double(INT->ncol-1);

    INT->mapColors = new osg::Vec4Array;
    osgSim::ColorRange* cr = new osgSim::ColorRange(float(vmin), float(vmax));//, INT->mapColors);
    for ( int i=0; i<INT->ncol; i++ )  {
        wart[i] = vmin + double(i)*stepcol;
	INT->mapColors->push_back(cr->getColor(wart[i]));
    }
  qWarning("  INT->ncol=%d", INT->ncol);
    for ( int i=0; i<INT->ncol; i++ ) qWarning(" i=%d,  wart[i]=%g", i, wart[i]);

    for (unsigned k=0; k<ne; k++)  {  // pętla po elementach
        vector< CGeometricObjectShapeNodal* > faces;       
	INT->Femi->GetElements()[k]->GetFaces(faces, true);
	
	for (unsigned l=0; l<faces.size(); l++)    { 
	   try{
	     CScalarShapeSpaceApprox face_approx11 = INT->Femi->GetElements()[k]->CreateResultApproximator( faces[l], INT->choosen_value );
   	     mvector x(2);
	     value = new double*[INT->ndiv];
	     for ( int m=0; m<INT->ndiv; m++ ) {
	        value[m] = new double[INT->ndiv];
	        for ( int n=0; n<INT->ndiv; n++ ) {
		   x[0] = -1. + m * stepdiv;
		   x[1] = -1 + n * stepdiv;
		   value[m][n] = face_approx11.interpolate(x);
	        }
	    //  qWarning( "%g %g %g %g %g %g %g %g", value[m][0], value[m][1], value[m][2], value[m][3], value[m][4], value[m][5], value[m][6], value[m][7]);
	     }
	   } // try
	   catch( CFEException &e )  {
              cout << "FEM EXCEPTION :" << e.GetMessage() << endl;
           }
           catch( CMException &e )  {
              cout << "MOLECULAR EXCEPTION :" << e.GetMessage() << endl;
           }
           catch( CLangException &e )  {
               cout << "LANGUAGE EXCEPTION :" << e.GetErrorMessage() << endl;
           }
	   catch( CMathException &e )  {
               cout << "MATH EXCEPTION :" << e.GetExceptionMessage() << endl;
           }
              catch( CXML_Exception &e )  {
               cout << "XML EXCEPTION :" << e.GetExceptionMessage() << endl;
           } // try - catch
//	     qWarning(" +1++++++++ elem nr %d    face nr. %d", k, l);
	   conrec(value, INT->ndiv-1, tablica, INT->ncol, wart, faces[l]);

           delete [] value;
	} // for (unsigned l=0; l<faces.size(); l++) 
    } // for (unsigned k=0; k<ne; k++)  {  // pętla po elementach

    INT->refrRes = false;
    m_res->addChild(geo.get());
    return m_res.get();
*/
}
/*
Copyright (c) 1996-1997 Nicholas Yue

This software is copyrighted by Nicholas Yue. This code is base on the work of
Paul D. Bourke CONREC.F routine

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/
//#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
//#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
//=============================================================================
//
//     CONREC is a contouring subroutine for rectangularily spaced data.
//
//     It emits calls to a line drawing subroutine supplied by the user
//     which draws a contour map corresponding to real*4data on a randomly
//     spaced rectangular grid. The coordinates emitted are in the same
//     units given in the x() and y() arrays.
//
//     Any number of contour levels may be specified but they must be
//     in order of increasing value.
//
//     As this code is ported from FORTRAN-77, please be very careful of the
//     various indices like ilb,iub,jlb and jub, remeber that C/C++ indices
//     starts from zero (0)
//
//=============================================================================
/*
int OsgScene::conrec(double **d,     // matrix of data to contour
	   int imax,                 // ilb,iub,jlb,jub ! index bounds of data matrix
	   double *t,                // data matrix row and column coordinates
	   int nc,                   // number of contour levels
	   double *z,                // contour levels in increasing order
	   CGeometricObjectShapeNodal* face )	   
{
  int m1, m2, m3, case_value;
  double dmin, dmax, tmax, tmin, x1, x2, y1, y2;
  register int i, j, k, m;
  int sh[5];
  mvector xx(2), XX(3);
  bool doneT;  
  //===========================================================================
  // The indexing of im and jm should be noted as it has to start from zero
  // unlike the fortran counter part
  //===========================================================================
  int im[4] = {0,1,1,0},jm[4]={0,0,1,1};
  //===========================================================================
  // Note that castab is arranged differently from the FORTRAN code because
  // Fortran and C/C++ arrays are transposed of each other, in this case
  // it is more tricky as castab is in 3 dimension
  //===========================================================================
  int castab[3][3][3] =
  {
    {  {0,0,8},{0,2,5},{7,6,9} },
    {  {0,3,4},{1,3,1},{4,3,0} },
    {  {9,6,7},{5,2,0},{8,0,0} }
  };
//    osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;
//    shared_normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
  std::ofstream fout("zapis.1");
  osg::ref_ptr<osg::Geometry> linesGeom = new osg::Geometry();
  osg::ref_ptr<osg::Vec3Array> vertL = new osg::Vec3Array;
  
  for (j=(imax-1);j>=0;j--) {
    for (i=0;i<=imax-1;i++) {
      double temp1,temp2;
      temp1 = std::min(d[i][j],d[i][j+1]);
      temp2 = std::min(d[i+1][j],d[i+1][j+1]);
      dmin = std::min(temp1,temp2);
      temp1 = std::max(d[i][j],d[i][j+1]);
      temp2 = std::max(d[i+1][j],d[i+1][j+1]);
      dmax = std::max(temp1,temp2);
      //if (dmax>=z[0]&&dmin<=z[nc-1]) {
      for (k=0; k<nc; k++) {
//  std::cout <<  "i=" << i << "  j=" << j << " .. k=" << k << "  dmin=" << dmin << "  dmax=" << dmax << std::endl;		  
          double zk = z[k];
	  double zk1 = ( k<nc-1 )? z[k+1] : dmax;
          int k1 = std::max(0, k-1);
	  
	  osg::ref_ptr<osg::Vec4Array> colorQ = new osg::Vec4Array;
          colorQ->push_back(INT->mapColors.get()->at(k));
          osg::ref_ptr<osg::Vec4Array> colorT = new osg::Vec4Array;
	  osg::ref_ptr<osg::Vec4Array> colorT1 = new osg::Vec4Array;
	  osg::ref_ptr<osg::Vec4Array> colorT2 = new osg::Vec4Array;vertP
	  osg::ref_ptr<osg::Vec4Array> colorP = new osg::Vec4Array;

	  if ( dmin>=zk && dmax<=zk1 ) {
	     
	     osg::ref_ptr<osg::Geometry> quadsGeom = new osg::Geometry();
             osg::ref_ptr<osg::Vec3Array> vertQ = new osg::Vec3Array;      
             xx[0] = t[i];     xx[1] = t[j];      face->GetX(xx, XX);   vertQ->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
             xx[0] = t[i+1];   xx[1] = t[j];      face->GetX(xx, XX);   vertQ->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
             xx[0] = t[i+1];   xx[1] = t[j+1];    face->GetX(xx, XX);   vertQ->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
	     xx[0] = t[i];     xx[1] = t[j+1];    face->GetX(xx, XX);   vertQ->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
//   fout << "i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;
//   fout << "   0    " << vertQ->at(0)[0] << "      " <<  vertQ->at(0)[1] << "      " <<  vertQ->at(0)[2] << std::endl;
             quadsGeom->setVertexArray(vertQ);
	     colorQ->push_back(INT->mapColors.get()->at(k));
	     quadsGeom->setColorArray(colorQ.get(), osg::Array::BIND_OVERALL);
             //quadsGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);
             quadsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));//numCoords));
	     geo->addDrawable (quadsGeom.get());
	     //vertices->clear();	
          }  else
	    //=================================================================
	    //
	    // Note: at this stage the relative heights of the corners and the
	    // centre are in the h array, and the corresponding coordinates are
	    // in the xh and yh arrays. The centre of the box is indexed by 0
	    // and the 4 corners by 1 to 4 as shown below.
	    // Each triangle is then indexed by the parameter m, and the 3
	    // vertices of each triangle are indexed by parameters m1,m2,and
	    // m3.
	    // It is assumed that the centre of the box is always vertex 2
	    // though this isimportant only when all 3 vertices lie exactly on
	    // the same contour level, in which case only the side of the box
	    // is drawn.
	    //
	    //
	    //      vertex 4 +-------------------+ vertex 3
	    //               | \               / |
	    //               |   \    m-3    /   |
	    //               |     \       /     |
	    //               |       \   /       |
	    //               |  m=2    X   m=2   |       the centre is vertex 0
	    //               |       /   \       |
	    //               |     /       \     |
	    //               |   /    m=1    \   |vertP
	    //               | /               \ |
	    //      vertex 1 +-------------------+ vertex 2
	    //
	    //
	    //
	    //               Scan each triangle in the box
	    //
	    //=================================================================
	    for (m=4; m>=0; m--) {
	      if ( m>0 ) {
		//=============================================================
		// The indexing of im and jm should be noted as it has to
		// start from zero
		//=============================================================
		conrec_h[m] = d[i+im[m-1]][j+jm[m-1]]-zk;
		conrec_xh[m] = t[i+im[m-1]];
		conrec_yh[m] = t[j+jm[m-1]];
	      } else {
		conrec_h[0] = 0.25*(conrec_h[1]+conrec_h[2]+conrec_h[3]+conrec_h[4]);
		conrec_xh[0]=0.5*(t[i]+t[i+1]);
		conrec_yh[0]=0.5*(t[j]+t[j+1]);
	      }
	      if ( conrec_h[m]>0.0 ) {
		sh[m] = 1;
	      } else if ( conrec_h[m]<0.0 ) {
		sh[m] = -1;
	      } else
		sh[m] = 0;
	    }
	    for (m=1; m<=4; m++) {
	      doneT = false;
	      m1 = m;
	      m2 = 0;
	      if (m!=4) m3 = m+1;
	      else      m3 = 1;
	      tmin = std::min(conrec_h[0], std::min(conrec_h[m1], conrec_h[m3])) + zk;
	      tmax = std::max(conrec_h[0], std::max(conrec_h[m1], conrec_h[m3])) + zk;
              if ( tmin>=zk && tmax<=zk1 ) {
                 osg::ref_ptr<osg::Geometry> triangles2Geom = new osg::Geometry();
                 osg::ref_ptr<osg::Vec3Array> vertT2 = new osg::Vec3Array;      
		 xx[0] = conrec_xh[m1];  xx[1] = conrec_yh[m1];    face->GetX(xx, XX);   vertT2->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		 xx[0] = conrec_xh[m3];  xx[1] = conrec_yh[m3];    face->GetX(xx, XX);   vertT2->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		 xx[0] = conrec_xh[0];   xx[1] = conrec_yh[0];     face->GetX(xx, XX);   vertT2->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		 colorT2->push_back(INT->mapColors.get()->at(k));
		 triangles2Geom->setVertexArray (vertT2.get()); 
                 triangles2Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertT2->size()));
	         triangles2Geom->setColorArray(colorT2.get());
	         triangles2Geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	         geo->addDrawable (triangles2Geom.get());
		 doneT = true;
	      }
              osg::ref_ptr<osg::Geometry> trianglesGeom = new osg::Geometry();
              osg::ref_ptr<osg::Vec3Array> vertT = new osg::Vec3Array;      
              osg::ref_ptr<osg::Geometry> triangles1Geom = new osg::Geometry();
              osg::ref_ptr<osg::Vec3Array> vertT1 = new osg::Vec3Array;      
              osg::ref_ptr<osg::Geometry> polyGeom = new osg::Geometry();
              osg::ref_ptr<osg::Vec3Array> vertP = new osg::Vec3Array;      

	      if (  !doneT && zk>=dmin && zk<=dmax ) {

	         case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1];
	      
              if ( case_value!=0 ) {
		switch (case_value) {
		  //===========================================================
		  //     Case 1 - Line between vertices 1 and 2
		  //===========================================================
		case 1:
		  x1 = conrec_xh[m1];
		  y1 = conrec_yh[m1];
		  x2 = conrec_xh[m2];
		  y2 = conrec_yh[m2];
    fout << "  --1---      i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		  //===========================================================
		  //     Case 2 - Line between vertices 2 and 3
		  //===========================================================
		case 2:
		  x1 = conrec_xh[m2];
		  y1 = conrec_yh[m2];
		  x2 = conrec_xh[m3];
		  y2 = conrec_yh[m3];
      fout << "  --2--    i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		  //===========================================================
		  //     Case 3 - Line between vertices 3 and 1
		  //===========================================================
		case 3:
		  x1 = conrec_xh[m3];
		  y1 = conrec_yh[m3];
		  x2 = conrec_xh[m1];
		  y2 = conrec_yh[m1];
       fout << "  --3--   i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		  //===========================================================
		  //     Case 4 - Line between vertex 1 and side 2-3
		  //===========================================================
		case 4:
		  x1 = conrec_xh[m1];
		  y1 = conrec_yh[m1];
		  x2 = xsect(m2,m3);
		  y2 = ysect(m2,m3);
        fout << "  --4--    i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		  //===========================================================
		  //     Case 5 - Line between vertex 2 and side 3-1
		  //===========================================================
		case 5:
		  x1 = conrec_xh[m2];
		  y1 = conrec_yh[m2];
		  x2 = xsect(m3,m1);
		  y2 = ysect(m3,m1);
          fout << "  --5--   i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		  //===========================================================
		  //     Case 6 - Line between vertex 3 and side 1-2
		  //===========================================================
		case 6:
		  x1 = conrec_xh[m3];
		  y1 = conrec_yh[m3];
		  x2 = xsect(m1,m2);
		  y2 = ysect(m1,m2);
         fout << "  --6--   i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		  //===========================================================
		  //     Case 7 - Line between sides 1-2 and 2-3
		  //===========================================================
		case 7:
		  x1 = xsect(m1,m2);   y1 = ysect(m1,m2);
		  x2 = xsect(m2,m3);   y2 = ysect(m2,m3);
		  xx[0] = x1;            xx[1] = y1;               face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x2;            xx[1] = y2;               face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[0];  xx[1] = conrec_yh[0];     face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  if ( conrec_h[0]<0. )  {  colorP->push_back(INT->mapColors.get()->at(k));    colorT1->push_back(INT->mapColors.get()->at(k1)); }
		  else                   {  colorP->push_back(INT->mapColors.get()->at(k1));   colorT1->push_back(INT->mapColors.get()->at(k));  }
		  xx[0] = x1;             xx[1] = y1;               face->GetX(xx, XX);   vertP->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
       		  xx[0] = conrec_xh[m1];  xx[1] = conrec_yh[m1];    face->GetX(xx, XX);   vertP->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[m3];  xx[1] = conrec_yh[m3];    face->GetX(xx, XX);   vertP->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x2;             xx[1] = y2;               face->GetX(xx, XX);   vertP->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
                  polyGeom->setVertexArray(vertP);
	          polyGeom->setColorArray(colorP.get(), osg::Array::BIND_OVERALL);
                  //polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);
                  polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, 4));//numCoords));
	          geo->addDrawable (polyGeom.get());
                  doneT = true;
		  break;
		  //===========================================================
		  //     Case 8 - Line between sides 2-3 and 3-1
		  //===========================================================
		case 8:
		  x1 = xsect(m2,m3);    y1 = ysect(m2,m3);
		  x2 = xsect(m3,m1);    y2 = ysect(m3,m1);
 		  xx[0] = x1;             xx[1] = y1;               face->GetX(xx, XX);     vertT->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x2;             xx[1] = y2;               face->GetX(xx, XX);     vertT->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[m3];  xx[1] = conrec_yh[m3];    face->GetX(xx, XX);     vertT->push_back(osg::Vec3(XX[0], XX[1], XX[2]));

		  if ( conrec_h[m3]>0. ) { colorT->push_back(INT->mapColors.get()->at(k));      colorT1->push_back(INT->mapColors.get()->at(k1)); }
		  else                   { colorT->push_back(INT->mapColors.get()->at(k1));     colorT1->push_back(INT->mapColors.get()->at(k)); }
		  xx[0] = x2;             xx[1] = y2;               face->GetX(xx, XX);     vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x1;             xx[1] = y1;               face->GetX(xx, XX);     vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[0];   xx[1] = conrec_yh[0];     face->GetX(xx, XX);     vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  
		  xx[0] = conrec_xh[m1];    xx[1] = conrec_yh[m1];    face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x2;               xx[1] = y2;               face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[0];     xx[1] = conrec_yh[0];     face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
                  doneT = true;
		  break;
		  //===========================================================
		  //     Case 9 - Line between sides 3-1 and 1-2
		  //===========================================================
		case 9:
		  x1 = xsect(m3,m1);  y1 = ysect(m3,m1);
		  x2 = xsect(m1,m2);  y2 = ysect(m1,m2);
 		  xx[0] = x1;             xx[1] = y1;               face->GetX(xx, XX);   vertT->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x2;             xx[1] = y2;               face->GetX(xx, XX);   vertT->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[m1];  xx[1] = conrec_yh[m1];    face->GetX(xx, XX);   vertT->push_back(osg::Vec3(XX[0], XX[1], XX[2]));

		  if ( conrec_h[m1]<0. ) {  colorT->push_back(INT->mapColors.get()->at(k1));     colorT1->push_back(INT->mapColors.get()->at(k));  }
		  else                   {  colorT->push_back(INT->mapColors.get()->at(k));     colorT1->push_back(INT->mapColors.get()->at(k1));  }

		  xx[0] = x2;      xx[1] = y2;                     face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = x1;      xx[1] = y1;                     face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[0];   xx[1] = conrec_yh[0];    face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  
		  xx[0] = x1;             xx[1] = y1;              face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[m3];  xx[1] = conrec_yh[m3];   face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[0];   xx[1] = conrec_yh[0];    face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
                  doneT = true;
		  break;
		
		default:
	fout << "  --default--   i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
		  break;
		} // switch
		//=============================================================
		// Put your processing code here and comment out the printf
	        //vertL->push_back(osg::Vec3(x1, y1, 0.));
		//vertL->push_back(osg::Vec3(x2, y2, 0.));		
		//=============================================================
		//printf("%f %f %f %f %f\n",x1,y1,x2,y2,z[k]);
                trianglesGeom->setVertexArray (vertT.get()); 
                trianglesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertT->size()));
	        trianglesGeom->setColorArray(colorT.get());
	        trianglesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	        geo->addDrawable (trianglesGeom.get());
		
                triangles1Geom->setVertexArray (vertT1.get()); 
                triangles1Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertT1->size()));
	        triangles1Geom->setColorArray(colorT1.get());
	        triangles1Geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	        geo->addDrawable (triangles1Geom.get());
		
	      } else if ( case_value==0 && tmin>=zk && tmax<=zk1 ) {
         fout << "  --** 0 **--   i=" << i << "  j=" << j << " .. k=" << k << "  zk=" << zk << "  zk1=" << zk1 << std::endl;		  
       		  xx[0] = conrec_xh[m1];  xx[1] = conrec_yh[m1];    face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[m3];  xx[1] = conrec_yh[m3];    face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
		  xx[0] = conrec_xh[0];   xx[1] = conrec_yh[0];     face->GetX(xx, XX);   vertT1->push_back(osg::Vec3(XX[0], XX[1], XX[2]));
                  colorT1->push_back(INT->mapColors.get()->at(k));//(k1));
		  triangles1Geom->setVertexArray (vertT1.get()); 
                  triangles1Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertT1->size()));
	          triangles1Geom->setColorArray(colorT1.get());
	          triangles1Geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	          geo->addDrawable (triangles1Geom.get());
	      } // ( else if )
	    } //for ( m
	  }// if ( zk>=dmin && zk<=dmax ) { 	  
	} // for ( k
    }  // for ( i
  }  // for ( j
  linesGeom->setVertexArray (vertL.get()); 
  linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertL->size()));
  linesGeom->setColorArray(colorL.get());
  linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
  geo->addDrawable (linesGeom.get());
  
  fout.close();
  return 0;
}
*/

//================================================================================================================================================================================
/*
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>

osg::Node* createScene()
{
    osg::Geode* pGeode = new osg::Geode();

    // we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),1.5f,3.0f) ) );

	int s = 64;
	unsigned char* pData = new unsigned char[s*s*3];
	for ( int y=0; y<s; y++ )
		for ( int x=0; x<s; x++ )
		{
			unsigned char  c = (( ((y&0x8)==0) ^ (((x&0x8))==0) ))*255;

			pData[x*3+y*s*3+0] = c;
			pData[x*3+y*s*3+1] = c;
			pData[x*3+y*s*3+2] = c;
 		}

	osg::Image* pImage = new osg::Image;
	pImage->setImage(
		s, s, 1,								// 1=r? depth perhaps?
		GL_RGB,									// internal format
        GL_RGB,GL_UNSIGNED_BYTE,				// pixelformat, type
        pData,									// data
        osg::Image::USE_NEW_DELETE,				// mode
        1 );									// packing

	osg::Texture2D* pTex = new osg::Texture2D;
	pTex->setImage( pImage );

    osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
	pStateSet->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );

    return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}

//=====================================================================================================================

#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

void createTexture1D( osg::StateSet& ss )
{
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->setImage( 256, 1, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
                     new unsigned char[4 * 256], osg::Image::USE_NEW_DELETE );
    
    unsigned char* ptr = image->data();
    for ( unsigned int i=0; i<256; ++i )
    {
        *ptr++ = i; *ptr++ = i; *ptr++ = 255;
        *ptr++ = 255;
    }
    
    osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D;
    texture->setImage( image.get() );
    texture->setWrap( osg::Texture1D::WRAP_S, osg::Texture1D::REPEAT );
    texture->setFilter( osg::Texture1D::MIN_FILTER, osg::Texture1D::LINEAR );
    
    ss.setTextureAttributeAndModes( 0, texture.get() );
}

void createTexture2D( osg::StateSet& ss )
{
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage( osgDB::readImageFile("clockface.jpg") );
    
    texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
    texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );
    texture->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER );
    texture->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER );
    texture->setBorderColor( osg::Vec4(1.0, 1.0, 0.0, 1.0) );
    
    ss.setTextureAttributeAndModes( 0, texture.get() );
}

void createTextureCubeMap( osg::StateSet& ss )
{
    osg::ref_ptr<osg::TextureCubeMap> texture = new osg::TextureCubeMap;
    
    texture->setImage( osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("posx.jpg") );
    texture->setImage( osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("negx.jpg") );
    texture->setImage( osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("posy.jpg") );
    texture->setImage( osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("negy.jpg") );
    texture->setImage( osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("posz.jpg") );
    texture->setImage( osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("negz.jpg") );
    
    texture->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
    texture->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
    texture->setWrap( osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE );
	texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
	texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );
	
	ss.setTextureAttributeAndModes( 0, texture.get() );
	ss.setTextureAttributeAndModes( 0, new osg::TexGen );
}

int main( int argc, char** argv )
{
    osg::ref_ptr<osg::Geode> quad1 = new osg::Geode;
    quad1->addDrawable( osg::createTexturedQuadGeometry(
        osg::Vec3(-3.0,0.0,-0.5), osg::Vec3(1.0,0.0,0.0), osg::Vec3(0.0,0.0,1.0), 0.0, 0.0, 3.0, 1.0) );
    createTexture1D( *(quad1->getOrCreateStateSet()) );
    
    osg::ref_ptr<osg::Geode> quad2 = new osg::Geode;
    quad2->addDrawable( osg::createTexturedQuadGeometry(
        osg::Vec3(-0.5,0.0,-0.5), osg::Vec3(1.0,0.0,0.0), osg::Vec3(0.0,0.0,1.0), -0.1, -0.1, 1.1, 1.1) );
    createTexture2D( *(quad2->getOrCreateStateSet()) );
    
    osg::ref_ptr<osg::Geode> box = new osg::Geode;
    box->addDrawable( new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(3.0,0.0,0.0), 1.0)) );
    createTextureCubeMap( *(box->getOrCreateStateSet()) );
    
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( quad1.get() );
    root->addChild( quad2.get() );
    root->addChild( box.get() );
    
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
    return viewer.run();
}
//=================================================================================================

#include <osg/Texture1D>
#include <osg/TransferFunction>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Uniform>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <iostream>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgvpgeometry/Geometry.hpp>

osg::Texture2D* geometryArrayToTexture2D(osg::Vec3dArray* geometryArray)
{
    osg::Texture2D* texture= new osg::Texture2D();
    osg::Image* im= new osg::Image();
    int size = geometryArray->size();
    if (size>4096)
    im->allocateImage(4096,(ceil)(size/4096.0),1,GL_RGBA, GL_FLOAT);
    else im->allocateImage(size,1,1,GL_RGBA, GL_FLOAT);
    osg::Vec4* imageData = reinterpret_cast<osg::Vec4*>(im->data());

    int value=0;
    for (osg::Vec3dArray::iterator it= geometryArray->begin(); it!= geometryArray->end();++it)
    {
       imageData[value] = osg::Vec4((float)it->x(),(float)it->y(),(float)it->z(),1.);
       value++;
   }

    texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::NEAREST);
    texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::NEAREST);
    texture->setResizeNonPowerOfTwoHint(false);
    im->setInternalTextureFormat(GL_RGBA32F_ARB);
    im->setPixelFormat(GL_RGBA);
    im->setDataType(GL_FLOAT);
    texture->setImage(im);
    return texture;
}

osg::Vec3Array* makeInstance()
{
    const float halfDimX( 5.f );
    const float halfDimZ( 5.f);
    const float halfDimY( 5.f );

    osg::Vec3Array* instaceArray = new osg::Vec3Array;
    //Create a geometry for use as instance
    //   Borrowed from osgdrawinstaced example**
    instaceArray->resize( 24 );
    // Geometry for a single quad.
    (*instaceArray)[ 0 ] = osg::Vec3f( -halfDimX, halfDimY, -halfDimZ );
    (*instaceArray)[ 1 ] = osg::Vec3f( halfDimX, halfDimY, -halfDimZ );
    (*instaceArray)[ 2 ] = osg::Vec3f( halfDimX, halfDimY, halfDimZ );
    (*instaceArray)[ 3 ] = osg::Vec3f( -halfDimX,halfDimY, halfDimZ );

    (*instaceArray)[ 4 ] = osg::Vec3f( -halfDimX,-halfDimY, -halfDimZ );
    (*instaceArray)[ 5 ] = osg::Vec3f( -halfDimX, halfDimY, -halfDimZ );
    (*instaceArray)[ 6 ] = osg::Vec3f( -halfDimX, halfDimY, halfDimZ );
    (*instaceArray)[ 7 ] = osg::Vec3f( -halfDimX,-halfDimY, halfDimZ );

    (*instaceArray)[ 8 ] = osg::Vec3f( -halfDimX, -halfDimY, -halfDimZ );
    (*instaceArray)[ 9 ] = osg::Vec3f( halfDimX,-halfDimY, -halfDimZ );
    (*instaceArray)[ 10 ] = osg::Vec3f( halfDimX, -halfDimY, halfDimZ );
    (*instaceArray)[ 11 ] = osg::Vec3f( -halfDimX,-halfDimY, halfDimZ );

    (*instaceArray)[ 12 ] = osg::Vec3f( halfDimX,-halfDimY, -halfDimZ );
    (*instaceArray)[ 13 ] = osg::Vec3f( halfDimX, halfDimY, -halfDimZ );
    (*instaceArray)[ 14 ] = osg::Vec3f( halfDimX, halfDimY, halfDimZ );
    (*instaceArray)[ 15 ] = osg::Vec3f( halfDimX,-halfDimY, halfDimZ );

    (*instaceArray)[ 16 ] = osg::Vec3f( -halfDimX,-halfDimY, halfDimZ );
    (*instaceArray)[ 17 ] = osg::Vec3f( -halfDimX, halfDimY, halfDimZ );
    (*instaceArray)[ 18 ] = osg::Vec3f( halfDimX, halfDimY, halfDimZ );
    (*instaceArray)[ 19 ] = osg::Vec3f( halfDimX,-halfDimY, halfDimZ );

    (*instaceArray)[ 20 ] = osg::Vec3f( -halfDimX,-halfDimY, -halfDimZ );
    (*instaceArray)[ 21 ] = osg::Vec3f( -halfDimX, halfDimY, -halfDimZ );
    (*instaceArray)[ 22 ] = osg::Vec3f( halfDimX, halfDimY, -halfDimZ );
    (*instaceArray)[ 23 ] = osg::Vec3f( halfDimX,-halfDimY, -halfDimZ );

   return instaceArray;

}
osg::Vec2Array* makeTextureCoords()
{


    osg::Vec2Array* textureCoords = new osg::Vec2Array;
    //Create a geometry for use as instance
    //** Borrowed from osgdrawinstaced example**
    textureCoords->resize( 24 );
    // Geometry for a single quad.

    (*textureCoords)[ 0 ] = osg::Vec2f( 0., 0.);
    (*textureCoords)[ 1 ] = osg::Vec2f( 0., 1.);
    (*textureCoords)[ 2 ] = osg::Vec2f( 1., 1.);
    (*textureCoords)[ 3 ] = osg::Vec2f( 1., 0.);

    (*textureCoords)[ 4 ] = osg::Vec2f( 0., 0.);
    (*textureCoords)[ 5 ] = osg::Vec2f( 0., 1.);
    (*textureCoords)[ 6 ] = osg::Vec2f( 1., 1.);
    (*textureCoords)[ 7 ] = osg::Vec2f( 1., 0.);

    (*textureCoords)[ 8 ] = osg::Vec2f( 0., 0.);
    (*textureCoords)[ 9 ] = osg::Vec2f( 0., 1.);
    (*textureCoords)[ 10 ] = osg::Vec2f( 1., 1.);
    (*textureCoords)[ 11 ] = osg::Vec2f( 1., 0.);

    (*textureCoords)[ 12 ] = osg::Vec2f( 0., 0.);
    (*textureCoords)[ 13 ] = osg::Vec2f( 0., 1.);
    (*textureCoords)[ 14 ] = osg::Vec2f( 1., 1.);
    (*textureCoords)[ 15 ] = osg::Vec2f( 1., 0.);

    (*textureCoords)[ 16 ] = osg::Vec2f( 0., 0.);
    (*textureCoords)[ 17 ] = osg::Vec2f( 0., 1.);
    (*textureCoords)[ 18 ] = osg::Vec2f( 1., 1.);
    (*textureCoords)[ 19 ] = osg::Vec2f( 1., 0.);

    (*textureCoords)[ 20 ] = osg::Vec2f( 0., 0.);
    (*textureCoords)[ 21 ] = osg::Vec2f( 0., 1.);
    (*textureCoords)[ 22 ] = osg::Vec2f( 1., 1.);
    (*textureCoords)[ 23 ] = osg::Vec2f( 1., 0.);


   return textureCoords;

}
int main (int argc, char *argv[])
{
     osg::Geode* g= new osg::Geode();
     osg::Geometry *geom = new osg::Geometry();
     geom->setVertexArray( makeInstance() );
     geom->setTexCoordArray(1,makeTextureCoords());
     geom->setUseDisplayList( false );
     geom->setUseVertexBufferObjects( true );

     //SET number of instances
    float ninstances=16384;
    // Use the DrawArraysInstanced PrimitiveSet and tell it to draw ninstances.
    geom->addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 24, ninstances ) );

    //osg::BoundingBox bb( -1, -1, -1, 200., 10., 200. );
    //geom->setInitialBound( bb );

    //osg::Vec3dArray* geometryArray = new osg::Vec3dArray;
    osg::ref_ptr<osgvp::geometry::GeoPointArray3d> geometryArray =
            new osgvp::geometry::GeoPointArray3d();
  // Fill geometry array
    for (int k=0;k<128;++k)
      for(int i=0;i<128;++i)
     //   for(int j =0 ;j<128;++j)
            geometryArray->push_back( osg::Vec3( i*15, k*15, 0.));

    osgvp::geometry::Envelope env= geometryArray->getEnvelope();
        osg::BoundingBox bb(env.min(), env.max() );
        geom->setInitialBound( bb);
    //Uniform stuff
    osg::ref_ptr<osg::Texture2D> tex = geometryArrayToTexture2D(geometryArray);
    geometryArray->clear();
    osg::ref_ptr<osg::StateSet>ss= geom->getOrCreateStateSet();
    ss->setTextureAttribute(0, tex);
    osg::Uniform* guniform= new osg::Uniform("texturePos", tex );
    guniform->set(0);
    ss->addUniform(guniform);
    ss->addUniform( new osg::Uniform( "color", osg::Vec4(1.,1.,0.,0.) ));

    //cube texture stuff
    osg::ref_ptr< osg::Image> iLogo = osgDB::readImageFile( "/home/jtorres/Projects/GIS/osgVP/osgvp-trunk/share/gas-icon-mod.jpg" );
      if( !iLogo.valid() )
      {
          osg::notify( osg::ALWAYS ) << "Can't open image file osg128.png" << std::endl;
          return( NULL );
      }
      osg::Texture2D* texLogo = new osg::Texture2D( iLogo.get() );
      texLogo->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
      texLogo->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );

      ss->setTextureAttribute( 1, texLogo );

      osg::ref_ptr< osg::Uniform > texLogoUniform =
          new osg::Uniform( "osgLogo", 1);

      ss->addUniform( texLogoUniform.get() );

    //shader
    std::string vertexSource =
            "#extension GL_EXT_gpu_shader4 : enable\n"
            "uniform sampler2D texturePos;\n"
            "uniform float osg_SimulationTime; \n"
            "varying vec3 N;\n"
            "varying vec3 v;\n"
            "void main() \n"
            "{ \n"
            //texture stuff
            "gl_TexCoord[1] = gl_MultiTexCoord1;\n"
            "float timeOffset = gl_InstanceID/8192; \n"
            "float angle = ( osg_SimulationTime - timeOffset ) * 6.283; \n"
            "float sa = sin( angle ); \n"
            "float ca = cos( angle ); \n"
             //New orientation, rotate around z axis.
            "vec4 newX = vec4( ca, -sa, 0., 0. ); \n"
            "vec4 newY = vec4( sa, ca, 0., 0. ); \n"
            "vec4 newZ = vec4( 0., 0., 1., 0. ); \n"
            "int y= gl_InstanceID/4096; \n"
            "int x= gl_InstanceID-y*4096; \n"
            "ivec2 coords= ivec2(x,y); \n"
    //      "vec4 pos=gl_Vertex+ texelFetch2D(texturePos,coords,0);\n"
            "vec4 pos=texelFetch2D(texturePos,coords,0);\n"
            "v = vec3(gl_ModelViewMatrix * pos);\n"
            "mat4 mV = mat4( newX, newY, newZ,pos); \n"
            "gl_Position= gl_ModelViewProjectionMatrix*mV*gl_Vertex;\n"

            "} \n";
        std::string fragSource =
//            "uniform vec4 color;\n"
              "uniform sampler2D osgLogo;\n"
//            "varying vec3 N;\n"
//            "varying vec3 v;\n"
              "void main() \n"
             "{ \n"
             //LIGTH STUFF
//            "vec3 L = normalize(gl_LightSource[0].position.xyz - v);\n"
//            "vec3 E = normalize(-v);\n" // we are in Eye Coordinates, so EyePos is (0,0,0)
//            "vec3 R = normalize(-reflect(L,N));\n"
//            //calculate Ambient Term:
//            "vec4 Iamb = gl_FrontLightProduct[0].ambient;\n"
//            //calculate Diffuse Term:
//            "vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);\n"
//            "Idiff = clamp(Idiff, 0.0, 1.0);\n"
//             // calculate Specular Term:
//            "vec4 Ispec = gl_FrontLightProduct[0].specular*pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);\n"
//            "Ispec = clamp(Ispec, 0.0, 1.0);\n"
//            // write Total Color:

              "vec4 colores = texture2D(osgLogo,gl_TexCoord[1].st);\n"
              "gl_FragColor = colores;\n"
//            "gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;\n"
               "} \n";

    //shader activation stuff
    osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->setShaderSource( vertexSource );
    osg::ref_ptr< osg::Shader > fragmentShader = new osg::Shader();
    fragmentShader->setType( osg::Shader::FRAGMENT);
    fragmentShader->setShaderSource( fragSource );
    osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
    program->addShader( fragmentShader.get() );
    ss->setAttribute( program.get(),osg::StateAttribute::ON );

    g->addDrawable(geom);
//     root->addChild(g);
//}

    osgViewer::Viewer* viewer= new osgViewer::Viewer();
    viewer->setSceneData(g );
    viewer->addEventHandler(new osgViewer::StatsHandler());
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    viewer->addEventHandler(new osgViewer::WindowSizeHandler());
    return viewer->run();
}
//    - - - - - - - - - - - - - - - - - -     C O N R E C  - - - - - - - - - - - - - - - - - - -


*/
/*
Copyright (c) 1996-1997 Nicholas Yue

This software is copyrighted by Nicholas Yue. This code is base on the work of
Paul D. Bourke CONREC.F routine

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/
/*	 FEMi->GetElements()[k]->GetFaces(faces, false);
	 vector< CGeometricObjectShapeNodal* > faces;      // wektor zawierający ściany elementu
         FEMi->GetElements()[k]->GetFaces(faces, false);      //pobranie brzegowych ścian ścian dla k-tego elementu ( do wektora o nazwie faces )
         //vector< CGeometricObject* > faces;      // wektor zawierający ściany elementu
         //FEMi->GetNodes()[k]->GetFaces(faces); //, true );      // pobranie brzegowych ścian ścian dla k-tego elementu ( do wektora o nazwie faces )
      //qWarning( " faces.size()= %d", faces.size());
         unsigned l;
	 std::string choosen_value = rnames[0];
         for (l=0; l<faces.size(); l++)    {     // pętla po widocznych krawędziach elementu;
	  //qWarning(" element nr. %d,  face nr. %d", k, l );
	    CScalarShapeSpaceApprox faprr = FEMi->GetElements()[k]->CreateResultApproximator(faces[l], choosen_value);
            mvector x(2),X(3);
            x[0] = 1.0; x[1] = 1.0;
            double value = faprr.interpolate( x );
            faces[l]->GetX(x,X);
	    unsigned nr = faces[l]->GetNumber();
	 if ( k<10 ) qWarning(" element nr. %d,  face nr. %d,   intern id=%d,      value=%g,   X=(%g, %g %g)", k, l, nr, value, X[0], X[1], X[2] );
      }  }	     
	     
    } else { // GraphData
//  Triangles
      if ( adata->trs3D.size()>0 && INT->alphaFemT>0. ) {
        for ( unsigned k=0; k<adata->trs3D.size(); k++ ) {
	   osg::ref_ptr<osg::Geometry> trianglesGeom = new osg::Geometry();
           osg::ref_ptr<osg::Vec3Array> vertT = new osg::Vec3Array;      
           osg::ref_ptr<osg::Vec4Array> colorT = new osg::Vec4Array;
           colorT->push_back(osg::Vec4(adata->trs3D[k].r, adata->trs3D[k].g, adata->trs3D[k].b, INT->alphaFemT));
	   for (unsigned l=0; l<adata->trs3D[k].size; l++) {
	      trig3D<double> &tr = adata->trs3D[k].data[l];
	      vertT->push_back(osg::Vec3(tr.p1.x, tr.p1.y, tr.p1.z));
	      vertT->push_back(osg::Vec3(tr.p2.x, tr.p2.y, tr.p2.z));
	      vertT->push_back(osg::Vec3(tr.p3.x, tr.p3.y, tr.p3.z));
	   }
           trianglesGeom->setVertexArray (vertT.get()); 
	   trianglesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertT->size())); // LINE_LOOP, 0, vertT->size()));
           trianglesGeom->setColorArray(colorT.get());
           trianglesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
           geode->addDrawable (trianglesGeom.get());	 
        }
      }
//  Lines
      if ( adata->lns3D.size()>0 && INT->alphaFemL>0. ) {
        for ( unsigned k=0; k<adata->lns3D.size(); k++ ) { //cout << "  lines=" << k << "   l=" << adata->lns3D[k].size << std::endl;
           osg::ref_ptr<osg::Geometry> linesGeom = new osg::Geometry();
           osg::ref_ptr<osg::Vec4Array> colorL = new osg::Vec4Array;
           osg::ref_ptr<osg::Vec3Array> vertL = new osg::Vec3Array;
           colorL->push_back(osg::Vec4(adata->lns3D[k].r, adata->lns3D[k].g, adata->lns3D[k].b, INT->alphaFemL));
	   for (unsigned l=0; l<adata->lns3D[k].size; l++) {
	      line3D<double> &ln = adata->lns3D[k].data[l];
	      vertL->push_back(osg::Vec3(ln.p1.x, ln.p1.y, ln.p1.z));
	      vertL->push_back(osg::Vec3(ln.p2.x, ln.p2.y, ln.p2.z));
	   }
           linesGeom->setVertexArray (vertL.get()); 
           linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertL->size()));
           linesGeom->setColorArray(colorL.get());
           linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	   geode->addDrawable (linesGeom.get());
        }
      }  
//  Points
      if ( adata->pts3D.size()>0 && INT->alphaFemP>0. ) {
           osg::ref_ptr<osg::Geometry> pointsGeom = new osg::Geometry();
           osg::ref_ptr<osg::Vec4Array> colorP = new osg::Vec4Array;
           osg::ref_ptr<osg::Vec3Array> vertP = new osg::Vec3Array;

        for ( unsigned k=0; k<adata->pts3D.size(); k++ ) { //cout << "  points=" << k << "   l=" << adata->pts3D[k].size << std::endl;
	   colorP->push_back(osg::Vec4(adata->pts3D[k].r, adata->pts3D[k].g, adata->pts3D[k].b, INT->alphaFemP));
	   for (unsigned l=0; l<adata->pts3D[k].size; l++)
	        vertP->push_back(osg::Vec3(adata->pts3D[k].data[l].x, adata->pts3D[k].data[l].y, adata->pts3D[k].data[l].z));
           pointsGeom->setVertexArray (vertP.get()); 
           pointsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertP->size()));
           pointsGeom->setColorArray(colorP.get());
           pointsGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
           pointsGeom->setStateSet(makeStateSet(4.0f));
           geode->addDrawable (pointsGeom.get());
	}
	MiscFunc::ComputeSizes(vertP, fxMin, fxMax, fyMin, fyMax, fzMin, fzMax);
        if ( INT->atName.empty() ) {
           INT->xMin = fxMin;  INT->xMax = fxMax;
           INT->yMin = fyMin;  INT->yMax = fyMax;
           INT->zMin = fzMin;  INT->zMax = fzMax;
        } else {
           INT->xMin = std::min(fxMin, LATT->xMin);  INT->xMax = std::max(fxMax, LATT->xMax);
           INT->yMin = std::min(fyMin, LATT->yMin);  INT->yMax = std::max(fyMax, LATT->yMax);
           INT->zMin = std::min(fzMin, LATT->zMin);  INT->zMax = std::max(fzMax, LATT->zMax);
        }
        INT->scDim = std::max(INT->xMax-INT->xMin, std::max(INT->yMax-INT->yMin, INT->zMax-INT->zMin)); 
  //std::cout << " INT->scDim=" << INT->scDim << std::endl;     
      }    
      INT->refresh = false;
      m_fem->addChild(geode.get()); 
    }
---------------------------------------
if ( !INT->imName.empty() ) m_worldReferenceFrame->addChild(openImage().get());//  m_worldReferenceFrame.get()->addChild(INT->camera1.get());

    worldReferenceFrame->setAttitude(osg::Quat(osg::PI, osg::Z_AXIS, osg::PI_2, osg::X_AXIS, 0.0, osg::Y_AXIS));
        
--------------------------------------
osg::ref_ptr<osg::Camera> OsgScene::openImage()
{    
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(INT->imName);
    if ( !image.valid() ) std::cout << " Bad image" << std::endl;
    texture->setImage( image.get() );
    
    osg::ref_ptr<osg::Drawable> quad = osg::createTexturedQuadGeometry(osg::Vec3(), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
    quad->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );
    
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( quad.get() );
    
    osg::ref_ptr<osg::Camera> camera1 = new osg::Camera;
    camera1->setClearMask( 0 );
    camera1->setCullingActive( false );
    camera1->setAllowEventFocus( false );
    camera1->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera1->setRenderOrder( osg::Camera::POST_RENDER );
    camera1->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
    camera1->addChild( geode.get() );
    
    osg::StateSet* ss = camera1->getOrCreateStateSet();
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    ss->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL, 1.0, 1.0) );    
    
    INT->refrImage = false;
    return camera1;
}
---------------------------------------------------
bool crysplane(glm::dvec3 coord, double point_x, double point_y, double point_z, int mill1, int mill2, int mill3)
{
  //cout << "coord=" << coord.x << ",  " << coord.y << ",  " << coord.z << endl;
  double aa = 1./a * double(mill1);
  double bb = 1./(sqrt(3.)*a) * double(mill1) + 2./(sqrt(3.)*b) * double(mill2);
  double cc = 1./c * double(mill3);
  glm::dvec3 c1 = glm::dvec3(double(mill1), double(mill2), double(mill3)) * o2c;// glm::inverse(c2o);
// cout << "c1=" << c1.x << ",  " << c1.y << ",  " << c1.z << endl;
  if ( abs(aa-c1.x)>1.e-8 || abs(bb-c1.y)>1.e-8 || abs(cc-c1.z)>1.e-8 ) cout << "coord=" << coord.x << ",  " << coord.y << ",  " << coord.z << endl;
//cout << "abc=" << aa << ",  " << bb << ",  " << cc << endl;
  double dd = sqrt(aa*aa+bb*bb+cc*cc);
  aa /= dd;
  bb /= dd;
  cc /= dd;
  dd = - aa*point_x - bb*point_y - cc*point_z;
//  cout << "dd=" << dd << endl;
  return (aa*coord.x+bb*coord.y+cc*coord.z+dd)<=0.;
}

void OsgScene::displayBottom() //osg::ref_ptr<osg::Camera>
{
    osg::ref_ptr<osg::Drawable> quad = osg::createTexturedQuadGeometry(osg::Vec3(), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
    //quad->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( quad.get() );
    
    osg::ref_ptr<osg::Camera> camera1 = new osg::Camera;
    camera1->setClearMask(0);
    camera1->setCullingActive(false);
    camera1->setAllowEventFocus(false);
    camera1->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera1->setRenderOrder( osg::Camera::POST_RENDER );
    camera1->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
    camera1->addChild( geode.get() );
    
    osg::StateSet* ss = camera1->getOrCreateStateSet();
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    ss->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL, 1.0, 1.0) );    
    
    //INT->refrImage = false;
    //return camera1;
    m_worldReferenceFrame.get()->addChild(camera1.get());

}

void OsgScene::displayBottom(glm::dmat3 rotTens)
{
   osg::ref_ptr<osg::MatrixTransform> m_worldAt = new osg::MatrixTransform;
   osg::StateSet* set = m_worldAt->getOrCreateStateSet();
   set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
   set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON); 
   double fxMin, fxMax, fyMin, fyMax, fzMin;
   int ixMin, ixMax, iyMin, iyMax, izMin;
   osg::ref_ptr<osg::Geode> geode0 = new osg::Geode();
   //osg::ref_ptr<osg::Billboard> geode0 = new osg::Billboard();
   osg::ref_ptr<osg::Geometry> square = new osg::Geometry();
    // draw the square
   osg::Vec3Array* squareVertices = new osg::Vec3Array;
       //glm::dmat3 rotTens = MiscFunc::quat2rot((mview1->getQuat()*ZtoY).conj());
   MiscFunc::ComputeBottom(LATT->coords, rotTens, fxMin, fxMax, fyMin, fyMax, fzMin);
   glm::dvec3 p1 = glm::dvec3(fxMin, fyMin, fzMin);
   glm::dvec3 p2 = glm::dvec3(fxMax, fyMin, fzMin);
   glm::dvec3 p3 = glm::dvec3(fxMax, fyMax, fzMin);
   glm::dvec3 p4 = glm::dvec3(fxMin, fyMax, fzMin);
   osg::Vec3 pp1 = MiscFunc::convert(p1 * rotTens);
   osg::Vec3 pp2 = MiscFunc::convert(p2 * rotTens);
   osg::Vec3 pp3 = MiscFunc::convert(p3 * rotTens);
   osg::Vec3 pp4 = MiscFunc::convert(p4 * rotTens);
   squareVertices->push_back(pp1);//  fxMin, fyMin, fzMin ) );
   squareVertices->push_back(pp2);//  fxMax, fyMin, fzMin ) );
   squareVertices->push_back(pp3);//  fxMax, fyMax, fzMin ) );
   squareVertices->push_back(pp4);//  fxMin, fyMax, fzMin ) );
   square->setVertexArray(squareVertices);
   square->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, 4));
   osg::ref_ptr<osg::Vec4Array> color0 = new osg::Vec4Array;
   color0->push_back(osg::Vec4(1.0, 1.0, 0.0, 0.2));    

   square->setColorArray(color0);
   square->setColorBinding(osg::Geometry::BIND_OVERALL);
   geode0->addDrawable(square.get());
//   m_worldAt->addChild(geode0.get());
   m_worldReferenceFrame.get()->addChild(geode0.get());//(m_worldAt.get());
   m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
   //m_worldAt->addChild(geode0.get());
}

void OsgScene::displayBottom1(glm::dmat3 rotTens, int nA)
{
   //osg::ref_ptr<osg::MatrixTransform> m_worldAt = new osg::MatrixTransform;
   osg::StateSet* set = m_worldReferenceFrame->getOrCreateStateSet();
   set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
   set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON); 
   double fxMin, fxMax, fyMin, fyMax, fzMin;
   int ixMin, ixMax, iyMin, iyMax, izMin;
   osg::ref_ptr<osg::Geode> geode0 = new osg::Geode();
   //osg::ref_ptr<osg::Billboard> geode0 = new osg::Billboard();
   osg::ref_ptr<osg::Geometry> square = new osg::Geometry();
    // draw the square
   osg::Vec3Array* squareVertices = new osg::Vec3Array;
   MiscFunc::ComputePlane(LATT->coords, rotTens, fxMin, fxMax, fyMin, fyMax, fzMin, nA);
   glm::dvec3 p1 = glm::dvec3(fxMin, fyMin, fzMin);
   glm::dvec3 p2 = glm::dvec3(fxMax, fyMin, fzMin);
   glm::dvec3 p3 = glm::dvec3(fxMax, fyMax, fzMin);
   glm::dvec3 p4 = glm::dvec3(fxMin, fyMax, fzMin);
   osg::Vec3 pp1 = MiscFunc::convert(p1 * rotTens);
   osg::Vec3 pp2 = MiscFunc::convert(p2 * rotTens);
   osg::Vec3 pp3 = MiscFunc::convert(p3 * rotTens);
   osg::Vec3 pp4 = MiscFunc::convert(p4 * rotTens);
   squareVertices->push_back(pp1);//  fxMin, fyMin, fzMin ) );
   squareVertices->push_back(pp2);//  fxMax, fyMin, fzMin ) );
   squareVertices->push_back(pp3);//  fxMax, fyMax, fzMin ) );
   squareVertices->push_back(pp4);//  fxMin, fyMax, fzMin ) );
   square->setVertexArray(squareVertices);
   square->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, 4));

   osg::Vec3Array* normals = new osg::Vec3Array;
   normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
   square->setNormalArray(normals);
   square->setNormalBinding(osg::Geometry::BIND_OVERALL);

   osg::ref_ptr<osg::Vec4Array> color0 = new osg::Vec4Array;
   color0->push_back(osg::Vec4(1.0, 1.0, 1.0, 0.15));    

   square->setColorArray(color0);
   square->setColorBinding(osg::Geometry::BIND_OVERALL);

   geode0->addDrawable(square.get());
//   m_worldAt->addChild(geode0.get());
   m_worldReferenceFrame.get()->addChild(geode0.get());//(m_worldAt.get());
   m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
   //m_worldAt->addChild(geode0.get());
}
*/
//}
/*
void OsgScene::displayPlane(glm::dmat3 rotTens, int nA)
{
   if ( nA<0 )  {
   std::cout << "Remove geode0" << std::endl;
      m_worldReferenceFrame.get()->removeChild(geode0.get());
      m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
      return;  
   }
   double fxMin, fxMax, fyMin, fyMax, fzMin;
   geode0 = new osg::Geode();   //osg::ref_ptr<osg::Billboard> geode0 = new osg::Billboard();
   osg::ref_ptr<osg::Geometry> square = new osg::Geometry();
   osg::Vec3Array* squareVertices = new osg::Vec3Array;
std::cout << " ++==++==++  displayPlane 1" << std::endl;
   MiscFunc::ComputePlane(LATT->coords, rotTens, fxMin, fxMax, fyMin, fyMax, fzMin, nA);
   //if ( nA>=0 ) fzMin = (LATT->coords.get()->at(nA)).z();
   glm::dvec3 p1 = glm::dvec3(fxMin, fyMin, fzMin);
   glm::dvec3 p2 = glm::dvec3(fxMax, fyMin, fzMin);
   glm::dvec3 p3 = glm::dvec3(fxMax, fyMax, fzMin);
   glm::dvec3 p4 = glm::dvec3(fxMin, fyMax, fzMin);
//   osg::Vec3 pp1 = MiscFunc::convert(p1 * rotTens);
//   osg::Vec3 pp2 = MiscFunc::convert(p2 * rotTens);
//   osg::Vec3 pp3 = MiscFunc::convert(p3 * rotTens);
//   osg::Vec3 pp4 = MiscFunc::convert(p4 * rotTens);
   glm::dvec3 pp1 = p1 * rotTens;
   glm::dvec3 pp2 = p2 * rotTens;
   glm::dvec3 pp3 = p3 * rotTens;
   glm::dvec3 pp4 = p4 * rotTens;   
   //pp1.x() = fzMin;
   //pp2.x() = fzMin;
   //pp3.x() = fzMin;
   //pp4.x() = fzMin;
 std::cout << " fxMin=" << fxMin << " fyMin=" << fyMin << " fzMin=" << fzMin << std::endl;
 std::cout << " fxMax=" << fxMax << " fyMax=" << fyMax << std::endl;
//   squareVertices->push_back(pp1);//  fxMin, fyMin, fzMin ) );
//   squareVertices->push_back(pp2);//  fxMax, fyMin, fzMin ) );
//   squareVertices->push_back(pp3);//  fxMax, fyMax, fzMin ) );
//   squareVertices->push_back(pp4);//  fxMin, fyMax, fzMin ) );
   squareVertices->push_back(MiscFunc::convert(pp1));//  fxMin, fyMin, fzMin ) );
   squareVertices->push_back(MiscFunc::convert(pp2));//  fxMax, fyMin, fzMin ) );
   squareVertices->push_back(MiscFunc::convert(pp3));//  fxMax, fyMax, fzMin ) );
   squareVertices->push_back(MiscFunc::convert(pp4));//  fxMin, fyMax, fzMin ) );
   square->setVertexArray(squareVertices);
   square->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, 4));
   osg::ref_ptr<osg::Vec4Array> color0 = new osg::Vec4Array;
   color0->push_back(osg::Vec4(1.0, 1.0, 0.5, 0.35));    
//std::cout << " ++==++==++  displayPlane 3" << std::endl;
   square->setColorArray(color0);
   square->setColorBinding(osg::Geometry::BIND_OVERALL);
   
   osg::ref_ptr<osg::StateSet> stateset = square->getOrCreateStateSet();//qWarning("osgScene 0");

   stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    // enable texture transparency
   stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
   stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

   geode0->setStateSet(stateset);
   geode0->addDrawable(square.get());
   m_worldReferenceFrame.get()->addChild(geode0.get());//(m_worldAt.get());

   m_switchRoot.get()->addChild(m_worldReferenceFrame.get());
 std::cout << " ++==++==++  displayPlane 6" << std::endl;

}


osg::ref_ptr<osg::MatrixTransform> OsgScene::createAdds()
{
    //if ( ADDS->n_adds<=0 ) 
 qWarning("createAdds 0");
    osg::ref_ptr<osg::MatrixTransform> m_worldAdds = new osg::MatrixTransform;
    osg::StateSet* set = m_worldAdds->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON); 
  qWarning("createAdds 1   ADDS->n_adds=%d", ADDS->n_adds);
   //osg::ref_ptr<osg::Geometry> draw = makeSphere(0.2, INT->sphSlices, INT->sphStacks, osg::Vec4(1.0, 0.0, 0.0, 0.5));
    //float length = static_cast<float>(INT->axL * INT->scDim);
  //std::cout << " length=" << length << std::endl;
    //float radius = INT->axRad * length;   
    for (int i=0; i<ADDS->n_adds; i++) {
       osg::ref_ptr<osg::Geode> geod = new osg::Geode;
       geod->addDrawable(ADDS->markers[0].get());
  qWarning("createAdds 2     i=%d", i);
       osg::Vec3 pos = ADDS->pos.get()->at(i);//MiscFunc::convert(a.pos);
       osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
       mt->setMatrix(osg::Matrix::translate(pos));
       mt->addChild(geod.get());
       m_worldAdds->addChild(mt.get());
  qWarning("createAdds 2.5     i=%d", i);
    }
    //INT->refrAdds = false;
    //INT->m_worldAdds = m_worldAdds;
    return m_worldAdds.get();
}
*/