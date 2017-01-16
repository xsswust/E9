#ifndef PARASETTING_H
#define PARASETTING_H

#include <QWidget>
#include <QTimer>
#include "src/HardWare/keyboard_dependence.h"

namespace Ui {
class ParaSetting;
}

class ParaSetting : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParaSetting(QWidget *parent = 0);
    ~ParaSetting();
	void Init();	// 初始化
	void UnInit();	// 界面跳转
    void WriteMachineParaIniPara();   // 预先写
    void readCurrentCalirationParameter();
    void floatToChar4(char* arrary,float write_data);// 将float转换为4个字节
    float char4ToFloat(char* arrary,float write_data);//将4个字节转换为float
	void showPara(void);  // 显示

	bool exit_flag;
	int TargetNo; // 菌种
	int ClassNo;	// 类别
	int ClassNo_1;	// 系列
	int UnitNo;		//单位

	QString Para_A;		// 参数A
	QString Para_B;		// 参数B
	QString Para_C;		// 参数C
	QString Para_D;		// 参数D
	QString Para_Q;		// 参数Q
	QString Para_R;		// 参数R

public slots:
    void insertValue(QString nustr);
    void keyboardDelete();

    
private slots:
    void on_pbExit_clicked();

   // void on_pbParaTable_clicked();



    void on_cbTestTarget_currentIndexChanged(int index);

    void on_cbUnit_currentIndexChanged(int index);

    void on_cbParaClass_currentIndexChanged(int index);

    void on_pbSave_clicked();


	void on_cbParaClass_2_currentIndexChanged(int index);

signals:
    void emitUnit();

private:
    Keyboard_dependence *im;
    QLineEdit *p_editer;
    Ui::ParaSetting *ui;
};

#endif // PARASETTING_H
