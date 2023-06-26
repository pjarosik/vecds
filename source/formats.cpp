#include "formats.h"
#include "miscfunc.h"
#include "globals.h"

/**
 * Reads input Alchemy stream to the Lattice object.
 *
 * @param in input stream to read
 * @return lattice read from the stream
 */
FormatReadResult readAlc(QTextStream &in, const QString &name) {
    Lattice lattice;
    FormatResultBuilder validationResultBuilder;
    QString line = in.readLine();
    if(line.isEmpty()) {
        validationResultBuilder.addError("First line is empty!");
        return {std::nullopt, validationResultBuilder.build()};
    }

    QStringList fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    int nAtoms = fields.at(0).toInt();
    if(nAtoms <= 0) {
        validationResultBuilder.addError("No defined atoms in this structure");
        return {std::nullopt, validationResultBuilder.build()};
    }
    int nBonds = fields.at(2).toInt();
    lattice.init(name, nAtoms);
    for(int i = 0; i < nAtoms; ++i) {
        line = in.readLine();
        fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        fields.removeFirst();
        QString nameA = fields.takeFirst();
        int ak = MiscFunc::whichAtom(nameA);
        if(ak < 0) {
            ak = 0;
            validationResultBuilder.addWarning(QString("Unidentified atom %2 on position %1").arg(i + 1).arg(nameA).toStdString());
        }
        // Nk -- the set of atoms
        if(!lattice.hasNk(ak)) {
            lattice.addNk(ak);
        }
        double x = fields.takeFirst().toDouble();
        double y = fields.takeFirst().toDouble();
        double z = fields.takeFirst().toDouble();
        lattice.setCoords(i, glm::dvec3(x, y, z));
        lattice.setMarked(i, 1);
        lattice.addNAt(ak);
    }
    for(int i = 0; i < nBonds; ++i) {
        line = in.readLine();
        fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        int i1 = fields.at(1).toInt();
        int i2 = fields.at(2).toInt();
        lattice.addBond1(i1);
        lattice.addBond2(i2);
    }
    return {lattice, validationResultBuilder.build()};
}

/**
 * Reads input XYZ stream to the Lattice object.
 *
 * @param in input stream to read
 * @return lattice read from the stream
 */
FormatReadResult readXyz(QTextStream &in, const QString &name) {
    Lattice lattice;
    FormatResultBuilder validationResultBuilder;
    QString line = in.readLine();
    if(line.isEmpty()) {
        validationResultBuilder.addError("First line is empty!");
        return {std::nullopt, validationResultBuilder.build()};
    }

    QStringList fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    int nAtoms = fields.at(0).toInt();
    if(nAtoms <= 0) {
        validationResultBuilder.addError("No defined atoms in this structure");
        return {std::nullopt, validationResultBuilder.build()};
    }

    in.readLine(); // Ignore one line.

    lattice.init(name, nAtoms);
    for(int i = 0; i < nAtoms; ++i) {
        line = in.readLine();
        fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        QString nameA = fields.takeFirst();
        int ak = MiscFunc::whichAtom(nameA);
        if(ak < 0) {
            ak = 0;
            validationResultBuilder.addWarning(QString("Unidentified atom %2 on position %1").arg(i + 1).arg(nameA).toStdString());
        }
        // Nk -- the set of atoms
        if(!lattice.hasNk(ak)) {
            lattice.addNk(ak);
        }
        double x = fields.takeFirst().toDouble();
        double y = fields.takeFirst().toDouble();
        double z = fields.takeFirst().toDouble();
        lattice.setCoords(i, glm::dvec3(x, y, z));
        lattice.setMarked(i, 1);
        lattice.addNAt(ak);
    }
    return {lattice, validationResultBuilder.build()};
}

/**
 * Writes input lattice in a XYZ format t the given output stream.
 */
FormatResult writeXyz(std::ostream &out, const Lattice &lattice) {
    out << lattice.getNAtoms() << std::endl << std::endl;
    for(int i = 0; i < lattice.getNAtoms(); ++i) {
        int numbAt = lattice.getNAt().at(i);
        auto coords = lattice.getCoords().at(i);
        out << setw(5) << AT->getName(numbAt).toStdString()
            << setw(15) << coords.x << setw(15) << coords.y << setw(15) << coords.z << std::endl;
    }
    return FormatResult{};
}

/**
 * Reads input Sybyl Mol2 stream to the Lattice object.
 *
 * @param in input stream to read
 * @return lattice read from the stream
 */
FormatReadResult readMol2(QTextStream &in, const QString &name) {
    Lattice lattice;
    FormatResultBuilder validationResultBuilder;
    in.readLine(); // @<TRIPOS>MOLECULE, ignore
    in.readLine(); // Name, ignore.

    QString line = in.readLine();
    QStringList fields = line.trimmed().split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    int nAtoms = fields.at(0).toInt();
    int nBonds = fields.at(1).toInt();
    if(nAtoms <= 0) {
        validationResultBuilder.addError("No defined atoms in this structure");
        return {std::nullopt, validationResultBuilder.build()};
    }

    in.readLine(); // Descriptor, .e.g. SMALL, ignore.
    in.readLine(); // Descriptor ?, ignore.
    in.readLine(); // Empty line.
    in.readLine(); // @<TRIPOS>ATOM, ignore.

    lattice.init(name, nAtoms);
    for(int i = 0; i < nAtoms; ++i) {
        line = in.readLine();
        fields = line.trimmed().split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        QString nameA = fields.at(1);
        int ak = MiscFunc::whichAtom(nameA);
        if(ak < 0) {
            ak = 0;
            validationResultBuilder.addWarning(QString("Unidentified atom %2 on position %1").arg(i + 1).arg(nameA).toStdString());
        }
        // Nk -- the set of atoms
        lattice.addNk(ak);
        double x = fields.at(2).toDouble();
        double y = fields.at(3).toDouble();
        double z = fields.at(4).toDouble();
        lattice.setCoords(i, glm::dvec3(x, y, z));
        lattice.setMarked(i, 1);
        lattice.addNAt(ak);
    }
    in.readLine(); // @<TRIPOS>BOND
    for(int i = 0; i < nBonds; ++i) {
        line = in.readLine();
        fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        // MOL2 has numbering starting from 1.
        int i1 = fields.at(1).toInt();
        int i2 = fields.at(2).toInt();
        lattice.addBond1(i1);
        lattice.addBond2(i2);
    }
    return {lattice, validationResultBuilder.build()};
}

/**
 * Writes input lattice in a Sybyl MOL2 format to the given output stream.
 * NOTE: currently this function does not save the proper valency value.
 */
FormatResult writeMol2(std::ostream &out, const Lattice &lattice) {
    out << "@<TRIPOS>MOLECULE" << std::endl;
    out << "VECDS" << std::endl;
    out << " " << lattice.getNAtoms() << " " << lattice.getNBonds() << " 0 0 0" << std::endl;
    out << "VECDS" << std::endl;
    out << "VECDS" << std::endl;
    out << std::endl;
    out << "@<TRIPOS>ATOM" << std::endl;
    for(int i = 0; i < lattice.getNAtoms(); ++i) {
        int numbAt = lattice.getNAt().at(i);
        auto coords = lattice.getCoords().at(i);
        std::string atomName = AT->getName(numbAt).toUpper().toStdString();
        out << setw(5) << (i+1)
            << setw(5) << atomName
            << setw(15) << coords.x << setw(15) << coords.y << setw(15) << coords.z
            << setw(5) << atomName << setw(5) << "1" << setw(5) << "UNL1" << setw(15) << 0.0
            << std::endl;
    }
    out << "@<TRIPOS>BOND" << std::endl;
    for(int i = 0; i < lattice.getNBonds(); ++i) {
        out << setw(5) << (i+1)
            << setw(5) << lattice.getBond1()[i]
            << setw(5) << lattice.getBond2()[i]
            << setw(5) << 1
            << std::endl; // TODO store valency value in the lattice and write proper value here
    }
    return FormatResult{};
}