#ifndef TESTMODESELECT_H
#define TESTMODESELECT_H

#include <QWidget>

namespace Ui {
class testModeSelect;
}

class testModeSelect : public QWidget
{
    Q_OBJECT

public:
    explicit testModeSelect(QWidget *parent = 0);
    ~testModeSelect();

private slots:
    void on_pb_Mpn_released();

    void on_pb_Fast_released();

    void on_pb_back_released();

private:
    Ui::testModeSelect *ui;
};

#endif // TESTMODESELECT_H
