#ifndef HISDATATEMP_H
#define HISDATATEMP_H

#include <QWidget>

namespace Ui {
class HisDataTemp;
}

class HisDataTemp : public QWidget
{
    Q_OBJECT
    
public:
    explicit HisDataTemp(QWidget *parent = 0);
    ~HisDataTemp();
	void DisPlayTemp();  // 显示数据
	void HisDataTemp_init();  // 界面初始化
    int hisTempMode_flag;

private slots:
	void on_pushExitButton_clicked();  // 界面退出

	void on_pushCleanButton_clicked();	// 数据清空

private:
    Ui::HisDataTemp *ui;
};

#endif // HISDATATEMP_H
