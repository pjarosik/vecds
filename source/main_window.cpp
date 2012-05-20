// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
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

//#include <QtGui>
#include "../include/vecds/main_viewer.h"
#include "../include/vecds/internal.h"
#include "../include/vecds/main_window.h"
#include "../include/vecds/help_browser.h"
#include "../include/vecds/dialogs.h"
#include "../include/vecds/constant.h"


Internal *ActualData;
extern bool qf_ok;

QString infotxt0, infotxtat, infotxtimg;
QString infsepar = " .......... ";


MainWindow::MainWindow ()
{
  Widg_widget0 = new QWidget;
  setCentralWidget(Widg_widget0);
  setWindowTitle("Visual Editor of Crystal Defects");
  
  ActualData = new Internal;
  
  ActualData->act_mill = "[2-1-10](0001)";
  ActualData->act_disl = "1/3[2-1-10](0001)";
  ActualData->act_core = "none";
  
  aname   = "none";
  iname   = "none";
//  fname   = "none";
//  resname = "none";
//    ActualData->kindOfDisl = 0;
  
  mview1 = new MainViewer(this);
  mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  // qWarning("mview -- 1");
  infoLabel = new QLabel(this);
  infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  infoLabel->setTextFormat(Qt::RichText);
  // qWarning("mview -- 2");
  createActions();
  // qWarning("mview -- 3");
  createMenus();
  //    createToolBars();
  createDockWindows();
  // qWarning("mview -- 4");
  createStatusBar();
  //---------------------------
  LAY_g_MainLayout = new QGridLayout;
  LAY_g_MainLayout->setMargin(3);
  LAY_g_MainLayout->addWidget(infoLabel, 0, 0);
  LAY_g_MainLayout->addWidget(mview1, 1, 0);
  Widg_widget0->setLayout(LAY_g_MainLayout);
  Widg_modesTab->setCurrentIndex(0);
  // qWarning("mview -- 5");
  InfoDisplay();
}

void MainWindow::createActions()
{
  defnewAct = new QAction(tr("Define new structure"), this);
  defnewAct->setCheckable(true);
  //    defnewAct->setShortcut(tr("Ctrl+D"));
  defnewAct->setStatusTip(tr("defines ..."));
  connect(defnewAct, SIGNAL(triggered()), this, SLOT(SL_defineStructure()));
  
  defnewAct = new QAction(tr("Define new structure"), this);
  defnewAct->setCheckable(true);
  //    defnewAct->setShortcut(tr("Ctrl+D"));
  defnewAct->setStatusTip(tr("defines ..."));
  connect(defnewAct, SIGNAL(triggered()), this, SLOT(SL_defineStructure()));
  
  //  defCoresAct = new QAction(tr("Define dislocation cores"), this);
  //    openAct->setShortcut(tr("Ctrl+O"));
  //  defCoresAct->setStatusTip(tr("Define dislocation cores"));
  //  connect(defCoresAct, SIGNAL(triggered()), this, SLOT(SL_defCores()));
  
  //  refreshAct = new QAction(tr("Field of view"), this);
  //  refreshAct->setStatusTip(tr("Camera angle"));
  //  connect(refreshAct, SIGNAL(triggered()), this, SLOT(SL_refresh()));
  
  chooseAct = new QAction(tr("Choose structure"), this);
  chooseAct->setCheckable(true);
  //    chooseAct->setShortcut(tr("Ctrl+S"));
  chooseAct->setStatusTip(tr("struct ..."));
  connect(chooseAct, SIGNAL(triggered()), this, SLOT(SL_chooseStructure()));
  
  openAct = new QAction(tr("Open Atoms"), this);
  //    openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file .xyz or .alc"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(SL_openAtoms()));
  
  genAct = new QAction(tr("Generate (number of cells)"), this);
  //  genAct = QWhatsThis::createAction(this);
  genAct->setText(tr("Generate (number of cells)"));
  //    genAct->setShortcut(tr("Ctrl+N"));
  genAct->setStatusTip(tr("generate a new file"));
  //  genAct->setWhatsThis(tr("use this button\n aaaaaaaaaaaaaaaa\n to generate a <h4>new</h4> file\n by giving number of repetitions"));
  //  QWhatsThis::hideText();
  connect(genAct, SIGNAL(triggered()), this, SLOT(SL_genAtoms()));
  
  QString str = QString("Generate (size of the structure)");
  gen1Act = new QAction(str, this);
  gen1Act->setStatusTip(tr("generate a new file"));
  connect(gen1Act, SIGNAL(triggered()), this, SLOT(SL_gen1Atoms()));
  
  openimgAct = new QAction(tr("Open HRTEM image"), this);
  //    openimgAct->setShortcut(tr("Ctrl+I"));
  openimgAct->setStatusTip(tr("Open an existing img"));
  connect(openimgAct, SIGNAL(triggered()), this, SLOT(SL_openImg()));
  
  closeimgAct = new QAction(tr("Close image"), this);
  //    closeimgAct->setShortcut(tr("Ctrl+X"));
  closeimgAct->setStatusTip(tr("Close an existing img"));
  connect(closeimgAct, SIGNAL(triggered()), this, SLOT(SL_closeImg()));
  
  saveAsAct = new QAction(tr("Save atoms"), this);
  //    saveAsAct->setShortcut(tr("Ctrl+W"));
  saveAsAct->setStatusTip(tr("Save existing coordinates"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(SL_saveAtomsAs()));
  
  cubBoxAct = new QAction(tr("Cuboid box"), this);
  cubBoxAct->setStatusTip(tr("Cuboid bounding box"));
  connect(cubBoxAct, SIGNAL(triggered()), this, SLOT(SL_cubBox()));
  
  hexBoxAct = new QAction(tr("Hexagonal box"), this);
  hexBoxAct->setStatusTip(tr("Hexagonal bounding box"));
  connect(hexBoxAct, SIGNAL(triggered()), this, SLOT(SL_hexBox()));
  
  MillerAct = new QAction(tr("Miller"), this);
  //    MillerAct->setShortcut(tr("Ctrl+M"));
  MillerAct->setStatusTip(tr("input Miller indices"));
  connect(MillerAct, SIGNAL(triggered()), this, SLOT(SL_millerAct()));
  
  settAct = new QAction(tr("Visibility"), this);
  settAct->setStatusTip(tr("visibility"));
  connect(settAct, SIGNAL(triggered()), this, SLOT(SL_sett()));
  
  multAct = new QAction(tr("Mult. factor"), this);
  multAct->setStatusTip(tr("mult. factor"));
  connect(multAct, SIGNAL(triggered()), this, SLOT(SL_mult()));
  
  aboutAct = new QAction(tr("About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(SL_about()));
  
  aboutQtAct = new QAction(tr("About Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
  
  //------------------------------------------------------------------
  connect(this, SIGNAL(SIG_prepareImg()), mview1, SLOT(SL_loadImage()));
  
  connect(this, SIGNAL(SIG_needDraw()), mview1, SLOT(SL_needDraw()));
  
  connect(this, SIGNAL(SIG_repaint()), mview1, SLOT(SL_repaint()));
  
  connect(this, SIGNAL(SIG_keypress(int)), mview1, SLOT(SL_keypress(int)));
  
  connect(mview1, SIGNAL(SIG_actPoint(QVector3D)), this, SLOT(SL_actPoint(QVector3D)));
  
  connect(mview1, SIGNAL(SIG_actPosition(QVector3D)), this, SLOT(SL_actPosition(QVector3D)));
}


void MainWindow::createMenus()
{
  menuBar()->clear();
  // qWarning("mainw - createMenus -- 0");
  fileMenu = menuBar()->addMenu(tr("File"));
  
  defstructMenu = fileMenu->addMenu("Structure definition");
  defstructMenu->addAction(chooseAct);
  defstructMenu->addSeparator();
  defstructMenu->addAction(defnewAct);
  fileMenu->addSeparator();
  
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  
  genMenu = fileMenu->addMenu("Generation");
  genMenu->addAction(genAct);
  genMenu->addSeparator();
  genMenu->addAction(gen1Act);
  fileMenu->addSeparator();
  fileMenu->addAction(openimgAct);
  fileMenu->addSeparator();
  fileMenu->addAction(closeimgAct);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  boxMenu = fileMenu->addMenu("Box");
  boxMenu->addAction(cubBoxAct);
  boxMenu->addSeparator();
  boxMenu->addAction(hexBoxAct);
  
  viewMenu = menuBar()->addMenu(tr("View"));
  
  settMenu = menuBar()->addMenu(tr("Settings"));
  settMenu->addAction(settAct);
  settMenu->addSeparator();
  settMenu->addAction(multAct);
  
  helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutQtAct);
  qWarning("mainw - createMenus O.K.");
}

void MainWindow::createStatusBar()
{
  //    statusBar()->showMessage("Ready");
  statusBar()->clearMessage();
}

void MainWindow::createDockWindows()
{
  qWarning("mainw - createDockWindows");
  DWidg_dock = new QDockWidget(tr("MODES"), this);
  DWidg_dock->setAllowedAreas(Qt::LeftDockWidgetArea | 
			      Qt::RightDockWidgetArea);

  //  QWhatsThis::enterWhatsThisMode();
  Widg_modesTab = new QTabBar;
  Widg_modesTab->addTab(QIcon(":VECDS_internal/icons/new.png"), "View");
  //  Widg_modesTab->addTab(QIcon(":VECDS_internal/icons/save.png"), "");
  Widg_modesTab->addTab(QIcon(":VECDS_internal/icons/print.png"), "Rot");
  Widg_modesTab->addTab(QIcon(":VECDS_internal/icons/undo.png"), "Add");
  //  Widg_modesTab->addTab(QIcon(":VECDS_internal/icons/camera.png"), "Bonds");
  //  Widg_modesTab->setTabWhatsThis(0, "Movement - continous");
  
  DWidg_dock->setWidget(Widg_modesTab);
  addDockWidget(Qt::RightDockWidgetArea, DWidg_dock);
  viewMenu->addAction(DWidg_dock->toggleViewAction());
  
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
  Lay_v_dist->addWidget(distSlider);

/*
  QVBoxLayout *Lay_v_ThetaSlid = new QVBoxLayout;
  QLabel *Lab_ThetaSlid = new QLabel(" Heading (theta)");
  Lay_v_ThetaSlid->addWidget(Lab_ThetaSlid);
  thetaSlider = createSlider(-180., 180., 45., 0.);
  Lay_v_ThetaSlid->addWidget(thetaSlider);
  
  QVBoxLayout *Lay_v_PhiSlid = new QVBoxLayout; 
  QLabel *Lab_PhiSlid = new QLabel(" Attitude (phi)");
  Lay_v_PhiSlid->addWidget(Lab_PhiSlid);
  phiSlider = createSlider(-90., 90., 15., 0.);
  Lay_v_PhiSlid->addWidget(phiSlider);
  
  QVBoxLayout *Lay_v_PsiSlid = new QVBoxLayout;
  QLabel *Lab_PsiSlid = new QLabel(" Bank (psi)");
  Lay_v_PsiSlid->addWidget(Lab_PsiSlid);
  psiSlider = createSlider(-180., 180., 45., 0.);
  Lay_v_PsiSlid->addWidget(psiSlider);
  
  QVBoxLayout *Lay_v_mx = new QVBoxLayout;
  QLabel *Lab_mx = new QLabel(" Move x");
  Lay_v_mx->addWidget(Lab_mx);
  mxSlider = createSlider(-1., 1., 0.25, 0.);
  Lay_v_mx->addWidget(mxSlider);

  QVBoxLayout *Lay_v_my = new QVBoxLayout;
  QLabel *Lab_my = new QLabel(" Move y");
  Lay_v_my->addWidget(Lab_my);
  mySlider = createSlider(-1., 1., 0.25, 0.);
  Lay_v_my->addWidget(mySlider);

  QVBoxLayout *Lay_v_dist = new QVBoxLayout;
  QLabel *Lab_dist = new QLabel(" Distance");
  Lay_v_dist->addWidget(Lab_dist);
  distSlider = createSlider(0.01, 1.0, 0.5, 1.0, true);
  Lay_v_dist->addWidget(distSlider);
*/
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
  viewMenu->addAction(DWidg_dock->toggleViewAction());
  
  connect(Butt_rotMiller, SIGNAL(clicked()), this, SLOT(SL_millerAct()));
  
  connect(Butt_disloc, SIGNAL(clicked()), this, SLOT(SL_dislocAct()));
  
  connect(Butt_sf, SIGNAL(clicked()), this, SLOT(SL_dislAct()));
  
  connect(Butt_qd, SIGNAL(clicked()), this, SLOT(SL_addCoordAct()));
  
  connect(Widg_modesTab, SIGNAL(currentChanged(int)),
                               this, SLOT(SL_changeMode(int)));
 
  connect(Widg_modesTab, SIGNAL(currentChanged(int)),
                             Lay_main0, SLOT(setCurrentIndex(int)));

  connect(phiSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(phiSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_dophiRotation()));
  connect(mview1, SIGNAL(SIG_phiRotationChanged(int)), phiSlider, SLOT(setValue(int)));

  connect(thetaSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(thetaSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_dothetaRotation()));
  connect(mview1, SIGNAL(SIG_thetaRotationChanged(int)), thetaSlider, SLOT(setValue(int)));

  connect(psiSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(psiSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_dopsiRotation()));
  connect(mview1, SIGNAL(SIG_psiRotationChanged(int)), psiSlider, SLOT(setValue(int)));

  connect(mxSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(mxSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_doXMovement()));
  connect(mview1, SIGNAL(SIG_xMovementChanged(int)), mxSlider, SLOT(setValue(int)));

  connect(mySlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(mySlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_doYMovement()));
  connect(mview1, SIGNAL(SIG_yMovementChanged(int)), mySlider, SLOT(setValue(int)));

  connect(distSlider, SIGNAL(valueChanged(int)), this, SLOT(SL_setSliderValue(int)));
  connect(distSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_doZMovement()));
  connect(mview1, SIGNAL(SIG_zMovementChanged(int)), distSlider, SLOT(setValue(int)));

/*
  connect(phiSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(phiSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_dophiRotation()));
  connect(mview1, SIGNAL(SIG_phiRotationChanged(double)), phiSlider, SLOT(setValue(double)));

  connect(thetaSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(thetaSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_dothetaRotation()));
  connect(mview1, SIGNAL(SIG_thetaRotationChanged(double)), thetaSlider, SLOT(setValue(double)));

  connect(psiSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(psiSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_dopsiRotation()));
  connect(mview1, SIGNAL(SIG_psiRotationChanged(double)), psiSlider, SLOT(setValue(double)));

  connect(mxSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(mxSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_doXMovement()));
  connect(mview1, SIGNAL(SIG_xMovementChanged(double)), mxSlider, SLOT(setValue(double)));

  connect(mySlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(mySlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_doYMovement()));
  connect(mview1, SIGNAL(SIG_yMovementChanged(double)), mySlider, SLOT(setValue(double)));

  connect(distSlider, SIGNAL(valueChanged(double)), this, SLOT(SL_setSliderValue(double)));
  connect(distSlider, SIGNAL(sliderReleased()), mview1, SLOT(SL_doZMovement()));
  connect(mview1, SIGNAL(SIG_zMovementChanged(double)), distSlider, SLOT(setValue(double)));
*/
}

// ====================    S L O T S    ===========================

void MainWindow::SL_saveAtomsAs()
{
  bool ok;
  QString current_dir1 = ActualData->current_dir;
  QString text = QInputDialog::getText(this, "Save coordinates",
                     "File name:", QLineEdit::Normal, "newAtoms.xyz", &ok);
// qWarning("file: %s", text.toAscii().data());
  if ( !ok || text.isEmpty()) return;
//  if ( Actual->num_choosedAtoms==0 ) saveAtoms(current_dir1.append("/V_atoms/").append(text));
//  else                               saveChoosedAtoms(current_dir.append("/V_atoms/").append(text));
  ActualData->saveAtoms(current_dir1.append("/data/atoms/").append(text));
  return;
}

void MainWindow::SL_actPoint(QVector3D res)
{
  QString str1;
// qWarning("SL_actPoint");
  ActualData->actPoint = res;
  int ix = mview1->lastPos.x();
  int iy = mview1->lastPos.y();
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
  int ix = mview1->lastPos.x();
  int iy = mview1->lastPos.y();
  
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
               items << ActualData->crstr[i].struct_name;
  bool ok;
  QString item = QInputDialog::getItem(this, "Choose structure", 
                     "structure:", items, ActualData->numbcrstr, false, &ok);
  if ( ok && !items.isEmpty() ) 
     for (int i=0; i<ActualData->numbcrstr; i++) 
        if ( item==ActualData->crstr[i].struct_name ) {
           ActualData->actcrstr = &(ActualData->crstr[i]);
           infotxt0.sprintf("Structure: %s%sMiller indices: %s", 
               ActualData->actcrstr->struct_name.toAscii().data(),
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
  QStringList quest, sug, ans, fields;
  QString aa;
  bool ok;
  QString descr = ("<h5>a, b, c - in &Aring;<br>alpha, beta, gamma - in degrees</h5>");
  quest << "<h6>name=</h6>" << "a=" << "b=" << "c=" << "alpha="
                    << "beta=" << "gamma=" << "numb. of atoms";
  sug << "B4_GaN" << "3.180" << "3.180" << "5.166" << "90.0"
                  << "90.0" << "120.0"<< "4";
  QuestionForm("Atomic structure definition", descr, quest, sug, ans, ok);
  if ( !ok ) return;
/*
  int at_dat_num1 = at_dat_num+1;
  Atoms[at_dat_num1].struct_name = ans.at(0);
  Atoms[at_dat_num1].a = ans.at(1).toFloat();
  Atoms[at_dat_num1].b = ans.at(2).toFloat();
  Atoms[at_dat_num1].c = ans.at(3).toFloat();
  Atoms[at_dat_num1].alpha = ans.at(4).toFloat();
  Atoms[at_dat_num1].beta = ans.at(5).toFloat();
  Atoms[at_dat_num1].gamma = ans.at(6).toFloat();
  int nat = Atoms[at_dat_num1].nchem = ans.at(7).toInt();

  descr.sprintf("<h5>%d atom's kind and coordinates x, y, z</h5>", nat);
  quest.clear();
  sug.clear();
  ans.clear();
  for (int i=0; i<nat; i++) {
     aa.sprintf("<h5>Atom nr. %d</h5>", i+1);
     quest << aa;
     switch (i) {
      case 0:
       sug << "N 0.0 0.0 0.0";
       break;
      case 1:
       sug << "N 2/3 1/3 1/2";
       break;
      case 2:
       sug << "Ga 2/3 1/3 1/8";
       break;
      case 3:
       sug << "Ga 0.0 0.0 5/8";
       break;
      default:
       sug << "atom? ? ? ?";
       break;
     }
  }
  QuestionForm1("Atom's kinds and coordinates", descr, quest, sug, ans);
  if ( !qf_ok ) return;
  for (int i=0; i<nat; i++) {
     fields = ans.at(i).split(QRegExp("\\s+"), QString::SkipEmptyParts);
     if ( fields.size()!=4 ) qWarning("Error - Atom coordinates %d", i);
     Atoms[at_dat_num1].cr_kind[i] = which_atom(fields.takeFirst());
     Atoms[at_dat_num1].cryst[i].x = read_fraction(fields.takeFirst());
     Atoms[at_dat_num1].cryst[i].y = read_fraction(fields.takeFirst());
     Atoms[at_dat_num1].cryst[i].z = read_fraction(fields.takeFirst());
  }
  at_dat_num = num_at_dat = at_dat_num1;
  At = Atoms[at_dat_num1];
  structureDefined = true;
  infotxt0.sprintf("Structure: %s%sMiller indices: %s", 
                   At.struct_name.toAscii().data(), infsepar.toAscii().data(),
                   toRichText(act_mill).toAscii().data());
  InfoDisplay();
*/
// qWarning ("SLOT define %d", at_dat_num);
}


//--------------------------------------------------------------
// ------------------- G E N E R A T I O N ---------------------
// -------------------------------------------------------------

void MainWindow::SL_genAtoms()
{
  QString descr = "<h5>How many repetitions in<br>x, y, z direction</h5>";
  QStringList questions;
  QStringList sugestions;
  QStringList answers;
  bool ok;

  questions << "Number X" <<"Number Y" << "Number Z";
  sugestions << "6" << "5" << "3";

  QuestionForm("nx, ny, nz", descr, questions, sugestions, answers, ok);
  if ( !ok ) return;

  int nx = answers.at(0).toInt();
  int ny = answers.at(1).toInt();
  int nz = answers.at(2).toInt();
// qWarning("nx, ny, nz: %d %d %d", nx, ny, nz);
  int nmax = nx*ny*nz * ActualData->actcrstr->nchem;
  ActualData->atoms->coord = new QVector3D[nmax];
  ActualData->atoms->u = new QVector3D[nmax];
  ActualData->atoms->type = new int[nmax];
  ActualData->atoms->n_atoms = ActualData->lattice(nx, ny, nz);
 qWarning("+++++++++ n_atoms=%d", ActualData->atoms->n_atoms);
//  ActualData->atoms->n_atoms = nmax;
  ActualData->atoms->n_bonds = 0;
  ActualData->minmax3(ActualData->atoms->coord, ActualData->atoms->n_atoms, ActualData->a_min_, ActualData->a_max_);
  ActualData->atoms_loaded = QString("neW_atoms.xyz");
//  ActualData->INT_nn = ActualData->atoms->n_atoms;
  InfoDisplay();
  emit SIG_needDraw();

}


void MainWindow::SL_gen1Atoms()
{
  QString descr = "<h5>Which sizes (in &Aring;) in<br>x, y direction</h5>";
  QStringList questions;
  QStringList sugestions;
  QStringList answers;
  bool ok;

  questions << "Size X" <<"Size Y" << "Number Z";
  sugestions << "50" << "50" << "3";

  QuestionForm("size x, size y, nz", descr, questions, sugestions, answers, ok);
  if ( !ok ) return;

  double x_size = answers.at(0).toDouble();
  double y_size = answers.at(1).toDouble();
  int nz = answers.at(2).toInt();

  double sg = sin(constant::deg2rad*ActualData->actcrstr->gamma);
  int nmax = ActualData->actcrstr->nchem * nz* 
             (int(x_size/ActualData->actcrstr->a)+1) *
             (int(y_size/(ActualData->actcrstr->b*sg))+1);
  ActualData->atoms->coord = new QVector3D[nmax];
  ActualData->atoms->u = new QVector3D[nmax];
  ActualData->atoms->type = new int[nmax];

  ActualData->atoms->n_atoms = ActualData->lattice2(x_size, y_size, nz);
  ActualData->atoms->n_bonds = 0;
  ActualData->minmax3(ActualData->atoms->coord, ActualData->atoms->n_atoms, ActualData->a_min_, ActualData->a_max_);
  ActualData->atoms_loaded = QString("neW_atoms.xyz");
//  ActualData->INT_nn = ActualData->atoms->n_atoms;
  InfoDisplay();
  emit SIG_needDraw();

}

//----------------------------------------------------------
//----------------------------------------------------------

//void MainWindow::SL_setSliderValue(double val)
void MainWindow::SL_setSliderValue(int val)

{
// qWarning ("SLIDER Val=%g", val);
  ActualData->sliderValue = val;
  ActualData->sliderMove = true;
}

void MainWindow::SL_about()
{
  HelpBrowser about;
  about.show_page ("index.html");
}

void MainWindow::SL_openAtoms()
{
  QString current_dir1 = ActualData->current_dir;
 qWarning("SL_openAtoms");//, current_dir1.toAscii().data() );
  QString aname1 = QFileDialog::getOpenFileName(this, "Select atoms", 
                current_dir1.append("/data/atoms"), "Molecules (*.xyz *.alc)");
  if ( aname1.isEmpty()  || aname1==ActualData->atoms_loaded ) return;
  ActualData->atoms_loaded = aname1;
  if ( !(aname1.contains(".xyz")) && !(aname1.contains(".alc")) )
     qWarning ("openAtoms -- unknown file format");
  else {
     ActualData->atoms_loaded = aname1;
     ActualData->read_alc_xyz(aname1);
     emit SIG_needDraw();
//    qWarning("sig needDraw");
  }
//  infotxtat.sprintf("%s", aname.toAscii().data());
  InfoDisplay();
  mview1->updateGL();
}

void MainWindow::SL_openImg()
{
  QString cdir = ActualData->current_dir;
  QString iname = QFileDialog::getOpenFileName(this, "Select an image",
    cdir.append("/data/images"), "Images (*.png *.xpm *.jpg *.tif)");

  if ( iname.isEmpty() ) return;

  ActualData->img_loaded = iname;
//  ActualData->imgChanged = (ActualData->img_loaded!=iname);
  ActualData->read_img(iname);
  emit SIG_prepareImg();
//  infotxtimg.sprintf("%s", iname.toAscii().data());
  InfoDisplay();
  mview1->updateGL();
}

void MainWindow::SL_closeImg()
{
  ActualData->img_loaded = "none";
//    delete &img;
  InfoDisplay();
}

void MainWindow::SL_millerAct()
{
 qWarning("----   SL_millerAct");
  QString descr = "<h4>Miller indices;<br>e.g. 1/3[10-10](11-20)<br>&nbsp;or&nbsp;[100](001)</h4>";
  QStringList quest, sug, ans;
  QString s;//, result_text;
  bool ok;
  s.sprintf("%s", ActualData->act_mill.toAscii().data());
  quest << "Miller indices";
   
  sug << s;

  QuestionForm("Size of structure", descr, quest, sug, ans, ok);
  if ( !ok ) return;
  ActualData->act_mill = ans.at(0);
//  act_mill = result_text = ans.at(0);
  ActualData->processMiller(1, ActualData->act_mill);
  InfoDisplay();
//  statusBar()->showMessage(QString("Rotation ").append(ActualData->act_mill));
//  emit SIG_needDraw();
  emit SIG_repaint();
}

void MainWindow::SL_dislocAct()
{
//  QString title = "Dislocation parameters";
  QString descr = "<h4>Dislocation parameters</h4>";
  QString s1, s2;
  QStringList quest, sug, ans;
  bool ok;
 qWarning("----   SL_dislocAct");
  
//  ActualData->kindOfDisl = 0;

  ActualData->act_core.sprintf("4");
  quest <<"Miller indices" << "disl. core";
  s1.sprintf("%s", ActualData->act_disl.toAscii().data());
  s2.sprintf("%s", ActualData->act_core.toAscii().data());
  sug << s1 << s2;
  QuestionForm("Dislocation parameters", descr, quest, sug, ans, ok);
  if ( !ok ) {
  qWarning("tu");
    return;
}
  s1 = ans.at(0);
  s2 = ans.at(1);
//  dialogDislWindow(title, prompt, act_disl, act_core);
  ActualData->processMiller(2, s1, s2);
  ActualData->act_disl = s1;
  ActualData->act_core = s2;
  InfoDisplay();
  mview1->updateGL();
//  statusBar()->showMessage
//  (QString("Dislocation ").append(ActualData->act_disl).append(" _ ").append(ActualData->act_core));
}

void MainWindow::SL_dislAct()
{
//  QString title = "Dislocation parameters";
  QString descr = "<h4>Disl0 parameters</h4>";
  QString s1;
  QStringList quest, sug, ans;
  bool ok;
 qWarning("----   SL_dislocAct");
//  ActualData->kindOfDisl = 1;
  ActualData->act_core.sprintf("none");
  quest <<"Miller indices" << "number of atom";
  s1.sprintf("%s", ActualData->act_disl.toAscii().data());
//  s2.sprintf("%s", ActualData->act_core.toAscii().data());
  sug << s1 << QString(" ");
  QuestionForm("Disloc parameters", descr, quest, sug, ans, ok);
//  if ( !ok ) return;
  if ( !ok ) {
  qWarning("tu -");
    return;
}

  s1 = ans.at(0);
  int n_a = ans.at(1).toInt();
//  dialogDislWindow(title, prompt, act_disl, act_core);
  ActualData->processMiller(1, s1, QString("none"));
 qWarning("SL_dislAct -- 1");
 
  ActualData->newdisl(n_a, true);
 qWarning("SL_dislAct -- 2");
  ActualData->act_disl = s1;
 qWarning("SL_dislAct -- 3");
  InfoDisplay();
  mview1->updateGL();
//  statusBar()->showMessage
//  (QString("Dislocation ").append(ActualData->act_disl).append(" _ ").append(ActualData->act_core));
}

void MainWindow::SL_addCoordAct()
{
  ActualData->addDisplacements();
  emit SIG_repaint();
  mview1->updateGL();
//  qWarning("SL_addCoordAct");
}


void MainWindow::SL_changeMode(int mode)
{
  if ( mode!=ActualData->Mode ) {
 qWarning("+++++           MODE = %d", mode);
     ActualData->Mode = mode;
  }
  if ( ActualData->Mode==2 ) mview1->setCursor(Qt::CrossCursor);
  else  mview1->setCursor(Qt::ArrowCursor);
}

void MainWindow::SL_sett()
{
  QStringList questions;
  bool ok;
  questions << "face_0" << "face_f" << "nodes_0" << "nodes_f" << "int.lines" << "ext_lines" << "spectrograms"
            << "arrows"  << "axis";
  Questions("Visibility", questions, ActualData->visible, ok);
  emit SIG_repaint();
}

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
  QVector3D box;
  QString descr = "<h4>Write sizes of structure;<br> ( in &Aring; )</h4>";
  QStringList quest, sug, ans;
  bool ok;
  quest << "Size X" << "Size Y" << "Size Z";
  sug << "20" << "20" << "20";

  QuestionForm("Size of structure", descr, quest, sug, ans, ok);
  if ( !ok ) return;

  box.setX(0.5 * ans.at(0).toDouble());
  box.setY(0.5 * ans.at(1).toDouble());
  box.setZ(0.5 * ans.at(2).toDouble());
/*
  Actual->num_choosedAtoms = cubBox(box, Actual->cent_);
  for (int i=0; i<Actual->n_atoms; i++ )
     Actual->atom_show[i] = Actual->at_bool[i];
  emit SIG_needDraw();
*/
}

void MainWindow::SL_hexBox()
{

  QString descr = "<h4>Write sizes of structure;<br> ( in &Aring; )</h4>";
  QStringList quest, sug, ans;
  bool ok;
  quest << "Hex radius" << "Size Z";
  sug << "30" << "25";

  QuestionForm ("Size of structure", descr, quest, sug, ans, ok);

  if (!ok) 
    return;

/*
  double hex_r = ans.at(0).toDouble();
  double hex_h = ans.at(1).toDouble();

  Actual->num_choosedAtoms = hexBox(hex_r, hex_h, Actual->cent_);
  for (int i=0; i<Actual->n_atoms; i++ ) 
         Actual->atom_show[i] = Actual->at_bool[i];

  emit SIG_needDraw();
*/
}

void MainWindow::InfoDisplay()
{
  QString inftxt;
  infoLabel->clear();
  infotxt0.sprintf("Structure: %s%sMiller indices: %s", 
          ActualData->actcrstr->struct_name.toAscii().data(),
          infsepar.toAscii().data(), toRichText(ActualData->act_mill).toAscii().data());
  infotxtat.sprintf("<br/>Atoms loaded: %s", 
                               ActualData->atoms_loaded.toAscii().data());
  infotxtimg.sprintf("<br/>Image loaded: %s", 
                               ActualData->img_loaded.toAscii().data());

  inftxt.sprintf("%s%s%s", infotxt0.toAscii().data(), 
                           infotxtat.toAscii().data(), 
                           infotxtimg.toAscii().data());
// qWarning("INFO: %s", inftxt.toAscii().data());
   infoLabel->setText(inftxt);
}


void MainWindow::keyPressEvent(QKeyEvent *keyEv)
{
  int k = keyEv->key();
  emit SIG_keypress(k);
  qWarning("--------key=%d", k);
  if ( mview1->isActiveWindow() ) qWarning("____________ 1");
  else                      qWarning("____________ 2");
}

QSlider *MainWindow::createSlider
                      (int from, int to, int step, int val)
{
  QSlider* slider = new QSlider(Qt::Horizontal);

  slider->setTickInterval(step);
  slider->setMaximum(to); // paging disabled
  slider->setMinimum(from);
//  slider->setScaleMaxMinor(10);
  slider->setValue(val);
  return slider;
}

QString MainWindow::toRichText(QString txt)
{
  QString txt1;
  txt1 = txt.replace("<", "&lt;").replace(">", "&gt;");
  return txt1;
}

//---------------------------------------------------------------------
/*
QwtSlider *MainWindow::createSlider
                      (double from, double to, double step, double val, bool logaritmic)
{
  QwtSlider* slider = new QwtSlider(this, 
                   Qt::Horizontal, QwtSlider::TopScale, QwtSlider::BgTrough);
  if ( logaritmic ) slider->setScaleEngine(new QwtLog10ScaleEngine);
  slider->setThumbWidth(10);
  slider->setRange(from, to); // paging disabled
  slider->setScale(from, to, step);
//  slider->setScaleMaxMinor(10);
  slider->setValue(val);
  return slider;
}


void MainWindow::saveAtoms(QString sname)
{
  QString line;
  QFile fil(sname);
 qWarning("file: %s, numChoosedAtoms=%d", sname.toAscii().data(), Actual->num_choosedAtoms);  
  if (!fil.open(QIODevice::WriteOnly)) { 
     qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
     return; 
  }
  QTextStream out(&fil);
  out << line.sprintf("%d\n ---\n", Actual->n_atoms);
  for (int i=0; i<Actual->n_atoms; i++)
     out << line.sprintf("%4s %12.7f %12.7f %12.7f\n",
                   namea[Actual->type[i]].toAscii().data(),
                   Actual->coord[i].x, Actual->coord[i].y, Actual->coord[i].z);
}

void MainWindow::saveChoosedAtoms (QString sname)
{
  QString line;
  QFile fil(sname);
 qWarning("file: %s, numChoosedAtoms=%d", sname.toAscii().data(), Actual->num_choosedAtoms);  
  if (!fil.open(QIODevice::ReadWrite)) { 
     qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
     return; 
  }
  QTextStream out(&fil);
  out << line.sprintf("%d\n ---\n", Actual->num_choosedAtoms);
  for (int i=0; i<Actual->n_atoms; i++)
     if ( Actual->at_bool[i] ) out << line.sprintf("%4s %12.7f %12.7f %12.7f\n",
                               namea[Actual->type[i]].toAscii().data(),
                               Actual->coord[i].x, Actual->coord[i].y,
                               Actual->coord[i].z);
}
*/

