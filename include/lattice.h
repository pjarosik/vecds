#ifndef LATTICE_H
#define LATTICE_H

#include "internal.h"

// TODO consider making this class immutable
class Lattice
{
 public:

    void init(QString name, int numbAt)
    {
        this->name = std::move(name);
        marked.assign(numbAt, 1);
        coords.assign(numbAt, glm::dvec3(0., 0., 0.));
        u.assign(numbAt, glm::dvec3(0., 0., 0.));
        du.assign(numbAt, glm::dvec3(0., 0., 0.));
    }

    Lattice() = default;

    ~Lattice() = default;

    void clear()
    {
        name = "";
        marked.clear();
        coords.clear();
        du.clear();
        u.clear();
        nAt.clear();
        nK.clear();
        bond1.clear();
        bond2.clear();
    }

  // TODO the below list of friend classes should be remove in favour of getters/setters.
  friend class MainWindow;
  friend class OsgScene;
  friend class MiscFunc;
  friend class Gener;
  friend class Calc;

    const vector<glm::dvec3> &getCoords() const {
        return coords;
    }
    void setCoords(size_t i, const glm::dvec3 &coords) {
        this->coords[i] = coords;
    }
    const vector<glm::dvec3> &getU() const {
        return u;
    }
    void setU(const vector<glm::dvec3> &u) {
        Lattice::u = u;
    }
    const vector<glm::dvec3> &getDu() const {
        return du;
    }
    void setDu(const vector<glm::dvec3> &du) {
        Lattice::du = du;
    }
    const vector<int> &getMarked() const {
        return marked;
    }
    void setMarked(size_t i, int marked) {
        this->marked[i] = marked;
    }
    const std::vector<int> &getNAt() const {
        return nAt;
    }
    void addNAt(int nAt) {
        this->nAt.push_back(nAt);
    }
    const std::vector<int> &getBond1() const {
        return bond1;
    }
    void addBond1(int b) {
        this->bond1.push_back(b);
    }
    const std::vector<int> &getBond2() const {
        return bond2;
    }
    void addBond2(int b) {
        this->bond2.push_back(b);
    }
    
    const std::set<int> &getNk() const {
        return this->nK;
    }

    void addNk(int n) {
        this->nK.insert(n);
    }

    bool hasNk(int k) const {
        return this->nK.find(k) != std::end(this->nK);
    }

    CrysCell *getCrC() const {
        return crC;
    }
    void setCrC(CrysCell *crC) {
        Lattice::crC = crC;
    }
    std::vector<double> getXCoords() const {
        std::vector<double> result(coords.size(), 0);
        std::transform(std::begin(coords), std::end(coords), std::begin(result),
                       [](const glm::dvec3 &coord){return (double)coord[0];});
        return result;
    }
    std::vector<double> getYCoords() const {
        std::vector<double> result(coords.size(), 0);
        std::transform(std::begin(coords), std::end(coords), std::begin(result),
                       [](const glm::dvec3 &coord){return (double)coord[1];});
        return result;
    }
    std::vector<double> getZCoords() const {
        std::vector<double> result(coords.size(), 0);
        std::transform(std::begin(coords), std::end(coords), std::begin(result),
                       [](const glm::dvec3 &coord){return (double)coord[2];});
        return result;
    }
    double getXMin() const {
        std::vector<double> c = this->getXCoords();
        if(c.empty()) {
            return -1e30;
        }
        return *std::min_element(std::begin(c), std::end(c));
    }
    double getYMin() const {
        std::vector<double> c = this->getYCoords();
        if(c.empty()) {
            return -1e30;
        }
        return *std::min_element(std::begin(c), std::end(c));
    }
    double getZMin() const {
        std::vector<double> c = this->getZCoords();
        if(c.empty()) {
            return -1e30;
        }
        return *std::min_element(std::begin(c), std::end(c));
    }
    double getXMax() const {
        std::vector<double> c = this->getXCoords();
        if(c.empty()) {
            return 1e30;
        }
        return *std::max_element(std::begin(c), std::end(c));
    }
    double getYMax() const {
        std::vector<double> c = this->getYCoords();
        if(c.empty()) {
            return 1e30;
        }
        return *std::max_element(std::begin(c), std::end(c));
    }
    double getZMax() const {
        std::vector<double> c = this->getZCoords();
        if(c.empty()) {
            return 1e30;
        }
        return *std::max_element(std::begin(c), std::end(c));
    }
    double getScDim() const {
        double scDim = std::max(getYMax()-getYMin(), getZMax()-getZMin());
        return std::max(getXMax()-getXMin(), scDim);
    }

    int getNAtoms() const {
        return this->nAt.size();
    }

    int getNBonds() const {
        assert(this->bond1.size() == this->bond2.size());
        return this->bond1.size();
    }

    int getNElements() const {
        return nK.size();
    }

    int getNMarked() const {
        return this->marked.size();
    }

    const QString &getName() const {
        return name;
    }

    void setName(const QString &name) {
        Lattice::name = name;
    }

    void append(const Lattice& other) {
        this->coords.insert(std::end(this->coords), std::begin(other.coords), std::end(other.coords));
        this->marked.insert(std::end(this->marked), std::begin(other.marked), std::end(other.marked));
        this->du.insert(std::end(this->du), std::begin(other.du), std::end(other.du));
        this->u.insert(std::end(this->u), std::begin(other.u), std::end(other.u));
        size_t numberOfAtoms = this->nAt.size();
        std::vector<int> bond1Copy(other.bond1.size(), 0);
        std::vector<int> bond2Copy(other.bond2.size(), 0);
        std::transform(std::begin(other.bond1), std::end(other.bond1), std::begin(bond1Copy),
                       [&](auto value) {return value + numberOfAtoms;});
        std::transform(std::begin(other.bond2), std::end(other.bond2), std::begin(bond2Copy),
                       [&](auto value) {return value + numberOfAtoms;});
        this->nAt.insert(std::end(nAt), std::begin(other.nAt), std::end(other.nAt));
        this->nK.insert(std::begin(other.nK), std::end(other.nK));
        // Keep the previous name.
    }

    Lattice getMarkedOnly() {
        Lattice result(*this);
        result.clear();
        result.setName(this->name);
        result.nK.insert(std::begin(this->nK), std::end(this->nK));

        // Copy atoms marked only.
        for(int i = 0; i < getNAtoms(); ++i) {
            if(this->marked[i]) {
                result.coords.push_back(this->coords[i]);
                result.u.push_back(this->u[i]);
                result.du.push_back(this->du[i]);
                result.marked.push_back(this->marked[i]);
                result.nAt.push_back(this->nAt[i]);
            }
        }
        // Copy bonds that are connected to marked atoms.
        assert(this->bond1.size() == this->bond2.size());
        for(int i = 0; i < this->bond1.size(); ++i) {
            int first = this->bond1[i];
            int second = this->bond2[i];
            if(this->marked[first] && this->marked[second]) {
                this->bond1.push_back(first);
                this->bond2.push_back(second);
            }
        }
        return result;
    }

    Lattice rotate(const glm::dmat3& rotationTensor) {
        Lattice result(*this);
        for(int i = 0; i < getNAtoms(); ++i) {
            glm::dvec3 p1 = rotationTensor*this->coords[i];
            result.coords[i] = p1;
        }
        // TODO What about u and du?
        return result;
    }

private:
    // Each entry in the below vectors represents a state of an atom.
  std::vector<glm::dvec3> coords;
  std::vector<glm::dvec3> u;
  std::vector<glm::dvec3> du;
  std::vector<int> marked;
  // The list of atomic numbers in the lattice.
  std::vector<int> nAt;
    // Each entry in the below vectors represents a state of a bond.
  // Atom bonds - 1st atom (index of nAt) note: the graph is undirected
  // !!!!! NOTE: The assumption is that the numbering of atoms in the below bond1 and bond2 lists starts from 1!!!!!!
  std::vector<int> bond1;
  // Atom bonds - 2nd atom (index of nAt) note: the graph is undirected
  std::vector<int> bond2;
  // The set (dictionary) of atomic numbers included in the lattice.
  std::set<int> nK;
  CrysCell *crC;
  QString name;
  
};  
#endif //LATTICE_H
