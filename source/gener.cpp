#include "../include/gener.h"
#include <iostream>
#include "globals.h"


Lattice Gener::genLattice(const int nx0, const int ny0, const int nz0, const int nx1, const int ny1, const int nz1) {
    int ak, count = 0;
    QList<int> numbers;
    numbers.clear();

    Lattice lattice;

    for(int i = 0; i < INT->crC->numCellAt; i++) {
        ak = INT->crC->cellAt.at(i);
        numbers << ak;
        lattice.addNk(ak);
    }
    for(int k = nz0; k < nz1; k++) {
        for(int j = ny0; j < ny1; j++) {
            for(int i = nx0; i < nx1; i++) {
                for(int an = 0; an < INT->crC->numCellAt; an++) {
                    count++;
                    glm::dvec3 hic = glm::dvec3(double(i) + INT->crC->cellCoord.at(an).x,
                                                double(j) + INT->crC->cellCoord.at(an).y,
                                                double(k) + INT->crC->cellCoord.at(an).z);
                    glm::dvec3 pos = INT->crC->c2o * hic;
                    lattice.coords.push_back(pos);
                    lattice.nAt.push_back(numbers.at(an));
                }
            }
        }
    }
    lattice.marked.assign(count, 0);
    lattice.u.assign(count, glm::dvec3(0., 0., 0.));
    lattice.du.assign(count, glm::dvec3(0., 0., 0.));
    return lattice;
}
