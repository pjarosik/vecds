#include "lattice.h"

Lattice::Lattice()
{
    name = "";
    n_atoms = n_bonds = n_k = 0;
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
    name = atname;
    n_atoms = numbAt;
    marked.assign(numbAt, 0);
    coords.assign(numbAt, glm::dvec3(0., 0., 0.));
    u.assign(numbAt, glm::dvec3(0., 0., 0.));
    du.assign(numbAt, glm::dvec3(0., 0., 0.));
}

void Lattice::clearL()
{
    name = "";
    n_atoms = n_bonds = n_k = 0;
    this->kName.clear();
    this->marked.clear();
    this->coords.clear();//this->coords.get()->clear();
    this->du.clear();
    this->u.clear();
    this->nAt.get()->clear();
    this->nK.get()->clear();
    this->bond1.get()->clear();
    this->bond2.get()->clear();
}
