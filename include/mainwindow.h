// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along      with      this      program.       If      not,      see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "osgscene.h"
#include "osgv.h"
#include "miscfunc.h"
#include "atoms.h"
//#include "points.h"
#include "internal.h"
#include "dialogs.h"
#include "gener.h"
#include "calc.h"
#include "simplepad.h"
#include "helpbrowser.h"

//#include "../FEMApp/FEMAppLib/FEMAppInterface.h"

#include <glm/gtc/quaternion.hpp>

QT_BEGIN_NAMESPACE
class QAction;
//class QActionGroup;
class QLabel;
class QMenu;
class QString;
class QLayout;
QT_END_NAMESPACE

class OsgViewerQt;
class OsgScene;
class Internal;
class QuestionForm2;
class Atoms;
class Gener;
class SimplePad;
//class osg::Group;
//class osgViewer::View;
//class osg::Camera;
//class osgGA::TrackballManipulator;


class MainWindow :  public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    osg::Quat qrot;
    glm::dmat3 rotTens;
    osg::Quat rotQuat;
    QString strBvect;
    QString strAtom;
    QString strPoint;
    QString strName;
    QString iAt;
    QString iStr;
    QDateTime dateTime; // = dateTime.currentDateTime();
    QString dateTimeString; // = dateTime.toString("dd-MM-yyyy hh:mm:ss");//.toStdString();
    bool moreFiles;
    bool screw;
    glm::dvec3 direction;
    
protected:    
    void keyPressEvent(QKeyEvent *keyEv);
    
private:
   
    osg::ref_ptr<OsgViewerQt> mview1;
    osg::ref_ptr<OsgScene> scene;
    double valTh, valPh, valPs, valDist, valCx, valCy;
    osg::Quat YtoZ;
    osg::Quat ZtoY;
    
    int mode;
    QString ansBurgers, ansPlane;
    int numMeth;
    glm::dvec3 sizeB;
    glm::dvec3 sumU;

    QTextEdit *textEdit;
    QWidget *window;
    QString line1;
    SimplePad *eee;

    void cutPlane(glm::dvec3 point);
    void cutBox();

 bool eventFilter(QObject *obj, QEvent *event);

    void createActions();
    void createMenus();
    void createDockWindows();
    void setSliders(double v1, double v2, double v3, double v4, double v5, double v6);//(int v1, int v2, int v3);
    //void keyPressEvent(QKeyEvent *keyEv);
    //void contextMenuEvent(QContextMenuEvent *event);
    void addPOINTS(); //int nr);
    void readAlcXyz(QString aname1);
    void refreshScene();
    void refreshViewer();
    void infoDisplay();
    void process(glm::dvec3 pos);
    
    QWidget *MainWidget;
    QGridLayout *MainLayout;
    
    QLabel *infoLabel;
    QDockWidget *dockW;
    
    QMenu *fileMenu;
    QMenu *defstructMenu;
    //QMenu *genMenu;
    QMenu *boxMenu;
    QMenu *toolsMenu;
    QMenu *displayMenu;
    QMenu *settMenu;
    QMenu *helpMenu;

    QAction *MillerAct;
    QAction *dislocAct;

    QAction *planeAct;
    QAction *cubBoxAct;
    QAction *hexBoxAct;    
    QAction *rombBoxAct;
    QAction *cylindAct;
  
    QAction *defnewAct;
    QAction *chooseAct;
    QAction *edStructAct;    
  
    QAction *openAct;
    QAction *closeAct;
  
    QAction *genAct;
    QAction *imageAct;
    QAction *closeimgAct;
    QAction *openFemAct;
    QAction *computeFemAct;
    QAction *showResAct;
    QAction *closeFemAct;
    
    QAction *settAct;
  
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *aboutOsgAct;
    QAction *atomsAct;
    QAction *bgAct;
    QAction *numAct;
    QAction *lightAct;
    QAction *materialAct;
    QAction *showPointsAct;
    QAction *nsAct;
    QAction *bsAct;
    QAction *bcAct;
    QAction *vectAct;
    QAction *axAct;
    QAction *fovAct;
    QAction *resetAct;
    QAction *editSettAct;
    QAction *chooseSettAct;
    QAction *visibAct;
    QAction *saveAct;
    QAction *calcDislAct;
    QAction *updateAct;
    QAction *moveAct;
    
    QLineEdit *thetaSlider;
    QLineEdit *phiSlider;
    QLineEdit *psiSlider;
    QLineEdit *mxSlider;
    QLineEdit *mySlider;
    QLineEdit *distSlider;
       
signals:
//    void SIG_rotate(osg::Matrix *mat);
    void SIG_keypress(int k);
    void SIG_save();
    //void SIG_actPoint(glm::dvec3 pos);
    
public slots:

    void SL_setPhiSlider();
    void SL_setPsiSlider();
    void SL_setThetaSlider();
    void SL_setDistSlider();//(int);
    void SL_setMxSlider();
    void SL_setMySlider();
    void SL_actPoint();
    void SL_keypress(int k);
    void SL_performDislCalc(QString ff);
    
private slots:

    void SL_move(); 
    void SL_openAtoms();
    void SL_image();
    void SL_closeImage();

    void SL_setBg();
    void SL_setNum();
    void SL_setLight();
    void SL_setMaterial();
    void SL_setPoints();
    void SL_setAx();
    void SL_setFov();

    void SL_atoms();
    void SL_setBonds();
    void SL_setColB();
    void SL_setVect();
    void SL_reset();
    void SL_setNS();
    void SL_chooseStructure();
    void SL_chooseSettings();
    void SL_editStruct();
    void SL_genAtoms();
    void SL_editSett();    
    void SL_about();
    void SL_aboutOsg();        
    void SL_openFem();
    void SL_computeFem();
    void SL_showRes();
    void SL_closeFem();
    void SL_visibility();
    void SL_closeAtoms();
    void SL_calcDisl();
    //void SL_changeMode(int);
    void SL_millerAct();
    void SL_dislocAct();
    void SL_update();
    void SL_bondsAct();
    void SL_saveAtoms();
    void SL_plane();
    void SL_rectBox();
    void SL_hexBox();
    void SL_rombBox();
    void SL_cylinder();
};

#endif // MAINWINDOW_H

