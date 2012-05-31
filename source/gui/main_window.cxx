// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010), Toby D. Young
// (2012).
//
// Copyright (C) 2010,2012 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

                                 // standard C/C++
#include <cassert>

                                 // vecds gui includes
#include <vecds/gui/main_window.h>


Internal *ActualData;
extern bool qf_ok;

QString infotxt0, infotxtat, infotxtimg;
QString infsepar = " .......... ";


MainWindow::MainWindow ()
{
  Widg_widget0 = new QWidget;

  setCentralWidget (Widg_widget0);

                                 // Set this window title to the
                                 // VECDS_PACKAGE_NAME
  setWindowTitle (VECDS_PACKAGE_NAME);
  
  ActualData = new Internal;
  
  ActualData->act_mill = "[2-1-10](0001)";
  ActualData->act_disl = "1/3[2-1-10](0001)";
  ActualData->act_core = "none";
  
  aname   = "none";
  iname   = "none";

                                 // Create the main viewer.
  vecds_main_viewer = new vecds::MainViewer (this);

                                 // Size policies?
  vecds_main_viewer->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);

  qWarning ("   Setting up info labels...");
  infoLabel = new QLabel (this);
  infoLabel->setFrameStyle (QFrame::StyledPanel | 
			    QFrame::Sunken);

  infoLabel->setTextFormat (Qt::RichText);

  qWarning ("   Assigning actions...");
  createActions();
  qWarning ("   Creating menus...");
  createMenus ();
  qWarning ("   Creating docks...");
  createDockWindows ();
  qWarning ("   Creating status bar...");
  createStatusBar();

  qWarning ("   Dealing with the main layout...");
  LAY_g_MainLayout = new QGridLayout;

  LAY_g_MainLayout->setMargin (3);
  LAY_g_MainLayout->addWidget (infoLabel, 0, 0);
  LAY_g_MainLayout->addWidget (vecds_main_viewer, 1, 0);

  Widg_widget0->setLayout (LAY_g_MainLayout);
  Widg_modesTab->setCurrentIndex(0);

  qWarning ("   Calling display information...");
  InfoDisplay ();

  qWarning ("class MainWindow: Successfully initialized");
}

MainWindow::~MainWindow ()
{
  qWarning ("class MainWindow:~MainWindow. Destructor called");
}

                                // This function creates the
void MainWindow::createActions ()
{

  { 
                                // Open data file
    action_open_atoms = new QAction (tr ("Open Atoms"), this);
    action_open_atoms->setStatusTip (tr ("Open an existing file .xyz or .alc"));
    connect (action_open_atoms, SIGNAL (triggered ()), this, SLOT (SL_open_atoms ()));
    
                                 // Open image
    action_open_image = new QAction (tr ("Open HRTEM image"), this);
    action_open_image->setStatusTip(tr ("Open an existing img"));
    connect (action_open_image, SIGNAL (triggered ()), this, SLOT (SL_open_image ()));
    
                                 // Save data files
    action_save_as = new QAction (tr ("Save atoms"), this);
    action_save_as->setStatusTip (tr ("Save existing coordinates"));
    connect (action_save_as, SIGNAL (triggered ()), this, SLOT (SL_save_as_atoms ()));
    
                                 // Close image
    action_close_image = new QAction (tr ("Close image"), this);
    action_close_image->setStatusTip (tr ("Close an existing img"));
    connect (action_close_image, SIGNAL (triggered ()), this, SLOT (SL_close_image ()));
  }

  {
                                 // Define a crystal structure
    action_define_crystal_structure = new QAction (tr ("Define crystal structure"), this);
    action_define_crystal_structure->setCheckable (true);
    action_define_crystal_structure->setStatusTip (tr ("defines ..."));
    connect (action_define_crystal_structure, SIGNAL (triggered ()), this, SLOT (SL_defineStructure ()));

                                 // Choose a predefined a crystal
                                 // structure
    action_choose_crystal_structure = new QAction (tr("Choose crystal structure"), this);
    action_choose_crystal_structure->setCheckable (true);
    action_choose_crystal_structure->setStatusTip (tr ("struct ..."));
    connect (action_choose_crystal_structure, SIGNAL(triggered ()), this, SLOT (SL_chooseStructure ()));
  }

  {
                                 // Generate crystal structure by
                                 // defining its size by cells in the
                                 // xyz-directions.
    action_generate_structure_by_cell = new QAction (tr ("Generate (by cell)"), this);
    action_generate_structure_by_cell->setStatusTip (tr ("generate a new file"));
    connect (action_generate_structure_by_cell, SIGNAL (triggered ()), this, SLOT (SL_genAtoms ()));
    
                                 // Generate crystal structure by
                                 // defining its size by lengths in
                                 // the xyz-directions.
    action_generate_structure_by_length = new QAction (tr ("Generate (by length)"), this);
    action_generate_structure_by_length->setStatusTip (tr ("generate a new file"));
    connect (action_generate_structure_by_length, SIGNAL (triggered ()), this, SLOT (SL_gen1Atoms ()));
  }

  {
                                 // Generate a cuboidal box
    action_make_cuboid_box = new QAction (tr ("Cuboid box"), this);
    action_make_cuboid_box->setStatusTip (tr ("Cuboid bounding box"));
    connect (action_make_cuboid_box, SIGNAL (triggered ()), this, SLOT (SL_cubBox ()));
    
                                 // Generate a hexagonal box
    action_make_hexagonal_box = new QAction (tr ("Hexagonal box"), this);
    action_make_hexagonal_box->setStatusTip (tr ("Hexagonal bounding box"));
    connect (action_make_hexagonal_box, SIGNAL (triggered ()), this, SLOT (SL_hexBox ()));
  }

  {
                                 // Add a documentation box.
    action_show_documentation = new QAction (tr ("Documentation"), this);
    action_show_documentation->setStatusTip (tr ("Show vecds' documentation box"));
    connect (action_show_documentation, SIGNAL (triggered ()), this, SLOT (SL_documentation ()));
  
                                 // Add an about box.
    action_show_about = new QAction (tr ("About"), this);
    action_show_about->setStatusTip (tr ("Show vecds' about box"));
    connect (action_show_about, SIGNAL (triggered ()), this, SLOT (SL_about ()));
  }


  MillerAct = new QAction(tr("Miller"), this);
  MillerAct->setStatusTip(tr("input Miller indices"));
  connect(MillerAct, SIGNAL(triggered()), this, SLOT(SL_millerAct()));
  
  settAct = new QAction(tr("Visibility"), this);
  settAct->setStatusTip(tr("visibility"));
  connect(settAct, SIGNAL(triggered()), this, SLOT(SL_sett()));
  
  multAct = new QAction(tr("Mult. factor"), this);
  multAct->setStatusTip(tr("mult. factor"));
  connect(multAct, SIGNAL(triggered()), this, SLOT(SL_mult()));

                                 // Make signals and slots to enable
                                 // actions on the main viewer.
  connect (this, SIGNAL (SIG_prepareImg ()),  vecds_main_viewer, SLOT (SL_loadImage ()));
  connect (this, SIGNAL (SIG_needDraw ()),    vecds_main_viewer, SLOT (SL_needDraw ()));
  connect (this, SIGNAL (SIG_repaint ()),     vecds_main_viewer, SLOT (SL_repaint ()));
  connect (this, SIGNAL (SIG_keypress (int)), vecds_main_viewer, SLOT (SL_keypress (int)));
  
                                 // Actually connect the main viewer
  connect (vecds_main_viewer, SIGNAL (SIG_actPoint (QVector3D)),    this, SLOT (SL_actPoint (QVector3D)));
  connect (vecds_main_viewer, SIGNAL (SIG_actPosition (QVector3D)), this, SLOT (SL_actPosition (QVector3D)));
}

                                 // Function that opens atom files
void MainWindow::SL_open_atoms ()
{
  qWarning ("class MainWindow::SL_open_atoms");

                                 // start by opening up the default
                                 // vecds examples directory
  QString filename 
    = QFileDialog::getOpenFileName (this, "Select atoms", VECDS_DATABASE, "(*.xyz *.alc)");

  if ((filename==ActualData->atoms_loaded) || (filename.isEmpty ())) 
    {
     qWarning ("Unknown file name or file name is empty");
      return;
    }

  ActualData->atoms_loaded = filename;

  if ((!(filename.contains (".xyz"))) && (!(filename.contains (".alc"))))
     qWarning ("Unknown file format");

  else 
    {
      ActualData->atoms_loaded = filename;
      ActualData->read_alc_xyz (filename);
      emit SIG_needDraw ();
    }

  InfoDisplay ();
  vecds_main_viewer->updateGL ();
}

                                 // Function that opens image files
void MainWindow::SL_open_image ()
{
  qWarning ("class MainWindow::SL_open_image");

  QString filename 
    = QFileDialog::getOpenFileName (this, "Select image", VECDS_DATABASE, "(*.png *.xpm *.jpg *.tif)");

  if (filename.isEmpty ()) 
    {
      qWarning ("Image name is empty");
      return;
    }

  ActualData->img_loaded = filename;
  ActualData->read_img (filename);
  emit SIG_prepareImg ();

  InfoDisplay ();
  vecds_main_viewer->updateGL ();
}

                                 // Function that saves atom files
void MainWindow::SL_save_as_atoms()
{
  bool ok;
  QString filename  = QInputDialog::getText (this, "Save coordinates", "File name:", QLineEdit::Normal, "atoms.xyz", &ok);

  if ((filename.isEmpty ()) || (!ok)) 
    {
      qWarning ("File name is empty, or unknown error");
      return;
    }

                                 // Until we learn how to open windows
                                 // correctly, just dump it in this
                                 // place.... 
  QString directory = VECDS_DATABASE;
  ActualData->saveAtoms (directory.append ("/" + filename));
  return;
}

                                 // Function that closes an image file
void MainWindow::SL_close_image ()
{

                                 // reset cache
  ActualData->img_loaded = "none";
  InfoDisplay ();
}


void MainWindow::createMenus()
{
  menuBar ()->clear ();

                                 // This deals with "file" (data
                                 // operations) on the menu bar.
  menu_file     = menuBar ()->addMenu (tr ("File"));
  menu_file->addAction (action_open_atoms);
  menu_file->addAction (action_open_image);
  menu_file->addAction (action_save_as);
  menu_file->addAction (action_close_image);
  //  menu_file->addSeparator ();

                                 // This deals with "edit" (data
                                 // operations) on the menu bar.
  menu_edit               = menuBar ()->addMenu (tr ("Edit"));
  menu_crystal_structure  = menu_edit->addMenu ("Crystal structure");
  menu_crystal_structure->addAction (action_define_crystal_structure);
  menu_crystal_structure->addAction (action_choose_crystal_structure);

  menu_generate_structure = menu_edit->addMenu ("Generate structure");
  menu_generate_structure->addAction (action_generate_structure_by_cell);
  menu_generate_structure->addAction (action_generate_structure_by_length);

  menu_box = menu_edit->addMenu ("Box");
  menu_box->addAction (action_make_cuboid_box);
  menu_box->addAction (action_make_hexagonal_box);

                                 // This deals with "view" and
                                 // "settings" - these don't seem to
                                 // do much...
  menu_view     = menuBar ()->addMenu (tr ("View"));
  menu_settings = menuBar ()->addMenu (tr ("Settings"));
  menu_settings->addAction (settAct);
  menu_settings->addAction (multAct);

                                 // This part all deals with help that
                                 // is available for the user;
                                 // ie. documentation and About box.
  menu_help = menuBar ()->addMenu (tr ("Help"));
  menu_help->addAction (action_show_documentation);
  menu_help->addAction (action_show_about);

}

void MainWindow::createStatusBar()
{
  statusBar()->clearMessage();
}

void MainWindow::createDockWindows ()
{
  qWarning ("class MainWindow: creating dock windows");
  DWidg_dock = new QDockWidget(tr ("MODES"), this);
  DWidg_dock->setAllowedAreas(Qt::LeftDockWidgetArea | 
			      Qt::RightDockWidgetArea);

  Widg_modesTab = new QTabBar;
  Widg_modesTab->addTab (QIcon (":VECDS_internal/icons/new.png"),   "View");
  Widg_modesTab->addTab (QIcon (":VECDS_internal/icons/print.png"), "Rot");
  Widg_modesTab->addTab (QIcon (":VECDS_internal/icons/undo.png"),  "Add");

  
  DWidg_dock->setWidget(Widg_modesTab);
  addDockWidget(Qt::RightDockWidgetArea, DWidg_dock);
  menu_view->addAction(DWidg_dock->toggleViewAction());
  
  DWidg_dock = new QDockWidget(tr("sliders"), this);
  DWidg_dock->setAllowedAreas(Qt::LeftDockWidgetArea | 
			      Qt::RightDockWidgetArea);
  
  Lay_main0 = new QStackedWidget(DWidg_dock);
  DWidg_dock->setWidget(Lay_main0);
  
  QWidget *Widg_3slid = new QWidget(DWidg_dock);
  QGridLayout *Lay_g_3slid = new QGridLayout;
  
  QVBoxLayout *Lay_v_ThetaSlid = new QVBoxLayout;
  QLabel *Lab_ThetaSlid = new QLabel(" Heading (theta)");
  Lay_v_ThetaSlid->addWidget(Lab_ThetaSlid);
//  thetaSlider = createSlider(-180., 180., 45., 0.);
  thetaSlider = createSlider(-180, 180, 1, 0.);
  Lay_v_ThetaSlid->addWidget(thetaSlider);
  
  QVBoxLayout *Lay_v_PhiSlid = new QVBoxLayout; 
  QLabel *Lab_PhiSlid = new QLabel(" Attitude (phi)");
  Lay_v_PhiSlid->addWidget(Lab_PhiSlid);
//  phiSlider = createSlider(-90., 90., 15., 0.);
  phiSlider = createSlider(-90, 90, 1, 0);
  Lay_v_PhiSlid->addWidget(phiSlider);
  
  QVBoxLayout *Lay_v_PsiSlid = new QVBoxLayout;
  QLabel *Lab_PsiSlid = new QLabel(" Bank (psi)");
  Lay_v_PsiSlid->addWidget(Lab_PsiSlid);
//  psiSlider = createSlider(-180., 180., 45., 0.);
  psiSlider = createSlider(-180, 180, 1, 0);
  Lay_v_PsiSlid->addWidget(psiSlider);
  
  QVBoxLayout *Lay_v_mx = new QVBoxLayout;
  QLabel *Lab_mx = new QLabel(" Move x");
  Lay_v_mx->addWidget(Lab_mx);
  mxSlider = createSlider(-100, 100, 1, 0);
  Lay_v_mx->addWidget(mxSlider);
  
  QVBoxLayout *Lay_v_my = new QVBoxLayout;
  QLabel *Lab_my = new QLabel(" Move y");
  Lay_v_my->addWidget(Lab_my);
  mySlider = createSlider(-100, 100, 1, 0);
  Lay_v_my->addWidget(mySlider);
  
  QVBoxLayout *Lay_v_dist = new QVBoxLayout;
  QLabel *Lab_dist = new QLabel(" Distance");
  Lay_v_dist->addWidget(Lab_dist);
  distSlider = createSlider(1, 180, 1, 180);
/*
  QVBoxLayout *Lay_v_ThetaSlid = new QVBoxLayout;
  QLabel *Lab_ThetaSlid = new QLabel(" Heading (theta)");
  Lay_v_ThetaSlid->addWidget(Lab_ThetaSlid);
  thetaSlider = createSlider(-180., 180., 45., 0.);
//  thetaSlider = createSlider(-180, 180, 1, 0.);
  Lay_v_ThetaSlid->addWidget(thetaSlider);
  
  QVBoxLayout *Lay_v_PhiSlid = new QVBoxLayout; 
  QLabel *Lab_PhiSlid = new QLabel(" Attitude (phi)");
  Lay_v_PhiSlid->addWidget(Lab_PhiSlid);
  phiSlider = createSlider(-90., 90., 15., 0.);
//  phiSlider = createSlider(-90, 90, 1, 0);
  Lay_v_PhiSlid->addWidget(phiSlider);
  
  QVBoxLayout *Lay_v_PsiSlid = new QVBoxLayout;
  QLabel *Lab_PsiSlid = new QLabel(" Bank (psi)");
  Lay_v_PsiSlid->addWidget(Lab_PsiSlid);
  psiSlider = createSlider(-180., 180., 45., 0.);
//  psiSlider = createSlider(-180, 180, 1, 0);
  Lay_v_PsiSlid->addWidget(psiSlider);
  
  QVBoxLayout *Lay_v_mx = new QVBoxLayout;
  QLabel *Lab_mx = new QLabel(" Move x");
  Lay_v_mx->addWidget(Lab_mx);
  mxSlider = createSlider(-100, 100, 1, 0);
  Lay_v_mx->addWidget(mxSlider);
  
  QVBoxLayout *Lay_v_my = new QVBoxLayout;
  QLabel *Lab_my = new QLabel(" Move y");
  Lay_v_my->addWidget(Lab_my);
  mySlider = createSlider(-100, 100, 1, 0);
  Lay_v_my->addWidget(mySlider);
  
  QVBoxLayout *Lay_v_dist = new QVBoxLayout;
  QLabel *Lab_dist = new QLabel(" Distance");
  Lay_v_dist->addWidget(Lab_dist);
  distSlider = createSlider(1, 180, 1, 180);
*/

  Lay_v_dist->addWidget(distSlider);
  Lay_g_3slid->addLayout(Lay_v_ThetaSlid, 0, 0);
  Lay_g_3slid->addLayout(Lay_v_PhiSlid, 1, 0);
  Lay_g_3slid->addLayout(Lay_v_PsiSlid, 2, 0);
  Lay_g_3slid->addLayout(Lay_v_mx, 3, 0);
  Lay_g_3slid->addLayout(Lay_v_my, 4, 0);
  Lay_g_3slid->addLayout(Lay_v_dist, 5, 0);
  Widg_3slid->setLayout(Lay_g_3slid);
  Lay_main0->addWidget(Widg_3slid);
  
  QWidget *Widg_rotMill = new QWidget(DWidg_dock);
  QHBoxLayout *Lay_h_rotMill = new QHBoxLayout;
  Butt_rotMiller = new QPushButton(tr("Rotation parameters(Miller)"));
  Lay_h_rotMill->addWidget(Butt_rotMiller);
  Widg_rotMill->setLayout(Lay_h_rotMill);
  Lay_main0->addWidget(Widg_rotMill);
  
  QWidget *Widg_add = new QWidget(DWidg_dock);
  QVBoxLayout *Lay_v_add = new QVBoxLayout;
  
  Butt_disloc = new QPushButton(tr("Dislocation pointed by mouse"));
  Lay_v_add->addWidget(Butt_disloc);
  Butt_sf = new QPushButton(tr("Dislocation by number of atom"));
  Lay_v_add->addWidget(Butt_sf);
  Butt_qd = new QPushButton(tr("Add displacements"));
  Lay_v_add->addWidget(Butt_qd);
  Widg_add->setLayout(Lay_v_add);
  Lay_main0->addWidget(Widg_add);
  
  DWidg_dock->setWidget(Lay_main0);
  addDockWidget(Qt::RightDockWidgetArea, DWidg_dock);
  menu_view->addAction(DWidg_dock->toggleViewAction());
  
  connect(Butt_rotMiller, SIGNAL(clicked()), this, SLOT(SL_millerAct()));
  
  connect(Butt_disloc, SIGNAL(clicked()), this, SLOT(SL_dislocAct()));
  
  connect(Butt_sf, SIGNAL(clicked()), this, SLOT(SL_dislAct()));
  
  connect(Butt_qd, SIGNAL(clicked()), this, SLOT(SL_addCoordAct()));
  
  connect(Widg_modesTab, SIGNAL(currentChanged(int)),
                               this, SLOT(SL_changeMode(int)));
 
  connect(Widg_modesTab, SIGNAL(currentChanged(int)),
                             Lay_main0, SLOT(setCurrentIndex(int)));

  connect(phiSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(phiSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_dophiRotation()));
  connect(vecds_main_viewer, SIGNAL(SIG_phiRotationChanged(int)), phiSlider, SLOT(setValue(int)));

  connect(thetaSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(thetaSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_dothetaRotation()));
  connect(vecds_main_viewer, SIGNAL(SIG_thetaRotationChanged(int)), thetaSlider, SLOT(setValue(int)));

  connect(psiSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(psiSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_dopsiRotation()));
  connect(vecds_main_viewer, SIGNAL(SIG_psiRotationChanged(int)), psiSlider, SLOT(setValue(int)));

  connect(mxSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(mxSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_doXMovement()));
  connect(vecds_main_viewer, SIGNAL(SIG_xMovementChanged(int)), mxSlider, SLOT(setValue(int)));

  connect(mySlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(mySlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_doYMovement()));
  connect(vecds_main_viewer, SIGNAL(SIG_yMovementChanged(int)), mySlider, SLOT(setValue(int)));

  connect(distSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(distSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_doZMovement()));
  connect(vecds_main_viewer, SIGNAL(SIG_zMovementChanged(int)), distSlider, SLOT(setValue(int)));
/*
  connect(phiSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(phiSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_dophiRotation()));
  connect(vecds_main_viewer, SIGNAL(SIG_phiRotationChanged(double)), phiSlider, SLOT(setValue(double)));

  connect(thetaSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(thetaSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_dothetaRotation()));
  connect(vecds_main_viewer, SIGNAL(SIG_thetaRotationChanged(double)), thetaSlider, SLOT(setValue(double)));

  connect(psiSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(psiSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_dopsiRotation()));
  connect(vecds_main_viewer, SIGNAL(SIG_psiRotationChanged(double)), psiSlider, SLOT(setValue(double)));

  connect(mxSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(mxSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_doXMovement()));
  connect(vecds_main_viewer, SIGNAL(SIG_xMovementChanged(double)), mxSlider, SLOT(setValue(double)));

  connect(mySlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(mySlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_doYMovement()));
  connect(vecds_main_viewer, SIGNAL(SIG_yMovementChanged(double)), mySlider, SLOT(setValue(double)));

  connect(distSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(distSlider, SIGNAL(sliderReleased()), vecds_main_viewer, SLOT(SL_doZMovement()));
  connect(vecds_main_viewer, SIGNAL(SIG_zMovementChanged(double)), distSlider, SLOT(setValue(double)));

*/

}

// ====================    S L O T S    ===========================


void MainWindow::SL_actPoint(QVector3D res)
{
  QString str1;
// qWarning("SL_actPoint");
  ActualData->actPoint = res;
  int ix = vecds_main_viewer->lastPos.x();
  int iy = vecds_main_viewer->lastPos.y();
  str1.sprintf("act point coordinates: ix=%d, iy=%d, x=%g, y=%g, z=%g",
                                               ix, iy, res.x(), res.y(), res.z());
  statusBar()->showMessage(str1);
  if ( ActualData->Mode==2 ){
      ActualData->SL_singleDisl(res);
      emit SIG_repaint();
 qWarning("SL_actPoint  --  ndisl=%d", ActualData->ndisl);
  }
}

void MainWindow::SL_actPosition(QVector3D res)
{
 qWarning("SL_actPosition");
  QString str1;
  ActualData->actPoint = res;
  int i0 = ActualData->atomize(res, 0);
  int ix = vecds_main_viewer->lastPos.x();
  int iy = vecds_main_viewer->lastPos.y();
  
  str1.sprintf("position: ix=%d, iy=%d, x=%g, y=%g, z=%g, i0=%d",
                                       ix, iy, res.x(), res.y(), res.z(), i0);
 qWarning("mainwindow:position: ix=%d, iy=%d, x=%g, y=%g, z=%g, i0=%d", ix, iy, res.x(), res.y(), res.z(), i0);
  statusBar()->showMessage(str1);
//  emit SIG_repaint();
}


void MainWindow::SL_chooseStructure()
{

  QStringList items;
  for (int i=0; i<ActualData->numbcrstr; i++) 
               items << ActualData->crstr[i].structure_name;
  bool ok;
  QString item = QInputDialog::getItem(this, "Choose structure", 
                     "structure:", items, ActualData->numbcrstr, false, &ok);
  if ( ok && !items.isEmpty() ) 
     for (int i=0; i<ActualData->numbcrstr; i++) 
        if ( item==ActualData->crstr[i].structure_name ) {
           ActualData->actcrstr = &(ActualData->crstr[i]);
           infotxt0.sprintf("Structure: %s%sMiller indices: %s", 
			    ActualData->actcrstr->structure_name.toAscii().data(),
			    infsepar.toAscii().data(), 
			    toRichText(ActualData->act_mill).toAscii().data());
	   
           InfoDisplay();
           return;
        }
//  structureDefined = false;
  return;
}

void MainWindow::SL_defineStructure()
{
  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "Atomic structure definition";
  QString description = "<h5>a, b, c - in &Aring;<br>alpha, beta, gamma - in degrees</h5>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "<h6>name=</h6>" << "a=" << "b=" << "c=" << "alpha=" << "beta=" << "gamma=" << "numb. of atoms";
  suggestions << "B4_GaN" << "3.180" << "3.180" << "5.166" << "90.0" << "90.0" << "120.0"<< "4";

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;
}


//--------------------------------------------------------------
// ------------------- G E N E R A T I O N ---------------------
// -------------------------------------------------------------

void MainWindow::SL_genAtoms()
{
  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "nx, ny, nz";
  QString description = "<h5>How many repetitions in<br>x, y, z direction</h5>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "Number X" <<"Number Y" << "Number Z";
  suggestions << "6" << "5" << "3";

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;

  int nx   = answers.at (0).toInt ();
  int ny   = answers.at (1).toInt ();
  int nz   = answers.at (2).toInt ();
  int nmax = nx*ny*nz * ActualData->actcrstr->n_materials;

  ActualData->atoms->coordinates = new QVector3D[nmax];
  ActualData->atoms->u           = new QVector3D[nmax];
  ActualData->atoms->atom_type   = new unsigned int[nmax];

  ActualData->atoms->n_atoms     = ActualData->lattice(nx, ny, nz);
  qWarning("+++++++++ n_atoms=%d", ActualData->atoms->n_atoms);

  ActualData->atoms->n_bonds = 0;
  ActualData->minmax3(ActualData->atoms->coordinates, ActualData->atoms->n_atoms, ActualData->a_min_, ActualData->a_max_);
  ActualData->atoms_loaded = QString("neW_atoms.xyz");

  InfoDisplay();
  emit SIG_needDraw();
}


void MainWindow::SL_gen1Atoms()
{
  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "size x, size y, size z";
  QString description = "<h5>Which sizes (in &Aring;) in<br>x, y direction</h5>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "Size X" <<"Size Y" << "Number Z";
  suggestions << "50" << "50" << "3";

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;

  double x_size = answers.at(0).toDouble();
  double y_size = answers.at(1).toDouble();
  int nz = answers.at(2).toInt();

  double sg = sin (vecds::constant::deg2rad*ActualData->actcrstr->gamma);
  int nmax  = ActualData->actcrstr->n_materials * nz * 
    (int(x_size/ActualData->actcrstr->a)+1) *
    (int(y_size/(ActualData->actcrstr->b*sg))+1);

  ActualData->atoms->coordinates = new QVector3D[nmax];
  ActualData->atoms->u           = new QVector3D[nmax];
  ActualData->atoms->atom_type   = new unsigned int[nmax];

  ActualData->atoms->n_atoms = ActualData->lattice2(x_size, y_size, nz);
  ActualData->atoms->n_bonds = 0;
  ActualData->minmax3(ActualData->atoms->coordinates, ActualData->atoms->n_atoms, ActualData->a_min_, ActualData->a_max_);
  ActualData->atoms_loaded = QString("neW_atoms.xyz");
//  ActualData->INT_nn = ActualData->atoms->n_atoms;
  InfoDisplay();
  emit SIG_needDraw();

}

//----------------------------------------------------------
//----------------------------------------------------------


void MainWindow::SL_setSliderValue (int val)
//void MainWindow::SL_setSliderValue (double val)
{
  ActualData->sliderValue = val;
  ActualData->sliderMove  = true;
}

                                 // Display the "about page" for vecds.
void MainWindow::SL_about ()
{
                                 // Create a help browser.
  vecds::HelpBrowser *about = new vecds::HelpBrowser (); 

                                 // The first help page is always
                                 // "about.html".
  (*about).show_page ("about.html");
}

                                 // Display the "documentation page"
                                 // for vecds.
void MainWindow::SL_documentation ()
{
                                 // Create a documentation browser.
  vecds::DocBrowser *documentation = new vecds::DocBrowser (); 

                                 // The first documentation page is
                                 // always "documentation.html".
  (*documentation).show_page ("documentation.html");
}




void MainWindow::SL_millerAct()
{
 vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "Size of structure";
  QString description = "<h4>Miller indices;<br>e.g. 1/3[10-10](11-20)<br>&nbsp;or&nbsp;[100](001)</h4>";
  QStringList questions;
  // QStringList suggestions;
  QStringList answers;

  questions << "Miller indices";

                                 // This was written without a suggestion...
  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  // (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;

  ActualData->act_mill = answers.at (0);
  ActualData->processMiller(1, ActualData->act_mill);
  InfoDisplay();

  emit SIG_repaint();
}

void MainWindow::SL_dislocAct()
{
  QString s1, s2;
  ActualData->act_core.sprintf("4");

  s1.sprintf ("%s", ActualData->act_disl.toAscii().data());
  s2.sprintf ("%s", ActualData->act_core.toAscii().data());

  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "Dislocation parameters";
  QString description = "<h4>Dislocation parameters</h4>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "Size X" <<"Size Y" << "Number Z";
  suggestions << s1 << s2;

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;

  s1 = answers.at (0);
  s2 = answers.at (1);

  ActualData->processMiller (2, s1, s2);
  ActualData->act_disl = s1;
  ActualData->act_core = s2;
  InfoDisplay ();
  vecds_main_viewer->updateGL ();
}

void MainWindow::SL_dislAct ()
{
  ActualData->act_core.sprintf("none");

  QString s1;
  s1.sprintf ("%s", ActualData->act_disl.toAscii().data());

  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "Dislocation parameters";
  QString description = "<h4>Dislocation parameters</h4>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "Miller indices" << "number of atom";
  suggestions << s1 << QString (" ");

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;

  s1 = answers.at (0);
  int n_a = answers.at (1).toInt ();
  ActualData->processMiller(1, s1, QString("none"));
  qWarning("SL_dislAct -- 1");
  
  ActualData->newdisl(n_a, true);
  qWarning("SL_dislAct -- 2");
  ActualData->act_disl = s1;
  qWarning("SL_dislAct -- 3");
  InfoDisplay();
  vecds_main_viewer->updateGL();
}

void MainWindow::SL_addCoordAct()
{
  ActualData->addDisplacements();
  emit SIG_repaint();
  vecds_main_viewer->updateGL();
//  qWarning("SL_addCoordAct");
}


void MainWindow::SL_changeMode(int mode)
{
  if ( mode!=ActualData->Mode ) 
    {
      qWarning("+++++           MODE = %d", mode);
      ActualData->Mode = mode;
    }

  if (ActualData->Mode==2) 
    {
      vecds_main_viewer->setCursor(Qt::CrossCursor);
    }
  else  
    {
      vecds_main_viewer->setCursor(Qt::ArrowCursor);
    }
}

                                 // TODO: If we knew what this did, we
                                 // could hack the Question class much
                                 // better!
void MainWindow::SL_sett ()
{
  vecds::Question *this_question = new vecds::Question (); 

  QString title = "Visibility";

  QStringList questions;
  questions << "face_0" << "face_f" 
   	    << "nodes_0" << "nodes_f" 
   	    << "int.lines" << "ext_lines" 
   	    << "spectrograms" << "arrows"  << "axis";
  
  (*this_question).set_title (title);
  (*this_question).set_question_list (questions);
  (*this_question).show_question (ActualData->visible);

  emit SIG_repaint();
}

                                 // TODO: Same goes for this crap
void MainWindow::SL_mult()
{
  bool ok;
  double d = QInputDialog::getDouble(this, tr("Mult. factor"),
                                       tr("factor:"), 10.00, -10000, 10000, 2, &ok);
  if ( ok ) ActualData->mfactor = d;
  emit SIG_repaint();
}

//---------------------------------------------------------------------

void MainWindow::SL_cubBox()
{
  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "Size of structure";
  QString description = "<h4>Write sizes of structure;<br> ( in &Aring; )</h4>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "Size X" << "Size Y" << "Size Z";
  suggestions << "20" << "20" << "20";

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;

  QVector3D box;
  box.setX (0.5 * answers.at (0).toDouble ());
  box.setY (0.5 * answers.at (1).toDouble ());
  box.setZ (0.5 * answers.at (2).toDouble ());
}

void MainWindow::SL_hexBox()
{
  vecds::QuestionForm *this_question_form = new vecds::QuestionForm (); 

  QString title       = "Size of structure";
  QString description = "<h4>Write sizes of structure;<br> ( in &Aring; )</h4>";
  QStringList questions;
  QStringList suggestions;
  QStringList answers;

  questions << "Hex radius" << "Size Z";
  suggestions << "30" << "25";

  (*this_question_form).set_title (title);
  (*this_question_form).set_question_list (questions);
  (*this_question_form).set_suggestion_list (suggestions);
  (*this_question_form).set_description (description);
  (*this_question_form).show_question (answers);

  if (!(*this_question_form).is_sane ())
    return;
}

void MainWindow::InfoDisplay()
{
  QString inftxt;
  infoLabel->clear();
  infotxt0.sprintf ("Structure: %s%sMiller indices: %s", 
		    ActualData->actcrstr->structure_name.toAscii ().data (),
		    infsepar.toAscii ().data (), toRichText (ActualData->act_mill ).toAscii ().data ());

  infotxtat.sprintf ("<br/>Atoms loaded: %s", 
		     ActualData->atoms_loaded.toAscii ().data ());
  infotxtimg.sprintf ("<br/>Image loaded: %s", 
		      ActualData->img_loaded.toAscii ().data ());
  
  inftxt.sprintf("%s%s%s", infotxt0.toAscii().data(), 
                           infotxtat.toAscii().data(), 
                           infotxtimg.toAscii().data());
// qWarning("INFO: %s", inftxt.toAscii().data());
   infoLabel->setText(inftxt);
}

                                 // Register a key event
void MainWindow::keyPressEvent(QKeyEvent *keyEv)
{
  const int k = keyEv->key ();
  emit SIG_keypress (k);

#ifdef DEBUG
  qWarning("class MainWindow: Key pressed was \"%d\"", k);

  if (vecds_main_viewer->isActiveWindow ()) 
    qWarning ("class MainWindow: vecds_main_viewer is now the active window");

  else                      
    qWarning ("class MainWindow: vecds_main_viewer is not the active window");
#endif
}

QSlider *MainWindow::createSlider (const unsigned int minimum, 
				   const unsigned int maximum, 
				   const unsigned int step, 
				   const unsigned int value)
{
  QSlider* slider = new QSlider (Qt::Horizontal);

  slider->setTickInterval (step);
  slider->setMinimum (minimum);
  slider->setMaximum (maximum); // paging disabled
  slider->setValue(value);

  return slider;
}

QString MainWindow::toRichText (QString txt)
{
  QString txt1;
  txt1 = txt.replace("<", "&lt;").replace(">", "&gt;");
  return txt1;
}
/*
QwtSlider *MainWindow::createSlider
                      (double from, double to, double step, double val, bool logaritmic)
{
  QwtSlider* slider = new QwtSlider(this, 
                   Qt::Horizontal, QwtSlider::TopScale, QwtSlider::Trough);
  if ( logaritmic ) slider->setScaleEngine(new QwtLog10ScaleEngine);
  slider->setBorderWidth(10);
  slider->setRange(from, to); // paging disabled
  slider->setScale(from, to, step);
//  slider->setScaleMaxMinor(10);
  slider->setValue(val);
  return slider;
}
*/
