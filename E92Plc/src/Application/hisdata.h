#ifndef HISDATA_H
#define HISDATA_H

#include <QWidget>

namespace Ui {
class HisData;
}

class HisData : public QWidget
{
    Q_OBJECT
    
public:
    explicit HisData(QWidget *parent = 0);
    ~HisData();
	void HisData_ShowHisData(int mode);  // 进入显示界面
	void init();
	void Uninit();
	bool exit_flag;
private slots:
	void on_pbExit_clicked();			//退出 界面
	void on_pbTPC_clicked();			// 显示　菌落总数的数据
	void on_pbTolColi_clicked();		// 显示　总大肠　的历史数据
	void on_pbFecColi_clicked();		// 显示　耐热大肠的历史数据
	void on_pbEColi_clicked();			// 显示	艾希氏菌的历史数据
	void on_pbTemp1_clicked();		// 显示　温区１　温度的历史数据
	void on_pbTemp2_clicked();			// 显示　温区２　温度的历史数据
	void pbTemp3_Reless();			// 显示　温区3　温度的历史数据
    void pbTemp4_Reless();			// 显示　温区4　温度的历史数据
    void pbTemp5_Reless();			// 显示　温区5　温度的历史数据
    void pbDataOut_Reless();

private:
    Ui::HisData *ui;
};

#endif // HISDATA_H
