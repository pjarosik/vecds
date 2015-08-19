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
#include "mainwindow.h"
#include <iostream>

extern Atoms *AT;
extern Lattice *LATT;
extern Internal *INT;
extern Adds *ADDS;


MainWindow::MainWindow()
{
    setMinimumSize(800, 600);
    setContentsMargins(0, 0, 0, 0);
    setAutoFillBackground(true);
    //setStyleSheet("background-color:white");    
    setBackgroundRole(QPalette::Base);//Window);//(QColor(255, 255, 255, 255));
    //setForegroundRole(QPalette::Base);//Window);//(QColor(255, 255, 255, 255));
    setWindowTitle("Visual Editor of Crystal Defects");
    MainWidget = new QWidget;
    //MainWidget->setAutoFillBackground(true);
    //MainWidget->setStyleSheet("margin:0px;" "border:0px;" "padding:0px;");//("background-color:white");    
    scene = new OsgScene();
    intscene = scene->getScene();
//std::cout << "---1" << std::endl;
    valTh = 0.;
    valPh = 0.;
    valPs = 90.;
    valDist = 1.;
    valCx = 0.0;
    valCy = 0.0;
    strBvect = "";
    strAtom = "";
    strCore = "";
    iAt = "";
    iStr = "";
    YtoZ.makeRotate(osg::Y_AXIS, osg::Z_AXIS);
    ZtoY.makeRotate(osg::Z_AXIS, osg::Y_AXIS);
    
    mview1 = new OsgViewerQt(intscene, INT->fov);
    mview1->setMinimumSize(600,400);
    mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel(this);
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setTextFormat(Qt::RichText);
//    infoLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    infoLabel->clear();
    //infoText.clear();
    infoLabel->setText("VECDS");
    //mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    MainLayout = new QGridLayout;
    MainLayout->setMargin(0);
    MainLayout->setSpacing(0);
    MainLayout->setContentsMargins(0, 0, 0, 0);    
    MainLayout->addWidget(infoLabel, 0, 0);
    MainLayout->addWidget(mview1, 1, 0);
    
    MainWidget->setLayout(MainLayout);
    setCentralWidget(MainWidget);
    
    createActions();
    createMenus();
//    createToolBars();
    createDockWindows();
    //createStatusBar();
    //statusBar = new(QStatusBar);
    QStatusBar *statusBar = this->statusBar();
    statusBar->setStyleSheet("{background: brown}");
    //this->setStyleSheet("QStatusBar { background: yellow }");
    //statusBar->clearMessage();
    //statusBar->show();
    //statusBar->setSizeGripEnabled(true);
    if ( !INT->atName.empty() ) {
       readAlcXyz(QString::fromStdString(INT->atName));
       INT->refrAtoms = true;
       refreshScene();
    }
    mode = 0;
    refreshViewer();
    //modesTab->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete AT;
    delete LATT;
    delete INT;
    //delete STR;
}

void MainWindow::createActions()
{
  chooseAct = new QAction(tr("Choose structure"), this);
  chooseAct->setStatusTip(tr("struct ..."));
  connect(chooseAct, SIGNAL(triggered()), this, SLOT(SL_chooseStructure()));

  edStructAct = new QAction(tr("Edit structures"), this);
  edStructAct->setStatusTip(tr("defines ..."));
  connect(edStructAct, SIGNAL(triggered()), this, SLOT(SL_editStruct()));

  genAct = new QAction(tr("Generate lattice)"), this);
//  genAct = QWhatsThis::createAction(this);
  genAct->setText(tr("Generate lattice"));
  genAct->setStatusTip(tr("generate a new file"));
//  genAct->setWhatsThis(tr("use this button\n aaaaaaaaaaaaaaaa\n to generate a <h4>new</h4> file\n by giving number of repetitions"));
//  QWhatsThis::hideText();
  connect(genAct, SIGNAL(triggered()), this, SLOT(SL_genAtoms()));

  atomsAct = new QAction(tr("Atoms"), this);
  atomsAct->setStatusTip (tr ("Set visibility"));
  connect(atomsAct, SIGNAL (triggered()), this, SLOT( SL_atoms() ));
  
  bgAct = new QAction("Bacground", this);
  bgAct->setStatusTip (tr ("Set background"));
  connect(bgAct, SIGNAL (triggered()), this, SLOT( SL_setBg() ));

  numAct = new QAction("Turn on numbers", this);
  numAct->setCheckable(true);
  numAct->setChecked(INT->showNum);
  numAct->setStatusTip (tr ("Set numbers"));
  connect(numAct, SIGNAL (triggered()), this, SLOT( SL_setNum() ));

  lightAct = new QAction("Turn on light", this);
  lightAct->setCheckable(true);
  lightAct->setChecked(INT->lightOn);
  lightAct->setStatusTip (tr ("Set lighting"));
  connect(lightAct, SIGNAL (triggered()), this, SLOT( SL_setLight() ));

  materialAct = new QAction("Turn on material properties", this);
  materialAct->setCheckable(true);
  materialAct->setChecked(INT->materialOn);
  materialAct->setStatusTip (tr ("Set material"));
  connect(materialAct, SIGNAL (triggered()), this, SLOT( SL_setMaterial() ));
  
  showAddsAct = new QAction("Turn on showing adds", this);
  showAddsAct->setCheckable(true);
  showAddsAct->setChecked(INT->showAdds);
  showAddsAct->setStatusTip (tr ("Set adds"));
  connect(showAddsAct, SIGNAL (triggered()), this, SLOT( SL_setAdds() ));
  

  nsAct = new QAction(tr("Size of numbers"), this);
  nsAct->setStatusTip(tr("Numbers"));
  connect(nsAct, SIGNAL(triggered()), this, SLOT(SL_setNS()));

  bsAct = new QAction(tr("Size of bonds"), this);
  bsAct->setStatusTip(tr("Bonds"));
  connect(bsAct, SIGNAL(triggered()), this, SLOT(SL_setBonds()));
  
  bcAct = new QAction(tr("Color of bonds"), this);
  bcAct->setStatusTip(tr("Bonds"));
  connect(bcAct, SIGNAL(triggered()), this, SLOT(SL_setColB()));

  axAct = new QAction(tr("Axis"), this);
  axAct->setStatusTip(tr("Axis"));
  connect(axAct, SIGNAL(triggered()), this, SLOT(SL_setAx()));

  fovAct = new QAction(tr("Field of view"), this);
  fovAct->setStatusTip(tr("Camera angle"));
  connect(fovAct, SIGNAL(triggered()), this, SLOT(SL_setFov()));
  
  vectAct = new QAction(tr("Vectors"), this);
  vectAct->setStatusTip(tr("Bonds"));
  connect(vectAct, SIGNAL(triggered()), this, SLOT(SL_setVect()));  

  resetAct = new QAction(tr("Reset"), this);
  resetAct->setStatusTip(tr("Reset"));
  connect(resetAct, SIGNAL(triggered()), this, SLOT(SL_reset()));  

// Open a file containing definitions of atomic structure in .xyz ot
// .alc format
  openAct = new QAction (tr ("Open atoms"), this);
  openAct->setStatusTip (tr ("Opens an existing file as .xyz or .alc"));
  connect(openAct, SIGNAL (triggered()), this, SLOT( SL_openAtoms() ));

  closeAct = new QAction (tr ("Close atoms"), this);
  closeAct->setStatusTip (tr ("Closes an existing file as .xyz or .alc"));
  connect(closeAct, SIGNAL (triggered()), this, SLOT( SL_closeAtoms() ));
  
  openFemAct = new QAction(tr("Import external FE data"), this);
  //    openimgAct->setShortcut(tr("Ctrl+I"));
  openFemAct->setStatusTip(tr("Imports external FE"));
  connect(openFemAct, SIGNAL(triggered()), this, SLOT(SL_openFem()));
  
  computeFemAct = new QAction(tr("Computes external FE data"), this);
  //    openimgAct->setShortcut(tr("Ctrl+I"));
  computeFemAct->setStatusTip(tr("Computes external FE"));
  connect(computeFemAct, SIGNAL(triggered()), this, SLOT(SL_computeFem()));

  showResAct = new QAction(tr("Show FE results"), this);
  //    openimgAct->setShortcut(tr("Ctrl+I"));
  showResAct->setStatusTip(tr("Shows external FE"));
  connect(showResAct, SIGNAL(triggered()), this, SLOT(SL_showRes()));

  closeFemAct = new QAction(tr("Close FE"), this);
  //    closeimgAct->setShortcut(tr("Ctrl+X"));
  closeFemAct->setStatusTip(tr("Close FE"));
  connect(closeFemAct, SIGNAL(triggered()), this, SLOT(SL_closeFem()));

  // Open a fem file
  imageAct = new QAction(tr("Open HRTEM image"), this);
  //    openimgAct->setShortcut(tr("Ctrl+I"));
  imageAct->setStatusTip(tr("Opens an image"));
  connect(imageAct, SIGNAL(triggered()), this, SLOT(SL_image()));
  
  closeimgAct = new QAction(tr("Close image"), this);
  //    closeimgAct->setShortcut(tr("Ctrl+X"));
  closeimgAct->setStatusTip(tr("Closes an existing img"));
  connect(closeimgAct, SIGNAL(triggered()), this, SLOT(SL_closeImage()));

  saveAct = new QAction(tr("Save atoms"), this);
  //    saveAsAct->setShortcut(tr("Ctrl+W"));
  saveAct->setStatusTip(tr("Saves existing coordinates"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(SL_saveAtoms()));

  planeAct = new QAction(tr("Plane defined by Miller Indices"), this);
  planeAct->setStatusTip("Plane defined by Miller Indices");
//  connect(planeAct, SIGNAL(triggered()), this, SLOT(SL_cubBox()));

  cubBoxAct = new QAction(tr("Rectangular box"), this);
  cubBoxAct->setStatusTip(tr("Rectangular bounding box"));
//  connect(cubBoxAct, SIGNAL(triggered()), this, SLOT(SL_cubBox()));
  
  hexBoxAct = new QAction(tr("Hexagonal box"), this);
  hexBoxAct->setStatusTip(tr("Hexagonal bounding box"));
//  connect(hexBoxAct, SIGNAL(triggered()), this, SLOT(SL_hexBox()));

  MillerAct = new QAction(tr("Rotation by Miller indices"), this);
  //    MillerAct->setShortcut(tr("Ctrl+M"));
  MillerAct->setStatusTip(tr("input Miller indices"));
  connect(MillerAct, SIGNAL(triggered()), this, SLOT(SL_millerAct()));

  dislocAct = new QAction(tr("Input single dislocation"), this);
  dislocAct->setStatusTip(tr("input Miller indices"));
  connect(dislocAct, SIGNAL(triggered()), this, SLOT(SL_dislocAct()));

  calcDislAct = new QAction(tr("Calculate displacement field for single dislocation"), this);
  calcDislAct->setStatusTip(tr("Calculate displacement field for single dislocation"));
  connect(calcDislAct, SIGNAL(triggered()), this, SLOT(SL_calcDisl()));

  updateAct = new QAction("Add displacement field to coordinates", this);
  updateAct->setStatusTip("Add displacement field to coordinates");
  connect(updateAct, SIGNAL(triggered()), this, SLOT(SL_update()));

  moveAct = new QAction("Move marked atoms", this);
  moveAct->setStatusTip("Move marked atoms");
  //connect(moveAct, SIGNAL(triggered()), this, SLOT(SL_move()));

  editSettAct = new QAction("Edit settings", this);
  editSettAct->setStatusTip("Edit settings");
  connect(editSettAct, SIGNAL(triggered()), this, SLOT(SL_editSett()));

  chooseSettAct = new QAction("choose settings", this);
  chooseSettAct->setStatusTip("choose settings");
  connect(chooseSettAct, SIGNAL(triggered()), this, SLOT(SL_chooseSettings()));
    
// About box for vecds
  aboutAct = new QAction (tr ("About"), this);
  aboutAct->setStatusTip (tr ("Show the vecds application's about box"));
  connect (aboutAct, SIGNAL (triggered()), this, SLOT (SL_about()));
  
// About box for Qt
  aboutQtAct = new QAction (tr ("About Qt"), this);
  aboutQtAct->setStatusTip (tr ("Show the Qt library's about box"));
  connect (aboutQtAct, SIGNAL (triggered()), qApp, SLOT (aboutQt()));

  aboutOsgAct = new QAction (tr ("About Osg"), this);
  aboutOsgAct->setStatusTip (tr ("Show the Osg library's about box"));
  connect (aboutOsgAct, SIGNAL (triggered()), this, SLOT (SL_aboutOsg()));
  
  visibAct = new QAction("Visibility", this);
  visibAct->setStatusTip ("Opacity of atoms, image and FEMs");
  connect (visibAct, SIGNAL(triggered()), this, SLOT(SL_visibility()));
  
  connect(this, SIGNAL(SIG_keypress(int)), this, SLOT(SL_keypress(int)));
  //connect(edit, SIGNAL(SIG_finished(QString)), this SLOT(SL_performCalc(QString)));
}


void MainWindow::createMenus()
{
// qWarning("mainw - createMenus -- 0");
  fileMenu = menuBar()->addMenu(tr("File"));

  defstructMenu = fileMenu->addMenu("Structure definition");
  defstructMenu->addAction(chooseAct);
  defstructMenu->addSeparator();
  defstructMenu->addAction(edStructAct);
  fileMenu->addSeparator();
 
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  
  fileMenu->addAction(closeAct);
  fileMenu->addSeparator();  
  
  fileMenu->addAction(imageAct);
  fileMenu->addSeparator();  
 
  fileMenu->addAction(closeimgAct);
  fileMenu->addSeparator();

  fileMenu->addAction(openFemAct);
  fileMenu->addSeparator();  
 
  fileMenu->addAction(computeFemAct);
  fileMenu->addSeparator();  
 
  fileMenu->addAction(showResAct);
  fileMenu->addSeparator();  

  fileMenu->addAction(closeFemAct);
  fileMenu->addSeparator();
 
  fileMenu->addAction(saveAct);
  fileMenu->addSeparator();

  displayMenu = menuBar()->addMenu(tr("Display"));
  displayMenu->addAction(resetAct);
  displayMenu->addSeparator();
  
  displayMenu->addAction(fovAct);
  displayMenu->addSeparator();

  displayMenu->addAction(bgAct);
  displayMenu->addSeparator();
  
  displayMenu->addAction(numAct);
  displayMenu->addSeparator();

  displayMenu->addAction(lightAct);
  displayMenu->addSeparator();
  
  displayMenu->addAction(materialAct);
  displayMenu->addSeparator();
  
  displayMenu->addAction(axAct);
  displayMenu->addSeparator();

  displayMenu->addAction(showAddsAct);
  displayMenu->addSeparator();
  
  displayMenu->addAction(atomsAct);
  displayMenu->addSeparator();

  displayMenu->addAction(vectAct);
  displayMenu->addSeparator();

  displayMenu->addAction(nsAct);
  displayMenu->addSeparator();

  displayMenu->addAction(bsAct);
  displayMenu->addSeparator();

  displayMenu->addAction(bcAct);
  displayMenu->addSeparator();
  
  displayMenu->addAction(visibAct);

  toolsMenu = menuBar()->addMenu("Tools");
  toolsMenu->addAction(genAct);
  toolsMenu->addSeparator();
  toolsMenu->addAction(MillerAct);
  toolsMenu->addSeparator();
  toolsMenu->addAction(dislocAct);
  toolsMenu->addSeparator();
  toolsMenu->addAction(calcDislAct);
  toolsMenu->addSeparator();
  toolsMenu->addAction(updateAct);
  toolsMenu->addSeparator();
  //genMenu = toolsMenu->addMenu("Generation");//genMenu->addAction(gen1Act);
  //toolsMenu->addSeparator();
  boxMenu = toolsMenu->addMenu("Mark atoms");
  boxMenu->addAction(planeAct);
  boxMenu->addSeparator();
  boxMenu->addAction(cubBoxAct);
  boxMenu->addSeparator();
  boxMenu->addAction(hexBoxAct);
  toolsMenu->addSeparator();
  toolsMenu->addAction(moveAct);

  settMenu = menuBar()->addMenu("Settings");
  //settMenu = fileMenu->addMenu("Settings definition");
  settMenu->addAction(chooseSettAct);
  settMenu->addSeparator();
  settMenu->addAction(editSettAct);
  
  helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutQtAct);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutOsgAct);
 qWarning("mainw - createMenus O.K.");
}

void MainWindow::createDockWindows()
{
  qWarning("mainw - createDockWindows");
  dockW = new QDockWidget("", this);
  dockW->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockW->setFeatures(QDockWidget::NoDockWidgetFeatures);
  
  QWidget *Widg_3slid = new QWidget(dockW);
  QGridLayout *Lay_g_3slid = new QGridLayout;
  
  QVBoxLayout *Lay_v_ThetaSlid = new QVBoxLayout;
  QLabel *Lab_ThetaSlid = new QLabel("Rotation around global &nbsp;<b><font color='blue'>Z</font></b>");
  Lay_v_ThetaSlid->addWidget(Lab_ThetaSlid);
  thetaSlider = new QLineEdit("0.");
  Lay_v_ThetaSlid->addWidget(thetaSlider);
  
  QVBoxLayout *Lay_v_PhiSlid = new QVBoxLayout; 
  QLabel *Lab_PhiSlid = new QLabel("Rotation around local &nbsp;<b><font color='green'>Y</font></b>");
  Lay_v_PhiSlid->addWidget(Lab_PhiSlid);
  phiSlider = new QLineEdit("0.");
  Lay_v_PhiSlid->addWidget(phiSlider);
  
  QVBoxLayout *Lay_v_PsiSlid = new QVBoxLayout;
  QLabel *Lab_PsiSlid = new QLabel("Rotation around local &nbsp;<b><font color='red'>X</font></b>");
  Lay_v_PsiSlid->addWidget(Lab_PsiSlid);( INT->bgColor );
  psiSlider = new QLineEdit("0.");//createSlider(-180, 180, 1, 0);
  Lay_v_PsiSlid->addWidget(psiSlider);

  QVBoxLayout *Lay_v_mx = new QVBoxLayout;
  QLabel *Lab_mx = new QLabel(" Position X");
  Lay_v_mx->addWidget(Lab_mx);
  //mxSlider = createSlider(-100, 100, 1, 0);
  mxSlider = new QLineEdit("0.");
  Lay_v_mx->addWidget(mxSlider);
  
  QVBoxLayout *Lay_v_my = new QVBoxLayout;
  QLabel *Lab_my = new QLabel(" Position Y");
  Lay_v_my->addWidget(Lab_my);
  mySlider = new QLineEdit("0.");//createSlider(-100, 100, 1, 0);
  Lay_v_my->addWidget(mySlider);
  
  QVBoxLayout *Lay_v_dist = new QVBoxLayout;
  QLabel *Lab_dist = new QLabel(" Distance");
  Lay_v_dist->addWidget(Lab_dist);
  distSlider = new QLineEdit("1.");//createSlider(-180, 180, 1, 0);
  Lay_v_dist->addWidget(distSlider);
  
  Lay_g_3slid->addLayout(Lay_v_ThetaSlid, 0, 0);
  Lay_g_3slid->addLayout(Lay_v_PhiSlid, 1, 0);
  Lay_g_3slid->addLayout(Lay_v_PsiSlid, 2, 0);
  Lay_g_3slid->addLayout(Lay_v_mx, 3, 0);
  Lay_g_3slid->addLayout(Lay_v_my, 4, 0);
  Lay_g_3slid->addLayout(Lay_v_dist, 5, 0);
  Widg_3slid->setLayout(Lay_g_3slid);
  dockW->setWidget(Widg_3slid);//(layModes);
  addDockWidget(Qt::RightDockWidgetArea, dockW);
  //viewMenu->addAction(dockW->toggleViewAction());
  
  setSliders(0., 0., 0., 1., 0., 0.);
}

void MainWindow::infoDisplay()
{
   infoLabel->text().clear();
   infoLabel->setText(QString("%1  %2  %3").arg(QString("VECDS "), iAt, iStr));
}

void MainWindow::setSliders(double v1, double v2, double v3, double v4, double v5, double v6)
{
  valTh = v1;  valPh = v2;  valPs = v3; valDist = v4; valCx = v5; valCy = v6;
  QString str;
  thetaSlider->setText(str.setNum(valTh, 'f', 3));
  phiSlider->setText(str.setNum(valPh, 'f', 3));
  psiSlider->setText(str.setNum(valPs, 'f', 3));
  distSlider->setText(str.setNum(valDist, 'f', 3));
  mxSlider->setText(str.setNum(valCx, 'f', 3));
  mySlider->setText(str.setNum(valCy, 'f', 3));
  
  connect(mview1, SIGNAL(SIG_thetaRotationChanged(QString)), thetaSlider, SLOT(setText(QString))); // this, SLOT(SL_ThetaSlider(QString))); //
  connect(thetaSlider, SIGNAL(returnPressed()), this, SLOT(SL_setThetaSlider()));
  
  connect(mview1, SIGNAL(SIG_phiRotationChanged(QString)), phiSlider, SLOT(setText(QString)));  // this, SLOT(SL_PhiSlider(QString))); //
  connect(phiSlider, SIGNAL(returnPressed()), this, SLOT(SL_setPhiSlider()));  
  
  connect(mview1, SIGNAL(SIG_psiRotationChanged(QString)), psiSlider, SLOT(setText(QString))); // this, SLOT(SL_PsiSlider(QString))); //
  connect(psiSlider, SIGNAL(returnPressed()), this, SLOT(SL_setPsiSlider()));    

  connect(mview1, SIGNAL(SIG_distChanged(QString)), distSlider, SLOT(setText(QString)));
  connect(distSlider, SIGNAL(returnPressed()), this, SLOT(SL_setDistSlider()));
  
  connect(mview1, SIGNAL(SIG_cxChanged(QString)), mxSlider, SLOT(setText(QString)));
  connect(mxSlider, SIGNAL(returnPressed()), this, SLOT(SL_setMxSlider()));    
  
  connect(mview1, SIGNAL(SIG_cyChanged(QString)), mySlider, SLOT(setText(QString)));
  connect(mySlider, SIGNAL(returnPressed()), this, SLOT(SL_setMySlider())); 
  
  if ( !connect (mview1, SIGNAL(SIG_actPoint(glm::dvec3)), this, SLOT(SL_actPoint(glm::dvec3))) ) std::cout << "CONNECTION ERROR" << std::endl; 

  SL_setPsiSlider();
}

void MainWindow::SL_setThetaSlider()//QString value)
{
  //if ( block ) return;
  QString value = thetaSlider->text();
  QString vPs = psiSlider->text();
  QString vPh = phiSlider->text();
  valTh = osg::clampBetween(value.toDouble(), -180., 180.);
//std::cout << "------------------------------" << std::endl << "SL_setThetaSlider  ..... valTh=" << valTh << std::endl;  
  qrot = MiscFunc::quatFromEuler(valTh * 0.01745329251994329576, valPh * 0.01745329251994329576, valPs * 0.01745329251994329576); //osg::Quat 
  mview1->setQuat(qrot);
//  osg::Quat q5 = mview1->getQuat();//  std::cout << " q5.x=" << q5.x() << " q5.y=" << q5.y() << " q5.z=" << q5.z() << " q5.w=" << q5.w() << std::endl;
  mview1->setDone(true);
  thetaSlider->setText(value);
  phiSlider->setText(vPh);
  psiSlider->setText(vPs);
  mview1->setDone(false);
}
  
void MainWindow::SL_setPhiSlider()
{
  QString value = phiSlider->text();
  QString vTh = thetaSlider->text();
  QString vPs = psiSlider->text();
  //double val = value.toDouble();
  //valPh = MiscFunc::myClampBetween(value.toDouble(), 90., -90.);
  valPh = osg::clampBetween(value.toDouble(), -90., 90.);
  qrot = MiscFunc::quatFromEuler(valTh * 0.01745329251994329576, valPh * 0.01745329251994329576, valPs * 0.01745329251994329576); //osg::Quat 
  mview1->setQuat(qrot);
//  osg::Quat q5 = mview1->getQuat();//  std::cout << " q5.x=" << q5.x() << " q5.y=" << q5.y() << " q5.z=" << q5.z() << " q5.w=" << q5.w() << std::endl;
  mview1->setDone(true);
  thetaSlider->setText(vTh);
  phiSlider->setText(value);
  psiSlider->setText(vPs);
  mview1->setDone(false);
}	

void MainWindow::SL_setPsiSlider()
{
  //if ( block ) return;
  QString value = psiSlider->text();
  QString vTh = thetaSlider->text();
  QString vPh = phiSlider->text();
  valPs = value.toDouble() + 90.;
 //std::cout << "------------------------------" << std::endl << "SL_setPsiSlider  ..... valPs=" << valPs << "   ..... vTh=" << vTh.toStdString() << "   ..... vPh=" << vPh.toStdString() << std::endl;  
  qrot = MiscFunc::quatFromEuler(valTh * 0.01745329251994329576, valPh * 0.01745329251994329576, valPs * 0.01745329251994329576); //osg::Quat 
  mview1->setQuat(qrot);
  mview1->setDone(true);
  thetaSlider->setText(vTh);
  phiSlider->setText(vPh);
  psiSlider->setText(value);
  mview1->setDone(false);
}

void MainWindow::SL_setDistSlider()
{
  QString value = distSlider->text();
  valDist = value.toDouble();
  std::cout << "SL_setDistSlider  val=" << valDist << "    dist0=" << INT->dist0 << std::endl;
  mview1->setDist(valDist*INT->dist0);
}

void MainWindow::SL_setMxSlider()
{
  QString value = mxSlider->text();
  valCx = value.toDouble();
  osg::Vec3d cc = mview1->getCentr();
  double newX = INT->centr0.x() - valCx*INT->scDim;
  mview1->setCentr(osg::Vec3d(newX, cc.y(), cc.z()));
}

void MainWindow::SL_setMySlider()//int val)//QString value)
{
  QString value = mySlider->text();
  valCy = value.toDouble();
  osg::Vec3d cc = mview1->getCentr();
  double newZ = INT->centr0.z() - valCy*INT->scDim;
  mview1->setCentr(osg::Vec3d(cc.x(), cc.y(), newZ));
}

void MainWindow::SL_millerAct()	
{
  mode = 1;
  std::cout << "SL_millerAct:  mode=" << mode << std::endl;
  mview1->setDone(true);
  if ( !INT->structureDefined ) { QMessageBox::warning(this, "PROBLEM", "You should define structure first");  mview1->setDone(false);  return; }  
  QString descr = "Miller indices";
  QString quest = "[hkl](uvw)";
  QString sug = "[100](001)";
  if ( !ansBurgers.isEmpty() ) sug = ansBurgers;
  OneQuestion *oq = new OneQuestion("Rotation parameters", descr, quest, sug, ansBurgers);
  mview1->setDone(false);
  if ( !oq->ok ) return;
// std::cout << "ans=" << ans.toStdString() << std::endl;
  if ( Calc::parse_miller(ansBurgers.toStdString()) ) {
     //MiscFunc::printVec("millerV", INT->millerV);
     INT->hklDefined = true;
     INT->atomDefined = false;
     strBvect = QString("Burger's vector: %1       ").arg(ansBurgers);
     strAtom = "";
     strCore = "";
     rotTens = Calc::rotation_tensor();//  glm::dmat3 rotInv = glm::transpose(rotTens);
     INT->rotTens = rotTens;
     glm::dquat qq = glm::quat_cast(rotTens);
     osg::Quat qq1 = osg::Quat(qq.x, qq.y, qq.z, qq.w);
     rotQuat = qq1.conj()*YtoZ; //  std::cout << " qrot1.x=" << qrot1.x() << " qrot1.y=" << qrot1.y() << " qrot1.z=" << qrot1.z() << " qrot1.w=" << qrot1.w() << std::endl;
     refreshScene();
     mview1->setQuat(rotQuat);
  }
}

void MainWindow::SL_dislocAct()	
{
  mode = 2;
  std::cout << "SL_dislocAct:  mode=" << mode << std::endl;
  mview1->setDone(true);
  if ( !INT->structureDefined ) { QMessageBox::warning(this, "PROBLEM", "You should define structure first");  mview1->setDone(false);  return; }  
  QString descr = "Burger's vector";
  QString sug = "[100](001)";
  if ( !ansBurgers.isEmpty() ) sug = ansBurgers;
  OneQuestion *oq = new OneQuestion("Burger's vector", descr, "", sug, ansBurgers);
  mview1->setDone(false);
  if ( !oq->ok ) return;
  if ( Calc::parse_miller(ansBurgers.toStdString()) ) {
     //MiscFunc::printVec("millerV", INT->millerV);
     strBvect = QString("Burger's vector: %1       ").arg(ansBurgers);
     strAtom = "";
     strCore = "";
     INT->hklDefined = true;
     INT->atomDefined = false;
     rotTens = Calc::rotation_tensor();
     INT->rotTens = rotTens;
     //mode = 2;
     glm::dquat qq = glm::quat_cast(rotTens);
     osg::Quat qq1 = osg::Quat(qq.x, qq.y, qq.z, qq.w);
     rotQuat = qq1.conj()*YtoZ;//  std::cout << " qrot1.x=" << qrot1.x() << " qrot1.y=" << qrot1.y() << " qrot1.z=" << qrot1.z() << " qrot1.w=" << qrot1.w() << std::endl;
     refreshScene();
     mview1->setQuat(rotQuat);          
     mview1->setDone(false);
     statusBar()->clearMessage();//showMessage("AAA");
     statusBar()->showMessage(strBvect);
  }

}

void MainWindow::SL_actPoint(glm::dvec3 pos)
{ 
  if ( mode==2 ) {
     mview1->setDone(true);
     int nA = Calc::identAtom(pos);
     INT->nA = nA;
     if ( INT->ctrl && INT->left ) {
        if ( nA<0 )  QMessageBox::warning(this, "PROBLEM", "You should show an atom pressing mouse left key and <CTRL>");
        else {
           if ( INT->hklDefined ) {
              if ( INT->atomDefined ) scene->displayPlane(rotTens, -1);
              scene->displayPlane(rotTens, nA); 
              scene->showOneAtom(nA);  
              INT->atomDefined = true;
              strAtom = QString("       atom nr. %1").arg(nA+1);
              strCore = "";
	      statusBar()->clearMessage();
	      statusBar()->showMessage(QString("%1 %2").arg(strBvect, strAtom));
	   } else //{
              QMessageBox::warning(this, "PROBLEM", "You should define Burger's vector");
        }
        mview1->setDone(false);  
        return;
     } //  INT->ctrl && INT->left	
     if ( INT->left && INT->shift ) {
        if ( !INT->atomDefined ) {
	   QMessageBox::warning(this, "PROBLEM", "You should show an atom pressing mouse left key and <CTRL>");
	   mview1->setDone(false);  
	   return;
	}
        if ( nA<0 ) {
	   scene->displayPlane(rotTens, -1);
	   INT->coreDefined = true;
	   strCore = QString("       core coordinates = { %1, %2, %3 }").arg(INT->actPoint.x, 0, 'f', 4).arg(INT->actPoint.y, 0, 'f', 4).arg(INT->actPoint.z, 0, 'f', 4);
	} else { // nA>=0
	   if ( QMessageBox::Yes == QMessageBox(QMessageBox::Information, "WARNING", "Do you want to put dislocation core in this atom?", QMessageBox::Yes|QMessageBox::No).exec() ) {
	      INT->actPoint = MiscFunc::convert(LATT->coords.get()->at(nA));
	      INT->coreDefined = true;
	      strAtom = QString("       atom nr. %1").arg(nA+1);
	      strCore = QString("       core coordinates = { %1, %2, %3 }").arg(INT->actPoint.x, 0, 'f', 4).arg(INT->actPoint.y, 0, 'f', 4).arg(INT->actPoint.z, 0, 'f', 4);
	   } else { // !qm->ok
	      mview1->setDone(false);  
	      return;     
	   } //QMessageBox
	} // nA>=0 
	if ( INT->atomDefined || INT->coreDefined ) {
	   statusBar()->clearMessage();
	   statusBar()->showMessage(QString("%1 %2 %3").arg(strBvect, strAtom, strCore));
	}
 	ADDS->pos->push_back(MiscFunc::convert(INT->actPoint));//std::cout << " ==++==++== SL_actPoint   ADDS pos" << std::endl; 
	ADDS->nMark->push_back(1);
   	ADDS->n_adds = ADDS->pos.get()->size(); //std::cout << " ==++==++== SL_actPoint   n_adds=" << ADDS->n_adds << std::endl; 
        //INT->points.push_back(INT->actPoint);
        ADDS->millerVs.push_back(INT->millerV);
        ADDS->millerPs.push_back(INT->millerP);
        ADDS->fracts.push_back(INT->fract);
	ADDS->rotTens.push_back(INT->rotTens);
	ADDS->crCNum.push_back(INT->crCNum);
	ADDS->strBV.push_back(ansBurgers);
	scene->displayAdds(true);
	std::cout << " ADDS->n_adds=" << ADDS->n_adds << std::endl;	   
     } // INT->left  && INT->shift	
     mview1->setQuat(rotQuat);
     mview1->setDone(false);
     mode = 1;
 std::cout << " ==++==++== SL_actPoint   setQuat end" << std::endl; 
  }  // mode!=2  
}

void MainWindow::SL_calcDisl()
{
 std::cout << "SL_calcDisl" << std::endl;
  mview1->setDone(true);
  for (int i=0; i<LATT->n_atoms; i++ )  LATT->u.push_back(glm::dvec3(0., 0., 0.));
  QString cd0 = INT->currDir;
  QString cd1 = cd0.append("/data/AAA");
  QFile file(cd1);
  if ( !file.open(QIODevice::WriteOnly|QIODevice::Text) )   std::cout << " ERROR writing " << cd1.toStdString() << std::endl;
  else {//      file.write(editor->toPlainText().toUtf8());
     QString str;
     str.append("Leave only the lines that are necessary\nLines startitng with // are comments\n");
     for ( int i=0; i<ADDS->n_adds; i++ ) {
        glm::dvec3 point = MiscFunc::convert(ADDS->pos.get()->at(i));
	str.append(QString("!! %1, ").arg(i));
        str.append(ADDS->strBV[i]);
	str.append(QString(", %1, %2, %3").arg(point.x, 0, 'f', 8).arg(point.y, 0, 'f', 8).arg(point.z, 0, 'f', 8));
        QString str1 = QString::fromStdString("\n");
        str.append(str1);
     }
     file.write(str.toUtf8());
     file.close();
  }
  SimpleEd *edit = new SimpleEd(cd1);
  connect(edit, SIGNAL(SIG_finished(QString)), this, SLOT(SL_performDislCalc(QString)));
  edit->show();
  //if ( edit->ok ) performDislCalc(cd1);
  //edit->setPlainText(INT->points);
  mview1->setDone(false);
}

void MainWindow::SL_update()
{
 std::cout << "SL_update" << std::endl;
  mview1->setDone(true);
  glm::dvec3 *res;
  res = new glm::dvec3[LATT->n_atoms];
  for (int i=0; i<LATT->n_atoms; i++ ) {
     glm::dvec3 coord = MiscFunc::convert(LATT->coords.get()->at(i));
     glm::dvec3 u0 = LATT->u.at(i);
     res[i] = coord+u0;
  if ( i<10 ) qWarning(" i=%d c=(%f, %f, %f) u0=(%f, %f, %f) res=(%f, %f, %f)", i, coord.x, coord.y, coord.z, u0.x, u0.y, u0.z, res[i].x, res[i].y, res[i].z);
  }
  LATT->coords.get()->clear();
  for (int i=0; i<LATT->n_atoms; i++ ) {
     //glm::dvec3& res = rs;
     LATT->coords->push_back(MiscFunc::convert(res[i]));
     //*(LATT->coords.get())[i] = MiscFunc::convert(res);
  //if ( i<10 ) qWarning(" i=%d c=(%f, %f, %f) u0=(%f, %f, %f) res=(%f, %f, %f)", i, coord.x, coord.y, coord.z, u0.x, u0.y, u0.z, res.x, res.y, res.z);
  }
  delete [] res;//res.clear();
  mview1->setDone(false);  
  scene->displayAdds(false);
  INT->refrAtoms = true;
  INT->showAdds = false;
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat); 
}

void MainWindow::SL_performDislCalc(QString ff)
{
 std::cout << "performDislCalc" << std::endl;
  QFile file(ff);
  if ( !file.open(QIODevice::ReadOnly|QIODevice::Text) )   { std::cout << " ERROR opening " << ff.toStdString() << std::endl; return; }
  QTextStream in(&file);
  int nl = 0;
  while (!in.atEnd()) {
     QString line = in.readLine();
     //++nl;  //std::cout << " nl=" << nl << "   " << line.toStdString() << std::endl;
     if ( line.isEmpty() || (line.at(0)=='/' && line.at(1)=='/') ) continue;
     if ( line.at(0)=='!' && line.at(1)=='!' ) {
        QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	QString BV = fields.at(2); glm::dvec3 point = glm::dvec3(fields.at(3).toDouble(), fields.at(4).toDouble(), fields.at(5).toDouble());
  //std::cout << " BV=" << BV.toStdString() << "  coords=(" << point.x << ", " << point.y << ", " << point.z << ")\n";
        int k = fields.at(1).toInt();
        glm::dvec3 pos = MiscFunc::convert(ADDS->pos.get()->at(k));//MiscFunc::convert
        glm::dmat3 rt = ADDS->rotTens.at(k);
        glm::dmat3 rtInv = glm::transpose(rt);
        glm::dvec3 bV = rt * ADDS->fracts.at(k) * (INT->structList[ADDS->crCNum.at(k)]).c2o * ADDS->millerVs.at(k);
        glm::dvec3 cd = rt * pos;
        double be=sqrt(bV.x*bV.x+bV.y*bV.y);
        double bz = bV.z;
        for (int i=0; i<LATT->n_atoms; i++ ) {
           glm::dvec3 coord1 = rt * MiscFunc::convert(LATT->coords.get()->at(i));
           glm::dvec3 dist1 = coord1 - cd;
           //if ( <
           glm::dvec3 du0 = rtInv * Calc::mixed_u(dist1, be, bz);
           //LATT->du.push_back(du0);//(MiscFunc::convert(du0));
	   (LATT->du)[i] = du0;
	   //glm::dvec3 du1 = LATT->du.at(i);//if ( i<10 ) qWarning(" i=%d du0=(%f, %f, %f), du1=(%f, %f, %f)", i, du0.x, du0.y, du0.z, du1.x, du1.y, du1.z);
        }
	for (int i=0; i<LATT->n_atoms; i++ ) {
           glm::dvec3 u0 = LATT->u.at(i) + LATT->du.at(i);
           //LATT->u.push_back(u0);
	   (LATT->u)[i] = u0;
	   //glm::dvec3 u1 = LATT->u.at(i);
  if ( i<10 ) qWarning(" i=%d  u0=(%f, %f, %f)", i, u0.x, u0.y, u0.z);
        }
     } // line.at(0)=='!' && line.at(1)=='!'
  }
  file.close();
}  

void MainWindow::SL_setAdds()
{
//qWarning("SL_setAdds"); 
  mview1->setDone(true);
  INT->showAdds = !INT->showAdds;
  showAddsAct->setChecked(INT->showAdds);
  mview1->setDone(false);
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_bondsAct()	
{
  std::cout << "SL_bondsAct:  mode=" << mode << std::endl;
}


void MainWindow::SL_atoms()
{
//qWarning("SL_atoms"); 
  mview1->setDone(true);
  QString descr = "";
  QStringList quest, sug, ans;
  QString rF = QString("%1").arg(INT->radFactor, 0, 'f', 3);
  QString aA = QString("%1").arg(INT->alphaAt, 0, 'f', 3);
  quest << "Radius" << "Opacity";
  sug << rF << aA;  //QCoreApplication::processEvents(); //sendPostedEvents();
  QuestionForm1 *qf = new QuestionForm1("Atoms", descr, quest, sug, ans);
  mview1->setDone(false);
  if ( !qf->ok ) return;
  double d = ans.at(0).toDouble();
  delete qf;
  osg::clampBetween(d, 0.05, 1.);
  INT->radFactor = d;
  float f = ans.at(1).toFloat();
  osg::clampBetween(f, 0.f, 1.f);
  INT->alphaAt = f;
  INT->refrAtoms = true;
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat); 
}

void MainWindow::SL_setVect()
{
qWarning("SL_setVect"); 
}

void MainWindow::SL_setAx()
{
//qWarning("SL_setAx");
  mview1->setDone(true);
  QString descr = "";
  QStringList quest, sug, ans;
  QString aL = QString("%1").arg(INT->axL, 0, 'f', 3);
  QString aR = QString("%1").arg(INT->axRad, 0, 'f', 3);
  QString aPr1 = QString("%1").arg(INT->axPr1, 0, 'f', 3);
  QString aPr2 = QString("%1").arg(INT->axPr2, 0, 'f', 3);
  QString aA = QString("%1").arg(INT->alphaAx, 0, 'f', 3);
  quest << "Legth" << "Radius" << "Proportion1" << "Proportion2" << "Opacity";
  sug << aL << aR << aPr1 << aPr2 << aA;

  QuestionForm2 *qf = new QuestionForm2("Axis", descr, quest, sug, ans);
  mview1->setDone(false);
  if ( !qf->ok ) {  delete qf;  return;  }
  INT->axInd = qf->indCombo;
  INT->axL = ans.at(0).toFloat();
  INT->axRad = ans.at(1).toFloat();
  INT->axPr1 = ans.at(2).toFloat();
  INT->axPr2 = ans.at(3).toFloat();
  INT->alphaAx = ans.at(4).toFloat();
  delete qf;
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_visibility()
{
 qWarning("SL_visibility");
  mview1->setDone(true);
  QString descr = "";
  QStringList quest, sug, ans;
  QString atAV = QString("%1").arg(INT->alphaAt, 0, 'f', 3);
  QString atBV = QString("%1").arg(INT->alphaB, 0, 'f', 3);
  QString imV = QString("%1").arg(INT->alphaIm, 0, 'f', 3);
  QString fTV = QString("%1").arg(INT->alphaFemT, 0, 'f', 3);
  QString fLV = QString("%1").arg(INT->alphaFemL, 0, 'f', 3);
  QString fPV = QString("%1").arg(INT->alphaFemP, 0, 'f', 3);
  quest << "Atoms" << "Bonds" << "HRTEM image" << "Fem - triangles" << "Fem - lines" << "Fem - points";
  sug << atAV << atBV << imV << fTV << fLV << fPV;

  QuestionForm1 *qf = new QuestionForm1("Opacity", descr, quest, sug, ans);
  mview1->setDone(false);
  if ( !qf->ok ) {  delete qf;  return;  }
  INT->alphaAt = ans.at(0).toFloat();
  INT->alphaB = ans.at(1).toFloat();
  INT->alphaIm = ans.at(2).toFloat();
  INT->alphaFemT = ans.at(3).toFloat();
  INT->alphaFemL = ans.at(4).toFloat();
  INT->alphaFemP = ans.at(5).toFloat();
  INT->refrImage = true;
  INT->refrFem = true;
  INT->refrAtoms = true;
  INT->refrRes = true;
  delete qf;
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_closeAtoms()
{
  mview1->setDone(true);
  strBvect = strAtom = strCore = iAt = iStr = "";
  INT->atName = "";
  LATT->name = "";
  INT->hklDefined = INT->atomDefined = INT->coreDefined = INT->structureDefined = false;
  ADDS->pos.get()->clear();
  ADDS->nMark.get()->clear();
  ADDS->n_adds = 0;
  LATT->kName.clear();
  LATT->marked.clear();
  LATT->coords.get()->clear();
  LATT->du.clear();
  LATT->u.clear();
  LATT->nAt.get()->clear();
  LATT->nK.get()->clear();
  LATT->bond1.get()->clear();
  LATT->bond2.get()->clear();
  infoDisplay();
  //qWarning(" read_alc_xyz - 1");   
  LATT->xMin = LATT->yMin = LATT->zMin = 1.e30;
  LATT->xMax = LATT->yMax = LATT->zMax = -1.e30;
  LATT->n_atoms = 0;
  // ADDS
  INT->refrAtoms = true;
  osg::Quat q = mview1->getQuat();
  mview1->setDone(false);  //saveSliders();
  refreshScene();//restoreSliders();  
  INT->refrAtoms = false;
  INT->xMin = INT->yMin = INT->zMin = 1e300;
  INT->xMax = INT->yMax = INT->zMax = -1e300;
  mview1->setQuat(q);
  infoLabel->clear();
}

void MainWindow::SL_setBg()
{
//qWarning("SL_setBg"); 
  mview1->setDone(true);
  QColor initCol = MiscFunc::qtColor(INT->bgColor);
  QColorDialog *qc = new QColorDialog(initCol, this);// "Select Color", QColorDialog::DontUseNativeDialog);
  qc->setOption(QColorDialog::ShowAlphaChannel, true);
// The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
 #ifdef Q_WS_MAC
  qc->setOption(QColorDialog::DontUseNativeDialog, true);
 #endif
  mview1->setDone(false);
  if ( qc->exec()==QDialog::Rejected ) return;
  QColor color = qc->selectedColor();
  delete qc;
  if ( color==initCol ) return;
//  QColor color = QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::DontUseNativeDialog);  
  if (color.isValid()) {
      osg::Vec4 col = MiscFunc::osgColor(color);
      INT->bgColor = col;
      osg::Matrixd mat = mview1->getMatrix();
      refreshViewer();
      mview1->setMatrix(mat);
  }
  //mview1->setDone(false);
}

void MainWindow::SL_setLight()
{
//qWarning("SL_setLight"); 
  mview1->setDone(true);
  INT->lightOn = !INT->lightOn;
  lightAct->setChecked(INT->lightOn);
  mview1->setDone(false);
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_setNum()
{
//qWarning("SL_setLight"); 
  mview1->setDone(true);
  INT->showNum = !INT->showNum;
  numAct->setChecked(INT->showNum);
  mview1->setDone(false);
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_setMaterial()
{
//qWarning("SL_setLight"); 
  mview1->setDone(true);
  INT->materialOn = !INT->materialOn;
  materialAct->setChecked(INT->materialOn);
  mview1->setDone(false);
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_setColB()
{
//qWarning("SL_setColB"); 
  mview1->setDone(true);
  QColor initCol = MiscFunc::qtColor(INT->bondColor);
  QColorDialog *qc = new QColorDialog(initCol, this);// "Select Color", QColorDialog::DontUseNativeDialog);
  qc->setOption(QColorDialog::ShowAlphaChannel, true);
// The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
 #ifdef Q_WS_MAC
  qc->setOption(QColorDialog::DontUseNativeDialog, true);
 #endif
  mview1->setDone(false);
  if ( qc->exec()==QDialog::Rejected ) return;
  QColor color = qc->selectedColor();
  delete qc;
  if ( color==initCol ) return;
//  QColor color = QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::DontUseNativeDialog);  
  if (color.isValid()) {
      osg::Vec4 col = MiscFunc::osgColor(color);
      INT->bondColor = col;
      INT->refrAtoms = true;  
      osg::Matrixd mat = mview1->getMatrix();
      refreshScene();
      mview1->setMatrix(mat);
  }
}

void MainWindow::SL_setFov()
{
//qWarning("SL_setFov"); 
  mview1->setDone(true);
  QString descr = "";
  QString quest = "Field of view";
  QString ans;
  QString sug = QString("%1").arg(INT->fov, 0, 'f', 3);
  OneQuestion *oq = new OneQuestion("Field of view", descr, quest, sug, ans);
  mview1->setDone(false);
  if ( !oq->ok ) return;
  double d = ans.toDouble(); //std::cout << "  fov=" << d << std::endl;
  delete oq;
  if ( d==INT->fov ) return;
  if ( d<0.0004 ) d =0.0004;  if ( d>70. ) d = 70.;// osg::clampBetween(d, 0.0004, 70.); 
  INT->fov = d;
  osg::Quat q = mview1->getQuat();
  refreshViewer();
  mview1->setQuat(q);
}

void MainWindow::SL_setBonds()
{
  mview1->setDone(true);
  QString descr = "";
  QString quest, sug, ans;
  sug = QString("%1").arg(INT->bondFactor, 0, 'f', 3);
 
  OneQuestion *oq = new OneQuestion("Size of bonds", descr, quest, sug, ans);
  mview1->setDone(false);
  if ( !oq->ok ) return;
  float f = ans.toFloat();
  delete oq;
  osg::clampBetween(f, 0.f, 0.1f);
  if ( f==INT->bondFactor ) return;
  INT->bondFactor = f;
  INT->refrAtoms = true;  
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_setNS()
{
  mview1->setDone(true);
  QString descr = "";
  QString quest, sug, ans;
  sug = QString("%1").arg(INT->sizeTxt, 0, 'f', 3);
 
  OneQuestion *oq = new OneQuestion("Size of numbers", descr, quest, sug, ans);
  mview1->setDone(false);
  if ( !oq->ok ) return;
  float f = ans.toFloat();
  delete oq;
  //osg::clampBetween(f, 0.f, 0.1f);
  if ( f==INT->sizeTxt ) return;
  INT->sizeTxt = f;
  //INT->refrAtoms = true;  
  osg::Matrixd mat = mview1->getMatrix();
  refreshScene();
  mview1->setMatrix(mat);
}

void MainWindow::SL_openAtoms()
{
  mview1->setDone(true);
  QString cd0 = INT->currDir;//AT->
  QFileDialog *qf = new QFileDialog(this, "Select atoms", cd0.append("/data/atoms"), "Molecules (*.xyz *.alc)");
  qf->exec();                                  
  if ( qf->result()!=1 ) {  mview1->setDone(false);  return;  }
  QString aname1 = qf->selectedFiles().at(0);
  delete qf;
 qWarning("SL_openAtoms aname1  :  %s", aname1.toLatin1().data() );                                                
  if ( aname1.isEmpty() )  {  mview1->setDone(false);  return; }
  if (!(aname1.contains(".xyz")) && !(aname1.contains(".alc")))  {  qWarning ("openAtoms -- unknown file format"); mview1->setDone(false);  return;  }
  else {
      INT->atName = aname1.toStdString();
      readAlcXyz(aname1);
      iAt = QString("   file: %1  %2 atoms  ").arg(aname1.section('/', -1)).arg(LATT->n_atoms+1);
      infoDisplay();
      mview1->setDone(false);
  }
  INT->refrAtoms = true;
  refreshScene();
}

void MainWindow::SL_image()
{
  if ( INT->atName.empty() && INT->compFem.empty() && INT->fem.empty() ) {  QMessageBox::warning(this, "PROBLEM", "No atoms, no fems");  return; }
  mview1->setDone(true);
  QString cd0 = INT->currDir;
  QFileDialog *qf = new QFileDialog(this, "Select HRTEM pictures", cd0.append("/data/images"), "Pictures (*.png *.tif)");
  qf->exec();                 
  if ( qf->result()!=1 ) {  mview1->setDone(false);  return;  }
  QString iname1 = qf->selectedFiles().at(0);
  delete qf;
 qWarning("SL_openImage iname1  :  %s", iname1.toLatin1().data() );                                                
  if ( iname1.isEmpty() )  {  mview1->setDone(false);  return;  }
  mview1->setDone(false);
  INT->image = iname1.toStdString();
  INT->refrImage = true;
  osg::Quat q = mview1->getQuat();
  refreshScene();
  mview1->setQuat(q);
}

void MainWindow::SL_showRes()
{
 if ( INT->results.empty() ) {  QMessageBox::warning(this, "PROBLEM", "No defined fems");  return; }
 bool ok;
 QStringList res1;
 res1 << "None";
 for (int i=0; i<INT->results.size(); i++) res1 << INT->results.at(i);
 QString item = QInputDialog::getItem(this, "Results", "", res1, 0, false, &ok);
 if ( QString::compare(item, "None", Qt::CaseSensitive)==0 )   INT->choosen_value = "";
 else if ( ok && !item.isEmpty()  )                            INT->choosen_value = item.toStdString(); //std::cout << "selected item = " << INT->choosen_value << std::endl;
 INT->refrRes = true;
 refreshScene();
}

void MainWindow::SL_closeImage()
{
  mview1->setDone(true);
  INT->image = "";
  INT->refrImage = false;
  osg::Quat q = mview1->getQuat();
  mview1->setDone(false);
  refreshScene();
  mview1->setQuat(q);
}

void MainWindow::SL_reset()
{
  refreshViewer();
}  

void MainWindow::readAlcXyz(QString aname1)
{
    bool alc_ = aname1.contains(".alc");
    QFile file(aname1);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qWarning("===== ERROR  file %s not found", aname1.toLatin1().data());
       return;
    }
    LATT->name = aname1;
    QTextStream in(&file);
    QString line = in.readLine();
    if ( line.isEmpty() ) {  QMessageBox::warning(this, "PROBLEM", "First line is empty!");  return;  }
    QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    LATT->n_atoms = fields.at(0).toInt();
    if ( alc_ ) LATT->n_bonds = fields.at(2).toInt();
    else {
	LATT->n_bonds = 0;
        line = in.readLine();
    }
 qWarning(" read_alc_xyz  - %s, n_atoms=%d, n_bonds=%d", LATT->name.toLatin1().constData(), LATT->n_atoms, LATT->n_bonds); 
    LATT->n_k = 0;
    for (int i=0; i<LATT->n_atoms; i++) {
       LATT->marked << i;
       LATT->u.push_back(glm::dvec3(0., 0., 0.));
       LATT->du.push_back(glm::dvec3(0., 0., 0.));
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//       int nf = fields.size(); 
       if ( alc_ ) fields.removeFirst();
       QString nameA = fields.takeFirst();
       int ak = MiscFunc::whichAtom(nameA);
       if ( ak<=0 ) { 
          ak = 0;
          QString str;
	  str.sprintf("Unidentified atom nr %d - %s", i+1, nameA.toLatin1().data());
          QMessageBox::warning(this, "PROBLEM", str); 
       }
       if ( LATT->n_k==0 ) { LATT->nK->push_back(ak); LATT->n_k=1; LATT->kName << nameA; }
       bool sw = true;
       for (int j=0; j<LATT->n_k; j++)  if ( ak==LATT->nK.get()->at(j) ) sw = false;
       if ( sw ) { LATT->nK->push_back(ak); ++LATT->n_k; LATT->kName << nameA; }
  //qWarning("   ... i=%d,   ak = %d,  n_k = %d", i, ak, LATT->n_k);
       double x = fields.takeFirst().toDouble();
       double y = fields.takeFirst().toDouble();
       double z = fields.takeFirst().toDouble();
       if ( x<LATT->xMin ) LATT->xMin = x;
       if ( x>LATT->xMax ) LATT->xMax = x;
       if ( y<LATT->yMin ) LATT->yMin = y;
       if ( y>LATT->yMax ) LATT->yMax = y;
       if ( z<LATT->zMin ) LATT->zMin = z;
       if ( z>LATT->zMax ) LATT->zMax = z;
       LATT->coords->push_back( osg::Vec3d(x, y, z));//, static_cast<double>(ak)) );
       LATT->nAt->push_back(ak);
     }
     osg::Vec3d v = osg::Vec3d(0.5*(LATT->xMin+LATT->xMax), 0.5*(LATT->yMin+LATT->yMax), 0.5*(LATT->zMin+LATT->zMax));
     std::cout << " READ : centrum = " << v.x() << ",    " << v.y() << ",    " << v.z() << std::endl;
     LATT->scDim = std::max(LATT->xMax-LATT->xMin, std::max(LATT->yMax-LATT->yMin, LATT->zMax-LATT->zMin));
     if ( alc_ ) {
      
         for (int i=0; i<LATT->n_bonds; i++) {
             line = in.readLine();
             fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts); //int nf = fields.size();
             int i1 = fields.at(1).toInt();
             int i2 = fields.at(2).toInt();
             LATT->bond1->push_back(i1);
             LATT->bond2->push_back(i2);
         }
     }
}
     
void MainWindow::refreshScene()
{
qWarning("refreshScene 0");
    scene = new OsgScene();
    intscene = scene->getScene();//qWarning("refreshScene 1");
    MainLayout->removeWidget(mview1);
    mview1 = new OsgViewerQt(intscene, INT->fov);
    setSliders(0., 0., 0., 1., 0., 0.);//qWarning("refreshScene 2");
    mview1->setMinimumSize(600,800);
    mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);//qWarning("refreshScene 3");
    MainLayout->addWidget(mview1, 1, 0); 
    infoDisplay();
//qWarning("refreshScene 4");

}

void MainWindow::refreshViewer()
{
    mview1 = new OsgViewerQt(intscene, INT->fov);
//    setSliders(valTheta, valPhi, valPsi);
    setSliders(0., 0., 0., 1., 0., 0.);
    mview1->setMinimumSize(600,800);
    mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    MainLayout->addWidget(mview1, 1, 0); 
    infoDisplay();
   //mview1->setIdle(true);
}
/*
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(resetAct);
    menu.exec(event->globalPos());
}
*/

void MainWindow::SL_genAtoms()
{
  //std::cout << "SL_genAtoms" << std::endl;
   if ( !INT->structureDefined ) { QMessageBox::warning(this, "PROBLEM", "You should define structure first");  return; }
   mview1->setDone(true);
   QStringList quest, sug, ans, fields;
   QString aa;
   QString descr = ("<h4>X, Y, Z ranges</h4><br>Cell nr.(0, 0, 0) is at coordinates(0, 0, 0)");
   quest << "Lattice name" << "X begin" << "X end" << "Y begin" << "Y end" << "Z begin" << "Z end" ;
   sug << "" << "0" << "" << "0" << "" << "0" << ""; 
   std::cout << "SL_genAtoms 1" << std::endl;
   QuestionForm1 *qf = new QuestionForm1("Generated structure definition", descr, quest, sug, ans);
   mview1->setDone(false);
   if ( !qf->ok ) return;
   int xb = ans.at(1).toInt(); int xe = ans.at(2).toInt();
   int yb = ans.at(3).toInt(); int ye = ans.at(4).toInt();
   int zb = ans.at(5).toInt(); int ze = ans.at(6).toInt();
   LATT->n_atoms = Gener::genLattice(xb, yb, zb, xe, ye, ze);// std::cout << "n_atoms=" << LATT->n_atoms << std::endl;   
   delete qf;
   iAt = QString("   file: %1  %2 atoms  ").arg(LATT->name).arg(LATT->n_atoms+1);
   LATT->name = ans.at(0);
   infoDisplay();
   INT->atName = LATT->name.toStdString();  //iAt = LATT->name.section('/', -1);
   INT->refrAtoms = true;
   refreshScene();

   return;
}

void MainWindow::SL_chooseSettings()
{
// std::cout << "SLchooseSettings" << std::endl;
  mview1->setDone(true);
  QStringList items;
  items << "Default";
  QString cd0 = INT->currDir;
  cd0.append("/data/internal/");
  QStringList nameFilter("*.sett");
  QDir directory(cd0);
  QStringList items1 = directory.entryList(nameFilter);
  items << items1;
  QString old = "";
  if ( INT->settDefined ) old = INT->sett;
  bool ok;
  QString item = QInputDialog::getItem(this, "Choose settings", "settings:", items, items.size(), false, &ok);
  if ( ok && !items.isEmpty() )  {
  std::cout << "SLchoose      item=" << item.toStdString() << std::endl;
     if ( item.compare(QString("Default"))==0 ) {
        INT->sett = "/data/internal/defaults0.txt";
	if ( INT->settDefined ) { delete AT;  AT = new Atoms();  INT->refrAtoms = true;  }//&& INT->nr!=-1 )
	INT->readDefaults(INT->sett);
	INT->settDefined = false;
	mview1->setDone(false);
        osg::Quat q = mview1->getQuat();
        refreshScene();
        mview1->setQuat(q);
	return;
     } else {
        for (int i=1; i<items.size(); i++) 
           if ( item==items.at(i) ) { 
	     INT->sett = QString("/data/internal/").append(items.at(i));  //std::cout << "Settings  i=" << i << "   sett=" << INT->sett.toStdString() << std::endl;
             INT->refrAtoms = true;
	     INT->settDefined = true;
	     INT->readDefaults(INT->sett);
	     mview1->setDone(false);
             osg::Quat q = mview1->getQuat();
             refreshScene();
             mview1->setQuat(q);
	     return;
           }
     }  // if - else
  }
}

void MainWindow::SL_editSett()
{
 //std::cout << "SL_editSett" << std::endl;
 mview1->setDone(true);
 eee = new SimplePad("/data/internal/", "settings (*.sett)");//std::cout << "1  url=" << INT->url.toStdString() << std::endl;
 eee->show();
 mview1->setDone(false);
 //delete eee;
}

void MainWindow::SL_chooseStructure()
{
  mview1->setDone(true);
  QString cd0 = INT->currDir;
  cd0.append("/data/internal/");
  QStringList nameFilter("*.struct");
  QDir directory(cd0);
  QStringList items = INT->structNames;//directory.entryList(nameFilter);
  bool ok;
  QString item = QInputDialog::getItem(this, "Choose structure", "structure:", items, items.size(), false, &ok);
 //std::cout << " == item == " << item.toStdString() << std::endl;
  if ( ok && !items.isEmpty() )  
     for (int i=0; i<items.size(); i++) { //std::cout << items.at(i).toStdString() << " ==== " << item.toStdString() << std::endl;
        if ( item==items.at(i) ) { 
           INT->crC = &(INT->structList[i]);  
	   INT->crCNum = i;
	   INT->structureDefined = true;
           iStr = QString("       Structure: %1").arg(item);
	   if ( LATT->n_atoms<=0 ) iAt = "  --------------------   ";
	   infoDisplay();
	   //} else {
	     //QMessageBox msg;
           mview1->setDone(false);
           return;
        }
     }
  mview1->setDone(false);
  INT->structureDefined = false;
  return;
}

void MainWindow::SL_editStruct()
{
 //std::cout << "SL_editStruct" << std::endl;
 mview1->setDone(true);
 eee = new SimplePad("/data/internal/", "structures (*.struct)");//std::cout << "1  url=" << INT->url.toStdString() << std::endl;
 eee->show();
 INT->initStructures();
 mview1->setDone(false);
 std::cout << "SL_editStruct ---- end" << std::endl;
 //delete eee;
}

void MainWindow::SL_about()
{
  HelpBrowser::showPage("index.html");
}

void MainWindow::SL_aboutOsg()
{
  HelpBrowser::showPage("osg.html");
}

void MainWindow::keyPressEvent(QKeyEvent *keyEv)
{
  int k = keyEv->key();
  emit SIG_keypress(k);
//  if ( mview1->isActiveWindow() ) qWarning("______1");  else                 qWarning("______ 2");
}

void MainWindow::SL_keypress(int k)
{
  qWarning("--------key=%d", k);
  if ( k==Qt::Key_Escape ) qWarning(" - ESCAPE -");
//  std::cout << "text   " << keyEv->text().toStdString() << std::endl;
}

void MainWindow::SL_openFem()
{
std::cout << "SL_openFem" << std::endl;
  INT->newcalc = true;
  INT->whichFem = 1;
  mview1->setDone(true);
  QString cd0 = INT->currDir;
  QFileDialog *qf = new QFileDialog(this, "Import FEAP data", cd0.append("/data/fems"), "data (*.txt *.o *.i)");
  qf->exec();                 
  if ( qf->result()!=1 ) {  mview1->setDone(false);  return;  }
  QString femname = qf->selectedFiles().at(0);
  delete qf;
 qWarning("SL_openF femname  :  %s", femname.toLatin1().data() );                                                
  if ( femname.isEmpty() )  {  mview1->setDone(false);  return;  }
  INT->fem = femname.toStdString();
  INT->compFem = "";
  INT->refrFem = true;
  mview1->setDone(false);
  osg::Quat q = mview1->getQuat();
  refreshScene();
  mview1->setQuat(q);
  if ( INT->atName.empty() ) refreshViewer();
}

void MainWindow::SL_computeFem()
{
std::cout << "SL_computeFem" << std::endl;
  INT->newcalc = true;
  INT->whichFem = 2;
  mview1->setDone(true);
  QString cd0 = INT->currDir;
  QFileDialog *qf = new QFileDialog(this, "Select extern FEM data", cd0.append("/data/extfems"), "data (*.txt *.o *.i)");
  qf->exec();                 
  if ( qf->result()!=1 ) {  mview1->setDone(false);  return;  }
  QString femname = qf->selectedFiles().at(0);
  delete qf;
 qWarning("SL_computeFem femname  :  %s", femname.toLatin1().data() );                                                
  if ( femname.isEmpty() )  {  mview1->setDone(false);  return;  }
  INT->compFem = femname.toStdString();
  INT->fem = "";
  INT->refrFem = true;
  mview1->setDone(false);
  osg::Quat q = mview1->getQuat();
  refreshScene();
  mview1->setQuat(q);
  if ( INT->atName.empty() ) refreshViewer();
}

void MainWindow::SL_closeFem()
{
std::cout << "SL_closeFem" << std::endl;
  mview1->setDone(true);
  INT->whichFem = 0;
  INT->fem = "";
  INT->compFem = "";
  INT->refrFem = false;
  INT->results.clear();
  if ( INT->atName.empty() ) {
     INT->xMin = INT->yMin = INT->zMin = 1e300;
     INT->xMax = INT->yMax = INT->zMax = -1e300;
  }
  osg::Quat q = mview1->getQuat();
  mview1->setDone(false);
  refreshScene();
  mview1->setQuat(q);
}

void MainWindow::SL_saveAtoms()
{
std::cout << "SL_saveAtoms" << std::endl;
  mview1->setDone(true);
  QString fname;
  SaveDialog *sd = new SaveDialog("File name:");//, "", fname);
  sd->exec();
  if ( sd->ok ) {
     std::cout << "SL_saveAtoms:  fname=" << sd->fileName .toStdString() << std::endl; // if ( sd->ok ) 
     if ( !(sd->fileName.contains(".xyz")) )  {  qWarning ("saveAtoms -- unknown file format"); mview1->setDone(false);  return;  }
     QFile fil(sd->fileName);
     if (!fil.open(QIODevice::ReadWrite)) { 
        qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
        return; 
     }
     QTextStream out(&fil);
     QString line;
     if ( sd->rotated ) {
        rotTens = MiscFunc::quat2rot((mview1->getQuat()*ZtoY).conj());
        for (int i=0; i<LATT->n_atoms; i++) {
            glm::dvec3 pos = MiscFunc::convert(LATT->coords.get()->at(i));
            (*LATT->coords)[i] = MiscFunc::convert(rotTens * pos);
        }
     }
     if ( sd->marked ) {
// std::cout << "------marked" << std::endl;
        out << line.sprintf("%d\n ---\n", LATT->marked.size());
        for (int i=0; i<LATT->marked.size(); i++) {
            int j = LATT->marked.at(i);
            int numbAt = LATT->nAt.get()->at(j);
            osg::Vec3 p1 = LATT->coords.get()->at(j);
            out << line.sprintf("%4s %12.7f %12.7f %12.7f\n", AT->namea.at(numbAt).toLatin1().data(), p1.x(), p1.y(), p1.z());
	}
     } else {               
 std::cout << "------not marked" << std::endl;
        out << line.sprintf("%d\n ---\n", LATT->n_atoms);
        for (int i=0; i<LATT->n_atoms; i++) {
            int numbAt = LATT->nAt.get()->at(i);
            osg::Vec3 p1 = LATT->coords.get()->at(i);
            out << line.sprintf("%4s %12.7f %12.7f %12.7f\n", AT->namea.at(numbAt).toLatin1().data(), p1.x(), p1.y(), p1.z());
	}
     }
     fil.close();
  }
  delete sd;
  mview1->setDone(false);
}
  