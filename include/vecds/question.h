
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
    Question (QWidget     *parent = 0);

                                 /* destructor */
    ~Question ();
    
                                 /* set the title of this question */
    void set_title (QString &title);

                                 /* set the question(s) for this
				    question */
    void set_question_list (QStringList &question);

                                 /* show the question? */
    void show_question (bool *results);

                                 /* return the vaiable "check". Note:
				    it needs to be careful, because
				    "tru" does not necessarily mean
				    *everything* is ok.  */
    bool is_sane ();

  private:

                                 /* a list of check boxes */
    QCheckBox *check_box[50];
  
                                 /* a pointer to the button box
				    created by this class */
    QDialogButtonBox *buttonBox;

                                 /* variable holding the title of this
				    question */
    QString question_title;

                                 /* variable holding a list of
				    questions of this question */
    QStringList question_list;

                                 /* A simple bool for checking. If
				    operations are ok, this returns
				    true, otherwise false is
				    retured. TODO: This needs to be
				    done in a much more intelligent
				    way...*/
    bool check;
  };

}                                // namespace vecds

#endif

