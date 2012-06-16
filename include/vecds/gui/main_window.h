
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

                                 /* vecds base includes */
#include <vecds/base/config.h>
#include <vecds/base/constant.h>

                                 /* vecds dialogs includes */
#include <vecds/dialogs/help_browser.h>
#include <vecds/dialogs/doc_browser.h>
#include <vecds/dialogs/question.h>
#include <vecds/dialogs/question_form.h>


                                 /* vecds gui includes */
#include <vecds/gui/main_viewer.h>
#include <vecds/gui/main_viewer.h>

#include <vecds/internal.h>


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

class MainWindow 
: 
public QMainWindow
{
  Q_OBJECT
    
    public:
  MainWindow ();

  ~MainWindow ();


  
  private slots:

                                 /* These are slots that deal with
				  * menu objects (actions).
				  *
				  * open atoms data file, images, save
				  * atoms data file, close image... */
  void SL_open_atoms ();
  void SL_open_image ();
  void SL_save_as_atoms ();
  void SL_close_image ();

				 /* generate atoms by cell or by length */
  void SL_generate_atoms_by_cell ();
  void SL_generate_atoms_by_length ();

				 /* make bounding boxes; cubic and
				    hexagonal */
  void SL_make_cubic_bbox ();
  void SL_make_hexagonal_bbox ();

                                 /* These are help pages that open in
				  * a new window.
				  *
				  * First, the "about vecds" page */
  void SL_about ();

                                 /* and second, the "documentation of
				    vecds" page */
  void SL_documentation ();


  void SL_setSliderValue (int);
  void SL_defineStructure ();
  void SL_chooseStructure ();

  // --------------------- WTF? ---------------------------------  

  void SL_millerAct ();
  void SL_dislocAct ();
  void SL_dislAct ();
  void SL_addCoordAct ();
  void SL_changeMode (int mode);
  void SL_actPoint (QVector3D);
  void SL_actPosition (QVector3D);
  void SL_sett ();
  void SL_mult ();
  
 signals:

  void SIG_prepareImg ();
  void SIG_needDraw ();
  void SIG_repaint ();
  void SIG_keypress (int);
  void SIG_actPoint (QVector3D);

 private:

                                 /* This is the main viewer for
				    vecds. Probably, we only ever want
				    one of these, so lets give it a
				    simple name to rememeber. */
  vecds::MainViewer *vecds_main_viewer;

                                 /* This is a list of the menus and
				    submenus we have in this
				    project. Most of them are probably
				    self-explanitory. */
  QMenu *menu_file;
  QMenu *menu_edit;
  QMenu *menu_crystal_structure;
  QMenu *menu_generate_structure;
  QMenu *menu_view;
  QMenu *menu_settings;
  QMenu *menu_box;
  QMenu *menu_help;

                                 /* Actions that do things. These
				    belong in the "help" menu because
				    they just show predefined
				    documentation. */
  QAction *action_show_documentation;
  QAction *action_show_about;

                                 /* Actions that belong in the "file"
				    menu because they operate on
				    files. */
  QAction *action_open_atoms;
  QAction *action_open_image;
  QAction *action_save_as;
  QAction *action_close_image;

                                 /* Actions that belong in the "edit"
				    menu because they edit the way the
				    sytem is displayed. */
  QAction *action_define_crystal_structure;
  QAction *action_choose_crystal_structure;

  QAction *action_generate_structure_by_cell;
  QAction *action_generate_structure_by_length;

                                 /* Note: These are shapes defined by
				    crystallographic groups (they are
				    not geometrical shapes) */
  QAction *action_make_cuboid_box;
  QAction *action_make_hexagonal_box;


                                 /* Register if a key is pressed */
  void keyPressEvent (const QKeyEvent *key_event);

                                 /* Actually create stuff */
  void createActions ();
  void createMenus ();
  void createStatusBar ();
  void createDockWindows ();
  
                                 /* Create a slider */
  QSlider* createSlider (const unsigned int minimum, 
			 const unsigned int maximum, 
			 const unsigned int step, 
			 const unsigned int value);

  // --------------------- What are these? ---------------------------------

  void InfoDisplay ();

  QString toRichText (QString txt);
  
  QGridLayout *LAY_g_MainLayout;
  QWidget     *Widg_widget0;
  QDockWidget *DWidg_dock;
  QTabBar     *Widg_modesTab;
  
  QString aname, iname, fname, resname;
  QString infotxt0, infotxtat, infotxtimg;
  
  QAction *MillerAct;
  QAction *transpAct;
  QAction *settAct;
  QAction *multAct;
  
  QLabel *infoLabel;

                                 /* Slider controls over the rotation
				    angles phi, theta, and psi */
  QSlider *slider_rangle_phi;
  QSlider *slider_rangle_theta;
  QSlider *slider_rangle_psi;

                                 /* Slider controls over
				    something... */
  QSlider *slider_mx;
  QSlider *slider_my;

                                 /* Slider controls over zooming in
				    and out of the computer screen */
  QSlider *slider_zoom;

  QPushButton *Butt_rotMiller;
  QPushButton *Butt_disloc;
  QPushButton *Butt_sf;
  QPushButton *Butt_qd;
  
  QStackedWidget *Lay_main0;
  
};
#endif
