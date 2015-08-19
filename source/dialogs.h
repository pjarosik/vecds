#ifndef DIALOGS_H
#define DIALOGS_H
/*
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QString>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
*/
#include <QtGui>
#include <iostream>

class QuestionForm1 : public QDialog
{
  Q_OBJECT

  public:
    QuestionForm1(QString title, QString descr, QStringList quest,
                  QStringList sug, QStringList &ans, QWidget *parent = 0);
    bool ok;
    //int numQuest;
        
  private:
    QLabel *lab0;
    //QLabel *label;
    std::vector<QLabel *> labels;
    std::vector<QLineEdit *> qEdit;
    //QDialogButtonBox *buttonBox;
};


class QuestionForm2 : public QDialog
{
  Q_OBJECT

  public:
    QuestionForm2(QString title, QString descr, QStringList quest,
                  QStringList sug, QStringList &ans, QWidget *parent = 0);
    bool ok;
    //int numQuest;
    int indCombo;
        
  private:
    QLabel *lab0;
    std::vector<QLabel *> labels;
    std::vector<QLineEdit *> qEdit;
  private slots:
    void res(int);
};

class OneQuestion : public QDialog
{
  Q_OBJECT

  public:
    OneQuestion(QString title, QString descr, QString quest,
                  QString sug, QString &ans, QWidget *parent = 0);
    bool ok;
  
  private:
    QLabel *lab0;
        
};


class SaveDialog : public QDialog
{
  Q_OBJECT

  public:
    SaveDialog(QString title, QWidget *parent = 0);
    bool ok;
    bool rotated;
    bool marked;
    QString fileName;
    
  private:
    QLabel *lab0;

  private slots:
   void checkBox1Changed(bool);
   void checkBox2Changed(bool);  
   void setSaveFileName();  
  
};
/*
class QuestionMessageDialog : public Dialog
{
  Q_OBJECT
  
  public:
    QuestionMessageDialog(QString str1, QString str2);
    bool ok;
};
*/    
#endif //DIALOGS_H
