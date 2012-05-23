
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


#include <iostream>

                                 // vecds includes
#include <vecds/browser_base.h>

                                 // pomocnicza klasa - przeglądarka
                                 // plików pomocy (HTML).

                                 // constructor
vecds::BrowserBase::BrowserBase (QWidget *parent)
  :
  parent_widget (parent),
  path (VECDS_DOCS)
{}

                                 // destructor
vecds::BrowserBase::~BrowserBase ()
{}
