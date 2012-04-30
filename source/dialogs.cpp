
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along      with      this      program.       If      not,      see
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
#include "../include/vecds/dialogs.h"
#include "../include/vecds/main_window.h"

//bool ok;

// -----------   class QuestionForm   --------------
// Pomocnicza klasa - okienka dialogowe

QuestionForm::QuestionForm (QString title, QString descr, 
			    QStringList quest, QStringList sug, QStringList &ans, 
			    bool &ok, 
			    QWidget *parent)
  : QDialog (parent)
{
  ok = false;
  int num_quest = quest.count();
  setWindowTitle(title);
  if ( !(descr.isEmpty()) ) 
    {
      lab0 = new QLabel(descr);
      lab0->setFrameStyle(QFrame::NoFrame);
      lab0->setTextFormat(Qt::RichText);
    }
  for (int i=0; i<num_quest; i++)  
    {
      qEdit[i] = new QLineEdit(sug.at(i));
      lab[i] = new QLabel(quest.at(i));
      lab[i]->setFrameStyle(QFrame::NoFrame);
      lab[i]->setTextFormat(Qt::RichText);
    }
  
  buttonBox = new QDialogButtonBox;
  buttonBox->addButton(tr("Accept"), QDialogButtonBox::AcceptRole);
  buttonBox->addButton(tr("Reject"), QDialogButtonBox::RejectRole);
  //   buttonBox->addButton(QDialogButtonBox::Reset);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  
  QFormLayout *layout = new QFormLayout;
  for (int i=0; i<num_quest; i++) layout->addRow(lab[i], qEdit[i]);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  if ( !(descr.isEmpty()) ) mainLayout->addWidget(lab0);
  mainLayout->addLayout(layout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);
  
  this->exec();
  
  if ( this->result()==1 ) 
    {
      ok = true;
      for (int i=0; i<num_quest; i++) 
	ans << qEdit[i]->text();
    }
  else 
    {
      ok = false;
      return;
    }
  
  if ( !(descr.isEmpty()) ) 
    delete lab0;

  for (int i=0; i<num_quest; i++)  
    {
      qWarning("quest nr %d - ans : %s", i, ans.at(i).toAscii().data() );
      delete qEdit[i];
      delete lab[i];
    }
  delete buttonBox;
}

// -----------   class Questions  ----------------

Questions::Questions (QString title, 
		      QStringList quest, 
		      bool *results, bool &ok, 
		      QWidget *parent) 
  : QDialog (parent)
{
  int num_quest = quest.count();
  setWindowTitle(title);

  for (int i=0; i<num_quest; i++)  
    {
      lab[i] = new QCheckBox(quest.at(i));
      lab[i]->setChecked(results[i]);
    }
  
  buttonBox = new QDialogButtonBox;
  buttonBox->addButton(tr("Accept"), QDialogButtonBox::AcceptRole);
  buttonBox->addButton(tr("Reject"), QDialogButtonBox::RejectRole);
  //   buttonBox->addButton(QDialogButtonBox::Reset);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  
  QVBoxLayout *layout = new QVBoxLayout;
  for (int i=0; i<num_quest; i++) 
    layout->addWidget(lab[i]);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  
  mainLayout->addLayout(layout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);
  
  this->exec();
  ok = true;
  
  if ( this->result()==1 )
    for (int i=0; i<num_quest; i++) 
      results[i]= lab[i]->isChecked();
  else 
    {
      ok = false;
      return;
    }
  
  for (int i=0; i<num_quest; i++)
    delete lab[i];

  delete buttonBox;
}
