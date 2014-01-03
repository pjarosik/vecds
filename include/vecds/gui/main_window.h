
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

//#include <QWidget>
#include <QtGui>
#include <QMainWindow>
//#include <qwt_slider.h>
#include <QSlider>
//#include <qwt_scale_engine.h>
//#include <qwt_color_map.h>
#include <vecds/base/algebra.h>

QT_BEGIN_NAMESPACE

class QAction;
//class QwtSlider;
class QSlider;
class QLabel;
class QMenu;
class QString;
class QTabBar;
class QFormWidget;
class QStackedWidget;
class QLayout;
class QWthatsThis;
class QVector3D;
//class QVector;

QT_END_NAMESPACE

class MainViewer;
class HelpBrowser;
class Questionform;
class ArcBall;

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
    public:
  MainWindow ();
  
  private slots:
//  void SL_setSliderValue(double);
  void SL_setSliderValue(int);
  void SL_defineStructure();
  void SL_chooseStructure();
  void SL_openAtoms();
  void SL_genAtoms();
  void SL_gen1Atoms();
  void SL_openImg();
  void SL_closeImg();
  void SL_openFems();
  void SL_openRes();
  void SL_about();
  void SL_millerAct();
  void SL_dislocAct();
  void SL_dislAct();
  void SL_changeMode(int mode);
  void SL_saveAtomsAs();
  void SL_saveMarkedAtomsAs();
  void SL_cubBox();
  void SL_hexBox();
//  void SL_performActions();
  //    void SL_bondsAct();
  //    void SL_showBonds(QVector3D);
  void SL_actPoint(QVector3D);
  void SL_actPosition(QVector3D);
  void SL_sett();
  void SL_mult();
  
 signals:
  void SIG_prepareRotation();
  void SIG_prepareImg();
  void SIG_needDraw();
  void SIG_repaint();
  void SIG_keypress(int);
  void SIG_actPoint(QVector3D);
  //    void SIG_actPointChanged(QVector3D);
  //    void SIG_updateSpheres();
 private:
  void InfoDisplay();
  //    void deleteActions();
  void saveAtoms(QString);
  void saveChoosedAtoms(QString);
  //    void deleteActions();
//  QwtSlider *createSlider(double from, double to, double step, double val, 
//                                          bool logaritmic=false);
  QSlider *createSlider(int from, int to, int step, int val);

  void keyPressEvent(QKeyEvent *keyEv);
  void createActions();
  void createMenus();
  void createStatusBar();
  void createDockWindows();
  
  QString toRichText(QString txt);
  
  
  QGridLayout *LAY_g_MainLayout;
  QWidget *Widg_widget0;
  QDockWidget *DWidg_dock;
  QTabBar *Widg_modesTab;
  
  QString aname, iname, fname, resname;
  QString infotxt0, infotxtat, infotxtimg;
  
  MainViewer *mview1;
  
  QMenu *fileMenu;
  QMenu *defstructMenu;
  QMenu *genMenu;
  QMenu *boxMenu;
  QMenu *viewMenu;
  QMenu *settMenu;
  //    QMenu *showMenu;
  QMenu *quitMenu;
  QMenu *helpMenu;
  //    QMenu *setMenu; 
  
  QAction *MillerAct;
  QAction *saveAsAct;
  QAction *saveMarkedAsAct;
  //    QAction *refreshAct;
  QAction *cubBoxAct;
  QAction *hexBoxAct;
  
  QAction *defnewAct;
  //    QAction *defCoresAct;
  QAction *chooseAct;    
  QAction *openAct;
  QAction *openFemAct;
  QAction *openResAct;
  QAction *genAct;
  QAction *gen1Act;
  QAction *openimgAct;
  QAction *closeimgAct;
  QAction *transpAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *settAct;
  QAction *multAct;
//  QAction *quitAct; // Action fot the quit button.
//  QAction *performAct;

  QLabel *infoLabel;
/*
  QwtSlider *phiSlider;
  QwtSlider *thetaSlider;
  QwtSlider *psiSlider;
  QwtSlider *mxSlider;
  QwtSlider *mySlider;
  QwtSlider *distSlider;
*/
  QSlider *phiSlider;
  QSlider *thetaSlider;
  QSlider *psiSlider;
  QSlider *mxSlider;
  QSlider *mySlider;
  QSlider *distSlider;

  QPushButton *Butt_rotMiller;
  QPushButton *Butt_disloc;
  QPushButton *Butt_sf;
  QPushButton *Butt_qd;
  
  QStackedWidget *Lay_main0;
  
};
#endif
