#ifndef SYSSTATE_H
#define SYSSTATE_H

#include <QWidget>

namespace Ui {
class SysState;
}

class SysState : public QWidget
{
    Q_OBJECT
    
public:
    explicit SysState(QWidget *parent = 0);
    ~SysState();
    
private slots:
    void on_pbExit_clicked();

private:
    Ui::SysState *ui;
};

#endif // SYSSTATE_H
