
// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski and Pawel Dluzewski 2010.
//    Toby D. Young 2012.
//
// Copyright (C) 2010 The vecds authors
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

#ifndef QUESTION_H
#define QUESTION_H

                                 /* Qt includes */
#include <QCheckBox>

                                 /* Class Question inherets from
				    DialogBase. */
#include <vecds/dialog_base.h>

namespace vecds
{

  class Question
    : 
  public DialogBase
  {
    Q_OBJECT
      
      public:

                                 /* constructor */
    Question (QWidget *parent = 0);

                                 /* destructor */
    ~Question ();

                                 /* show the question? */
    void show_question (bool *results);

  private:

                                 /* a list of check boxes */
    QCheckBox *check_box[50];

  };                             /* Question */

}                                /* namespace vecds */

#endif

