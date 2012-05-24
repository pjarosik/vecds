
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



                                 // vecds includes
#include <vecds/dialogs/dialog_base.h>



                                 // Pomocnicza klasa - okienka
                                 // dialogowe
                                 // 
                                 // The constructor sets up a
                                 // completely empty and unchecked box
                                 // which can be drawn later.
vecds::DialogBase::DialogBase (QWidget *parent) 
  : 
  QDialog         (parent),
  dialog_title    ("Unknown title"),
  question_list   ("Unknown question"),
  suggestion_list ("No suggestions")
{}


                                 // destructor
vecds::DialogBase::~DialogBase ()
{}

                                 // set title for this dialog
void
vecds::DialogBase::set_title (QString &title) 
{
  this->dialog_title = title;
}

                                 // set description of this question
void
vecds::DialogBase::set_description (QString &description) 
{
  this->dialog_description = description;
}

                                 // set question(s) for this dialog
void
vecds::DialogBase::set_question_list (QStringList &questions) 
{
  this->question_list = questions;
}

                                 // set suggestion(s) for this dialog
void
vecds::DialogBase::set_suggestion_list (QStringList &suggestions) 
{
  this->suggestion_list = suggestions;
}

                                 // return the value of "check"
bool 
vecds::DialogBase::is_sane () 
{
  return this->check;
}
