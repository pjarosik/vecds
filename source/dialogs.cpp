#include "dialogs.h"
#include "internal.h"

extern Internal *INT;
// Pomocnicze klasy - okienka dialogowe

QuestionForm1::QuestionForm1(QString title, QString descr, QStringList quest,
        QStringList sug, QStringList &ans, QWidget *parent) : QDialog(parent)
{
   int numQuest = quest.count();
   setWindowTitle(title);
   //setAttribute(Qt::WA_DeleteOnClose);//   this->setStyleSheet("QLineEdit { background-color: yellow }");
   for (int i=0; i<numQuest; i++)  {
      qEdit.push_back(new QLineEdit(sug.at(i)));
      QLabel *label = new QLabel(quest.at(i));
      label->setFrameStyle(QFrame::NoFrame);
      label->setTextFormat(Qt::RichText);
      labels.push_back(label);
   }
   QDialogButtonBox *buttonBox = new QDialogButtonBox;
   buttonBox->addButton(tr("Accept"), QDialogButtonBox::AcceptRole);
   buttonBox->addButton(tr("Reject"), QDialogButtonBox::RejectRole);
//   buttonBox->addButton(QDialogButtonBox::Reset);
   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

   QVBoxLayout *mainLayout = new QVBoxLayout;
   if ( !(descr.isEmpty()) ) {
      QLabel *lab0 = new QLabel(descr);
      lab0->setFrameStyle(QFrame::NoFrame);
      lab0->setTextFormat(Qt::RichText);
      mainLayout->addWidget(lab0);
   }
   QFormLayout *layout = new QFormLayout;
   for (int i=0; i<numQuest; i++) layout->addRow(labels.at(i), qEdit.at(i));
   mainLayout->addLayout(layout);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);

   this->exec();
   ok = true;
   
   if ( this->result()==1 )
      for (int i=0; i<numQuest; i++) ans << qEdit.at(i)->text();
   else                     ok = false;
// std::cout << "SL_genAtoms **3" << std::endl; //if ( !(descr.isEmpty()) ) delete lab0;
   qEdit.clear();
   labels.clear();// std::cout << "SL_genAtoms **4" << std::endl;
}

QuestionForm2::QuestionForm2(QString title, QString descr, QStringList quest,
               QStringList sug, QStringList &ans, QStringList combo, int indC, QWidget *parent) : QDialog(parent)
{
   indCombo = -1;  
   int numQuest = quest.count();
   setWindowTitle(title);
   //setAttribute(Qt::WA_DeleteOnClose);
   if ( !(descr.isEmpty()) ) {
      QLabel *lab0 = new QLabel(descr);
      lab0->setFrameStyle(QFrame::NoFrame);
      lab0->setTextFormat(Qt::RichText);
   }
   for (int i=0; i<numQuest; i++)  {
      qEdit.push_back(new QLineEdit(sug.at(i)));
      QLabel *label = new QLabel(quest.at(i));
      label->setFrameStyle(QFrame::NoFrame);
      label->setTextFormat(Qt::RichText);
      labels.push_back(label);
   }
   QDialogButtonBox *buttonBox = new QDialogButtonBox;
   buttonBox->addButton(tr("Accept"), QDialogButtonBox::AcceptRole);
   buttonBox->addButton(tr("Reject"), QDialogButtonBox::RejectRole);
//   buttonBox->addButton(QDialogButtonBox::Reset);
   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

   QFormLayout *layout = new QFormLayout;
   for (int i=0; i<numQuest; i++) layout->addRow(labels.at(i), qEdit.at(i));
   QVBoxLayout *mainLayout = new QVBoxLayout;
   if ( !(descr.isEmpty()) ) mainLayout->addWidget(lab0);
   QComboBox *comboB = new QComboBox;
   comboB->addItem(combo.at(0));//("No axes");
   comboB->addItem(combo.at(1));//("Axes at the center");
   comboB->addItem(combo.at(2));//("Axes at the corner");
   indCombo = indC;//INT->axInd;
   comboB->setCurrentIndex(indCombo); //(INT->axInd);
   connect(comboB, SIGNAL(activated(int)), this, SLOT(res(int)));
   mainLayout->addLayout(layout);
   mainLayout->addWidget(comboB);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);

   this->exec();
   ok = true;
   
   if ( this->result()==1 )
      for (int i=0; i<numQuest; i++) ans << qEdit.at(i)->text();
   else                     ok = false;

   if ( !(descr.isEmpty()) ) delete lab0;
   qEdit.clear();
   labels.clear();
}

void QuestionForm2::res(int i)
{
   indCombo = i;
}

OneQuestion::OneQuestion(QString title, QString descr, QString quest, QString sug, QString &ans, QWidget *parent) : QDialog(parent)
{
//  qWarning("oneQ 0");
   setWindowTitle(title);
  // setAttribute(Qt::WA_DeleteOnClose);
   if ( !(descr.isEmpty()) ) {
      lab0 = new QLabel(descr);
      lab0->setFrameStyle(QFrame::NoFrame);
      lab0->setTextFormat(Qt::RichText);
   }
   QLineEdit *qEdit = new QLineEdit(sug);
   QLabel *label = new QLabel(quest);
   label->setFrameStyle(QFrame::NoFrame);
   label->setTextFormat(Qt::RichText);
   QDialogButtonBox *buttonBox = new QDialogButtonBox;
   buttonBox->addButton(tr("Accept"), QDialogButtonBox::AcceptRole);
   buttonBox->addButton(tr("Reject"), QDialogButtonBox::RejectRole);
//   buttonBox->addButton(QDialogButtonBox::Reset);
   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

   QFormLayout *layout = new QFormLayout;
   layout->addRow(label, qEdit);
   QVBoxLayout *mainLayout = new QVBoxLayout;
   if ( !(descr.isEmpty()) ) mainLayout->addWidget(lab0);
   mainLayout->addLayout(layout);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);
   this->exec();
   ok = true;
   
   if ( this->result()==1 ) ans = qEdit->text();
   else                     ok = false;
   if ( !(descr.isEmpty()) ) delete lab0;
}

class MyComboBox: public QComboBox
{
  Q_OBJECT
  public:    
	MyComboBox(QWidget* parent):QComboBox(parent)
	{
	  this->setParent(parent);
	  connect(this , SIGNAL(currentIndexChanged(int)),this,SLOT(handleSelectionChanged(int)));
          result = -1;
	};
	~ MyComboBox(){};
        int result;	
	
  public slots:
	//Slot that is called when QComboBox selection is changed
	void handleSelectionChanged(int index)
	{
	    QMessageBox* msg = new QMessageBox();
            result = index;
	    msg->setText(itemText(index));//"Selected Index is :"+QString::number(index));	     
	    msg->show();	  
	};
  
};

SaveDialog::SaveDialog(QString title, QWidget *parent)
{
  setWindowTitle(title);
  ok = marked = rotated = false;
  QGroupBox *horizontalGroupBox = new QGroupBox("Options");
  QHBoxLayout *layout = new QHBoxLayout;
  QCheckBox *checkBox1 = new QCheckBox("rotated atoms");
  QCheckBox *checkBox2 = new QCheckBox("only marked atoms");
  layout->addWidget(checkBox1);
  layout->addWidget(checkBox2);
  horizontalGroupBox->setLayout(layout);
  QPushButton *button3 = new QPushButton("Save atoms as");
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(horizontalGroupBox);
  mainLayout->addWidget(button3);
  setLayout(mainLayout);
 
  connect(checkBox1, SIGNAL(clicked(bool)), this, SLOT(checkBox1Changed(bool))); // clicked(bool)  stateChanged(int)
  connect(checkBox2, SIGNAL(clicked(bool)), this, SLOT(checkBox2Changed(bool)));
  connect(button3, SIGNAL(released()), this, SLOT(setSaveFileName()));
}


void SaveDialog::checkBox1Changed(bool state)
{
//   std::cout << "CB1 - " << state << std::endl;
   rotated = state;
}
  
void SaveDialog::checkBox2Changed(bool state)
{
//   std::cout << "CB2 - " << state << std::endl;
   marked = state;
}

  
void SaveDialog::setSaveFileName()
{
  QString cd0 = INT->currDir;
  fileName = QFileDialog::getSaveFileName(this, "Save atoms", cd0.append("/data/atoms/").append(QString::fromStdString(INT->atName)), "Molecules (*.xyz)");
  if ( !fileName.isEmpty() )  ok = true;  
//  qWarning("SD:   file: %s", fileName.toAscii().data());
  close();
} 
/*
QuestionMessageDialog::QuestionMessageDialog(QString str1, QString str2)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, str1, str2, QMessageBox::Yes | QMessageBox::No );
    if (reply == QMessageBox::Yes)
        ok = true;
    else
        ok = false;
} 
*/
