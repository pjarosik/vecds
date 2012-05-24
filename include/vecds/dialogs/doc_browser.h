
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010), Toby D. Young
// (2012)
//
// Copyright (C) 2010, 2012 The vecds authors, Toby D. Young
//
// This program is free software: you can redistribute it and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#ifndef DOC_BROWSER_H
#define DOC_BROWSER_H

                                 /* Qt includes */
#include <QtGui>
#include <QWidget> 

                                 /* vecds includes */
#include <vecds/dialogs/monolog_base.h>

                                 /* forward declarations of qt
				    clases */
class QPushButton;
class QTextBrowser;

namespace vecds
{

                                 /* This class represents a basic
				    documentation browser. The idea
				    here is to create a window that
				    displays some text and maybe some
				    figures too. This is almost
				    identical to the HelpBrowser
				    class, so we should think about
				    adding virtual classes to the
				    browser section of the code. */
  class DocBrowser 
    : 
  public MonologBase
  /* public QWidget */
  {
    Q_OBJECT 
      
      public: 
  
                                 /* constructor of the main clas, just
				    create a pointer to the parent */
    DocBrowser (QWidget *parent = 0);
  
                                 /* destructor of the main class */
    ~DocBrowser ();

                                 /* create data (eg. geometry, styles,
				    etc.) for a browser window. */  
    void init_window (const QString &page);
  
                                 /* TODO: display a page? */  
    void show_page (const QString &page);
  
    private slots: 
    
    void updateWindowTitle ();
  
  private: 

                                 /* TODO: some kind of parent
				    widget(?) */
    QWidget      *parent_widget;

                                 /* the browser itself */
    QTextBrowser *text_browser;

                                 /* string denoting path to the
				    documentation */
    QString       doc_path;

                                 /* standard browser-type buttons */
    QPushButton  *home_button;
    QPushButton  *back_button;
    QPushButton  *close_button;
  };                             /* DocBrowser */

}                                /* namespace vecds */

#endif
