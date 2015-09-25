#include "internal.h"
#include "miscfunc.h"
#include "atoms.h"
 
extern Atoms *AT;
//extern CFEMAppInterface *Femi;
 
Internal::Internal()
{
//             0              1            2         3           4             5             6           7           8             9             10               11              12
  words << "bgColor" << "bondColor" << "optim" << "fov" << "radFactor" << "alphaAt" << "bondWidth" << "axis" << "switches" << "sizeNumb" << "matDiffuse" << "matSpecular" << "matShiness" 
//      13            14              15              16               17               18              19              20               21              22            23         24
  << "sphere" << "ls1Ambient" << "ls1Diffuse" << "ls1Specular" << "ls1Position" << "ls2Ambient" << "ls2Diffuse" << "ls2Specular" << "ls2Position" << "detRatio" << "alphaIm" << "atom"
//       25              26            27             28              29                30                31
  << "structDef" << "structAtom" << "alphaFem" << "femColorsL" << "femColorsP" << "femColorsResL" << "femColorsNumb";	

  colRed = osg::Vec4(1.0, 0.0, 0.0, 1.0);
  colGreen = osg::Vec4(0.0, 1.0, 0.0, 1.0);
  colBlue = osg::Vec4(0.0, 0.0, 1.0, 1.0);
  colWhite = osg::Vec4(1.0, 1.0, 1.0, 1.0);
  colBlack = osg::Vec4(0.0, 0.0, 0.0, 1.0);
  colGray  = osg::Vec4(0.5, 0.5, 0.5, 1.0);
  colDGray  = osg::Vec4(0.2, 0.2, 0.2, 1.0);
  colLGray  = osg::Vec4(0.8, 0.8, 0.8, 1.0);
  outLog.open("vecds_log.txt", std::ofstream::app);
  image = "";
  atName = "";
  fem = "";
  compFem = "";
  name  = "";
  choosen_value = "";
  nu = 0.35;
  r0_fact = 1.0;
  ndisl = 0;
  npoints = 0;
  //points = " a b \n c";
  nr = -1;
  refrAtoms = false;
  refrAdds = false;
  addsDisplay = false;
  refrImage = false;
  refrFem = false;
  refrRes = false;
  msgGraph = false;
  hklDefined = false;
  atomDefined = false;
  pointDefined = false;
  refrMarked = false;
  dist0 = -1.;
  centr0 = osg::Vec3d(0., 0., 0.);
  currDir = QApplication::applicationDirPath();//QDir::currentPath();
  currDir.replace(QString("/bin"), QString(""));

  whichFem = 0;
//std::cout << "fov=" << fov << "  sizeTxt=" << sizeTxt << "  axInd=" << axInd << "  alphaAx=" << alphaAx << "   alphaAt=" << alphaAt << std::endl;
  structureDefined = false;
  initStructures();

  ndiv = 10;
  ncol = 6;
  pointSize = 4.0;    
  sett = "/data/internal/defaults0.txt";
  readDefaults(sett);
  std::cout << "DEFAULTS" << std::endl;
  settDefined = false;
  crCDefined = false;
  
  pressed = false;
  //calcLog = "";
  //calcLog.clear();

}

void Internal::initStructures()
{
  QString cd0 = currDir;
  cd0.append("/data/internal/");
  QStringList nameFilter("*.struct");
  QDir directory(cd0);
  structList.clear();
  structNames.clear();
  QStringList structNames1 = directory.entryList(nameFilter);
  QString one;
  foreach (one, structNames1) {
     if ( readDefaults(QString("/data/internal/").append(one)) && crCDefined ) {
        structList << *crC;
	structNames << crC->crCName;
	//structDesc[i++] = 1;
        delete crC;
	crCDefined = false;
     } else {
	QString str = QString("Structure %1 is not defined").arg(one);
	QMessageBox::warning(0, "PROBLEM", str);
     }
  }  
  std::cout << "+++++++++++++  Structures initialized" << std::endl;
}

bool Internal::readDefaults(QString sett)
{
  QString cd0 = currDir;
  QFile file (cd0.append(sett));
  if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) { qWarning("-------- defaults file not found");  return false; }
  QTextStream in(&file);
std::cout << " file=" << sett.toStdString() << std::endl;
  int nl = 0;
  while (!in.atEnd()) {
     QString line = in.readLine();
     ++nl;
//  std::cout << line.toStdString() << std::endl;
     if ( line.isEmpty() || (line.at(0)=='/' && line.at(1)=='/') ) continue;
     if ( line.at(0)=='!' && line.at(1)=='!' ) {
        QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);//line.split(' ', QString::SkipEmptyParts); 
        int what = MiscFunc::whichString(fields.at(1), words);
        if ( fields.size()<3 || what<0 ) {  std::cout << " readDefaults: error at line " << nl << std::endl; return false;  }
	switch ( what ) {
	 case 0: // bgColor
//    std::cout << fields.at(0).toStdString() << "  " << fields.at(1).toStdString() << "  " << fields.at(2).toStdString() << "  " << std::endl;
            bgColor = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());  break;
         case 1:  // bondColor
	    bondColor = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());  break;
	 case 2: // optim
	    optim = ( fields.at(2).compare("T")==0 || fields.at(2).compare("t")==0 );  break;
	 case 3: // fov
	    fov = fields.at(2).toDouble();  break;
	 case 4: // radFactor
	    radFactor = fields.at(2).toFloat();  break;
	 case 5: // alphaAt
	    alphaAt = fields.at(2).toFloat();  alphaB = fields.at(3).toFloat();  break;
	 case 6: // bondWidth
	    bondFactor = fields.at(2).toFloat();  break;
	 case 7: // axis
	    axInd = fields.at(2).toInt(); axPr1 = fields.at(3).toFloat(); axPr2 = fields.at(4).toFloat(); axL = fields.at(5).toFloat(); axRad = fields.at(6).toFloat();  alphaAx = fields.at(7).toFloat(); break;
	 case 8: // switches
	    lightOn = ( fields.at(2).compare("T")==0 );  materialOn = ( fields.at(3).compare("T")==0 ); showNum = ( fields.at(4).compare("T")==0 ); showAdds = ( fields.at(5).compare("T")==0 );  break;
	 case 9: // sizeNumb
            sizeTxt = fields.at(2).toFloat();  break;
	 case 10:  // matDiffuse
	    matDiffuse = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());    break; 
	 case 11:  // matSpecular
	    matSpecular = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
	 case 12:  // matShiness
	    matShiness = fields.at(2).toFloat();   break;            
	 case 13:  // sphere
	    sphSlices = fields.at(2).toInt();  sphStacks = fields.at(3).toInt();   break; 
	 case 14:  // ls1Ambient
	    ls1Ambient = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
	 case 15:  // ls1Diffuse
	    ls1Diffuse = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
         case 16:  // ls1Specular
	    ls1Specular = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break; 
	 case 17:  // ls1Position
	    ls1Position = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
	 case 18:  // ls2Ambient
	    ls2Ambient = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
	 case 19:  // ls2Diffuse
	    ls2Diffuse = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
         case 20:  // ls2Specular
	    ls2Specular = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break; 
	 case 21:  // ls2Position
	    ls2Position = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());     break;
	 case 22:  // detRatio
	    detRatio = fields.at(2).toFloat(); //  std::cout << " detRatio=" << detRatio << std::endl;    break;
	 case 23:  // alphaIm
	    alphaIm = fields.at(2).toFloat();   break;
	 case 24:  // atom
	    nr = fields.at(2).toInt(); name = fields.at(3); //std::cout << " nr=" << nr << "     namea=" << AT->namea.at(nr).toStdString() << std::endl;	    
	    if ( AT->namea.at(nr).compare(name)==0 ) {
	       AT->a_rad1[nr] = fields.at(4).toFloat(); 
	       AT->a_colors[nr] = osg::Vec4(fields.at(5).toFloat(), fields.at(6).toFloat(), fields.at(7).toFloat(), 1.0f);
      	       break;
	    } else { std::cout << "-------------- Bad Atom" << std::endl;  break; }
	 case 25:  // structDef
	    crCDefined = true;
	    crC = new CrysCell;
	    crC->crCName = fields.at(2);
	    crC->numCellAt = 0;
	    crC->a = fields.at(3).toDouble();
	    crC->b = fields.at(4).toDouble();
	    crC->c = fields.at(5).toDouble();     
	    crC->alpha = fields.at(6).toDouble();
	    crC->beta = fields.at(7).toDouble();
	    crC->gamma = fields.at(8).toDouble();
	    cs6 = cos(MiscFunc::deg2rad*crC->gamma);
	    sn6 = sin(MiscFunc::deg2rad*crC->gamma);
            if ( crC->gamma==90. ) { cs6 = 0.;  sn6 = 1.; }
            cs5 = cos(MiscFunc::deg2rad*crC->beta);
            cs4 = cos(MiscFunc::deg2rad*crC->alpha);
            if ( crC->alpha==90. ) cs4 = 0.;
            if ( crC->beta==90. ) cs5 = 0.;
            crC->c2o = glm::dmat3(crC->a,     0.,         0.,
                                  crC->b*cs6, crC->b*sn6, 0.,
                                  crC->c*cs5, crC->c*(cs4-cs5*cs6)/sn6, crC->c*sqrt(1.+2.*cs4*cs5*cs6-cs4*cs4-cs5*cs5-cs6*cs6)/sn6);
            crC->o2c = glm::inverse(crC->c2o);
	    break;
	 case 26:  // structAtom
     	    if ( !crCDefined )  {  std::cout << " ERROR: structure not defined - line nr. " << nl << std::endl;  return false;  }
	    crC->cellAt << MiscFunc::whichAtom(fields.at(2));
	    crC->cellCoord << glm::dvec3(MiscFunc::readFraction(fields.at(3)), MiscFunc::readFraction(fields.at(4)), MiscFunc::readFraction(fields.at(5)));
	    ++crC->numCellAt;
	    break;
	 case 27:  // alphaFem
	    alphaFemT = fields.at(2).toFloat();  alphaFemL = fields.at(3).toFloat();  alphaFemP = fields.at(4).toFloat();
	    break;
	 case 28:  //femColorsL
	    ColorL = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());   break;
	 case 29:  //femColorsP
	    ColorP = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());  pointSize = fields.at(6).toFloat();  break;
	 case 30:  //femColorsResL
	    ColorResL = osg::Vec4(fields.at(2).toFloat(), fields.at(3).toFloat(), fields.at(4).toFloat(), fields.at(5).toFloat());   break;
	 case 31:  //femColorsNumb
	    ndiv = fields.at(2).toInt();    ncol = fields.at(3).toInt();     break;
	 default:
	    std::cout << " ERROR readDefaults - line nr. " << nl << std::endl;
        } // switch ( what )
     } else return false; // if !isEmpty && !"//"
  } // while
  file.close();
  return true;
}
