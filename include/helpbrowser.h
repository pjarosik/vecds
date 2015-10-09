#include "internal.h"

class QPushButton; 
class QTextBrowser;
 
class HelpBrowser : public QWidget 
{ 
    Q_OBJECT 
public: 
    HelpBrowser(const QString &path, const QString &page, 
                QWidget *parent = 0); 
    static void showPage(const QString &page); 
private slots: 
    void updateWindowTitle(); 
private: 
    QTextBrowser *textBrowser; 
    QPushButton *homeButton; 
    QPushButton *backButton; 
    QPushButton *closeButton; 
}; 

