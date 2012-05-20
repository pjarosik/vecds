
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


                                 // vecds includes
#include <vecds/help_browser.h>

// pomocnicza klasa - przeglądarka plików pomocy (HTML)

HelpBrowser::HelpBrowser (const QString &path, const QString &page, QWidget *parent)
  : 
  QWidget (parent) 
{
  setAttribute (Qt::WA_DeleteOnClose); 
  setAttribute (Qt::WA_GroupLeader); 

                                 // create a help browser and
                                 // navigation buttons.
  textBrowser = new QTextBrowser; 
  homeButton  = new QPushButton (tr ("&Home")); 
  backButton  = new QPushButton (tr ("&Back")); 
  closeButton = new QPushButton (tr ("Close")); 

  closeButton->setShortcut (tr ("Escape")); 

  QHBoxLayout *buttonLayout = new QHBoxLayout; 
  buttonLayout->addWidget(homeButton); 
  buttonLayout->addWidget(backButton); 
  buttonLayout->addStretch(); 
  buttonLayout->addWidget(closeButton); 

  QVBoxLayout *mainLayout = new QVBoxLayout; 

  mainLayout->addLayout(buttonLayout); 
  mainLayout->addWidget(textBrowser); 

  setLayout (mainLayout); 

  connect (homeButton,  SIGNAL (clicked ()), textBrowser, SLOT (home ())); 
  connect (backButton,  SIGNAL (clicked ()), textBrowser, SLOT (backward ())); 

  connect (closeButton, SIGNAL (clicked ()),                   this, SLOT (close ())); 
  connect (textBrowser, SIGNAL (sourceChanged (const QUrl &)), this, SLOT (updateWindowTitle ())); 

                                 // path to images
  textBrowser->setSearchPaths (QStringList() << path << ":/images"); 
  textBrowser->setSource (page); 
}

HelpBrowser::~HelpBrowser ()
{}

void HelpBrowser::updateWindowTitle () 
{ 
  setWindowTitle (tr ("Help: %1").arg (textBrowser->documentTitle ())); 
}

void HelpBrowser::show_page (const QString &page) 
{ 
  QString path = QApplication::applicationDirPath () + "/VECDS_internal/doc"; 

  HelpBrowser *browser = new HelpBrowser (path, page); 
  browser->resize (500, 400); 
  browser->show (); 
}


