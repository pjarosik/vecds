#include <QtWidgets>
#include "../include/simplepad.h"
#include "../include/internal.h"
#include <iostream>

#include "globals.h"

SimplePad::SimplePad(QString s1, QString s2, QWidget *parent) : QWidget(parent)
{
   setAcceptDrops(true);
   setWindowTitle("SimplePad");
   setAttribute(Qt::WA_DeleteOnClose);
   editor = new QPlainTextEdit;
   editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   url = "";
   str1 = s1; str2 = s2;
   mainLayout = new QGridLayout;
   newB = new QPushButton("New");
   openB = new QPushButton("Open");
   saveB = new QPushButton("Save");
   deleteB = new QPushButton("Delete");
   exitB = new QPushButton("Exit"); 
        
   connect(newB, SIGNAL(clicked()), this, SLOT(newFile()));
   connect(openB, SIGNAL(clicked()), this, SLOT(openFile()));
   connect(saveB, SIGNAL(clicked()), this, SLOT(saveFile()));
   connect(deleteB, SIGNAL(clicked()), this, SLOT(deleteFile()));
   connect(exitB, SIGNAL(clicked()), this, SLOT(deleteLater()));
   
   mainLayout->addWidget(newB, 0, 0);
   mainLayout->addWidget(openB, 0, 1);
   mainLayout->addWidget(saveB, 0, 2);
   mainLayout->addWidget(deleteB, 0, 3);
   mainLayout->addWidget(exitB, 0, 4);
   mainLayout->addWidget(editor, 1, 0, 1, -1);
   mainLayout->setRowStretch(0, -1);
   mainLayout->setRowStretch(1, 3);
   mainLayout->setHorizontalSpacing(10);
   setLayout(mainLayout);
}  

SimplePad::~SimplePad()
{
   delete editor;
   delete mainLayout;
   delete newB;
   delete openB;
   delete saveB;
   delete deleteB;
   delete exitB;
  
}

void SimplePad::newFile()
{
    QString cd0 = INT->currDir;
    bool ok;
    QString text = QInputDialog::getText(this, "New file", "File name:", QLineEdit::Normal, "", &ok);
    if ( ok  &&  !text.isEmpty() )  {  url = cd0.append(str1.append(text));  openFile();  }
    else std::cout << " ERROR creating " << text.toStdString() << std::endl;
}

void SimplePad::openFile()
{
  std::cout << " openFile  " << url.toStdString() << std::endl;
    if ( url.isEmpty() ) {
       QString cd0 = INT->currDir;
       QFileDialog *qf = new QFileDialog(this, "Select file", cd0.append(str1), str2);
       qf->exec();                 
       if ( qf->result()!=1 ) return;                   
       url = qf->selectedFiles().at(0);
       delete qf;
    } 
    QFile file(url);
    if ( file.open(QIODevice::ReadWrite) ) {   //(QIODevice::ReadOnly|QIODevice::Text) ) {
        editor->setPlainText(QString::fromUtf8(file.readAll()));
        setWindowTitle(url.section('/', -1));
	//INT->url = url;
    } else std::cout << " ERROR opening " << url.toStdString() << std::endl;
}

void SimplePad::deleteFile()
{
    QFile file(url);
    if ( QFile::remove(url) ) {
       std::cout << " delete: url=" << url.section('/', -1).toStdString() << std::endl;
       editor->setPlainText("");
       //editor->setDocumentTitle("deleted");
    } else  std::cout << " File " << url.section('/', -1).toStdString() << " was not deleted!" << std::endl;
}

void SimplePad::saveFile()
{
    QFile file(url);
    //INT->url = url;
 std::cout << " save: url=" << url.section('/', -1).toStdString() << std::endl;
    if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )  file.write(editor->toPlainText().toUtf8());
    else  {  std::cout << " ERROR writing " << url.toStdString() << std::endl;  url = "";  }
    // else statusBar()->showMessage(tr("error in saving file %1").arg(url), 1000);
    file.close();
}

//-----------------------------------------------------------------------
SimpleEd::SimpleEd(QString url, QWidget *parent) : QWidget(parent)
{
   setAcceptDrops(true);
   setWindowTitle("SimpleEd");
   setAttribute(Qt::WA_DeleteOnClose);
   editor = new QPlainTextEdit;
   editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   mainLayout = new QGridLayout;
   saveB = new QPushButton("Save");
        
   connect(saveB, SIGNAL(clicked()), this, SLOT(saveFile()));
   //connect(exitB, SIGNAL(clicked()), this, SLOT(deleteLater()));
   
   mainLayout->addWidget(saveB, 0, 2);
   mainLayout->addWidget(editor, 1, 0, 1, -1);
   mainLayout->setRowStretch(0, -1);
   mainLayout->setRowStretch(1, 3);
   mainLayout->setHorizontalSpacing(10);
   setLayout(mainLayout);
   str1 = url;
   QFile file(str1);
   if ( file.open(QIODevice::ReadWrite) ) {   //(QIODevice::ReadOnly|QIODevice::Text) ) {
        editor->setPlainText(QString::fromUtf8(file.readAll()));
        setWindowTitle("aa");//(url.section('/', -1));
	//INT->url = url;
   } else std::cout << " ERROR opening " << url.toStdString() << std::endl;
}  

SimpleEd::~SimpleEd()
{
   delete editor;
   delete mainLayout;
   delete saveB;
  
}

void SimpleEd::saveFile()
{
    QFile file(str1);
    //INT->url = url;
 std::cout << " save: url=" << str1.toStdString() << std::endl;//url.section('/', -1)
    if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )  file.write(editor->toPlainText().toUtf8());
    else  {  std::cout << " ERROR writing " << str1.toStdString() << std::endl;  url = "";  }
    // else statusBar()->showMessage(tr("error in saving file %1").arg(url), 1000);
    file.close();
    emit SIG_finished(str1);
    deleteLater();
   
}
