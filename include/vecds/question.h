
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
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

#include <vecds/question_form.h>

namespace vecds
{

  class Question
    : 
  public QDialog
  {
    Q_OBJECT
      
      public:

                                 /* constructor */
    Question (QString      title, 
	       QStringList &question, 
	       bool        *results, 
	       bool        &ok,
	       QWidget     *parent = 0);

                                 /* destructor */
    ~Question ();
    
  private:

                                 /* a list of check boxes */
    QCheckBox *check_box[50];
  
                                 /* a pointer to the button box
				    created by this class */
    QDialogButtonBox *buttonBox;
  };

}                                // namespace vecds

#endif

