#include "atoms.h"

extern Internal *INT;

Atoms::Atoms()
{
  QString cd0 = QApplication::applicationDirPath();//QDir::currentPath();
  cd0.replace(QString("/bin"), QString(""));
  //  qWarning("%s", cd0.toLatin1().data());
  QFile file (cd0.append("/data/internal/atoms.babel"));
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
