#include "../include/points.h"
#include "../include/atoms.h"
#include "../include/internal.h"

extern Atoms *AT;
extern Internal *INT;


 Points::Points()
 {
    n_points = 0;
    n_markers = 6;
    pos = new osg::Vec3Array;	
    nKind = new osg::IntArray;    
    for (int i=0; i<n_markers; i++) {
       osg::Vec4 color = AT->a_colors[i+117];
       float r = INT->radFactor * AT->a_rad1[i+117];
       markers[i] = MiscFunc::makeSphere(r, INT->sphSlices, INT->sphStacks, color);
    } 
  std::cout << "POINTS CREATED" << std::endl;   
 }
  
 Points::~Points()
 {
    clearP();
 }
 
 void Points::clearP()
 {
    this->n_points = 0;
    this->pos.get()->clear();
    this->nKind.get()->clear();
    this->millerVs.clear();
    this->millerPs.clear();
    this->vectors.clear();
    this->fracts.clear();
    this->rotTens.clear();
    this->strName.clear();
    this->strBV.clear();
    this->crCNum.clear();
    this->numMeth.clear();
 }
/*
     POINTS->crCNum.push_back(INT->crCNum);
     POINTS->strBV.push_back(ansBurgers);
     POINTS->strName.push_back(strName);
     POINTS->numMeth.push_back(numMeth);
*/     
