
// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski and Pawel Dluzewski 2010.
//    Toby D. Young 2012.
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
#include <vecds/question_form.h>



                                 // Pomocnicza klasa - okienka
                                 // dialogowe

                                 // Constructor.
vecds::QuestionForm::QuestionForm (QWidget *parent)
  : 
  QDialog         (parent),
  check           (false),
  question_title  ("Unknown title"),
  question_list   ("No questions"),
  suggestion_list ("No suggestions")
{}


                                 // destructor
vecds::QuestionForm::~QuestionForm ()
{}



void
vecds::QuestionForm::show_question (QStringList &answer)
{

                                 // be paranoid! Always set this to
                                 // false before going on.
  this->check = false;

                                 // count the number of questions we
                                 // have to deal with.
  const unsigned int n_questions = this->question_list.count ();

                                 // set title to this title
  setWindowTitle (this->question_title);

                                 // set description if none is
                                 // available, set the style of the
                                 // description too.
  if (!this->question_description.isEmpty ()) 
    {
      description_label = new QLabel (this->question_description);

      description_label->setFrameStyle (QFrame::NoFrame);
      description_label->setTextFormat (Qt::RichText);
    }

                                 // get a list of questions and
                                 // suggestions.
  for (unsigned int i=0; i<n_questions; ++i)  
    {
      question_label[i] = new QLabel (this->question_list.at (i));
      qEdit[i]          = new QLineEdit (this->suggestion_list.at (i));

      question_label[i]->setFrameStyle (QFrame::NoFrame);
      question_label[i]->setTextFormat (Qt::RichText);
    }
  
                                 // create a button box.
  buttonBox = new QDialogButtonBox;
  buttonBox->addButton (tr ("Accept"), QDialogButtonBox::AcceptRole);
  buttonBox->addButton (tr ("Reject"), QDialogButtonBox::RejectRole);

                                 // signal/slots for user interaction.
  connect (buttonBox, SIGNAL (accepted ()), this, SLOT (accept ()));
  connect (buttonBox, SIGNAL (rejected ()), this, SLOT (reject ()));
  
  QFormLayout *layout = new QFormLayout;

  for (unsigned int i=0; i<n_questions; ++i) 
    layout->addRow(question_label[i], qEdit[i]);

  QVBoxLayout *mainLayout = new QVBoxLayout;

  if (!this->question_description.isEmpty()) 
    mainLayout->addWidget (description_label);

  mainLayout->addLayout (layout);
  mainLayout->addWidget (buttonBox);

  setLayout (mainLayout);
  
  this->exec ();
  
                                 // check if we have a good result.
  if (this->result ()) 
    {
      for (unsigned int i=0; i<n_questions; i++) 
	answer << qEdit[i]->text();

      this->check = true;
    }
  else 
    {
      this->check = false;
      return;
    }
  
  if (!this->question_description.isEmpty ()) 
    delete description_label;

  for (unsigned int i=0; i<n_questions; ++i)      
    {
      qWarning ("question nr %d - answer : %s", i, answer.at (i).toAscii ().data ());
      delete qEdit[i];
      delete question_label[i];
    }

  delete buttonBox;
}


                                 // set title of this question
void
vecds::QuestionForm::set_title (QString &title) 
{
  this->question_title = title;
}

                                 // set question of this question
void
vecds::QuestionForm::set_question_list (QStringList &questions) 
{
  this->question_list = questions;
}

                                 // set suggestions of this question
void
vecds::QuestionForm::set_suggestion_list (QStringList &suggestions) 
{
  this->suggestion_list = suggestions;
}

                                 // set description of this question
void
vecds::QuestionForm::set_description (QString &description) 
{
  this->question_description = description;
}

                                 // return the value of "check"
bool 
vecds::QuestionForm::is_sane () 
{
  return this->check;
}

