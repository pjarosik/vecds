
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010), Toby D. Young
// (2012).
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------


#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <QtGui>
#include <QMainWindow>
#include <QSlider>

#include <qwt_scale_engine.h>

#include <vecds/algebra.h>
#include <vecds/main_viewer.h>

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

QT_END_NAMESPACE

class MainViewer;
class DocBrowser;
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

                                 /* These are help pages that open in
				  * a new window.
				  *
				  * First, the "about vecds" page */
  void SL_about ();

                                 /* and second, the "documentation of
				    vecds" page */
  void SL_documentation ();

  void SL_millerAct();
  void SL_dislocAct();
  void SL_dislAct();
  void SL_addCoordAct();
  void SL_changeMode(int mode);
  void SL_saveAtomsAs();
  void SL_cubBox();
  void SL_hexBox();
  //    void SL_bondsAct();
  //    void SL_showBonds(QVector3D);
  void SL_actPoint(QVector3D);
  void SL_actPosition(QVector3D);
  void SL_sett();
  void SL_mult();
  
 signals:
  void SIG_prepareImg();
  void SIG_needDraw();
  void SIG_repaint();
  void SIG_keypress(int);
  void SIG_actPoint(QVector3D);
  //    void SIG_actPointChanged(QVector3D);
  //    void SIG_updateSpheres();
 private:

                                 /* Actions that do things */
  QAction *aboutAct;
  QAction *documentationAct;
  QAction *aboutQtAct;



  void InfoDisplay();
  //    void deleteActions();
  void saveAtoms(QString);
  void saveChoosedAtoms(QString);
  //    void deleteActions();
//  QwtSlider *createSlider(double from, double to, double step, double val, bool logaritmic=false);
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
  
  vecds::MainViewer *mview1;
  
  QMenu *fileMenu;
  QMenu *defstructMenu;
  QMenu *genMenu;
  QMenu *boxMenu;
  QMenu *viewMenu;
  QMenu *settMenu;
  //    QMenu *showMenu;
  QMenu *helpMenu;
  //    QMenu *setMenu; 
  
  QAction *MillerAct;
  QAction *saveAsAct;
  //    QAction *refreshAct;
  QAction *cubBoxAct;
  QAction *hexBoxAct;
  
  QAction *defnewAct;
  //    QAction *defCoresAct;
  QAction *chooseAct;    
  QAction *openAct;
  QAction *genAct;
  QAction *gen1Act;
  QAction *openimgAct;
  QAction *closeimgAct;
  QAction *transpAct;

  QAction *settAct;
  QAction *multAct;
  
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
