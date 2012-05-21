
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



                                 // Pomocnicza klasa - okienka
                                 // dialogowe
vecds::Question::Question (QString      title, 
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
vecds::Question::~Question ()
{}
