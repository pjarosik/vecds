
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

#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

                                 // Qt includes
#include <cassert>

                                 // Qt includes
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace vecds
{
                                 /* Dialogs, questions forms, and so
				    on quite often have exactly the
				    same structure. The
				    <code>DialogBase</code> class
				    provides a basis on which derived
				    classes can be built in a
				    simplified way.*/
  class DialogBase
    : 
  public QDialog
  {
    Q_OBJECT
      
      public:

                                 /* constructor */
    DialogBase (QWidget *parent = 0);

                                 /* destructor */
    virtual ~DialogBase ();

  protected:

                                 /* variable holding the title of this
				    fialog */
    QString dialog_title;

                                 /* variable holding the description
				    of this dialog */
    QString dialog_description;

                                 /* variable holding a list of
				    questions for this dialog */
    QStringList question_list;

                                 /* variable holding a list of
				    suggestions of this dialog */
    QStringList suggestion_list;

  
                                 /* a pointer to the button box
				    created by this class */
    QDialogButtonBox *button_box;

                                 /* A simple bool for checking. If
				    operations are ok, this returns
				    true, otherwise false is
				    retured. TODO: This needs to be
				    done in a much more intelligent
				    way...*/
    bool check;

  public:

                                 /* set the title of this question */
    virtual void set_title (QString &title);

                                 /* set the description of this
				    dialog */
    virtual void set_description (QString &description);

                                 /* set the question(s) for this
				    dialog */
    virtual void set_question_list (QStringList &question);

                                 /* set the suggestions(s) for this
				    dialog */
    virtual void set_suggestion_list (QStringList &suggestions);


                                 /* return the vaiable "check". Note:
				    it needs to be careful, because
				    "tru" does not necessarily mean
				    *everything* is ok.  */
    virtual bool is_sane ();

  };

}                                // namespace vecds

#endif

