#ifndef PARACALIBRATE_H
#define PARACALIBRATE_H

#include <QWidget>
#include "src/HardWare/keyboard_dependence.h"
#include "common.h"

#define	MAX_COUNT		4  // 最大点数


namespace Ui {
class ParaCalibrate;
}

class ParaCalibrate : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParaCalibrate(QWidget *parent = 0);
    ~ParaCalibrate();
    void ParaCalibrate_Init();      //界面初始化处理
    void ParaCalibrate_UnInit();    //界面跳转处理
	bool exit_flag;

	void Get_Param(int mode, ST_PARA_SETTING *st);
public slots:
    void insertValue(QString nustr); // 键盘键值处理
    void keyboardDelete();  // 键盘删除

private slots:
    void on_pbExit_clicked();

    void on_pbCalibrate_clicked();

//    void on_leDensity1_textChanged(const QString &arg1);

//    void on_leDensity2_textChanged(const QString &arg1);

//    void on_leDensity3_textChanged(const QString &arg1);

//    void on_leDensity4_textChanged(const QString &arg1);

//    void on_leQuantity1_textChanged(const QString &arg1);

 //   void on_leQuantity2_textChanged(const QString &arg1);

  //  void on_leQuantity3_textChanged(const QString &arg1);
//	void on_pb_calc_clicked();


//    void on_leQuantity4_textChanged(const QString &arg1);

private:
    Keyboard_dependence *im;
    QLineEdit *p_editer;
    Ui::ParaCalibrate *ui;
};

#endif // PARACALIBRATE_H
