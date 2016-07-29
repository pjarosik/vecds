#include "../include/lattice.h"

Lattice::Lattice()
{
    name = "";
    n_atoms = n_bonds = n_k = n_marked = 0;
    nAt = new osg::IntArray;
    bond1 = new osg::IntArray;
    bond2 = new osg::IntArray;
    nK = new osg::IntArray;
    xMin = yMin = zMin = 1.e30;
    xMax = yMax = zMax = -1.e30;
}

Lattice::~Lattice()
{
    clearL();
}

void Lattice::init(QString atname, int numbAt)//, int numbBond,
{
  std::cout << "LATT - init" << std::endl;
    name = atname;
    n_atoms = n_marked = numbAt;
    marked.assign(numbAt, 1);
    coords.assign(numbAt, glm::dvec3(0., 0., 0.));
    u.assign(numbAt, glm::dvec3(0., 0., 0.));
    du.assign(numbAt, glm::dvec3(0., 0., 0.));
    //if ( nb>0 ) {
}

void Lattice::clearL()
{
    name = "";
    n_atoms = n_bonds = n_k = n_marked = 0;
    kName.clear();
    marked.clear();
    coords.clear();//coords.get()->clear();
    du.clear();
    u.clear();
    nAt.get()->clear();
    nK.get()->clear();
    bond1.get()->clear();
    bond2.get()->clear();
    xMin = yMin = zMin = 1.e30;
    xMax = yMax = zMax = -1.e30;
}
