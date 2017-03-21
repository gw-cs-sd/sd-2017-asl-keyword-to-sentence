#ifndef DISPLAYWORDS_H
#define DISPLAYWORDS_H

#include <QMainWindow>

namespace Ui {
    class displayWords;
}

class displayWords : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit displayWords(QWidget *parent = 0);
    ~displayWords();
    
private slots:
    void on_startButton_clicked();
    
private:
    Ui::displayWords *ui;
};

#endif // DISPLAYWORDS_H
