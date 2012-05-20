
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
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

#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H

                                 /* first the usual set of Qt crap */
#include <QtGui>
#include <QWidget> 

                                 /* vecds includes */
#include <vecds/config.h>

                                 /* forward declarations of qt
				    clases */
class QPushButton;
class QTextBrowser;

class HelpBrowser 
: 
public QWidget
{
  Q_OBJECT 
    
    public: 
  
                                 /* constructor of the main clas, just
				    create a pointer to the parent */
  HelpBrowser (QWidget *parent = 0);
  
                                 /* destructor of the main class */
  ~HelpBrowser ();
  
  void create_browser (const QString &page);
  
  void show_page (const QString &page);
  
  private slots: 
  
  void updateWindowTitle ();
  
 private: 

  QWidget *parent_widget;
  QString  help_path;   

  QTextBrowser *textBrowser;
  QPushButton *homeButton;
  QPushButton *backButton;
  QPushButton *closeButton;
};

#endif
