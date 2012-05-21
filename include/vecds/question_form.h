
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

#ifndef QUESTION_FORM_H
#define QUESTION_FORM_H

                                 // Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QCheckBox>


                                 // vecds includes
#include <vecds/main_window.h>

namespace vecds
{

  class QuestionForm 
    : 
  public QDialog
  {
    Q_OBJECT
      
      public:

                                 /* constructor */
    QuestionForm (QWidget *parent = 0);
    
                                 /* set the title of this question */
    void set_title (QString &title);

                                 /* set the question(s) for this
				    question */
    void set_question_list (QStringList &question);

                                 /* set the suggestions(s) for this
				    question */
    void set_suggestion_list (QStringList &suggestions);

                                 /* set the suggestions(s) for this
				    question */
    void set_description (QString &description);

                                 /* show the question?
				    get_the_results? */
    void show_question (QStringList &results);

                                 /* return the vaiable "check". Note:
				    it needs to be careful, because
				    "tru" does not necessarily mean
				    *everything* is ok.  */
    bool is_sane ();
  
                                 /* destructor */
    ~QuestionForm ();

  private:

                                 /* label descriptions */
    QLabel *description_label;

                                 /* label a list of questions */
    QLabel *question_label[50];

                                 /* TODO: a list of somethings */
    QLineEdit *qEdit[50];
  
                                 /* a pointer to the button box
				    created by this class */
    QDialogButtonBox *buttonBox;


                                 /* variable holding the title of this
				    question form */
    QString question_title;

                                 /* variable holding a list of
				    questions of this question form */
    QStringList question_list;

                                 /* variable holding a list of
				    suggestions of this question form */
    QStringList suggestion_list;

                                 /* variable holding the description
				    of this question form */
    QString question_description;

                                 /* A simple bool for checking. If
				    operations are ok, this returns
				    true, otherwise false is
				    retured. TODO: This needs to be
				    done in a much more intelligent
				    way...*/
    bool check;
  };                             // QuestionForm

}                                // namespace vecds
  
#endif
