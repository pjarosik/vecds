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
#include "../include/mainwindow.h"
#include <glm/gtx/norm.hpp>
#include <iostream>
#include "../include/formats.h"
#include "globals.h"

MainWindow::MainWindow() {
    setMinimumSize(800, 600);
    setContentsMargins(0, 0, 0, 0);
    setAutoFillBackground(true);
    setStyleSheet("border: none");//("background-color:white");    
    setBackgroundRole(QPalette::Base);//Window);//(QColor(255, 255, 255, 255));
    //setForegroundRole(QPalette::Base);//Window);//(QColor(255, 255, 255, 255));
    setWindowTitle("Visual Editor of Crystal Defects");
    MainWidget = new QWidget;
    //MainWidget->setAutoFillBackground(true);//std::cout << "---1" << std::endl;
    //MainWidget->setStyleSheet("margin:0px;" "border:0px;" "padding:0px;");//("background-color:white");    
    //setFocusPolicy(Qt::StrongFocus);
    //grabKeyboard();
    //setFocus();
//qApp->installEventFilter(this); 
    scene = new OsgScene();
    valTh = 0.;
    valPh = 0.;
    valPs = 90.;
    valDist = 1.;
    valCx = 0.0;
    valCy = 0.0;
    strBvect = "";
    strAtom = "";
    strPoint = "";
    iAt = "";
    iStr = "";
    ansBurgers = "";
    ansPlane = "";
    YtoZ.makeRotate(osg::Y_AXIS, osg::Z_AXIS);
    ZtoY.makeRotate(osg::Z_AXIS, osg::Y_AXIS);

    mview1 = new OsgViewerQt(scene, INT->fov);
    mview1->setMinimumSize(600, 400);
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
    if(!INT->atName.empty()) {
        readInputFile(QString::fromStdString(INT->atName));
        INT->refrAtoms = true;
        refreshScene();
    }
    mview1->setKeyEventSetsDone(0);
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "=========== B E G I N ===========    MainWindow created at " << dateTimeString
                << "   ========== B E G I N =============" << std::endl;
    INT->calcLog << "=========== B E G I N ===========    MainWindow created at " << dateTimeString
                 << "    ========== B E G I N =============" << std::endl;
    mode = 0;
    moreFiles = false;
    screw = false;
    refreshViewer();
    //qApp->processEvents();
}

MainWindow::~MainWindow() {
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "===========  E N D ===========    MainWindow destructed at " << dateTimeString
                << "    ========== E N D =============" << std::endl;
    INT->outLog.close();
    INT->calcLog << "===========  E N D ===========    MainWindow destructed  at " << dateTimeString
                 << "    ========== E N D =============" << std::endl;
    INT->calcLog.close();
    // TODO avoid using globals
    AT.reset();
    LATT.reset();
    INT.reset();
}

void MainWindow::createActions() {
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
    atomsAct->setStatusTip(tr("Set visibility"));
    connect(atomsAct, SIGNAL (triggered()), this, SLOT(SL_atoms()));

    bgAct = new QAction("Bacground", this);
    bgAct->setStatusTip(tr("Set background"));
    connect(bgAct, SIGNAL (triggered()), this, SLOT(SL_setBg()));

    numAct = new QAction("Turn on numbers", this);
    numAct->setCheckable(true);
    numAct->setChecked(INT->showNum);
    numAct->setStatusTip(tr("Set numbers"));
    connect(numAct, SIGNAL (triggered()), this, SLOT(SL_setNum()));

    lightAct = new QAction("Turn on light", this);
    lightAct->setCheckable(true);
    lightAct->setChecked(INT->lightOn);
    lightAct->setStatusTip(tr("Set lighting"));
    connect(lightAct, SIGNAL (triggered()), this, SLOT(SL_setLight()));

    materialAct = new QAction("Turn on material properties", this);
    materialAct->setCheckable(true);
    materialAct->setChecked(INT->materialOn);
    materialAct->setStatusTip(tr("Set material"));
    connect(materialAct, SIGNAL (triggered()), this, SLOT(SL_setMaterial()));

    showPointsAct = new QAction("Turn on showing points", this);
    showPointsAct->setCheckable(true);
    showPointsAct->setChecked(INT->showPoints);
    showPointsAct->setStatusTip(tr("Set points"));
    connect(showPointsAct, SIGNAL (triggered()), this, SLOT(SL_setPoints()));

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

// Open a file containing definitions of atomic structure
    openAct = new QAction(tr("Open atoms"), this);
    openAct->setStatusTip(tr("Opens an existing file as .xyz, .alc or sybyl mol2"));
    connect(openAct, SIGNAL (triggered()), this, SLOT(SL_openAtoms()));

    closeAct = new QAction(tr("Close atoms"), this);
    closeAct->setStatusTip(tr("Closes an existing file"));
    connect(closeAct, SIGNAL (triggered()), this, SLOT(SL_closeAtoms()));

    openFemAct = new QAction(tr("Import external FE data"), this);
    //    openimgAct->setShortcut(tr("Ctrl+I"));
    openFemAct->setStatusTip(tr("Imports external FE"));
    connect(openFemAct, SIGNAL(triggered()), this, SLOT(SL_openFem()));

    computeFemAct = new QAction(tr("Computes external FE data"), this);
    computeFemAct->setStatusTip(tr("Computes external FE"));
    connect(computeFemAct, SIGNAL(triggered()), this, SLOT(SL_computeFem()));

    showResAct = new QAction(tr("Show FE results"), this);
    showResAct->setStatusTip(tr("Shows external FE"));
    connect(showResAct, SIGNAL(triggered()), this, SLOT(SL_showRes()));

    closeFemAct = new QAction(tr("Close FE"), this);
    //    closeimgAct->setShortcut(tr("Ctrl+X"));
    closeFemAct->setStatusTip(tr("Close FE"));
    connect(closeFemAct, SIGNAL(triggered()), this, SLOT(SL_closeFem()));

    // Open a fem file
    imageAct = new QAction(tr("Open HRTEM image"), this);
    imageAct->setStatusTip(tr("Opens an image"));
    connect(imageAct, SIGNAL(triggered()), this, SLOT(SL_image()));

    closeimgAct = new QAction(tr("Close image"), this);
    closeimgAct->setStatusTip(tr("Closes an existing img"));
    connect(closeimgAct, SIGNAL(triggered()), this, SLOT(SL_closeImage()));

    saveAct = new QAction(tr("Save atoms"), this);
    //    saveAsAct->setShortcut(tr("Ctrl+W"));
    saveAct->setStatusTip(tr("Saves existing coordinates"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(SL_saveAtoms()));

    planeAct = new QAction(tr("Plane defined by Miller indices"), this);
    planeAct->setStatusTip("Plane defined by Miller indices");
    connect(planeAct, SIGNAL(triggered()), this, SLOT(SL_plane()));

    togglemarkAct = new QAction(tr("Toggle marked atoms"), this);
    togglemarkAct->setStatusTip("Toggles marked atoms");
    connect(togglemarkAct, SIGNAL(triggered()), this, SLOT(SL_togglemark()));

    cubBoxAct = new QAction(tr("Rectangular box"), this);
    cubBoxAct->setStatusTip(tr("Rectangular bounding box"));
    connect(cubBoxAct, SIGNAL(triggered()), this, SLOT(SL_rectBox()));

    hexBoxAct = new QAction(tr("Hexagonal box"), this);
    hexBoxAct->setStatusTip(tr("Hexagonal bounding box"));
    connect(hexBoxAct, SIGNAL(triggered()), this, SLOT(SL_hexBox()));

    rombBoxAct = new QAction(tr("Romboidal box"), this);
    rombBoxAct->setStatusTip(tr("Romboidal bounding box"));
    connect(rombBoxAct, SIGNAL(triggered()), this, SLOT(SL_rombBox()));

    cylindAct = new QAction(tr("Cylinder"), this);
    hexBoxAct->setStatusTip(tr("Cyliner bounding box"));
    connect(cylindAct, SIGNAL(triggered()), this, SLOT(SL_cylinder()));

    MillerAct = new QAction(tr("Rotation by Miller indices"), this);
    //    MillerAct->setShortcut(tr("Ctrl+M"));
    MillerAct->setStatusTip(tr("input Miller indices"));
    connect(MillerAct, SIGNAL(triggered()), this, SLOT(SL_millerAct()));

    dislocAct = new QAction(tr("Input single dislocation"), this);
    dislocAct->setStatusTip(tr("input Miller indices"));
    connect(dislocAct, SIGNAL(triggered()), this, SLOT(SL_dislocAct()));

    calcDislAct = new QAction(tr("Calculate displacement field for dislocations"), this);
    calcDislAct->setStatusTip(tr("Calculate displacement field for dislocations"));
    connect(calcDislAct, SIGNAL(triggered()), this, SLOT(SL_calcDisl()));

    updateAct = new QAction("Add displacement field to coordinates", this);
    updateAct->setStatusTip("Add displacement field to coordinates");
    connect(updateAct, SIGNAL(triggered()), this, SLOT(SL_update()));

    moveAct = new QAction("Move atoms", this);
    moveAct->setStatusTip("Move atoms");
    connect(moveAct, SIGNAL(triggered()), this, SLOT(SL_move()));

    editSettAct = new QAction("Edit settings", this);
    editSettAct->setStatusTip("Edit settings");
    connect(editSettAct, SIGNAL(triggered()), this, SLOT(SL_editSett()));

    chooseSettAct = new QAction("choose settings", this);
    chooseSettAct->setStatusTip("choose settings");
    connect(chooseSettAct, SIGNAL(triggered()), this, SLOT(SL_chooseSettings()));

// About box for vecds
    aboutAct = new QAction(tr("About"), this);
    aboutAct->setStatusTip(tr("Show the vecds application's about box"));
    connect(aboutAct, SIGNAL (triggered()), this, SLOT (SL_about()));

// About box for Qt
    aboutQtAct = new QAction(tr("About Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's about box"));
    connect(aboutQtAct, SIGNAL (triggered()), qApp, SLOT (aboutQt()));

    aboutOsgAct = new QAction(tr("About Osg"), this);
    aboutOsgAct->setStatusTip(tr("Show the Osg library's about box"));
    connect(aboutOsgAct, SIGNAL (triggered()), this, SLOT (SL_aboutOsg()));

    visibAct = new QAction("Visibility", this);
    visibAct->setStatusTip("Opacity of atoms, image and FEs");
    connect(visibAct, SIGNAL(triggered()), this, SLOT(SL_visibility()));

    connect(this, SIGNAL(SIG_keypress(int)), this, SLOT(SL_keypress(int)));
    //qApp->processEvents();
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu("File");

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

    displayMenu = menuBar()->addMenu("Display");
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
    displayMenu->addAction(showPointsAct);
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

    boxMenu = toolsMenu->addMenu("Mark atoms");
    boxMenu->addAction(planeAct);
    boxMenu->addSeparator();
    boxMenu->addAction(cubBoxAct);
    boxMenu->addSeparator();
    boxMenu->addAction(hexBoxAct);
    boxMenu->addSeparator();
    boxMenu->addAction(rombBoxAct);
    boxMenu->addSeparator();
    boxMenu->addAction(cylindAct);

    toolsMenu->addSeparator();
    toolsMenu->addAction(togglemarkAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(moveAct);

    settMenu = menuBar()->addMenu("Settings");
    settMenu->addAction(chooseSettAct);
    settMenu->addSeparator();
    settMenu->addAction(editSettAct);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutOsgAct);
    qApp->processEvents();
    //qWarning("mainw - createMenus O.K.");
}

void MainWindow::createDockWindows() {
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
    Lay_v_PsiSlid->addWidget(Lab_PsiSlid);
    (INT->bgColor);
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

void MainWindow::infoDisplay() {
    infoLabel->text().clear();
    infoLabel->setText(QString("%1  %2  %3").arg(QString("VECDS "), iAt, iStr));
}

void MainWindow::keyPressEvent(QKeyEvent *keyEv) {
    if(keyEv->key() == Qt::Key_Escape) qWarning(" - ESCAPE -");
    int k = keyEv->key();
    std::cout << "keyPressEvent -   text:" << keyEv->text().toStdString() << "    k=" << k << std::endl;
    //keyEv->ignore();
    keyEv->accept();
    emit SIG_keypress(k);
//  if ( mview1->isActiveWindow() ) qWarning("______1");  else                 qWarning("______ 2");
}

void MainWindow::SL_keypress(int k) {
    qWarning("---- SL_keypress ----key=%d", k);
    if(k == Qt::Key_Escape) qWarning(" - ESCAPE -");
}

void MainWindow::SL_actPoint() {
    glm::dvec3 pos = INT->actPoint;
    int nA = INT->nA = Calc::identAtom(pos); //qApp->processEvents();
    INT->outLog << "SL_actpoint  mode=" << mode << "   nA=" << nA << std::endl;
    if(nA >= 0) {
        INT->atomDefined = true;
        strAtom = QString(" atom  %1 - %2").arg(nA + 1).arg(AT->namea.at(LATT->nAt.at(nA)));
        pos = LATT->coords[nA];
        strPoint = QString(" coordinates = { %1, %2, %3 }")
                        .arg(pos.x, 0, 'f', 4)
                        .arg(pos.y, 0, 'f', 4)
                        .arg(pos.z, 0, 'f', 4);
        INT->outLog << " mode=" << mode << strAtom.toStdString() << strPoint.toStdString() << std::endl;
        if(INT->right && mode < 2) { // info
            mview1->setDone(true);
            QMessageBox::information(this, strAtom, strPoint);
        }
        if(mode >= 2 && mode <= 7) {
            if(!INT->hklDefined) {
                QMessageBox::warning(this, "PROBLEM", "You should define orientation of the lattice");
                return;
            }
            mview1->setDone(true);
            if(INT->left && INT->ctrl) {
                // if ( INT->atomDefined ) scene->displayPlane(rotTens, -1);
                scene->displayPlane(rotTens, nA);
                scene->showOneAtom(nA);
                statusBar()->clearMessage();
                statusBar()->showMessage(QString("%1 %2").arg(strBvect, strAtom));
            } //  INT->ctrl && INT->left
            if(INT->left && INT->shift) {
                if(QMessageBox::Yes == QMessageBox(QMessageBox::Information,
                                                   "WARNING",
                                                   "Do you want to mark this atom?",
                                                   QMessageBox::Yes | QMessageBox::No).exec())
                    INT->pointDefined = true;
                else INT->pointDefined = false;
                process(pos);
            } // INT->left && INT->shift
        } // mode>=2 && mode<=6
    } else {  // nA<0
        if(mode >= 2 && mode <= 7 && INT->atomDefined && INT->left && INT->shift) {
            mview1->setDone(true);
            pos = INT->actPoint;
            process(pos);
        }
    }
    mview1->setDone(false);
}

void MainWindow::process(glm::dvec3 pos) {
    if(mode == 2) {
        if(INT->atomDefined || INT->pointDefined) {
            statusBar()->clearMessage();
            qApp->processEvents();
            statusBar()->showMessage(QString("%1 %2 %3").arg(strBvect, strAtom, strPoint));
        }
        addPOINTS();
        scene->displayBvect(true);//std::cout << " POINTS->n_points=" << POINTS->n_points << std::endl;
    } //mode==2
    if(mode == 3) {
        glm::dvec3 point = INT->pointDefined ? LATT->coords[INT->nA] : INT->actPoint;
        cutPlane(point);    //
        INT->refrAtoms = true;
        statusBar()->clearMessage();//showMessage("AAA");
        QString qs = QString("Plane %1 at point (%2, %3, %4)")
                .arg(ansPlane)
                .arg(INT->actPoint.x, 0, 'f', 4)
                .arg(INT->actPoint.y, 0, 'f', 4)
                .arg(INT->actPoint.z, 0, 'f', 4);
        statusBar()->showMessage(qs);
        INT->outLog << qs.toStdString() << std::endl;
        scene->displayMarked();
    } //mode==3
    if(mode >= 4) {
        cutBox();
        statusBar()->clearMessage();
        QString qs;
        if(mode == 4)
            qs = QString("Rectangular box  size X=%1, size Y=%2, size Z=%3 at point (%4, %5, %6)").arg(sizeB.x,
                                                                                                       0,
                                                                                                       'f',
                                                                                                       4).arg(sizeB.y,
                                                                                                              0,
                                                                                                              'f',
                                                                                                              4).arg(
                            sizeB.z,
                            0,
                            'f',
                            4)
                    .arg(pos.x, 0, 'f', 4).arg(pos.y, 0, 'f', 4).arg(pos.z, 0, 'f', 4);
        if(mode == 5)
            qs = QString("Hexagonal box  length=%1, size Z=%2 at point (%3, %4, %5)").arg(sizeB.x,
                                                                                          0,
                                                                                          'f',
                                                                                          4).arg(sizeB.z,
                                                                                                 0,
                                                                                                 'f',
                                                                                                 4).arg(pos.x,
                                                                                                        0,
                                                                                                        'f',
                                                                                                        4).arg(pos.y,
                                                                                                               0,
                                                                                                               'f',
                                                                                                               4).arg(pos.z,
                                                                                                                      0,
                                                                                                                      'f',
                                                                                                                      4);
        if(mode == 6)
            qs = QString("Romboidal box  length=%1, size Z=%2 at point (%3, %4, %5)").arg(sizeB.x,
                                                                                          0,
                                                                                          'f',
                                                                                          4).arg(sizeB.z,
                                                                                                 0,
                                                                                                 'f',
                                                                                                 4).arg(pos.x,
                                                                                                        0,
                                                                                                        'f',
                                                                                                        4).arg(pos.y,
                                                                                                               0,
                                                                                                               'f',
                                                                                                               4).arg(pos.z,
                                                                                                                      0,
                                                                                                                      'f',
                                                                                                                      4);
        if(mode == 7)
            qs = QString("Cylinder  radius=%1, size Z=%2 at point (%3, %4, %5)").arg(sizeB.x,
                                                                                     0,
                                                                                     'f',
                                                                                     4).arg(sizeB.z,
                                                                                            0,
                                                                                            'f',
                                                                                            4).arg(pos.x,
                                                                                                   0,
                                                                                                   'f',
                                                                                                   4).arg(pos.y,
                                                                                                          0,
                                                                                                          'f',
                                                                                                          4).arg(pos.z,
                                                                                                                 0,
                                                                                                                 'f',
                                                                                                                 4);
        INT->outLog << qs.toStdString() << std::endl;
        statusBar()->showMessage(qs);
        scene->displayMarked();
    } //mode>=4
    mview1->setQuat(rotQuat);
    mview1->setDone(false);
    mode = 0;
    //qApp->processEvents();

}

void MainWindow::SL_millerAct() {
    mode = 1;
    INT->outLog << "SL_millerAct:  mode=" << mode << "    ";
    mview1->setDone(true);
    if(!INT->structureDefined) {
        QMessageBox::warning(this, "PROBLEM", "You should define structure first");
        mview1->setDone(false);
        return;
    }
    QString descr = "Miller indices";
    QString quest = "[hkl](uvw)";
    QString sug = "[100](001)";
    if(!ansBurgers.isEmpty()) sug = ansBurgers;
    OneQuestion *oq = new OneQuestion("Rotation parameters", descr, quest, sug, ansBurgers);
    mview1->setDone(false);
    if(!oq->ok) return;
    INT->outLog << "   ansBurgers=" << ansBurgers.toStdString() << std::endl;
    if(Calc::parse_miller(ansBurgers.toStdString())) {
        //MiscFunc::printVec("millerV", INT->millerV);
        INT->hklDefined = true;
        INT->atomDefined = false;
        strBvect = QString("Burger's vector: %1       ").arg(ansBurgers);
        strAtom = "";
        strPoint = "";
        rotTens = Calc::rotation_tensor(INT->millerV, INT->millerP);//  glm::dmat3 rotInv = glm::transpose(rotTens);
        INT->rotTens = rotTens;
        glm::dquat qq = glm::quat_cast(rotTens);
        osg::Quat qq1 = osg::Quat(qq.x, qq.y, qq.z, qq.w);
        rotQuat = qq1.conj() * YtoZ;
        refreshScene();
        mview1->setQuat(rotQuat);
    }
}

void MainWindow::SL_dislocAct() {
    mode = 2;
    INT->outLog << "SL_dislocAct:  mode=" << mode << "    ";
    mview1->setDone(true);
    if(!INT->structureDefined) {
        QMessageBox::warning(this, "PROBLEM", "You should define structure first");
        mview1->setDone(false);
        return;
    }
    ++(INT->ndisl);
    QString descr = "";
    QStringList quest, sug, ans, combo;
    QString name = QString("disloc. nr %1").arg(INT->ndisl);
    QString bv = "[100](001)";
    if(!ansBurgers.isEmpty()) bv = ansBurgers;
    quest << "Name" << "Burger's vector" << "point X" << "point Y" << "point Z";
    sug << name << bv << "" << "" << "";
    combo << "classic" << "newton" << "powell";
    QuestionForm2 *qf = new QuestionForm2("Dislocation", descr, quest, sug, ans, combo);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        --(INT->ndisl);
        return;
    }//qApp->processEvents();
    strName = ans.at(0);
    ansBurgers = ans.at(1);
    numMeth = qf->indCombo;
    INT->outLog << "  name=" << strName.toStdString() << " Burger's vector=" << ansBurgers.toStdString()
                << "  method : " << combo.at(numMeth).toStdString() << std::endl;
    if(Calc::parse_miller(ansBurgers.toStdString())) {  //MiscFunc::printVec("millerV", INT->millerV);
        strBvect = QString("Burger's vector: %1       ").arg(ansBurgers);
        strAtom = "";
        strPoint = "";
        INT->atomDefined = false;
        //INT->hklDefined = true;
        if(INT->hklDefined) INT->rotTens = rotTens = Calc::rotation_tensor(INT->millerV, INT->millerP);
        else {
            screw = true;
            mview1->setDone(true);
            QString descr = "Miller indices";
            QString quest = "[hkl]";
            QString sug = "";
            QString ans1;
            OneQuestion *oq = new OneQuestion("Additional rotation parameters", descr, quest, sug, ans1);
            if(!oq->ok) return;
            direction = Calc::plane1(ans1.toStdString());
            INT->rotTens = rotTens = Calc::rotation_tensor(direction, INT->millerP);
            INT->hklDefined = true;
        }
        glm::dquat qq = glm::quat_cast(rotTens);
        osg::Quat qq1 = osg::Quat(qq.x, qq.y, qq.z, qq.w);
        rotQuat = qq1.conj() * YtoZ;
        mview1->setDone(false);
        refreshScene();
        mview1->setQuat(rotQuat);
        //mview1->setDone(false);
        statusBar()->clearMessage();//showMessage("AAA");
        statusBar()->showMessage(strBvect);
    } // else { ERROR
    if(!ans.at(2).isEmpty() && !ans.at(3).isEmpty() && !ans.at(4).isEmpty()) {
        mview1->setDone(true);
        INT->actPoint = glm::dvec3(ans.at(2).toDouble(), ans.at(3).toDouble(), ans.at(4).toDouble());
        INT->pointDefined = true;
        strPoint = QString("  point coordinates = { %1, %2, %3 }").arg(INT->actPoint.x, 0, 'f', 4).arg(INT->actPoint.y,
                                                                                                       0,
                                                                                                       'f',
                                                                                                       4).arg(INT->actPoint.z,
                                                                                                              0,
                                                                                                              'f',
                                                                                                              4);
        INT->outLog << strPoint.toStdString() << std::endl;
        statusBar()->clearMessage();
        statusBar()->showMessage(QString("%1  *****  %2").arg(strBvect, strPoint));
        addPOINTS();
        screw = false;
        scene->displayBvect(true);//std::cout << " POINTS->n_points=" << POINTS->n_points << std::endl;
        mview1->setDone(false);
        mview1->setQuat(rotQuat);
        mode = 0;
    }
}

void MainWindow::SL_plane() {
    mode = 3;
    INT->outLog << "SL_plane:  mode=" << mode << "    ";
    if(!INT->structureDefined) {
        QMessageBox::warning(this, "PROBLEM", "You should define structure first");
        return;
    }
    mview1->setDone(true);
    ++(INT->npoints);
    QString descr = "";
    QStringList quest, sug, ans;
    QString name = QString("point nr %1").arg(INT->npoints);
    QString mill = "(001)";
    if(!ansPlane.isEmpty()) mill = ansPlane;
    quest << "Name" << "Miller's indices for plane" << "point X" << "point Y" << "point Z";
    sug << name << mill << "" << "" << "";
    QuestionForm1 *qf = new QuestionForm1("Plane", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        --(INT->npoints);
        return;
    }
    name = ans.at(0);
    ansPlane = ans.at(1);
    INT->outLog << "   name=" << name.toStdString() << " Plane=" << ansPlane.toStdString() << std::endl;
    INT->plane = Calc::plane1(ansPlane.toStdString());
    INT->hklDefined = true;
    qApp->processEvents();
    INT->atomDefined = false;
    rotTens = MiscFunc::quat2rot((mview1->getQuat() * ZtoY).conj());
    INT->rotTens = rotTens;
    glm::dquat qq = glm::quat_cast(rotTens);
    osg::Quat qq1 = osg::Quat(qq.x, qq.y, qq.z, qq.w);
    rotQuat = qq1.conj() * YtoZ;
    statusBar()->clearMessage();
    statusBar()->showMessage(QString("Plane %1").arg(ansPlane));
    if(!ans.at(2).isEmpty() && !ans.at(3).isEmpty() && !ans.at(4).isEmpty()) {
        INT->actPoint = glm::dvec3(ans.at(2).toDouble(),
                                   ans.at(3).toDouble(),
                                   ans.at(4).toDouble()); //std::cout << "INT->actPoint " << INT->actPoint.x << "     " << INT->actPoint.y << std::endl;
        cutPlane(INT->actPoint);
        statusBar()->clearMessage();
        QString qs =
                QString("Plane %1 at point (%2, %3, %4)").arg(ansPlane).arg(ans.at(2)).arg(ans.at(3)).arg(ans.at(4));
        statusBar()->showMessage(qs);
        INT->outLog << qs.toStdString() << std::endl;
        INT->refrAtoms = true;
        refreshScene();
        mview1->setQuat(rotQuat);
        mview1->setDone(false);
        mode = 0;
    }
}

void MainWindow::SL_rectBox() {
    mode = 4;
    INT->outLog << "SL_rectBox:  mode=" << mode << "    ";
    mview1->setDone(true);
    ++(INT->npoints);
    QString descr = "";
    QStringList quest, sug, ans;
    QString name = QString("point nr %1").arg(INT->npoints);
    QString mill = "(001)";
    if(!ansPlane.isEmpty()) mill = ansPlane;
    quest << "Name" << "point X" << "point Y" << "point Z" << "size X" << "size Y" << "size Z";
    sug << name << "" << "" << "" << "" << "" << "";
    QuestionForm1 *qf = new QuestionForm1("Rectangular box", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        --(INT->npoints);
        return;
    }
    QString qs =
            QString("Rectangular box  size X=%1, size Y=%2, size Z=%3").arg(ans.at(4)).arg(ans.at(5)).arg(ans.at(6));
    INT->outLog << "name=" << name.toStdString() << " --- " << qs.toStdString() << std::endl;
    INT->hklDefined = true;
    qApp->processEvents();
    INT->atomDefined = false;
    rotQuat = mview1->getQuat();
    INT->rotTens = rotTens = MiscFunc::quat2rot((rotQuat * ZtoY).conj());
    sizeB = glm::dvec3(ans.at(4).toDouble(), ans.at(5).toDouble(), ans.at(6).toDouble());
    statusBar()->clearMessage();
    statusBar()->showMessage(QString("Rectangular box  size X=%1, size Y=%2, size Z=%3").arg(ans.at(4)).arg(ans.at(5)).arg(
            ans.at(6)));
    if(!ans.at(1).isEmpty() && !ans.at(2).isEmpty() && !ans.at(3).isEmpty()) {
        INT->actPoint = glm::dvec3(ans.at(1).toDouble(), ans.at(2).toDouble(), ans.at(3).toDouble());
        std::cout << "INT->actPoint " << INT->actPoint.x << "     " << INT->actPoint.y << std::endl;
        cutBox();
        statusBar()->clearMessage();
        QString qs =
                QString("Rectangular box  size X=%1, size Y=%2, size Z=%3 at point (%4, %5, %6)").arg(ans.at(4)).arg(ans.at(
                        5)).arg(ans.at(6)).arg(ans.at(1)).arg(ans.at(2)).arg(ans.at(3));
        statusBar()->showMessage(qs);
        INT->outLog << qs.toStdString() << std::endl;
        INT->refrAtoms = true;
        refreshScene();
        mview1->setQuat(rotQuat);
        mview1->setDone(false);
        mode = 0;
    }
}

void MainWindow::SL_hexBox() {
    mode = 5;
    INT->outLog << "SL_hexBox:  mode=" << mode << "    ";
    mview1->setDone(true);
    ++(INT->npoints);
    QString descr = "";
    QStringList quest, sug, ans;
    QString name = QString("point nr %1").arg(INT->npoints);
    quest << "Name" << "point X" << "point Y" << "point Z" << "length" << "size Z";
    sug << name << "" << "" << "" << "" << "" << "";
    QuestionForm1 *qf = new QuestionForm1("Hexagonal box", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        --(INT->npoints);
        return;
    }
    INT->hklDefined = true;//qApp->processEvents();
    INT->atomDefined = false;
    rotQuat = mview1->getQuat();
    INT->rotTens = rotTens = MiscFunc::quat2rot((rotQuat * ZtoY).conj());
    sizeB = glm::dvec3(ans.at(4).toDouble(), 0., ans.at(5).toDouble());
    statusBar()->clearMessage();
    QString qs = QString("Hexagonal box  length=%1, size Z=%2").arg(ans.at(4)).arg(ans.at(5));
    INT->outLog << "name=" << name.toStdString() << " --- " << qs.toStdString() << std::endl;
    statusBar()->showMessage(qs);
    if(!ans.at(1).isEmpty() && !ans.at(2).isEmpty() && !ans.at(3).isEmpty()) {
        INT->actPoint = glm::dvec3(ans.at(1).toDouble(), ans.at(2).toDouble(), ans.at(3).toDouble());
        cutBox();
        statusBar()->clearMessage();
        QString qs =
                QString("Hexagonal box  length=%1, size Z=%2 at point (%3, %4, %5)").arg(ans.at(4)).arg(ans.at(5)).arg(
                        ans.at(1)).arg(ans.at(2)).arg(ans.at(3));
        statusBar()->showMessage(qs);
        INT->outLog << qs.toStdString() << std::endl;
        INT->refrAtoms = true;
        refreshScene();
        mview1->setQuat(rotQuat);
        mview1->setDone(false);
        mode = 0;
    }
}

void MainWindow::SL_rombBox() {
    mode = 6;
    INT->outLog << "SL_rombBox:  mode=" << mode << "    ";
    mview1->setDone(true);
    ++(INT->npoints);
    QString descr = "";
    QStringList quest, sug, ans;
    QString name = QString("point nr %1").arg(INT->npoints);
    quest << "Name" << "point X" << "point Y" << "point Z" << "length" << "size Z";
    sug << name << "" << "" << "" << "" << "";
    QuestionForm1 *qf = new QuestionForm1("Romboidal box", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        --(INT->npoints);
        return;
    }
    INT->hklDefined = true;//qApp->processEvents();
    INT->atomDefined = false;
    rotQuat = mview1->getQuat();
    INT->rotTens = rotTens = MiscFunc::quat2rot((rotQuat * ZtoY).conj());
    sizeB = glm::dvec3(ans.at(4).toDouble(), 0., ans.at(5).toDouble());
    statusBar()->clearMessage();
    QString qs = QString("Romboidal box  length=%1, size Z=%2").arg(ans.at(4)).arg(ans.at(5));
    statusBar()->showMessage(qs);
    INT->outLog << "name=" << name.toStdString() << " --- " << qs.toStdString() << std::endl;
    if(!ans.at(1).isEmpty() && !ans.at(2).isEmpty() && !ans.at(3).isEmpty()) {
        INT->actPoint = glm::dvec3(ans.at(1).toDouble(), ans.at(2).toDouble(), ans.at(3).toDouble());
        cutBox();
        statusBar()->clearMessage();
        QString qs =
                QString("Romboidal box  length=%1, size Z=%2 at point (%3, %4, %5)").arg(ans.at(4)).arg(ans.at(5)).arg(
                        ans.at(1)).arg(ans.at(2)).arg(ans.at(3));
        statusBar()->showMessage(qs);
        INT->outLog << qs.toStdString() << std::endl;
        INT->refrAtoms = true;
        refreshScene();
        mview1->setQuat(rotQuat);
        mview1->setDone(false);
        mode = 0;
    }
}

void MainWindow::SL_cylinder() {
    mode = 7;
    INT->outLog << "SL_cylinder:  mode=" << mode << "    ";
    mview1->setDone(true);
    ++(INT->npoints);
    QString descr = "";
    QStringList quest, sug, ans;
    QString name = QString("point nr %1").arg(INT->npoints);
    quest << "Name" << "point X" << "point Y" << "point Z" << "radius" << "size Z";
    sug << name << "" << "" << "" << "" << "";
    QuestionForm1 *qf = new QuestionForm1("Cylinder", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        --(INT->npoints);
        return;
    }
    INT->hklDefined = true;//qApp->processEvents();
    INT->atomDefined = false;
    rotQuat = mview1->getQuat();
    INT->rotTens = rotTens = MiscFunc::quat2rot((rotQuat * ZtoY).conj());
    sizeB = glm::dvec3(ans.at(4).toDouble(), 0., ans.at(5).toDouble());
    statusBar()->clearMessage();
    QString qs = QString("Cylinder  radius=%1, size Z=%2").arg(ans.at(4)).arg(ans.at(5));
    statusBar()->showMessage(qs);
    INT->outLog << "name=" << name.toStdString() << " --- " << qs.toStdString() << std::endl;
    if(!ans.at(1).isEmpty() && !ans.at(2).isEmpty() && !ans.at(3).isEmpty()) {
        INT->actPoint = glm::dvec3(ans.at(1).toDouble(), ans.at(2).toDouble(), ans.at(3).toDouble());
        cutBox();
        statusBar()->clearMessage();
        QString qs =
                QString("Cylinder  radius=%1, size Z=%2 at point (%3, %4, %5)").arg(ans.at(4)).arg(ans.at(5)).arg(ans.at(
                        1)).arg(ans.at(2)).arg(ans.at(3));
        statusBar()->showMessage(qs);
        INT->outLog << qs.toStdString() << std::endl;
        INT->refrAtoms = true;
        refreshScene();
        mview1->setQuat(rotQuat);
        mview1->setDone(false);
        mode = 0;
    }
}

void MainWindow::cutPlane(glm::dvec3 point) {
    glm::dvec3 c1 = glm::normalize(INT->plane * INT->crC->o2c);
    double dd = glm::dot(c1, point);
    LATT->marked.clear();
    for(int i = 0; i < LATT->nAt.size(); i++) LATT->marked[i] = ((glm::dot(c1, LATT->coords[i]) > dd) ? -1 : 1);
    qApp->processEvents();
}

void MainWindow::cutBox() {
    LATT->marked.clear();
    if(mode == 4) {
        for(int i = 0; i < LATT->nAt.size(); i++) {
            LATT->marked[i] = ((Calc::rect_box(LATT->coords[i], sizeB)) ? 1 : -1);
        }
    }
    if(mode == 5) {
        for(int i = 0; i < LATT->nAt.size(); i++) {
            LATT->marked[i] = ((Calc::hex_box(LATT->coords[i], sizeB)) ? 1 : -1);
        }
    }
    if(mode == 6) {
        for(int i = 0; i < LATT->nAt.size(); i++) {
            LATT->marked[i] = ((Calc::romb_box(LATT->coords[i], sizeB)) ? 1 : -1);
        }
    }
    if(mode == 7) {
        for(int i = 0; i < LATT->nAt.size(); i++)  {
            LATT->marked[i] = ((Calc::cylinder(LATT->coords[i], sizeB)) ? 1 : -1);
        }
    }
    qApp->processEvents();
}

void MainWindow::SL_togglemark() {
    mview1->setDone(true);
    for(int i = 0; i < LATT->nAt.size(); i++) LATT->marked[i] *= -1;
    INT->refrAtoms = true;
    refreshScene(); //mview1->setQuat(rotQuat);
    mview1->setDone(false);
    mode = 0;
    return;

}

void MainWindow::addPOINTS() {
    int nr = screw ? 2 : 1;
    std::cout << "------ ADD point nr=" << nr << std::endl;
    POINTS->pos->push_back(MiscFunc::convert(INT->actPoint));
    POINTS->nKind->push_back(nr);
    POINTS->n_points = POINTS->pos.get()->size();
    POINTS->millerVs.push_back(INT->millerV);
    POINTS->millerPs.push_back(INT->millerP);
    POINTS->fracts.push_back(INT->fract);
    POINTS->rotTens.push_back(INT->rotTens);
    POINTS->crCNum.push_back(INT->crCNum);
    POINTS->strBV.push_back(ansBurgers);
    POINTS->strName.push_back(strName);
    if(nr == 1) {
        POINTS->numMeth.push_back(numMeth);
        POINTS->vectors.push_back(glm::dvec3(0., 0., 0.));
    } //else {
    if(nr == 2) {
        POINTS->numMeth.push_back(0);
        POINTS->vectors.push_back(direction);
    }
    qApp->processEvents();
}

void MainWindow::SL_update() {
    mview1->setDone(true);
    INT->outLog << "SL_update()" << std::endl;

    for(int i = 0; i < LATT->nAt.size(); i++) {
        LATT->coords[i] += LATT->u[i];
    }
    mview1->setDone(false);
    scene->displayBvect(false);
    INT->refrAtoms = true;
    INT->showPoints = false;
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
}

void MainWindow::SL_calcDisl() {
    std::cout << "SL_calcDisl" << std::endl;
    INT->outLog << "SL_calcDisl()" << std::endl;
    mview1->setDone(true);
    for(int i = 0; i < LATT->nAt.size(); i++) {
        LATT->u[i] = glm::dvec3(0., 0., 0.);
    }
    QString cd0 = INT->currDir;
    QString cd1 = cd0.append("/data/AAA");
    QFile file(cd1);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        QMessageBox::warning(this,
                             "PROBLEM",
                             QString(" ERROR writing %1").arg(cd1));
    else {//      file.write(editor->toPlainText().toUtf8());
        QString str;
        QString str1 = QString::fromStdString("\n");
        str.append("Leave only the lines that are necessary\nLines startitng with // are comments\n");
        for(int i = 0; i < POINTS->n_points; i++) {
            glm::dvec3 point = MiscFunc::convert(POINTS->pos.get()->at(i));
            std::cout << "POINTS kind=" << POINTS->nKind.get()->at(i) << std::endl;
            if(POINTS->nKind.get()->at(i) == 1) {   // normal dislocation
                str.append(QString("!! single_disl nr= %1 method= %2 ").arg(i).arg(POINTS->numMeth[i]));
                str.append(POINTS->strBV[i]);
                str.append(QString(" %1 %2 %3").arg(point.x, 0, 'f', 8).arg(point.y, 0, 'f', 8).arg(point.z,
                                                                                                    0,
                                                                                                    'f',
                                                                                                    8));
                str.append(str1);
            }
            if(POINTS->nKind.get()->at(i) == 2) {   // pure screw dislocation
                str.append(QString("!! screw_disl nr= %1 ").arg(i));
                str.append(POINTS->strBV[i]);
                str.append(QString(" %1 %2 %3 ").arg(point.x, 0, 'f', 8).arg(point.y, 0, 'f', 8).arg(point.z,
                                                                                                     0,
                                                                                                     'f',
                                                                                                     8));
                glm::dvec3 dir = POINTS->vectors[i];
                str.append(QString(" direction %1 %2 %3 ").arg(dir.x, 0, 'f', 8).arg(dir.y, 0, 'f', 8).arg(dir.z,
                                                                                                           0,
                                                                                                           'f',
                                                                                                           8));
                str.append(str1);
            }
        } // for
        file.write(str.toUtf8());
        file.close();
    }
    SimpleEd *edit = new SimpleEd(cd1);
    connect(edit, SIGNAL(SIG_finished(QString)), this, SLOT(SL_performDislCalc(QString)));
    edit->show();
    mview1->setDone(false);
}

void MainWindow::SL_performDislCalc(QString ff) {
    QFile file(ff);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             "PROBLEM",
                             QString(" ERROR opening %1").arg(ff));
        return;
    }
    QTextStream in(&file);//int nl = 0;
    INT->outLog << "SL_performDislCalc(QString ff)" << std::endl;
//    stringstream sss;
//    sss << "wynik" << ".txt";
//    ofstream f_numb(sss.str().c_str());  
    //int nl =0;
    while(!in.atEnd()) {
        QString line = in.readLine(); //++nl;  std::cout << " nl=" << nl << "   " << line.toStdString() << std::endl;
        if(line.isEmpty() || (line.at(0) == '/' && line.at(1) == '/')) continue;
        if(line.at(0) == '!' && line.at(1) == '!') {
            QStringList fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
            //int nf = fields.size(); for (int i=0; i<nf; i++) std::cout << i << ":" << fields.at(i).toStdString() << "   $$   "; std::cout << std::endl;//std::string s = fields.at(3).toStdString();
            int nm = 0;
            int k =
                    fields.at(3).toInt(); //MiscFunc::toInt(s);// std::cout << " nl=" << nl << "   s=" << s << "    k=" << k << std::endl;
            if(POINTS->nKind.get()->at(k) == 1) nm = fields.at(5).toInt();
            glm::dvec3 pos =
                    MiscFunc::convert(POINTS->pos.get()->at(k)); // std::cout << " pos = ( " << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
            glm::dmat3 rt = POINTS->rotTens.at(k); // MiscFunc::printMat("rt", rt);
            glm::dmat3 rtInv = glm::transpose(rt); // MiscFunc::printMat("rtInv", rtInv);
            glm::dvec3 bV =
                    rt * POINTS->fracts.at(k) * (INT->structList[POINTS->crCNum.at(k)]).c2o * POINTS->millerVs.at(k);
            glm::dvec3 cd =
                    rt * pos; //  std::cout << " cd = ( " << cd.x << ", " << cd.y << ", " << cd.z << ")" << std::endl;
            double be = sqrt(bV.x * bV.x + bV.y * bV.y);
            double bz = bV.z;
            std::cout << "  nm=" << nm << "  be=" << be << "  bz=" << bz << std::endl;
            LATT->du.clear();
            LATT->du.resize(LATT->nAt.size());

            if(nm == 0) {  // classical
                for(int i = 0; i < LATT->nAt.size(); i++) {
                    glm::dvec3 coord1 = rt * LATT->coords[i];
                    glm::dvec3 dist1 = coord1 - cd;
                    if(glm::length2(dist1) < 1.e-8) {
                        QMessageBox::warning(this,
                                             "PROBLEM",
                                             QString(" ERROR  atom nr %1 is too close to disl core").arg(i + 1));
                        return;
                    }
                    LATT->du[i] = rtInv * Calc::mixed_u(dist1, be, bz);
                }
            }
            if(nm > 0) { // iterations
                int count_moved = 0;
                int n_Errors = 0;
                int n_errors = 0;
                size_t countN_R = 20;
                double crit_stop = 1e-7;
                size_t count;
                struct params p;
                p.be = be;
                p.bz = bz;
                int status;
                glm::dvec3 diff;
                double crit = 0.;
                const gsl_multiroot_fdfsolver_type *T;
                if(nm == 2) T = gsl_multiroot_fdfsolver_hybridsj;
                else T = gsl_multiroot_fdfsolver_newton;
                for(int i = 0; i < LATT->nAt.size(); i++) {
                    glm::dvec3 coord1 = rt * LATT->coords[i];
                    glm::dvec3 dist1 = coord1 - cd;
                    if(glm::length2(dist1) < 1.e-8) {
                        QMessageBox::warning(this,
                                             "PROBLEM",
                                             QString(" ERROR  atom nr %1 is too close to disl core").arg(i + 1));
                        return;
                    }
                    count = 0;
                    glm::dvec3 du0 = Calc::mixed_u(dist1, be, bz);
                    double duNorm = 0.8 * glm::length(du0);
                    double ddd = 0.;
                    do {
                        gsl_multiroot_fdfsolver *s; // = gsl_multiroot_fdfsolver_alloc(T, 3);
                        gsl_vector *x; // = gsl_vector_alloc(3);
                        count++;
                        p.u0x = LATT->du[i].x;
                        p.u0y = LATT->du[i].y;
                        p.u0z = LATT->du[i].z;
                        glm::dvec3 temp = coord1 + LATT->du[i] - cd;
                        if((temp.x * temp.x + temp.y * temp.y) < 1.e-10) {
                            LATT->du[i] = glm::dvec3(0., 0., 0.);
                            std::cout << " Atom " << i << " in the center of dislocation core" << std::endl;
                            n_errors++;
                            goto _END;
                        }
                        gsl_multiroot_function_fdf
                                f = {&Calc::Love_function, &Calc::Beta_function, &Calc::Love_fdf, 3, &p};
                        x = gsl_vector_alloc(3);
                        gsl_vector_set(x, 0, temp.x);
                        gsl_vector_set(x, 1, temp.y);
                        gsl_vector_set(x, 2, temp.z);
                        s = gsl_multiroot_fdfsolver_alloc(T, 3);
                        gsl_multiroot_fdfsolver_set(s, &f, x);
                        status = gsl_multiroot_fdfsolver_iterate(s);

                        if(status) break;
                        diff.x = gsl_vector_get(s->f, 0);
                        diff.y = gsl_vector_get(s->f, 1);
                        diff.z = gsl_vector_get(s->f, 2);
                        LATT->du[i] -=
                                diff; //std::cout << "=1=   i=" << i << "   count=" << count << " ------    du=  " << LATT->du[i].x << "   " << LATT->du[i].y << "    " << LATT->du[i].z << std::endl;
                        ddd = glm::distance(du0, LATT->du[i]);

                        status = gsl_multiroot_test_residual(s->f, crit_stop);
                        gsl_multiroot_fdfsolver_free(s);
                        gsl_vector_free(x);
                    } while(status == GSL_CONTINUE && count < countN_R);
                    crit += diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

                    if(ddd > duNorm) {
                        std::cout << " i=" << i << "   count=" << count << "   diff  " << diff.x << "   " << diff.y
                                  << "    " << diff.z << std::endl;
                        std::cout << "  du0  = " << du0.x << ",  " << du0.y << ",  " << du0.z << std::endl;
                        std::cout << "  du  = " << LATT->du[i].x << ",  " << LATT->du[i].y << ",  " << LATT->du[i].z
                                  << std::endl;  //glm::dvec3 point = MiscFunc::convert(POINTS->pos.get()->at(i));
                        glm::dvec3 ac = LATT->coords[i];
                        std::cout << " duNorm = " << duNorm << "        ddd = " << ddd << "       coord X = " << ac.x
                                  << "       coord Y = " << ac.y << std::endl;
                        double rrr = sqrt(ac.x * ac.x + ac.y * ac.y);
                        std::cout << "       x = " << ac.x / rrr << "       y = " << ac.y / rrr << std::endl;
                        std::cout << "+++++++++   atom nr. " << i << " was moved!" << std::endl;
                        LATT->du[i].x = du0.x;
                        count_moved++;
                    } //if ( ddd>duNorm )
                    continue;
                    _END:
                    std::cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
                    n_Errors++;
                } // for
                std::cout << "  count_moved = " << count_moved << std::endl;
            } // if ( nm>0 )


            for(int i = 0; i < LATT->nAt.size(); i++)
                LATT->du[i] = rtInv * LATT->du[i];// atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;}
            sumU = glm::dvec3(0.0, 0.0, 0.0);
            for(int i = 0; i < LATT->nAt.size(); i++) {
                sumU += LATT->du[i];
            }
            sumU /= double(LATT->nAt.size());
            std::cout << "Dislocation nr. " << k + 1 << "     sumU = (" << sumU.x << ", " << sumU.y << ", " << sumU.z
                      << ")" << std::endl; // " << n_dislocations << "

            for(int i = 0; i < LATT->nAt.size(); i++) {
                LATT->du[i] -= sumU;
                LATT->u[i] += LATT->du[i];//u0;
//  f_numb << i << " ***du****    " << LATT->du[i].x << "    " << LATT->du[i].y << "    " << LATT->du[i].z << std::endl;
//  f_numb << i << " ***U****    " << LATT->u[i].x << "    " << LATT->u[i].y << "    " << LATT->u[i].z << std::endl;
            }
        } // line.at(0)=='!' && line.at(1)=='!'
    }
//  f_numb.close();  
    file.close();
}

void MainWindow::SL_move() {
    qWarning("SL_move");
    mview1->setDone(true);
    MoveDialog *md = new MoveDialog("Vector coordinates:");
    if(md->ok) {//std::cout << "SL_MoveAtoms:  x=" << md->vectX << "   y=" << md->vectY << "   z=" << md->vectZ << std::endl;
        if(md->rotated) rotTens = MiscFunc::quat2rot((mview1->getQuat() * ZtoY).conj());
        for(int i = 0; i < LATT->nAt.size(); i++) {
            if(md->marked && LATT->marked[i] < 0) continue;
            glm::dvec3 p1 = (md->rotated) ? rotTens * LATT->coords[i] : LATT->coords[i];
            p1 += glm::dvec3(md->vectX, md->vectY, md->vectZ);
            LATT->coords[i] = p1;
        }
    } //else std::cout << "SL_MoveAtoms:  DUPA" << std::endl;
    mview1->setDone(false);
}

void MainWindow::SL_setPoints() {
//qWarning("SL_setPoints"); 
    mview1->setDone(true);
    INT->showPoints = !INT->showPoints;
    showPointsAct->setChecked(INT->showPoints);
    mview1->setDone(false);
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
}

void MainWindow::SL_bondsAct() {
    std::cout << "SL_bondsAct:  mode=" << mode << std::endl;
}

void MainWindow::SL_atoms() {
//qWarning("SL_atoms"); 
    mview1->setDone(true);
    QString descr = "";
    QStringList quest, sug, ans;
    QString rF = QString("%1").arg(INT->radFactor, 0, 'f', 3);
    QString aA = QString("%1").arg(INT->alphaAt, 0, 'f', 3);
    quest << "Radius" << "Opacity";
    sug << rF << aA;
    QuestionForm1 *qf = new QuestionForm1("Atoms", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) return;
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

void MainWindow::SL_setVect() {
    qWarning("SL_setVect");
}

void MainWindow::SL_setAx() {
    mview1->setDone(true);
    QString descr = "";
    QStringList quest, sug, ans, combo;
    QString aL = QString("%1").arg(INT->axL, 0, 'f', 3);
    QString aR = QString("%1").arg(INT->axRad, 0, 'f', 3);
    QString aPr1 = QString("%1").arg(INT->axPr1, 0, 'f', 3);
    QString aPr2 = QString("%1").arg(INT->axPr2, 0, 'f', 3);
    QString aA = QString("%1").arg(INT->alphaAx, 0, 'f', 3);
    quest << "Legth" << "Radius" << "Proportion1" << "Proportion2" << "Opacity";
    sug << aL << aR << aPr1 << aPr2 << aA;
    combo << "No axis" << "Axis at the center" << "Axis at the corner";
    QuestionForm2 *qf = new QuestionForm2("Axis", descr, quest, sug, ans, combo, INT->axInd);
    mview1->setDone(false);
    if(!qf->ok) {
        delete qf;
        return;
    }
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

void MainWindow::SL_visibility() {
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
    if(!qf->ok) {
        delete qf;
        return;
    }
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

void MainWindow::SL_setBg() {
    mview1->setDone(true);
    QColor initCol = MiscFunc::qtColor(INT->bgColor);
    QColorDialog *qc = new QColorDialog(initCol, this);
    qc->setOption(QColorDialog::ShowAlphaChannel, true);
#ifdef Q_WS_MAC // The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
    qc->setOption(QColorDialog::DontUseNativeDialog, true);
#endif
    mview1->setDone(false);
    if(qc->exec() == QDialog::Rejected) return;
    QColor color = qc->selectedColor();
    delete qc;
    if(color == initCol) return;
    if(color.isValid()) {
        osg::Vec4 col = MiscFunc::osgColor(color);
        INT->bgColor = col;
        osg::Matrixd mat = mview1->getMatrix();
        refreshViewer();
        mview1->setMatrix(mat);
    }
    //mview1->setDone(false);
}

void MainWindow::SL_setLight() {
    mview1->setDone(true);
    INT->lightOn = !INT->lightOn;
    lightAct->setChecked(INT->lightOn);
    mview1->setDone(false);
    //INT->refrAtoms = true;
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
}

void MainWindow::SL_setNum() {
    mview1->setDone(true);
    INT->showNum = !INT->showNum;
    numAct->setChecked(INT->showNum);
    mview1->setDone(false);
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
    //mview1->setDone(false);
    //scene->displayPoints(false);
}

void MainWindow::SL_setMaterial() {
    mview1->setDone(true);
    INT->materialOn = !INT->materialOn;
    materialAct->setChecked(INT->materialOn);
    mview1->setDone(false);
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
}

void MainWindow::SL_setColB() {
    mview1->setDone(true);
    QColor initCol = MiscFunc::qtColor(INT->bondColor);
    QColorDialog *qc = new QColorDialog(initCol, this);
    qc->setOption(QColorDialog::ShowAlphaChannel, true);
#ifdef Q_WS_MAC // The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
    qc->setOption(QColorDialog::DontUseNativeDialog, true);
#endif
    mview1->setDone(false);
    if(qc->exec() == QDialog::Rejected) return;
    QColor color = qc->selectedColor();
    delete qc;
    if(color == initCol) return;
    if(color.isValid()) {
        osg::Vec4 col = MiscFunc::osgColor(color);
        INT->bondColor = col;
        INT->refrAtoms = true;
        osg::Matrixd mat = mview1->getMatrix();
        refreshScene();
        mview1->setMatrix(mat);
    }
}

void MainWindow::SL_setFov() {
    mview1->setDone(true);
    QString descr = "";
    QString quest = "Field of view";
    QString ans;
    QString sug = QString("%1").arg(INT->fov, 0, 'f', 3);
    OneQuestion *oq = new OneQuestion("Field of view", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!oq->ok) return;
    double d = ans.toDouble();
    delete oq;
    if(d == INT->fov) return;
    osg::clampBetween(d, 0.0004, 70.);
    INT->fov = d;
    osg::Quat q = mview1->getQuat();
    refreshViewer();
    mview1->setQuat(q);
    //mview1->setDone(false);
}

void MainWindow::SL_setBonds() {
    mview1->setDone(true);
    QString descr = "";
    QString quest, sug, ans;
    sug = QString("%1").arg(INT->bondFactor, 0, 'f', 3);
    OneQuestion *oq = new OneQuestion("Size of bonds", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!oq->ok) return;
    float f = ans.toFloat();
    delete oq;
    osg::clampBetween(f, 0.f, 0.1f);
    if(f == INT->bondFactor) return;
    INT->bondFactor = f;
    INT->refrAtoms = true;
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
}

void MainWindow::SL_setNS() {
    mview1->setDone(true);
    QString descr = "";
    QString quest, sug, ans;
    sug = QString("%1").arg(INT->sizeTxt, 0, 'f', 3);
    OneQuestion *oq = new OneQuestion("Size of numbers", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!oq->ok) return;
    float f = ans.toFloat();
    delete oq;
    //osg::clampBetween(f, 0.f, 0.1f);
    if(f == INT->sizeTxt) return;
    INT->sizeTxt = f;
    //INT->refrAtoms = true;
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
    //mview1->setDone(false);
}

void MainWindow::SL_openAtoms() {
    mview1->setDone(true);
    if(!INT->atName.empty()) {
        QString quest =
                QString("You have opened %1 file. Do you want to close it?")
                .arg(QString::fromStdString(INT->atName)
                .section('/', -1)); // QString::fromStdString(str)
        if(QMessageBox::Yes == QMessageBox(QMessageBox::Information, "WARNING", quest, QMessageBox::Yes | QMessageBox::No).exec()) {
            moreFiles = false;
        } else {
            moreFiles = true;
        }
    }
    QString cd0 = INT->currDir;
    auto qf = std::make_shared<QFileDialog>(this, "Select atoms", cd0.append("/data/atoms"), "Molecules (*.xyz *.alc *.mol2)");
    qf->exec();
    if(qf->result() != 1) {
        mview1->setDone(false);
        return;
    }
    QString aname1 = qf->selectedFiles().at(0);
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "SL_openAtoms aname1  :  " << aname1.toStdString() << "   date & time is " << dateTimeString
                << std::endl;
    INT->calcLog << "!! read_atoms " << aname1.toStdString() << std::endl;
    if(aname1.isEmpty()) {
        mview1->setDone(false);
        return;
    }
    if(!(aname1.contains(".xyz")) && !(aname1.contains(".alc")) && !(aname1.contains(".mol2"))) {
        QMessageBox::warning(this, "PROBLEM", QString("openAtoms -- unknown file format"));
        mview1->setDone(false);
        return;
    }
    else {
        if(moreFiles) {
            int oldN = LATT->getNAtoms();
            INT->atName = (QString("%1 + %2")
                            .arg(QString::fromStdString(INT->atName))
                            .arg(aname1.section('/', -1))).toStdString();
            readInputFile(aname1);
            iAt = QString("   file: %1   %2+%3=%4 atoms  ")
                    .arg(QString::fromStdString(INT->atName))
                    .arg(oldN).arg(LATT->getNAtoms() - oldN)
                    .arg(LATT->getNAtoms());
            moreFiles = false;
            INT->refrMarked = false;
        } else {
            INT->atName = aname1.toStdString();
            readInputFile(aname1);
            iAt = QString("   file: %1  %2 atoms  ").arg(aname1.section('/', -1)).arg(LATT->getNAtoms());
        }
        infoDisplay();
        mview1->setDone(false);
    }
    INT->refrAtoms = true;
    refreshScene();
}

void MainWindow::SL_closeAtoms() {
    mview1->setDone(true);
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << strAtom.toStdString() << " closed  at " << dateTimeString << std::endl;
    strBvect = strAtom = strPoint = iAt = iStr = "";
    INT->atName = "";
    INT->hklDefined = INT->atomDefined = INT->pointDefined = INT->structureDefined = false;
    LATT->clear();
    infoDisplay();

    POINTS->clearP();
    INT->refrAtoms = true;
    osg::Quat q = mview1->getQuat();
    mview1->setDone(false);
    refreshScene();
    INT->refrAtoms = false;
    INT->xMin = INT->yMin = INT->zMin = 1e300;
    INT->xMax = INT->yMax = INT->zMax = -1e300;
    mview1->setQuat(q);
    infoLabel->clear();
}

void MainWindow::SL_image() {
    if(INT->atName.empty() && INT->compFem.empty() && INT->fem.empty()) {
        QMessageBox::warning(this,
                             "PROBLEM",
                             "No atoms, no fems");
        return;
    }
    mview1->setDone(true);
    QString cd0 = INT->currDir;
    QFileDialog
            *qf = new QFileDialog(this, "Select HRTEM pictures", cd0.append("/data/images"), "Pictures (*.png *.tif)");
    qf->exec();
    if(qf->result() != 1) {
        mview1->setDone(false);
        return;
    }
    QString iname1 = qf->selectedFiles().at(0);
    delete qf;
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "SL_openImage iname1  :  " << iname1.toStdString() << "  date & time is " << dateTimeString
                << std::endl;
    if(iname1.isEmpty()) {
        mview1->setDone(false);
        return;
    }
    mview1->setDone(false);
    INT->image = iname1.toStdString();
    INT->refrImage = true;
    osg::Quat q = mview1->getQuat();
    refreshScene();
    mview1->setQuat(q);
}

void MainWindow::SL_showRes() {
    if(INT->results.empty()) {
        QMessageBox::warning(this, "PROBLEM", "No defined fems");
        return;
    }
    bool ok;
    QStringList res1;
    res1 << "None";
    for(int i = 0; i < INT->results.size(); i++) res1 << INT->results.at(i);
    QString item = QInputDialog::getItem(this, "Results", "", res1, 0, false, &ok);
    if(QString::compare(item, "None", Qt::CaseSensitive) == 0) INT->choosen_value = "";
    else if(ok && !item.isEmpty()) INT->choosen_value = item.toStdString();
    INT->refrRes = true;
    refreshScene();
}

void MainWindow::SL_closeImage() {
    mview1->setDone(true);
    INT->image = "";
    INT->refrImage = false;
    osg::Quat q = mview1->getQuat();
    mview1->setDone(false);
    refreshScene();
    mview1->setQuat(q);
}

void MainWindow::SL_reset() {
    mview1->setDone(false);
    scene->displayBvect(false);
    INT->refrAtoms = true;
    INT->showPoints = false;
    osg::Matrixd mat = mview1->getMatrix();
    refreshScene();
    mview1->setMatrix(mat);
/*
  osg::Quat q = mview1->getQuat();
  refreshViewer();
  mview1->setQuat(q);
*/
}

void MainWindow::readInputFile(const QString& aname1) {
    QFile file(aname1);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString qstr = QString("***  ERROR  file  %1 not found").arg(aname1);
        INT->outLog << qstr.toStdString() << std::endl;
        QMessageBox::warning(this, "PROBLEM", qstr);
        return;
    }
    QTextStream in(&file);
    FormatReadResult result;
    if(aname1.endsWith(".alc")) {
        result = readAlc(in, aname1);
    } else if(aname1.endsWith(".xyz")) {
        result = readXyz(in, aname1);
    } else if(aname1.endsWith(".mol2")) {
        result = readMol2(in, aname1);
    }
    else {
        QMessageBox::warning(this, "PROBLEM", "Unsupported input format.");
    }
    if(! handleFormatResult(result.getResult())) {
        return;
    }
    Lattice lattice = result.getLattice().value();
    if(moreFiles) {
        LATT->append(lattice);
    }
    else {
        LATT = std::make_unique<Lattice>(lattice);
    }
    qApp->processEvents();
}

void MainWindow::refreshScene() {
    scene = new OsgScene();
    MainLayout->removeWidget(mview1);
    mview1 = new OsgViewerQt(scene, INT->fov);
    mview1->setKeyEventSetsDone(0);
    setSliders(0., 0., 0., 1., 0., 0.);
    mview1->setMinimumSize(600, 800);
    mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    MainLayout->addWidget(mview1, 1, 0);
    infoDisplay();
    setFocus();
    qApp->processEvents();
}

void MainWindow::refreshViewer() {
    mview1 = new OsgViewerQt(scene, INT->fov);//    setSliders(valTheta, valPhi, valPsi);
    mview1->setKeyEventSetsDone(0);
    setSliders(0., 0., 0., 1., 0., 0.);
    mview1->setMinimumSize(600, 800);
    mview1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    MainLayout->addWidget(mview1, 1, 0);
    infoDisplay();
    qApp->processEvents(); //mview1->setIdle(true);
}

void MainWindow::SL_genAtoms() {
    if(!INT->structureDefined) {
        QMessageBox::warning(this, "PROBLEM", "You should define structure first");
        return;
    }
    mview1->setDone(true);
    QStringList quest, sug, ans, fields;
    QString descr = ("<h4>X, Y, Z ranges</h4><br>Cell nr.(0, 0, 0) is at coordinates(0, 0, 0)");
    quest << "Lattice name" << "X begin" << "X end" << "Y begin" << "Y end" << "Z begin" << "Z end";
    sug << "" << "0" << "" << "0" << "" << "0" << "";
    auto qf = std::make_unique<CreateLatticeDialog>("Generated structure definition", descr, quest, sug, ans);
    mview1->setDone(false);
    if(!qf->ok) {
        return;
    }
    int xb = ans.at(1).toInt();
    int xe = ans.at(2).toInt();
    int yb = ans.at(3).toInt();
    int ye = ans.at(4).toInt();
    int zb = ans.at(5).toInt();
    int ze = ans.at(6).toInt();
    auto generationResult = Gener::genLattice(xb, yb, zb, xe, ye, ze);
    if(qf->isCreateBonds()) {
        // Generate bonds using obabel.
        std::ofstream xyzStream{"/tmp/vecds_obabel.xyz", std::ios::trunc};
        writeXyz(xyzStream, generationResult);
        xyzStream.close();
        INT->outLog << "Starting obabel in order to generate bonds..." << std::endl;
        QProcess obabel;
        obabel.start("obabel /tmp/vecds_obabel.xyz -ixyz -osy2");
        iStr = "Running babel to generate lattice bonds...";
        iAt = "";
        infoDisplay();
        QApplication::processEvents();
        // TODO avoid long running tasks in the main thread
        obabel.waitForFinished(-1);
        infoDisplay();
        iStr = "";
        iAt = "";
        int exitCode = obabel.exitCode();
        std::string err = QString(obabel.readAllStandardError()).trimmed().toStdString();
        bool isError = err != "1 molecule converted";
        if(obabel.exitStatus() != QProcess::NormalExit || isError || exitCode) {
            std::string out = QString(obabel.readAllStandardOutput()).trimmed().toStdString();
            auto errorMsg = QString("The obabel program ended with the error code: %1, std err: %2. "
                                    "The lattice will have no bonds defined. Standard output: %3")
                    .arg(exitCode)
                    .arg(err.c_str())
                    .arg(out.c_str());
            QMessageBox::warning(this, "PROBLEM", errorMsg);
            LATT = std::make_unique<Lattice>(std::move(generationResult));
        } else {
            auto inStream = QTextStream(obabel.readAllStandardOutput());
            auto result = readMol2(inStream, QString("generated")); // TODO memory?
            if(result.getResult().hasErrors()) {
                QMessageBox::warning(this, "PROBLEM", "Babel generated incorrect SYBYL MOL2 file.");
                return;
            }
            LATT = std::make_unique<Lattice>(result.getLattice().value());
        }
    } else {
        LATT = std::make_unique<Lattice>(std::move(generationResult));
    }

    iAt = QString(" file: %1  %2 atoms  ").arg(LATT->name).arg(LATT->getNAtoms() + 1);
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "Generated atoms " << iAt.toStdString() << " Date & time is " << dateTimeString << std::endl;
    LATT->name = ans.at(0);
    infoDisplay();
    INT->atName = LATT->name.toStdString();  //iAt = LATT->name.section('/', -1);
    INT->refrAtoms = true;
    refreshScene();
}

void MainWindow::SL_openFem() {
//std::cout << "SL_openFem" << std::endl;
    INT->newcalc = true;
    INT->whichFem = 1;
    mview1->setDone(true);
    QString cd0 = INT->currDir;
    QFileDialog *qf = new QFileDialog(this, "Import FEAP data", cd0.append("/data/fems"), "data (*.txt *.o *.i)");
    qf->exec();
    if(qf->result() != 1) {
        mview1->setDone(false);
        return;
    }
    QString femname = qf->selectedFiles().at(0);
    delete qf; // qWarning("SL_openF femname  :  %s", femname.toLatin1().data() );
    if(femname.isEmpty()) {
        mview1->setDone(false);
        return;
    }
    INT->fem = femname.toStdString();
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "Opened FE " << INT->fem << "   date & time = " << dateTimeString << std::endl;
    INT->compFem = "";
    INT->refrFem = true;
    mview1->setDone(false);
    osg::Quat q = mview1->getQuat();
    refreshScene();
    mview1->setQuat(q);
    if(INT->atName.empty()) refreshViewer();
}

void MainWindow::SL_computeFem() {
//std::cout << "SL_computeFem" << std::endl;
    INT->newcalc = true;
    INT->whichFem = 2;
    mview1->setDone(true);
    QString cd0 = INT->currDir;
    QFileDialog *qf = new QFileDialog(this, "Select extern FEM data", cd0.append("/data/extfems"), "data (*.txt *.o *.i)");
    qf->exec();
    if(qf->result() != 1) {
        mview1->setDone(false);
        return;
    }
    QString femname = qf->selectedFiles().at(0);
    delete qf;
    if(femname.isEmpty()) {
        mview1->setDone(false);
        return;
    }
    INT->compFem = femname.toStdString();
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "Computed FE " << INT->compFem << "   date & time = " << dateTimeString << std::endl;
    INT->fem = "";
    INT->refrFem = true;
    mview1->setDone(false);
    osg::Quat q = mview1->getQuat();
    refreshScene();
    mview1->setQuat(q);
    if(INT->atName.empty()) refreshViewer();
}

void MainWindow::SL_closeFem() {
    mview1->setDone(true);
    INT->whichFem = 0;
    INT->fem = "";
    INT->compFem = "";
    INT->refrFem = false;
    INT->results.clear();
    if(INT->atName.empty()) {
        INT->xMin = INT->yMin = INT->zMin = 1e300;
        INT->xMax = INT->yMax = INT->zMax = -1e300;
    }
    std::string dateTimeString = MiscFunc::dateTime().toStdString();
    INT->outLog << "Closed FE " << "   date & time = " << dateTimeString << std::endl;
    osg::Quat q = mview1->getQuat();
    mview1->setDone(false);
    refreshScene();
    mview1->setQuat(q);
}

void MainWindow::SL_saveAtoms() {
    if(LATT->getNAtoms() == 0) {
        QMessageBox::warning(this, "PROBLEM", "No atoms in this structure!");
        return;
    }
    mview1->setDone(true);
    auto sd = std::make_unique<SaveDialog>("File name:");
    sd->exec();
    if(sd->ok) {
        std::ofstream out(sd->fileName.toStdString().c_str());
        if(!out.is_open()) {
            QMessageBox::warning(this, "PROBLEM", "Cannot open file for writing");
            mview1->setDone(false);
            return;
        }
        // Saving.
        Lattice toSave(*LATT);
        if(sd->marked) {
            toSave = toSave.getMarkedOnly();
        }
        if(sd->rotated) {
            rotTens = MiscFunc::quat2rot((mview1->getQuat() * ZtoY).conj());
            toSave = toSave.rotate(rotTens);
        }
        if(sd->fileName.endsWith(".xyz")) {
            writeXyz(out, toSave);
        } else if(sd->fileName.endsWith(".mol2")) {
            writeMol2(out, toSave);
        } else {
            QMessageBox::warning(this, "PROBLEM", ("Unsupported output format: " + sd->fileName.toStdString()).c_str());
            mview1->setDone(false);
            return;
        }
        INT->outLog << "Saved atoms: " << sd->fileName.toStdString() << std::endl;
    }
    else {
        mview1->setDone(false);
    }
}

void MainWindow::SL_chooseSettings() {
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
    if(INT->settDefined) old = INT->sett;
    bool ok;
    QString item = QInputDialog::getItem(this, "Choose settings", "settings:", items, items.size(), false, &ok);
    if(ok && !items.isEmpty()) {  //std::cout << "SLchoose      item=" << item.toStdString() << std::endl;
        if(item.compare(QString("Default")) == 0) {
            INT->sett = "/data/internal/defaults0.txt";
            if(INT->settDefined) {
                AT = std::make_unique<Atoms>();
                INT->refrAtoms = true;
            }//&& INT->nr!=-1 )
            INT->readDefaults(INT->sett);
            INT->settDefined = false;
            mview1->setDone(false);
            osg::Quat q = mview1->getQuat();
            refreshScene();
            mview1->setQuat(q);
        } else {
            for(int i = 1; i < items.size(); i++)
                if(item == items.at(i)) {
                    INT->sett =
                            QString("/data/internal/").append(items.at(i));  //std::cout << "Settings  i=" << i << "   sett=" << INT->sett.toStdString() << std::endl;
                    INT->refrAtoms = true;
                    INT->settDefined = true;
                    INT->readDefaults(INT->sett);
                    mview1->setDone(false);
                    osg::Quat q = mview1->getQuat();
                    refreshScene();
                    mview1->setQuat(q);
                }
        }  // if - else
        std::string dateTimeString = MiscFunc::dateTime().toStdString();
        INT->outLog << " Settings:" << INT->sett.toStdString() << "   date & time = " << dateTimeString << std::endl;
        return;
    }
}

void MainWindow::SL_editSett() {
    mview1->setDone(true);
    eee = new SimplePad("/data/internal/",
                        "settings (*.sett)");//std::cout << "1  url=" << INT->url.toStdString() << std::endl;
    eee->show();
    mview1->setDone(false);
    //delete eee;
}

void MainWindow::SL_chooseStructure() {
    mview1->setDone(true);
    QString cd0 = INT->currDir;
    cd0.append("/data/internal/");
    QStringList nameFilter("*.struct");
    QDir directory(cd0);
    QStringList items = INT->structNames;//directory.entryList(nameFilter);
    bool ok;
    QString item = QInputDialog::getItem(this,
                                         "Choose structure",
                                         "structure:",
                                         items,
                                         items.size(),
                                         false,
                                         &ok); //std::cout << " == item == " << item.toStdString() << std::endl;
    if(ok && !items.isEmpty())
        for(int i = 0; i < items.size();
            i++) { //std::cout << items.at(i).toStdString() << " ==== " << item.toStdString() << std::endl;
            if(item == items.at(i)) {
                INT->crC = &(INT->structList[i]);
                INT->crCNum = i;
                INT->structureDefined = true;
                iStr = QString(" Structure: %1").arg(item);
                if(LATT->nAt.size() <= 0) {
                    iAt = " ---------- ";
                }
                infoDisplay();
                std::string dateTimeString = MiscFunc::dateTime().toStdString();
                INT->outLog << iStr.toStdString() << "   date & time = " << dateTimeString << std::endl;
                mview1->setDone(false);
                return;
            }
        }
    mview1->setDone(false);
    INT->structureDefined = false;
}

void MainWindow::SL_editStruct() {
    mview1->setDone(true);
    eee = new SimplePad("/data/internal/",
                        "structures (*.struct)");//std::cout << "1  url=" << INT->url.toStdString() << std::endl;
    eee->show();
    INT->initStructures();
    mview1->setDone(false);
    std::cout << "SL_editStruct ---- end" << std::endl;
    //delete eee;
}

void MainWindow::SL_about() {
    HelpBrowser::showPage("index.html");
}

void MainWindow::SL_aboutOsg() {
    HelpBrowser::showPage("osg.html");
}

void MainWindow::setSliders(double v1, double v2, double v3, double v4, double v5, double v6) {
    valTh = v1;
    valPh = v2;
    valPs = v3;
    valDist = v4;
    valCx = v5;
    valCy = v6;
    QString str;
    thetaSlider->setText(str.setNum(valTh, 'f', 3));
    phiSlider->setText(str.setNum(valPh, 'f', 3));
    psiSlider->setText(str.setNum(valPs, 'f', 3));
    distSlider->setText(str.setNum(valDist, 'f', 3));
    mxSlider->setText(str.setNum(valCx, 'f', 3));
    mySlider->setText(str.setNum(valCy, 'f', 3));

    connect(mview1,
            SIGNAL(SIG_thetaRotationChanged(QString)),
            thetaSlider,
            SLOT(setText(QString))); // this, SLOT(SL_ThetaSlider(QString))); //
    connect(thetaSlider, SIGNAL(returnPressed()), this, SLOT(SL_setThetaSlider()));

    connect(mview1,
            SIGNAL(SIG_phiRotationChanged(QString)),
            phiSlider,
            SLOT(setText(QString)));  // this, SLOT(SL_PhiSlider(QString))); //
    connect(phiSlider, SIGNAL(returnPressed()), this, SLOT(SL_setPhiSlider()));

    connect(mview1,
            SIGNAL(SIG_psiRotationChanged(QString)),
            psiSlider,
            SLOT(setText(QString))); // this, SLOT(SL_PsiSlider(QString))); //
    connect(psiSlider, SIGNAL(returnPressed()), this, SLOT(SL_setPsiSlider()));

    connect(mview1, SIGNAL(SIG_distChanged(QString)), distSlider, SLOT(setText(QString)));
    connect(distSlider, SIGNAL(returnPressed()), this, SLOT(SL_setDistSlider()));

    connect(mview1, SIGNAL(SIG_cxChanged(QString)), mxSlider, SLOT(setText(QString)));
    connect(mxSlider, SIGNAL(returnPressed()), this, SLOT(SL_setMxSlider()));

    connect(mview1, SIGNAL(SIG_cyChanged(QString)), mySlider, SLOT(setText(QString)));
    connect(mySlider, SIGNAL(returnPressed()), this, SLOT(SL_setMySlider()));

    if(!connect(mview1, SIGNAL(SIG_actPoint()), this, SLOT(SL_actPoint()))) {
        QMessageBox::warning(this, "PROBLEM", "CONNECTION ERROR");
    }
    SL_setPsiSlider();
}

void MainWindow::SL_setThetaSlider() {
    QString value = thetaSlider->text();
    QString vPs = psiSlider->text();
    QString vPh = phiSlider->text();
    valTh = osg::clampBetween(value.toDouble(), -180., 180.);
//std::cout << "------------------------------" << std::endl << "SL_setThetaSlider  ..... valTh=" << valTh << std::endl;  
    qrot = MiscFunc::quatFromEuler(valTh * 0.01745329251994329576,
                                   valPh * 0.01745329251994329576,
                                   valPs * 0.01745329251994329576); //osg::Quat
    mview1->setQuat(qrot);
//  osg::Quat q5 = mview1->getQuat();//  std::cout << " q5.x=" << q5.x() << " q5.y=" << q5.y() << " q5.z=" << q5.z() << " q5.w=" << q5.w() << std::endl;
    mview1->setDone(true);
    thetaSlider->setText(value);
    phiSlider->setText(vPh);
    psiSlider->setText(vPs);
    mview1->setDone(false);
}

void MainWindow::SL_setPhiSlider() {
    QString value = phiSlider->text();
    QString vTh = thetaSlider->text();
    QString vPs = psiSlider->text();
    valPh = osg::clampBetween(value.toDouble(), -90., 90.);
    qrot = MiscFunc::quatFromEuler(valTh * 0.01745329251994329576,
                                   valPh * 0.01745329251994329576,
                                   valPs * 0.01745329251994329576); //osg::Quat
    mview1->setQuat(qrot);
    mview1->setDone(true);
    thetaSlider->setText(vTh);
    phiSlider->setText(value);
    psiSlider->setText(vPs);
    mview1->setDone(false);
}

void MainWindow::SL_setPsiSlider() {
    QString value = psiSlider->text();
    QString vTh = thetaSlider->text();
    QString vPh = phiSlider->text();
    valPs = value.toDouble() + 90.;
    //std::cout << "------------------------------" << std::endl << "SL_setPsiSlider  ..... valPs=" << valPs << "   ..... vTh=" << vTh.toStdString() << "   ..... vPh=" << vPh.toStdString() << std::endl;
    qrot = MiscFunc::quatFromEuler(valTh * 0.01745329251994329576,
                                   valPh * 0.01745329251994329576,
                                   valPs * 0.01745329251994329576); //osg::Quat
    mview1->setQuat(qrot);
    mview1->setDone(true);
    thetaSlider->setText(vTh);
    phiSlider->setText(vPh);
    psiSlider->setText(value);
    mview1->setDone(false);
}

void MainWindow::SL_setDistSlider() {
    QString value = distSlider->text();
    valDist = value.toDouble();
    std::cout << "SL_setDistSlider  val=" << valDist << "    dist0=" << INT->dist0 << std::endl;
    mview1->setDist(valDist * INT->dist0);
}

void MainWindow::SL_setMxSlider() {
    QString value = mxSlider->text();
    valCx = value.toDouble();
    osg::Vec3d cc = mview1->getCentr();
    double newX = INT->centr0.x() - valCx * INT->scDim;
    mview1->setCentr(osg::Vec3d(newX, cc.y(), cc.z()));
}

void MainWindow::SL_setMySlider() {
    QString value = mySlider->text();
    valCy = value.toDouble();
    osg::Vec3d cc = mview1->getCentr();
    double newZ = INT->centr0.z() - valCy * INT->scDim;
    mview1->setCentr(osg::Vec3d(cc.x(), cc.y(), newZ));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        //qDebug(obj->objectName().toAscii());
        std::cout << " ----name:" << obj->objectName().toStdString() << std::endl;
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

bool MainWindow::handleFormatResult(const FormatResult &result) {
    if(result.hasErrors()) {
        QMessageBox::warning(this, "PROBLEM", ("Error: " + result.getErrorMessage()).c_str());
        return false;
    }
    if(result.hasWarnings()) {
        QMessageBox::warning(this, "PROBLEM", ("Warning: " + result.getWarningMessage()).c_str());
        return true;
    }
    return true;
}

/*

setFocusPolicy(Qt::StrongFocus);

    bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    	if(event->type() == QEvent::KeyPress) {
            qDebug(obj->objectName().toAscii());
    		return true;
    	} else {
    		return QObject::eventFilter(obj, event);
    	}
     }
qApp->installEventFilter(this); 
*/
