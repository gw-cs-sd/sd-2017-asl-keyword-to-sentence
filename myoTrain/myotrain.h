#ifndef MYOTRAIN_H
#define MYOTRAIN_H

#include <QMainWindow>

namespace Ui {
class myoTrain;
}

class myoTrain : public QMainWindow
{
    Q_OBJECT

public:
    explicit myoTrain(QWidget *parent = 0);
    ~myoTrain();

private slots:
    void on_startAnalyzing_clicked();

private:
    Ui::myoTrain *ui;
};

#endif // MYOTRAIN_H
