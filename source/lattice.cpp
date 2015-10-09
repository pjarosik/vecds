#include "../include/lattice.h"

Lattice::Lattice()
{
	n_atoms = n_bonds = n_k = 0;
	name = "";
        coords = new osg::Vec3Array;
        //u = new osg::Vec3Array;
        //du = new osg::Vec3Array;				
	nAt = new osg::IntArray;
        bond1 = new osg::IntArray;
        bond2 = new osg::IntArray;
        nK = new osg::IntArray;
}
