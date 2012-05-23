
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010), Toby D. Young
// (2012) 
//
// Copyright (C) 2010, 2012 The vecds authors
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


#include <iostream>

                                 // vecds includes
#include <vecds/doc_browser.h>

                                 // pomocnicza klasa - przeglądarka
                                 // plików pomocy (HTML).

                                 // constructor
vecds::DocBrowser::DocBrowser (QWidget *parent)
  :
  parent_widget (parent),
  doc_path (VECDS_DOCS)
{}

                                 // constructor
vecds::DocBrowser::~DocBrowser ()
{}

                                 // initialise a "doc browser" window
                                 // where documentation can be shown.
void
vecds::DocBrowser::init_window (const QString &page)
{
  setAttribute (Qt::WA_DeleteOnClose); 
  setAttribute (Qt::WA_GroupLeader); 

                                 // create a doc browser and
                                 // navigation buttons.
  text_browser = new QTextBrowser; 
  home_button  = new QPushButton (tr ("&Home")); 
  back_button  = new QPushButton (tr ("&Back")); 
  close_button = new QPushButton (tr ("Close")); 

                                 // pseudonom for close is escape.
  close_button->setShortcut (tr ("Escape")); 

                                 // fix button layout (horizontal).
  QHBoxLayout *button_layout = new QHBoxLayout; 
  button_layout->addStretch (); 
  button_layout->addWidget (home_button); 
  button_layout->addWidget (back_button); 
  button_layout->addWidget (close_button); 

                                 // fix button layout (vertical).
  QVBoxLayout *main_layout = new QVBoxLayout; 
  main_layout->addLayout (button_layout); 
  main_layout->addWidget(text_browser); 

                                 // assign layout specified above.
  setLayout (main_layout); 

                                 // signals and slots giving actions
                                 // to buttons.
  connect (home_button,  SIGNAL (clicked ()), text_browser, SLOT (home ())); 
  connect (back_button,  SIGNAL (clicked ()), text_browser, SLOT (backward ())); 

  connect (close_button, SIGNAL (clicked ()),                   this, SLOT (close ())); 
  connect (text_browser, SIGNAL (sourceChanged (const QUrl &)), this, SLOT (updateWindowTitle ())); 

                                 // path to images
  text_browser->setSearchPaths (QStringList() << doc_path << ":/images"); 
  text_browser->setSource (page); 
}

void 
vecds::DocBrowser::updateWindowTitle () 
{ 
  setWindowTitle (tr ("Doc: %1").arg (text_browser->documentTitle ())); 
}

                                 // the procedure used to show a page
                                 // in a window.
void 
vecds::DocBrowser::show_page (const QString &page) 
{ 
  this->init_window (page); 
  std::cout << "   Init done." << std::endl;

  this->resize (500, 400); 
  std::cout << "   Resize done." << std::endl;

                                 // display page.
  this->show (); 
}

