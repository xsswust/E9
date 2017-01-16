#ifndef CONFIRMCLEAN_H
#define CONFIRMCLEAN_H

#include <QWidget>

namespace Ui {
class ConfirmClean;
}

class ConfirmClean : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfirmClean(QWidget *parent = 0);
    ~ConfirmClean();
    
private:
    Ui::ConfirmClean *ui;
};

#endif // CONFIRMCLEAN_H
