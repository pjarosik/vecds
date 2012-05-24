
// -------------------------------------------------------------------
//
// Author: 
//    Toby D. Young 2012.
//
// Copyright (C) 2012 The vecds authors
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

                                 /* C, C++ includes */
#include <iostream>

                                 /* vecds includes */
#include <vecds/dialogs/monolog_base.h>

                                 // constructor
vecds::MonologBase::MonologBase (QWidget *parent)
  :
  parent_widget (parent),
  path (VECDS_DOCS)
{}

                                 // destructor
vecds::MonologBase::~MonologBase ()
{}

                                 // initialise a "doc browser" window,
                                 // except we can not do that in a
                                 // virtual function of this kind. So
                                 // just throw an error.
void
vecds::MonologBase::init_window (const QString &/*page*/)
{
  qWarning ("class MonologBase: Fatal error, pure virtual function called");
  assert (false);
}

                                 // the procedure used to show a page
                                 // in a window.
void
vecds::MonologBase::show_page (const QString &page) 
{ 
  this->init_window (page); 
  this->resize (500, 400); 

                                 // display page.
  this->show (); 
}

