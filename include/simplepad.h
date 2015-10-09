#ifndef SIMPLEPAD_H
#define SIMPLEPAD_H

#include <QtGui>

class QGridLayout;
class QPlainTextEdit;
class QPushButton;

class SimplePad : public QWidget
{
     Q_OBJECT

 public:
     SimplePad(QString str1, QString str2, QWidget *parent = 0);
     ~SimplePad();
     QString url, str1, str2; // the url of the file

 private:
    QPlainTextEdit *editor; //defining the editor
    QGridLayout *mainLayout;
    QPushButton *newB;
    QPushButton *openB;
    QPushButton *saveB;
    QPushButton *deleteB;
    QPushButton *exitB;
     
 private slots:
    void saveFile();
    void newFile();
    void openFile();
    void deleteFile();
    //void closeEvent(QCloseEvent *event);
};

class SimpleEd : public QWidget
{
     Q_OBJECT

 public:
     SimpleEd(QString url, QWidget *parent = 0);
     ~SimpleEd();
     QString url, str1; // the url of the file
//     bool ok;
 signals:
    void SIG_finished(QString ff);

 private:
    QPlainTextEdit *editor; //defining the editor
    QGridLayout *mainLayout;
    QPushButton *saveB;
     
 private slots:
    void saveFile();
    //void closeEvent(QCloseEvent *event);
};
#endif
