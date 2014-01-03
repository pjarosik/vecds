
// -------------------------------------------------------------------
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


#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QCheckBox>

/**
 * Question forms
 */
class QuestionForm : public QDialog
{
  Q_OBJECT
    
    public:
  QuestionForm (QString title, QString descr, 
		QStringList quest, QStringList sug, QStringList &ans, 
		bool &ok, 
		QWidget *parent = 0);
  
 private:
  QLabel *lab0;
  QLabel *lab[50];
  QLineEdit *qEdit[50];
  QDialogButtonBox *buttonBox;
};

class Questions : public QDialog
{
  Q_OBJECT
    
    public:
  Questions (QString title, 
	     QStringList quest, 
	     bool *results, 
	     bool &ok,
	     QWidget *parent = 0);
  
 private:
  QCheckBox *lab[50];
  QDialogButtonBox *buttonBox;
};

#endif
