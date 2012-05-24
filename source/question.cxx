
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010), Toby D. Young
// (2012).
//
// Copyright (C) 2010, 2012 The vecds authors
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

                                 // Qt includes
#include <QString>
#include <QStringList>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

                                 // vecds includes
#include <vecds/question.h>
#include <vecds/dialog_base.h>


                                 // Pomocnicza klasa - okienka
                                 // dialogowe
                                 // 
                                 // The constructor sets up a
                                 // completely empty and unchecked box
                                 // which can be drawn later.
vecds::Question::Question (QWidget *parent) 
  : 
  DialogBase (parent)
{}


                                 // destructor
vecds::Question::~Question ()
{}


                                 // TODO: this is probably badly
                                 // named, but I can't think of what
                                 // else to call it. (TDY)
void
vecds::Question::show_question (bool *results)
{
                                 // set this to false
  this->check = false;

                                 // get the number of questions we
                                 // have to deal with
  const unsigned int n_questions = this->question_list.count ();

                                 // to make sense, at least one
                                 // question must exist
  assert (n_questions!=0);

                                 // set title to this title
  setWindowTitle (this->dialog_title);

                                 // put all questions into their own
                                 // little check box
  for (unsigned int i=0; i<n_questions; ++i)  
    {
      check_box[i] = new QCheckBox (this->question_list.at (i));
      check_box[i]->setChecked (results[i]);
    }
  
                                 // accept and reject buttons
  button_box = new QDialogButtonBox;
  button_box->addButton (tr ("Accept"), QDialogButtonBox::AcceptRole);
  button_box->addButton (tr ("Reject"), QDialogButtonBox::RejectRole);

  connect (button_box, SIGNAL (accepted ()), this, SLOT (accept ()));
  connect (button_box, SIGNAL (rejected ()), this, SLOT (reject ()));

                                 // set up the layout for the local
                                 // components (check_boxes) and the
                                 // main layout.
  QVBoxLayout *local_layout = new QVBoxLayout;
  QVBoxLayout *main_layout  = new QVBoxLayout;

  for (unsigned int i=0; i<n_questions; ++i)
    local_layout->addWidget (check_box[i]);
  
  main_layout->addLayout (local_layout);
  main_layout->addWidget (button_box);

                                 // fix the layout as it is.
  setLayout (main_layout);
  
                                 // execute (something)
  this->exec ();
  
  if (this->result ()==1)
    {
      for (unsigned int i=0; i<n_questions; ++i) 
	results[i] = check_box[i]->isChecked ();
      this->check = true;
    }
  else 
    {
      this->check = false;
      return;
    }
  
  for (unsigned int i=0; i<n_questions; ++i)
    delete check_box[i];
  
  delete button_box;
}



