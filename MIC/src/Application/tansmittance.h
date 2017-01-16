#ifndef TANSMITTANCE_H
#define TANSMITTANCE_H

#include <QWidget>

namespace Ui {
class Tansmittance;
}

class Tansmittance : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tansmittance(QWidget *parent = 0);
    ~Tansmittance();
    
private slots:
    void on_pushButton_6_clicked();

private:
    Ui::Tansmittance *ui;
};

#endif // TANSMITTANCE_H
