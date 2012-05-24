
// -------------------------------------------------------------------
//
// Author: 
//    Toby D. Young 2012.
//
// Copyright (C) 2012 The vecds authors
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

#ifndef MONOLOG_BASE_H
#define MONOLOG_BASE_H

                                 /* standard C, C++ */
#include <cassert>


                                 /* first the usual set of Qt crap */
#include <QtGui>
#include <QWidget> 

                                 /* vecds includes */
#include <vecds/config.h>

                                 /* forward declarations of qt
				    clases */
class QPushButton;
class QTextBrowser;

namespace vecds
{

                                 /* This class represents a basic
				    monolog base from which browser
				    classes can be constructed. The
				    idea here is to create a window
				    that displays some text and maybe
				    some figures too based (mostly) on
				    html input files. No interaction
				    is required or wanted, and
				    therefore this is all very simple
				    (ie. it is a monolog). */
  class MonologBase 
    : 
  public QWidget
  {
    Q_OBJECT 
      
      public: 
  
                                 /* constructor of the main clas, just
				    create a pointer to the parent */
    MonologBase (QWidget *parent = 0);
  
                                 /* destructor of the main class */
    virtual ~MonologBase ();
  
  protected: 

                                 /* TODO: some kind of parent
				    widget(?) */
    QWidget      *parent_widget;

                                 /* the browser itself */
    QTextBrowser *text_browser;

                                 /* string denoting a generic path */
    QString       path;

                                 /* standard browser-type buttons that
				    are typical when monologs are
				    going on */
    QPushButton  *home_button;
    QPushButton  *back_button;
    QPushButton  *close_button;

  public:

                                 /* create data (eg. geometry, styles,
				    etc.) for a browser window. */  
    virtual void init_window (const QString &page);

                                 /* Display a page */  
    virtual void show_page (const QString &page);


  };                             /* MonologBase */

}                                /* namespace vecds */

#endif
