#include "../include/atoms.h"
#include <filesystem>
#include "globals.h"

Atoms::Atoms()
{
  std::filesystem::path cd = QApplication::applicationDirPath().toStdString();
  std::filesystem::path atomsP = cd.parent_path() / "data/internal/atoms.babel";
  QFile file (atomsP.c_str());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qWarning("atoms.babel not found");
      return;
  }
  
  QTextStream in(&file);
  numbOfAtoms = 0;
  while (!in.atEnd()) {
      QString line = in.readLine();
      QStringList fields = line.split(' ', QString::SkipEmptyParts);
      //         int ile = fields.size();
      namea << fields.at(1);
      a_rad1[numbOfAtoms] = fields.at(2).toFloat(); 
      //a_rad2[numbOfAtoms] = fields.at(3).toFloat();
      a_colors[numbOfAtoms] = osg::Vec4(fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat(), 1.0f);
      //         qWarning("nr=%d, rgb=%f,%f,%f", i, atom_red[i], atom_green[i], atom_blue[i]);
      ++numbOfAtoms;
  }
 qWarning("+++++++++++++++  ATOMS INITIALIZED  +++++++++++++++++++");
    //     if  ( i!=117 ) qWarning("aaaa");
}
