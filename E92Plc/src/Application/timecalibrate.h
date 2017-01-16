#ifndef TIMECALIBRATE_H
#define TIMECALIBRATE_H

#include <QWidget>
#include <QTimer>
#include <QString>
#include "src/HardWare/keyboard_dependence.h"

namespace Ui {
class TimeCalibrate;
}

class TimeCalibrate : public QWidget
{
    Q_OBJECT
    
public:
    explicit TimeCalibrate(QWidget *parent = 0);
    ~TimeCalibrate();

	bool exit_flag;
public slots:
	void init();  //初始化  界面第一次执行函数
	void Un_Init();
    void timeUpData();
    void insertValue(QString nustr);
    void keyboardDelete();


private slots:
    void on_pbExit_clicked();

    void on_pbCalibrate_clicked();

private:
    QTimer *ptimerTime;
    Keyboard_dependence *im;
    QLineEdit *p_editer;
    Ui::TimeCalibrate *ui;
};

#endif // TIMECALIBRATE_H
