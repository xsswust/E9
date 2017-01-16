#ifndef CONFIRMSTOP_H
#define CONFIRMSTOP_H

#include <QWidget>

namespace Ui {
class ConfirmStop;
}

class ConfirmStop : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfirmStop(QWidget *parent = 0);
    ~ConfirmStop();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ConfirmStop *ui;
};

#endif // CONFIRMSTOP_H
