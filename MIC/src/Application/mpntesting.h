#ifndef MPNTESTING_H
#define MPNTESTING_H

#include <QWidget>

namespace Ui {
class mpnTesting;
}

class mpnTesting : public QWidget
{
    Q_OBJECT

public:
    explicit mpnTesting(QWidget *parent = 0);
    ~mpnTesting();

private slots:
    void on_pb_FEC_released();

    void on_pb_TOL_released();

    void on_pb_back_released();

    void on_comboBox_currentIndexChanged(int index);
    void timeUpdate();		 // 时间更新显
private:
     QTimer *ptimerUpdateTime;
    Ui::mpnTesting *ui;
};

#endif // MPNTESTING_H
