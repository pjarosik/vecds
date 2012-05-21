
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
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


#include <QString>
#include <QStringList>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

                                 // vecds includes
#include <vecds/dialogs.h>
#include <vecds/main_window.h>


                                 // Pomocnicza klasa - okienka
                                 // dialogowe

                                 // Class Question forms.
vecds::QuestionForm::QuestionForm (QString      title, 
				   QString     &description, 
				   QStringList &question, 
				   QStringList &suggestion, 
				   QStringList &answer, 
				   bool        &ok, 
				   QWidget     *parent)
  : 
  QDialog (parent)
{
  ok = false;

                                 // count the number of questions we
                                 // have to deal with.
  const unsigned int n_questions = question.count ();

                                 // set title to this title
  setWindowTitle (title);

                                 // set description if none is
                                 // available, set the style of the
                                 // description too.
  if (!description.isEmpty ()) 
    {
      description_label = new QLabel (description);

      description_label->setFrameStyle (QFrame::NoFrame);
      description_label->setTextFormat (Qt::RichText);
    }

                                 // get a list of questions and
                                 // suggestions.
  for (unsigned int i=0; i<n_questions; ++i)  
    {
      question_label[i] = new QLabel (question.at (i));
      qEdit[i]          = new QLineEdit (suggestion.at (i));

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

  if (!description.isEmpty()) 
    mainLayout->addWidget (description_label);

  mainLayout->addLayout (layout);
  mainLayout->addWidget (buttonBox);

  setLayout (mainLayout);
  
  this->exec ();
  
                                 // check if we have a good result.
  if (this->result ()) 
    {
      ok = true;
      for (unsigned int i=0; i<n_questions; i++) 
	answer << qEdit[i]->text();
    }
  else 
    {
      ok = false;
      return;
    }
  
  if (!description.isEmpty ()) 
    delete description_label;

  for (unsigned int i=0; i<n_questions; ++i)      
    {
      qWarning ("question nr %d - answer : %s", i, answer.at (i).toAscii ().data ());
      delete qEdit[i];
      delete question_label[i];
    }

  delete buttonBox;
}

                                 // destructor
vecds::QuestionForm::~QuestionForm ()
{}

// -----------   class Questions  ----------------

vecds::Questions::Questions (QString      title, 
			     QStringList &question, 
			     bool        *results, 
			     bool        &ok, 
			     QWidget     *parent) 
  : 
  QDialog (parent)
{
  ok = false;
  const unsigned int n_questions = question.count ();

                                 // set title to this title
  setWindowTitle (title);

  for (unsigned int i=0; i<n_questions; ++i)  
    {
      check_box[i] = new QCheckBox (question.at (i));
      check_box[i]->setChecked (results[i]);
    }
  
  buttonBox = new QDialogButtonBox;
  buttonBox->addButton (tr ("Accept"), QDialogButtonBox::AcceptRole);
  buttonBox->addButton (tr ("Reject"), QDialogButtonBox::RejectRole);

  connect (buttonBox, SIGNAL (accepted ()), this, SLOT (accept ()));
  connect (buttonBox, SIGNAL (rejected ()), this, SLOT (reject ()));

  QVBoxLayout *layout = new QVBoxLayout;

  for (unsigned int i=0; i<n_questions; ++i)
    layout->addWidget(check_box[i]);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  
  mainLayout->addLayout (layout);
  mainLayout->addWidget (buttonBox);

  setLayout(mainLayout);
  
  this->exec ();
  
  if (this->result()==1)
    {
      ok = true;
      for (unsigned int i=0; i<n_questions; ++i) 
	results[i] = check_box[i]->isChecked ();
    }
  else 
    {
      ok = false;
      return;
    }
  
  for (unsigned int i=0; i<n_questions; ++i)
    delete check_box[i];
  
  delete buttonBox;
}

                                 // destructor
vecds::Questions::~Questions ()
{}
